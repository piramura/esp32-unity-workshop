#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// 第3回：ESP-NOWで無線早押しクイズ（sender）
//
// このESP32はボタンを押したら、receiverへ無線でデータを送ります。
//
// 通信仕様:
//   sender → receiver: player=N,button=1（ESP-NOW）
//   receiver → sender: led=1 / led=0（ESP-NOW）
//
// 配線:
//   D0 --- ボタン --- GND
//   D1 --- 抵抗 --- LED --- GND

// ---- 書き込み前にここを確認する ----

// 自分の参加者番号。講師から割り当てられた番号に変更してください。
const int PLAYER_ID = 1;

// 講師用 receiver の MAC アドレス。
// Serial Monitor に表示された AA:BB:CC:DD:EE:FF を
// 下の形式で1バイトずつ入力してください。
// 例: AA:BB:CC:DD:EE:FF → {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}
uint8_t RECEIVER_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// ------------------------------------

const int BUTTON_PIN = D0;
const int LED_PIN = D1;

int lastButtonState = -1;
unsigned long lastReadyLogTime = 0;
bool hasSentButton = false;

void printSenderMacAddress() {
  Serial.print("[起動] sender MAC アドレス: ");
  Serial.println(WiFi.macAddress());
}

void printReadyStatus() {
  printSenderMacAddress();
  Serial.print("[起動] receiver MAC 設定済み: ");
  for (int i = 0; i < 6; i++) {
    if (i > 0) Serial.print(":");
    if (RECEIVER_MAC[i] < 0x10) Serial.print("0");
    Serial.print(RECEIVER_MAC[i], HEX);
  }
  Serial.println();
  Serial.println("[起動] sender 準備完了");
  Serial.print("[起動] PLAYER_ID = ");
  Serial.println(PLAYER_ID);
}

bool isReceiverMacNotSet() {
  for (int i = 0; i < 6; i++) {
    if (RECEIVER_MAC[i] != 0xFF) {
      return false;
    }
  }

  return true;
}

void onDataReceived(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  String message = String((char *)data, len);
  message.trim();

  Serial.print("[受信] ");
  Serial.println(message);

  if (message == "led=1") {
    digitalWrite(LED_PIN, HIGH);
  } else if (message == "led=0") {
    digitalWrite(LED_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);

  // INPUT_PULLUP: 内蔵プルアップ抵抗を有効にします
  // ボタンを押していないとき HIGH、押したとき LOW になります
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // ESP-NOW を使うには WiFi を STA モードにする必要があります
  // Wi-Fi接続はしませんが、モード設定は必須です
  WiFi.mode(WIFI_STA);

  delay(1500);
  printSenderMacAddress();

  if (isReceiverMacNotSet()) {
    while (true) {
      Serial.println("[エラー] receiver の MAC アドレスが未設定です");
      Serial.println("        RECEIVER_MAC を講師用 receiver の MAC アドレスに書き換えてください");
      printSenderMacAddress();
      delay(2000);
    }
  }

  if (esp_now_init() != ESP_OK) {
    Serial.println("[エラー] ESP-NOW の初期化に失敗しました");
    while (true) delay(1000); // 止める
  }

  // receiver を送信先（peer）として登録する
  // memset でゼロ初期化しないと、channel や encrypt に不定値が入ることがあります
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, RECEIVER_MAC, 6);
  peerInfo.channel = 0;     // 0 = 現在のチャンネルを自動で使う
  peerInfo.encrypt = false; // 暗号化なし

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[エラー] receiver の登録に失敗しました。MAC アドレスを確認してください");
    while (true) delay(1000);
  }

  esp_now_register_recv_cb(onDataReceived);

  printReadyStatus();
}

void loop() {
  if (!hasSentButton && millis() - lastReadyLogTime >= 2000) {
    printReadyStatus();
    lastReadyLogTime = millis();
  }

  int buttonState = 0;

  if (digitalRead(BUTTON_PIN) == LOW) {
    buttonState = 1;
  } else {
    buttonState = 0;
  }

  // 状態が変わったときだけ処理する
  if (buttonState != lastButtonState) {
    lastButtonState = buttonState;

    // ボタンが押された瞬間だけ送信する
    if (buttonState == 1) {
      // 送信するデータを文字列で組み立てる
      String message = "player=" + String(PLAYER_ID) + ",button=1";
      Serial.print("[送信] ");
      Serial.println(message);

      // ESP-NOW で receiver へ送信する
      esp_err_t result = esp_now_send(RECEIVER_MAC, (uint8_t *)message.c_str(), message.length());

      if (result == ESP_OK) {
        Serial.println("[送信] 成功");
        hasSentButton = true;
      } else {
        Serial.println("[送信] 失敗");
      }
    }

    // チャタリング対策: ボタンの接点の揺れで複数回押したように読まれるのを防ぎます
    delay(20);
  }
}
