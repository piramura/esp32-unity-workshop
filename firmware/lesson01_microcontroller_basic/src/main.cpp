#include <Arduino.h>

const int pinLed = D0;
const int pinButton = D3;

void setup() {
  Serial.begin(115200);
  pinMode(pinLed, OUTPUT);
  pinMode(pinButton, INPUT);
}

void loop() {
  // 段階的に書いていこう。一番下のdelay(1000);だけは先に書こう
  
  Serial.println("Hello from XIAO ESP32C6");

  // ここまでで書き込もう

  int buttonState = digitalRead(pinButton);
  Serial.printf("Button state: %d\n", buttonState);

  // ここまでで書き込もう

  digitalWrite(pinLed, buttonState);

  // ここまでで書き込もう

  delay(1000);
}
