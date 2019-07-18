#include "Arduino.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h" //Работа с веб сервером
#include "ArduinoJson.h"
#include "Update.h" //Обновлятор
#include "HTTPClient.h"
#include "Adafruit_NeoPixel.h"

int attentionPin = 2; //Светодиод на плате
int mls = 0; //Количество миллисекунд сейчас

const float version = 2;
float remote_v = 2;

const char* ssid = "NeRostelecom";
const char* password = "tipira21";

const int stripCount = 147; //Количество светодиодов в ленте
const int stripPin = 23; //Пин ленты
String hexColor = "#000000";
int red = 0;
int green = 0;
int blue = 0;
int Brightness = 0;
int new_brightness = 0; 
int led_type = 0; //Тип по умолчанию

bool strobo_start = false; //Сужебная переменная для стробоскопа
bool strobo_flag = false; //Сужебная переменная для стробоскопа
int frequency = 1000; //Частота стробоскопа

String jsonOut;

/* Переменные для OTA */
long contentLength = 0;
bool isValidContentType = false;
int port = 80;
String host = "home.pavel.one";
String bin = "/firmware.bin";

/* ws2812b лента */
Adafruit_NeoPixel strip = Adafruit_NeoPixel(stripCount, stripPin, NEO_GRB + NEO_KHZ800);

// Создание веб серверов
AsyncWebServer server(80);
AsyncWebSocket ws("/ws:80");
HTTPClient http;
HTTPClient http2;
WiFiClient client;

//JSON
DynamicJsonBuffer JBuffer;
JsonObject& root = JBuffer.createObject();