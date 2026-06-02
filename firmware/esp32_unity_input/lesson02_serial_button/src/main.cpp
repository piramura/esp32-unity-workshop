#include <Arduino.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("Hello from XIAO ESP32C6");
  delay(1000);
}

