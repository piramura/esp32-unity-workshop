# トラブルシューティング

## PlatformIO: Configuring project: XX% で止まっている

正常です。初回は Seeed のプラットフォームをダウンロードするため数分かかります。キャンセルしないで待ってください。

## ESP32 に書き込めない

- USB ケーブルがデータ通信対応か確認する
- Arduino IDE または PlatformIO のボード設定を確認する
- 書き込みポートを確認する
- 書き込み開始時に ESP32 の BOOT ボタンを押す

## Serial Monitor に何も出ない

- baud rate が `115200` になっているか確認する
- ESP32 のプログラムが `Serial.begin(115200)` を実行しているか確認する
- 正しいポートを開いているか確認する

## Unity が Serial ポートを開けない

- Serial Monitor や他のアプリが同じポートを開いていないか確認する
- Unity 側のポート名を確認する
- macOS では `/dev/cu.usbserial-*` または `/dev/cu.usbmodem*` を使う
- Windows では `COM3` のような COM ポート名を使う

## 第3回: ESP-NOW が届かない

- 参加者senderに設定した MAC アドレスが講師用receiverのものか確認する
- sender と receiver の Wi-Fi channel を一致させる
- 講師用receiverが講師PCにUSB接続され、電源が入っているか確認する
- ESP32 同士を近づけて確認する

## Unity のオブジェクトが動かない

- Unity Console に Serial 受信ログが出ているか確認する
- ESP32 から送られる文字列と Unity 側の解析処理が一致しているか確認する
- Play 中に対象オブジェクトがシーン上に存在するか確認する
