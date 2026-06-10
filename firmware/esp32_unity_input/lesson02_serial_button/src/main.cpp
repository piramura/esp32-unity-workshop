#include <Arduino.h>

// 第2回：Unity ⇄ ESP32 の有線双方向通信
//
// 通信仕様:
//   ESP32 → Unity: button=0 / button=1
//   Unity → ESP32: led=0  / led=1
//
// 配線:
//   D0 --- ボタン --- GND
//   D1 --- 抵抗 --- LED --- GND
//   D0 は XIAO ESP32C6 上のピン名です。
//   ボタンを D0 と GND の間につなぎます。
//   LED は D1 から抵抗を通して GND へつなぎます。

// ボタン入力ピン（XIAO ESP32C6 のピン名）
const int BUTTON_PIN = D0;
const int LED_PIN = D1;

int lastButtonState = -1;

void setup() {
  Serial.begin(115200);

  // LED は Unity から届いた命令だけで制御します
  // ボタン状態には直接連動しません
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // INPUT_PULLUP: 内蔵プルアップ抵抗を有効にします
  // これにより、ボタンを押していないときは HIGH、押したときは LOW になります
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // ---- ボタン入力を読み取る ----

  int buttonState = 0;

  // LOW のとき押されている（INPUT_PULLUP のため）
  if (digitalRead(BUTTON_PIN) == LOW) {
    buttonState = 1;
  } else {
    buttonState = 0;
  }

  // 状態が変わったときだけ Unity へ送る
  if (buttonState != lastButtonState) {
    Serial.print("button=");
    Serial.println(buttonState);
    lastButtonState = buttonState;

    // チャタリング対策: ボタンを1回押しても接点の揺れで
    // 複数回押したように読まれることがある（チャタリング）。
    // 20ms 待つことで、揺れが収まってから次の読み取りに進みます。
    delay(20);
  }

  // ---- Unity からの LED 命令を受け取る ----
  // Unity は改行付きで "led=0" / "led=1" を送ってきます

  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim(); // 前後の空白・改行を除去

    if (line == "led=1") {
      digitalWrite(LED_PIN, HIGH);
    } else if (line == "led=0") {
      digitalWrite(LED_PIN, LOW);
    }
    // 不明な文字列は無視する
  }
}
