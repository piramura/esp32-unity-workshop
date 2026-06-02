# 第2回：Unity と連携

## 目的

ESP32 から USB Serial で PC へ値を送り、Unity で受信してオブジェクト操作に使います。

## ゴール

- ESP32 から Serial 通信で値を送る
- Unity で SerialPort を開く
- 受信した値を Unity オブジェクトの移動や回転に反映する

## 使用するもの

- XIAO ESP32C6
- USB ケーブル
- タクトスイッチ
- Unity

## 開くフォルダ

```text
firmware/esp32_unity_input/lesson02_serial_button/
```

## 配線

作成予定。

## Serial データ形式

ESP32 から Unity へ、1行ごとに値を送ります。

```text
button=0
button=1
```

Unity 側では改行単位で読み取り、`button=1` のときにオブジェクトを動かします。

## 手順

1. `firmware/esp32_unity_input/lesson02_serial_button/` を PlatformIO で開く
2. Serial Monitor で `button=0` または `button=1` が表示されることを確認する
3. Serial Monitor を閉じる
4. Unity プロジェクト `unity/Esp32UnityWorkshop/` を開く
5. Unity 側の Serial ポート名と baud rate を設定する
6. Play して、ボタン入力でオブジェクトが動くことを確認する

## 確認ポイント

- Serial Monitor を開いたまま Unity を実行していない
- ESP32 と Unity の baud rate がどちらも `115200`
- ポート名が OS に合わせて正しい

## トラブルシューティング

[トラブルシューティング](troubleshooting.md) を参照してください。
