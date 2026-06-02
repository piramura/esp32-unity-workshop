# 第3回：ESP-NOW で遊ぶ

## 目的

送信側 ESP32 から受信側 ESP32 へ ESP-NOW で値を送り、受信側 ESP32 から USB Serial で Unity に渡します。

## ゴール

- ESP-NOW の送信側プログラムを作る
- ESP-NOW の受信側プログラムを作る
- 受信側 ESP32 から Unity へ Serial 送信する
- Unity 上で無線入力デバイスとして遊べるデモを作る

## 使用するもの

- XIAO ESP32C6 2台
- USB ケーブル 2本
- タクトスイッチ
- Unity

## 開くフォルダ

| 役割 | 開くフォルダ |
|---|---|
| 送信側 | `firmware/esp32_unity_input/lesson03_espnow_play/sender/` |
| 受信側 | `firmware/esp32_unity_input/lesson03_espnow_play/receiver/` |

## 配線

作成予定。

## 構成

```text
送信側 ESP32
  ボタン入力
  ↓ ESP-NOW
受信側 ESP32
  ↓ USB Serial
PC / Unity
  オブジェクト操作
```

## Serial データ形式

受信側 ESP32 から Unity へ、1行ごとに値を送ります。

```text
button=0
button=1
```

## 手順

1. 受信側 ESP32 の MAC アドレスを確認する
2. 送信側プログラムに受信側 MAC アドレスを設定する
3. 送信側 ESP32 に送信用スケッチを書き込む
4. 受信側 ESP32 に受信用スケッチを書き込む
5. 受信側 ESP32 を Unity を動かす PC に USB 接続する
6. Unity を Play して、送信側ボタンでデモを操作できることを確認する

## 確認ポイント

- 送信側に設定した MAC アドレスが受信側 ESP32 のもの
- 送信側と受信側の Wi-Fi channel が一致している
- Unity が受信側 ESP32 の Serial ポートを開いている

## トラブルシューティング

[トラブルシューティング](troubleshooting.md) を参照してください。
