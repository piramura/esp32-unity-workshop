# Unity × ESP32 × ESP-NOW Workshop

ESP32を使って、Unityと連携する入力デバイスを作るための講習教材です。

この講習では、まずESP32単体の基本操作を学び、その後UnityとのUSB Serial連携、最後にESP-NOWによる無線化まで進めます。

## 3回の流れ

| 回 | テーマ | この回でできるようになること |
|---|---|---|
| 第1回 | マイコンのみ | ESP32にプログラムを書き込み、LEDやボタンを動かせる |
| 第2回 | Unityと連携 | ESP32の入力をUnityに送り、Unity上のオブジェクトを操作できる |
| 第3回 | ESP-NOWで遊ぶ | ESP32同士を無線通信させ、無線入力デバイスとして使える |

## 最終的に作るもの

ESP32を使った無線入力デバイスです。

第3回では、送信側ESP32から受信側ESP32へESP-NOWで入力を送り、受信側ESP32からUnityへUSB Serialで値を渡します。

```txt
送信側ESP32
  ↓ ESP-NOW
受信側ESP32
  ↓ USB Serial
Unity

## 必要機材

- ESP32 開発ボード 2台
- USB ケーブル 2本
- ブレッドボード
- ジャンパーワイヤ
- LED
- 抵抗 220Ω 程度
- タクトスイッチ
- PC
- Arduino IDE または PlatformIO
- Unity

詳細は [docs/parts_list.md](docs/parts_list.md) を参照してください。

## リポジトリ構成

```text
.
├── README.md
├── docs/
│   ├── lesson01_microcontroller_basic.md
│   ├── lesson02_unity_serial.md
│   ├── lesson03_espnow_play.md
│   ├── parts_list.md
│   └── troubleshooting.md
├── firmware/
│   ├── lesson01_microcontroller_basic/
│   ├── lesson02_unity_serial/
│   └── lesson03_espnow_play/
├── unity/
│   └── Esp32UnityWorkshop/
└── images/
    ├── lesson01/
    ├── lesson02/
    └── lesson03/
```

## 教材

- [第1回：マイコンのみ](docs/lesson01_microcontroller_basic.md)
- [第2回：Unity と連携](docs/lesson02_unity_serial.md)
- [第3回：ESP-NOW で遊ぶ](docs/lesson03_espnow_play.md)
- [部品表](docs/parts_list.md)
- [トラブルシューティング](docs/troubleshooting.md)
