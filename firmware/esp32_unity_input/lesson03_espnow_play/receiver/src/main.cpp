#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// 第3回：ESP-NOWで無線早押しクイズ（receiver）
//
// このESP32は参加者のsenderからESP-NOWでデータを受け取り、
// USB Serial経由でUnityへそのまま流します。
//
// データの流れ:
//   sender（参加者ESP32）
//     ↓ ESP-NOW: player=N,button=1
//   receiver（このESP32 / 講師PC接続）
//     ↓ USB Serial: player=N,button=1
//   Unity

// 受信コールバック（senderからデータが届いたら呼ばれる）
//
// 注意: このコールバックはWiFiの内部タスクから呼ばれます。
//       今回は Serial.println をそのまま呼んでいますが、
//       厳密にはスレッドセーフではありません。
//       最小デモとして動作上は問題ありません。
void onDataReceived(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  // 受け取ったバイト列を文字列に変換する
  // String(char*, len) でnull終端がなくても安全に読める
  String message = String((char *)data, len);

  // そのままSerial（USB）へ出力する。Unityがこれを読む。
  Serial.println(message);
}

void setup() {
  Serial.begin(115200);

  // ESP-NOW を使うには WiFi を STA モードにする必要があります
  WiFi.mode(WIFI_STA);

  // 自分のMACアドレスを表示する
  // 参加者はこの値をsenderのコードに設定します
  Serial.print("[起動] receiver MAC アドレス: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("[エラー] ESP-NOW の初期化に失敗しました");
    while (true) delay(1000);
  }

  // senderからの受信を受け取るコールバックを登録する
  esp_now_register_recv_cb(onDataReceived);

  Serial.println("[起動] receiver 準備完了。senderからの入力を待っています...");
}

void loop() {
  // 受信はコールバック駆動なので loop には何もしない
}
