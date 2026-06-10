# 第1回：マイコンのみ

# ゴール
マイコン、センサー、アクチュエータといったマイコン関連の基本事項を知ってもらい、実際にSeeed Studio XIAO ESP32C6を用いてセンサーを読み取れることを確認し、LEDをチカチカさせます。

# この回の流れ
1. GitHubからレポジトリを持ってくる
1. VScodeの拡張機能のPlatformIO・SerialMonitorを入れる
1. PlatformIOでプロジェクトを開く
1. マイコンのはんだ付け
1. LEDとタクトスイッチの回路作り
1. PlatformIOプロジェクトをコンパイルできるようにする
1. マイコンとシリアル通信
1. マイコンでLEDチカチカ
1. マイコンでスイッチ読んでLEDを光らせる
1. 時間あったらセンサー・アクチュエータ紹介

# GitHubからレポジトリを持ってくる
GitHubからレポジトリを持ってきます。
https://github.com/piramura/esp32-unity-workshop 

# VScodeの拡張機能のPlatformIO・SerialMonitorを入れる
VScodeを開いて拡張機能を入れましょう。
1. VScodeの拡張機能を入れます。拡張機能からPlatformIOと調べて、アリのようなアイコンのものを入れます。入れるところまでは[このサイト](https://qiita.com/nextfp/items/f54b216212f08280d4e0)が参考になりそうです。
1. また、SerialMonitorと調べてMicroSoftのSerialMonitorを入れます。

ここまで来たら、一旦VScodeを閉じます。
1. 何も開いていない新しいVScodeのウィンドウを出します。
1. ここで、拡張機能として左側にアリマークが増えるので、押します。
1. PlatformIOのPIO Homeのタブが出てくると思う(出てこなければOpen)のでOpenボタンを押します。
1. ここで、GitHubから持ってきた中の以下のフォルダを選択してOpenを押します。```esp32-unity-workshop\firmware\lesson01_microcontroller_basic```
こっから**めちゃくちゃ**時間がかかるので違う作業に移ります。

# マイコンのはんだ付け
その場で教えるので特に書かないでおきます。

# LEDとタクトスイッチの回路作り
プログラムを書き込む前に回路を作りましょう。
ブレッドボードに部品を挿していきます。
部品一覧
- はんだ付けしたマイコン
- LED
- タクトスイッチ
- 約10kΩ抵抗
- 200Ω以上の抵抗なんでも
- ジャンパワイヤー

回路図は以下のようになります。D0とD3はマイコンのピン名で、D0がマイコンのRって書いてあるところの近くの一番端で、D0を端から1番目としたとき、D3は端から4番目です。[これ](https://wiki.seeedstudio.com/ja/XIAO_ESP32C3_Getting_Started/#%E8%A1%A8%E9%9D%A2)を見ると分かります。

![回路図](https://github.com/piramura/esp32-unity-workshop/blob/origin/lesson1/images/lesson01/kairozu.png "回路図")

サンプルはこれです。

![回路サンプル](https://github.com/piramura/esp32-unity-workshop/blob/origin/lesson1/images/lesson01/IMG_7145.JPG "サンプル")

ジャンパワイヤを使ってどんどんつなげてみましょう。一応チェックしてから電源入れるので見せてください。

# PlatformIOプロジェクトをコンパイルできるようにする
回路が終わったころにはPlatformIOの初期化が終わってると思うので一旦そのままコンパイルできるか試します。
PlatformIOを動かしてるときだけVScodeの左下にいろんなマークが出ます。その中のチェックマークを押してみましょう。コンパイルが始まります。
エラーが出たら教えてください。

環境構築の鬼門がここなので通ればあとはスムーズです。

# マイコンとシリアル通信・マイコンでLEDチカチカ・マイコンでスイッチ読んでLEDを光らせる
これ以降は写経タイムです。写経してみましょう。

解説付きコード
```
// ArduinoIDEの機能を使うために必要なおまじない
#include <Arduino.h>

// LED用とボタン用のピン番号を定義
const int pinLed = D0;
const int pinButton = D3;

void setup() {
  // baud rateを決める関数
  // パソコンと通信する際に通信速度を合わせる必要がある
  // 引数の115200は一般的な値で、安定して通信できる速度
  Serial.begin(115200);
  
  // pinMode関数は、指定したピンのモードを設定するための関数
  // 入力モード（INPUT）か出力モード（OUTPUT）を指定する
  // 引数のpinLedはLED用のピン番号、pinButtonはボタン用のピン番号
  pinMode(pinLed, OUTPUT);
  pinMode(pinButton, INPUT);
}

void loop() {
  // Serial.println関数は、引数の内容をシリアルモニタに表示するための関数
  // "Hello from XIAO ESP32C6"は表示する文字列
  Serial.println("Hello from XIAO ESP32C6");

  // digitalRead関数は、指定したピンの状態を読み取るための関数
  // 引数のpinButtonはボタン用のピン番号
  // 返り値は入力(電圧)がHIGH（1）かLOW（0）かを表す整数
  int buttonState = digitalRead(pinButton);

  // digitalWrite関数は、指定したピンのON/OFFを変えられる関数
  // 引数のpinLedはLED用のピン番号、二つ目の引数は0(OFF)か1(ON)
  digitalWrite(pinLed, buttonState);

  // 先述の通り
  Serial.printf("Button state: %d\n", buttonState);

  // delay関数は、引数の時間（ミリ秒）だけ処理を一時停止するための関数
  // 引数の10は10ミリ秒（0.01秒）を意味
  delay(10);
}

```

# 時間あったらセンサー・アクチュエータ紹介
時間ができたらnotionのハードウェアのところにセンサー・アクチュエータ紹介が書いてあるので見てみましょう。