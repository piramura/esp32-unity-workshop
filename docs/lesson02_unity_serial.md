# 第2回：Unity と連携

## ゴール

ESP32 と Unity を USB Serial でつなぎ、ESP32 を Unity の有線入出力デバイスとして使います。

この回では、ESP32 のボタン入力を Unity に送り、Unity 側で受け取った入力に応じてオブジェクトを変化させます。さらに、Unity から ESP32 へ LED 制御命令を送り返し、Unity からマイコンを制御できることも確認します。

第3回では、この有線通信の考え方を ESP-NOW で無線化します。

## この回で作るもの

第2回では、次のような有線入力デバイスを作ります。

```text
ESP32
  ボタン入力
  ↓ USB Serial
Unity
  オブジェクト操作
  ↓ USB Serial
ESP32
  LED制御
```

ESP32 から Unity へ入力を送り、Unity から ESP32 へ出力命令を返します。

つまり、ESP32 は単なる入力装置ではなく、Unity から制御できる入出力デバイスとして扱います。

## 全体構成

```text
XIAO ESP32C6
  ボタン入力
  LED出力
  ↑↓ USB Serial
PC / Unity
  オブジェクト操作
  状態判定
```

ESP32 は PC に USB 接続します。

ESP32 側は Serial でボタン状態を送り、Unity 側は改行ごとに受信して値を判定します。
Unity 側は、受け取った入力に応じてオブジェクトを操作し、必要に応じて ESP32 に LED 制御命令を返します。

## 使用するPlatformIOフォルダ

PlatformIO では、次のフォルダを開きます。

```text
firmware/esp32_unity_input/lesson02_serial_button/
```

リポジトリ直下ではなく、`platformio.ini` が入っているこのフォルダを開いてください。

## 使用するUnityプロジェクト

第2回と第3回では、同じ Unity プロジェクトを使います。

```text
unity/Esp32UnityWorkshop/
```

第2回では、ESP32 と Unity を USB Serial で直接つなぎます。
第3回では、受信側 ESP32 が ESP-NOW と USB Serial のブリッジになりますが、Unity 側の Serial 受信処理は第2回と同じものを使います。

## 通信の考え方

第2回では、ESP32 と Unity の間で次の2方向の通信を行います。

| 方向            | 内容         |
| ------------- | ---------- |
| ESP32 → Unity | ボタン入力を送る   |
| Unity → ESP32 | LED制御命令を送る |

最小構成では、ボタン1個と LED 1個だけを扱います。

## ESP32からUnityへ送るデータ形式

ESP32 から Unity へ、1行ごとに次の形式で送ります。

```text
button=0
button=1
```

意味は次のとおりです。

| 値          | 意味          | Unity側の扱い    |
| ---------- | ----------- | ------------ |
| `button=0` | ボタンが押されていない | 通常状態にする      |
| `button=1` | ボタンが押されている  | オブジェクトを変化させる |

Unity 側では、改行単位で文字列を読み取り、受け取った文字列が `button=1` かどうかを判定します。

## UnityからESP32へ送るデータ形式

Unity から ESP32 へ、1行ごとに次の形式で送ります。

```text
led=0
led=1
```

意味は次のとおりです。

| 値       | 意味      | ESP32側の扱い  |
| ------- | ------- | ---------- |
| `led=0` | LEDを消す  | LEDをOFFにする |
| `led=1` | LEDをつける | LEDをONにする  |

第2回では、Unity がボタン入力を受け取ったあと、Unity 側の状態に応じて LED 制御命令を ESP32 に返します。

たとえば、`button=1` を受け取ったら Unity 上のオブジェクトの色を変え、同時に `led=1` を ESP32 に送る、という動きにします。

## ボタン配線

ボタンは、ESP32 の入力ピンと GND をつなぐ形で使います。

```text
ESP32 入力ピン ---- タクトスイッチ ---- GND
```

ESP32 側のプログラムでは、入力ピンを内部プルアップして使います。

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

この場合、ボタンの状態は次のようになります。

| 状態     | digitalRead の値 | 意味   |
| ------ | -------------- | ---- |
| 押していない | HIGH           | 入力なし |
| 押している  | LOW            | 入力あり |

そのため、プログラムでは `LOW` のときに「ボタンが押された」と判定します。

## LEDについて

第2回では、まず ESP32 の内蔵LEDを使います。

外付けLEDを使う場合は、LEDに直接電流を流しすぎないように、抵抗を直列に入れてください。

```text
ESP32 出力ピン ---- 抵抗 ---- LED ---- GND
```

ただし、第2回の最小構成では配線ミスを減らすため、まずは内蔵LEDを使います。

重要なのは、ボタンとLEDを物理的に直結しないことです。
ボタン入力を ESP32 が読み取り、その結果を Unity に送り、Unity からの命令で ESP32 の LED を制御します。

この構成にすることで、次の流れが見えるようになります。

```text
現実のボタン入力
  ↓
ESP32
  ↓
Unity
  ↓
ESP32
  ↓
LED出力
```

## Serial Monitorで確認する内容

Unity を起動する前に、PlatformIO の Serial Monitor で ESP32 から値が出ていることを確認します。

1. PlatformIO で Build する
2. ESP32 に Upload する
3. Serial Monitor を開く
4. baud rate が `115200` になっていることを確認する
5. ボタンを押していないときと押したときで値が変わることを確認する

確認する表示は次の形式です。

```text
button=0
button=1
```

Serial Monitor を開いたままだと、Unity が同じ Serial ポートを開けないことがあります。Unity で確認するときは Serial Monitor を閉じます。

## Unity側で受け取る流れ

Unity 側では、次の流れで Serial 通信を扱います。

1. ESP32 の Serial ポート名を設定する
2. baud rate を `115200` に設定する
3. Play 開始時に Serial ポートを開く
4. ESP32 から届いた1行の文字列を読む
5. `button=1` のときにオブジェクトを変化させる
6. Unity 側の状態に応じて `led=0` / `led=1` を ESP32 に送る
7. Play 終了時に Serial ポートを閉じる

macOS では `/dev/cu.usbserial-*` または `/dev/cu.usbmodem*`、Windows では `COM3` のようなポート名を使います。

## Unity側の最小デモ

第2回では、まず次のような最小デモを作ります。

```text
button=0 のとき
  Unity上のオブジェクトを通常色にする
  ESP32へ led=0 を送る

button=1 のとき
  Unity上のオブジェクトの色を変える
  ESP32へ led=1 を送る
```

これにより、ESP32 から Unity へ入力が届いていることと、Unity から ESP32 へ命令を返せていることを同時に確認できます。


## 次回とのつながり

第2回では、ESP32 と Unity を USB Serial でつなぎ、入力と出力を双方向にやり取りしました。

第3回では、この有線通信を ESP-NOW で無線化します。

第2回：
ESP32 ⇄ USB Serial ⇄ Unity

第3回：
送信側ESP32 ⇄ ESP-NOW ⇄ 受信側ESP32 ⇄ USB Serial ⇄ Unity

Unity 側は第2回で作った Serial 受信処理を使い回します。

## 当日の進行手順

1. 第1回で使った PlatformIO の操作を確認する
2. `firmware/esp32_unity_input/lesson02_serial_button/` を開く
3. ESP32 とボタンを配線する
4. Build / Upload する
5. Serial Monitor で `button=0` / `button=1` を確認する
6. Serial Monitor を閉じる
7. `unity/Esp32UnityWorkshop/` を開く
8. Unity 側で Serial ポート名と baud rate を設定する
9. Unity を Play して、ボタン入力でオブジェクトが変化することを確認する
10. Unity から `led=0` / `led=1` を送り、ESP32 側のLEDが変化することを確認する

## 詰まりやすい点

* PlatformIO でリポジトリ直下を開いている
* Serial Monitor の baud rate が `115200` ではない
* Serial Monitor を開いたまま Unity を Play している
* Unity 側のポート名が間違っている
* ボタンの片側が GND につながっていない
* ボタンを押したときの HIGH / LOW の意味を逆に考えている
* ESP32 から送る文字列と Unity 側で判定する文字列が一致していない
* Unity から送る `led=0` / `led=1` の改行が抜けている
* Unity の Play 終了時に Serial ポートを閉じていない

## トラブルシューティング

[トラブルシューティング](troubleshooting.md) を参照してください。
