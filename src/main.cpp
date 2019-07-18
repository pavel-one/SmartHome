#include "var.h" //Переменные
#include "functions.h" //Общие Функции
#include "led.h" //Функции работы с лентой
#include "routs.h" //Роуты

void setup() {
  init();
  routs();
}

void loop() {
  mls = millis();
  strobo();
  checkOTA();
}