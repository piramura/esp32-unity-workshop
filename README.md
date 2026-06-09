# Unity × ESP32 × ESP-NOW Workshop

ESP32を使って、Unityと連携する入力デバイスを作るための講習教材です。

この講習では、まずESP32単体の基本操作を学び、その後UnityとのUSB Serial連携、最後にESP-NOWによる無線化まで進めます。

## 3回の流れ

| 回 | テーマ | この回でできるようになること |
|---|---|---|
| 第1回 | マイコンのみ | ESP32にプログラムを書き込み、LEDやボタンを動かせる |
| 第2回 | Unity ⇄ ESP32 のUSB Serial双方向通信 | ESP32からUnityへ `button=0/1`、UnityからESP32へ `led=0/1` で双方向制御できる |
| 第3回 | ESP-NOWで無線早押しクイズ | 参加者のESP32を無線早押しボタンとして使い、Unity上で早押しクイズに参加できる |

## 最終的に作るもの

ESP32を使った無線早押しクイズシステムです。

第2回では、ESP32とUnityをUSB Serialで双方向に接続します。ESP32からUnityへ `button=0` / `button=1` を送り、UnityからESP32へ `led=0` / `led=1` を返します。

第3回では、参加者のESP32を無線早押しボタン（sender）として使い、講師が用意したESP32（receiver）を経由してUnity上の早押しクイズに参加します。

```txt
参加者のESP32（sender）
  ボタンを押す
  ↓ ESP-NOW
講師用ESP32（receiver）
  ↓ USB Serial
Unity
  早押しクイズを判定
```

発展として、Unityの判定結果を講師用receiver経由で勝者のsenderへ返し、LEDを光らせます。

## 必要機材

- XIAO ESP32C6 1台（第3回のreceiverは講師が用意します）
- USB ケーブル 1本（第3回のreceiverへの接続ケーブルは講師が用意します）
- ブレッドボード
- ジャンパーワイヤ
- LED
- 抵抗 220Ω 程度
- タクトスイッチ
- PC
- PlatformIO（VS Code拡張として使用）
- Unity 6000.3.6f1

XIAO ESP32C6のPlatformIO設定はSeeed Studio公式Wikiの「XIAO ESP32C6 with PlatformIO」を基準にしています。platformにはSeeed提供のカスタムプラットフォームを使用します。

PlatformIOで「Configuring project: XX%」と表示されて止まっているように見える場合があります。初回はSeeedのプラットフォームをダウンロードするため数分かかるので、キャンセルせずに待ってください。

詳細は [docs/parts_list.md](docs/parts_list.md) を参照してください。

## 第2回・第3回を始める前の前提

第2回・第3回では、PlatformIO の基本操作（Build / Upload / Serial Monitor）は第1回で確認済みとして進めます。

Unity は [Unity Hub](https://unity.com/download) から `6000.3.6f1` をインストールして使います。別バージョンでも開ける場合がありますが、講習ではこのバージョンを基準にします。

## Unityプロジェクト

第2回と第3回では、同じUnityプロジェクトを使います。

```text
unity/Esp32UnityWorkshop/
```

第2回は、1台のESP32とPCをUSB Serialでつなぎ、Unityと双方向に値をやり取りします。第3回は、参加者のESP32（sender）から講師用ESP32（receiver）へESP-NOWで値を送り、receiverから同じUnityプロジェクトへUSB Serialで値を送ります。

Unity Hub で `Open` を選び、`unity/Esp32UnityWorkshop/` フォルダを指定して開きます。初回はインポートに数分かかることがあります。

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
│   └── esp32_unity_input/
│       ├── lesson02_serial_button/
│       └── lesson03_espnow_play/
│           ├── sender/
│           └── receiver/
├── unity/
│   └── Esp32UnityWorkshop/
└── images/
    ├── lesson01/
    ├── lesson02/
    └── lesson03/
```

## PlatformIOで開くフォルダ

PlatformIOでは、リポジトリ直下ではなく `platformio.ini` があるフォルダを開いてください。

第2回と第3回は、次のフォルダを間違えないように確認してからBuild / Uploadします。

| 回 | 開くフォルダ | 内容 |
|---|---|---|
| 第1回 | `firmware/lesson01_microcontroller_basic/` | マイコン基礎 |
| 第2回 | `firmware/esp32_unity_input/lesson02_serial_button/` | UnityへSerial送信 |
| 第3回（参加者） | `firmware/esp32_unity_input/lesson03_espnow_play/sender/` | 無線早押しボタン |
| 第3回（講師用） | `firmware/esp32_unity_input/lesson03_espnow_play/receiver/` | ESP-NOW / USB Serial ブリッジ |

## 教材

- [第1回：マイコンのみ](docs/lesson01_microcontroller_basic.md)
- [第2回：Unity ⇄ ESP32 のUSB Serial双方向通信](docs/lesson02_unity_serial.md)
- [第3回：ESP-NOWで無線早押しクイズ](docs/lesson03_espnow_play.md)
- [部品表](docs/parts_list.md)
- [トラブルシューティング](docs/troubleshooting.md)

## ライセンス

このリポジトリは MIT License で公開します。詳細は [LICENSE](LICENSE) を参照してください。

## 参考資料・公式ドキュメント

本教材では、できるだけ公式ドキュメントを基準にしています。

| 資料 | 用途 | URL |
|---|---|---|
| Seeed Studio Wiki: XIAO ESP32C6 with PlatformIO | XIAO ESP32C6用のPlatformIO設定 | https://wiki.seeedstudio.com/ja/xiao_esp32c6_with_platform_io/ |
| PlatformIO Documentation | PlatformIOプロジェクト構成、Build / Upload / Serial Monitor | https://docs.platformio.org/ |
| Arduino Documentation | Serial、digitalRead、digitalWrite などの基本API | https://docs.arduino.cc/ |
| Espressif ESP-NOW Documentation | ESP-NOW通信の仕様・実装確認 | https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html |
