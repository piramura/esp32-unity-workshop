#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// 第3回：ESP-NOWで無線早押しクイズ（receiver）
//
// このESP32は参加者のsenderからESP-NOWでデータを受け取り、
// USB Serial経由でUnityへそのまま流します。
// Unityから winner=N や clear_leds が届いたら、該当senderへLED命令を返します。
//
// データの流れ:
//   sender（参加者ESP32）
//     ↓ ESP-NOW: player=N,button=1
//   receiver（このESP32 / 講師PC接続）
//     ↓ USB Serial: player=N,button=1
//   Unity
//     ↓ USB Serial: winner=N / clear_leds
//   receiver
//     ↓ ESP-NOW: led=1 / led=0
//   sender

const int MAX_PLAYERS = 16;

struct PlayerPeer {
  bool used;
  int playerId;
  uint8_t mac[6];
};

PlayerPeer players[MAX_PLAYERS];
unsigned long lastMacLogTime = 0;
bool hasReceivedSender = false;

void printReceiverMacAddress() {
  Serial.print("[起動] receiver MAC アドレス: ");
  Serial.println(WiFi.macAddress());
}

int parsePlayerId(const String &message) {
  const String prefix = "player=";
  int start = message.indexOf(prefix);
  if (start < 0) return -1;

  start += prefix.length();
  int end = message.indexOf(',', start);
  if (end < 0) end = message.length();

  return message.substring(start, end).toInt();
}

void ensurePeer(const uint8_t *mac) {
  if (esp_now_is_peer_exist(mac)) return;

  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, mac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);
}

void rememberPlayer(int playerId, const uint8_t *mac) {
  if (playerId <= 0) return;

  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (players[i].used && players[i].playerId == playerId) {
      memcpy(players[i].mac, mac, 6);
      ensurePeer(mac);
      return;
    }
  }

  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (!players[i].used) {
      players[i].used = true;
      players[i].playerId = playerId;
      memcpy(players[i].mac, mac, 6);
      ensurePeer(mac);
      return;
    }
  }
}

PlayerPeer *findPlayer(int playerId) {
  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (players[i].used && players[i].playerId == playerId) {
      return &players[i];
    }
  }

  return nullptr;
}

void sendLedToMac(const uint8_t *mac, bool on) {
  const char *message = on ? "led=1" : "led=0";
  esp_now_send(mac, (const uint8_t *)message, strlen(message));
}

void turnOffAllLeds() {
  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (players[i].used) {
      sendLedToMac(players[i].mac, false);
    }
  }
}

void sendWinnerLed(int playerId) {
  turnOffAllLeds();

  PlayerPeer *winner = findPlayer(playerId);
  if (winner == nullptr) {
    Serial.print("[警告] 未登録の player ID です: ");
    Serial.println(playerId);
    return;
  }

  sendLedToMac(winner->mac, true);
}

void handleSerialCommand(String command) {
  command.trim();
  if (command.length() == 0) return;

  if (command.startsWith("winner=")) {
    int playerId = command.substring(String("winner=").length()).toInt();
    sendWinnerLed(playerId);
  } else if (command == "clear_leds") {
    turnOffAllLeds();
  }
}

// 受信コールバック（senderからデータが届いたら呼ばれる）
//
// 注意: このコールバックはWiFiの内部タスクから呼ばれます。
//       今回は Serial.println をそのまま呼んでいますが、
//       厳密にはスレッドセーフではありません。
//       最小デモとして動作上は問題ありません。
void onDataReceived(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  hasReceivedSender = true;

  // 受け取ったバイト列を文字列に変換する
  // String(char*, len) でnull終端がなくても安全に読める
  String message = String((char *)data, len);

  int playerId = parsePlayerId(message);
  // ESP-NOWの受信情報には送信元MACが入っているので、payloadにMACを入れる必要はない
  rememberPlayer(playerId, info->src_addr);

  // そのままSerial（USB）へ出力する。Unityがこれを読む。
  Serial.println(message);
}

void setup() {
  Serial.begin(115200);

  // ESP-NOW を使うには WiFi を STA モードにする必要があります
  WiFi.mode(WIFI_STA);

  // 自分のMACアドレスを表示する
  // 参加者はこの値をsenderのコードに設定します
  delay(1500);
  printReceiverMacAddress();

  if (esp_now_init() != ESP_OK) {
    Serial.println("[エラー] ESP-NOW の初期化に失敗しました");
    while (true) delay(1000);
  }

  // senderからの受信を受け取るコールバックを登録する
  esp_now_register_recv_cb(onDataReceived);

  Serial.println("[起動] receiver 準備完了。senderからの入力を待っています...");
}

void loop() {
  if (!hasReceivedSender && millis() - lastMacLogTime >= 2000) {
    printReceiverMacAddress();
    lastMacLogTime = millis();
  }

  if (Serial.available()) {
    handleSerialCommand(Serial.readStringUntil('\n'));
  }
}
