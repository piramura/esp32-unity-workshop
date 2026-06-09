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
- XIAO ESP32C6 では `platformio.ini` に USB CDC 設定が入っているか確認する

```ini
build_flags =
  -DARDUINO_USB_MODE=1
  -DARDUINO_USB_CDC_ON_BOOT=1
```

## XIAO ESP32C6 をリセットすると Serial Monitor が閉じる

正常な挙動です。リセット時に USB Serial が一瞬切断されるため、PC側の Serial Monitor がポートを閉じることがあります。

1. Serial Monitor を閉じる
2. 数秒待つ
3. ポートを選び直す
4. Serial Monitor を開き直す

macOS では `/dev/tty.usbmodem*` ではなく `/dev/cu.usbmodem*` を使います。

## Unity が Serial ポートを開けない

- **Serial Monitor を開いたままにしていないか確認する**（最もよくある原因）
  - PlatformIO の Serial Monitor と Unity は同じポートを同時に開けない
  - Unity を Play する前に Serial Monitor を閉じる
- Unity 側のポート名を確認する
- macOS では `/dev/cu.usbserial-*` または `/dev/cu.usbmodem*` を使う
- Windows では `COM3` のような COM ポート名を使う

## 第3回: receiver のポートを Unity に設定し忘れている

- Unity の `SerialController` の `portName` が receiver のポートになっているか確認する
- PC に複数の ESP32 を接続している場合、別の ESP32 のポートを設定している可能性がある
- receiver だけを接続した状態でポート名を確認するとわかりやすい

## 第3回: RECEIVER_MAC が未設定のまま書き込んだ

`RECEIVER_MAC` を変更せずに書き込むと、sender は起動後に次のエラーを出力して止まります。

```text
[エラー] receiver の MAC アドレスが未設定です
        RECEIVER_MAC を講師用 receiver の MAC アドレスに書き換えてください
```

Serial Monitor に上記が表示されたまま動かなくなります。`RECEIVER_MAC` を正しい値に書き換えて再度書き込んでください。

## 第3回: ESP-NOW が届かない

- 参加者senderに設定した MAC アドレスが講師用receiverのものか確認する
- `RECEIVER_MAC` がデフォルト値 `{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}` のままになっていないか確認する
- sender と receiver の Wi-Fi channel が合っているか確認する。第3回のコードでは `channel = 0` で現在のチャンネルを使う
- 会場Wi-Fiが混んでいる場合は、まず sender と receiver を近づけて1台ずつ確認する
- 講師用receiverが講師PCにUSB接続され、電源が入っているか確認する
- ESP32 同士を近づけて確認する

## 第3回: 勝者が誰か判別できない

- `PLAYER_ID` が参加者全員で同じ番号になっていないか確認する
- 各参加者が異なる `PLAYER_ID` を設定しているか確認する
- 第3回のreceiverは最大16人分の `PLAYER_ID` を記録する
- receiver の Serial Monitor で `player=N,button=1` の `N` が参加者ごとに異なることを確認する

## Unity のオブジェクトが動かない

- Unity Console に Serial 受信ログが出ているか確認する
- ESP32 から送られる文字列と Unity 側の解析処理が一致しているか確認する
- Play 中に対象オブジェクトがシーン上に存在するか確認する
