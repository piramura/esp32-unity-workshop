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
