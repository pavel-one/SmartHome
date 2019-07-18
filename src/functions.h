/* Возращает json state переменных */
String getOut(String msg) {
    jsonOut = "";
    root.set("msg", msg);
    root.set("color", hexColor);
    root.set("Brightness", Brightness);
    root.printTo(jsonOut);
    return jsonOut;
}
/* Парсинг заголовков */
String getHeaderValue(String header, String headerName) {
  return header.substring(strlen(headerName.c_str()));
}
// OTA Логика
void execOTA() {
  Serial.println("Подготовка к обновлению");
  Serial.println("Подключение к: " + String(host));
  if (client.connect(host.c_str(), port)) {
    Serial.println("Достаем прошивку: " + String(bin));
    client.print(String("GET ") + bin + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Cache-Control: no-cache\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("Таймаут подключения!");
        client.stop();
        return;
      }
    }
    while (client.available()) {
      String line = client.readStringUntil('\n');
      line.trim();
      if (!line.length()) {
        break;
      }

      // Проверяем сервер на ответ 200
      // Иначе выходим из обновления
      if (line.startsWith("HTTP/1.1")) {
        if (line.indexOf("200") < 0) {
          Serial.println("Сервер ответил не 200 ответ, выходим из обновления.");
          break;
        }
      }

      // Достаем заголовки
      // Начинаем обновление
      if (line.startsWith("Content-Length: ")) {
        contentLength = atol((getHeaderValue(line, "Content-Length: ")).c_str());
        Serial.println("Получаем " + String(contentLength) + " байт с сервера");
      }

      if (line.startsWith("Content-Type: ")) {
        String contentType = getHeaderValue(line, "Content-Type: ");
        Serial.println("Получаем " + contentType + " содержимое.");
        if (contentType == "application/octet-stream") {
          isValidContentType = true;
        }
      }
    }
  } else {
    Serial.println("Подключение к " + String(host) + " прервалось. Проверь конфигурацию");
  }
  Serial.println("contentLength : " + String(contentLength) + ", isValidContentType : " + String(isValidContentType));

  if (contentLength && isValidContentType) {
    bool canBegin = Update.begin(contentLength);
    if (canBegin) {
      Serial.println("Пошло обновление, жди!");
      size_t written = Update.writeStream(client);

      if (written == contentLength) {
        Serial.println("Записано : " + String(written) + " успешно");
      } else {
        Serial.println("Записано только : " + String(written) + "/" + String(contentLength) + ". Повторить?" );
      }

      if (Update.end()) {
        Serial.println("Обновление закончено!");
        if (Update.isFinished()) {
          Serial.println("Обновление закончено успешно, перезагрузка");
          ESP.restart();
        } else {
          Serial.println("Обновление прервалось");
        }
      } else {
        Serial.println("Ошибка #: " + String(Update.getError()));
      }
    } else {
      client.flush();
    }
  } else {
    Serial.println("Супервыход");
    ESP.restart();
    client.flush();
  }
}

/* Отлов эвентов вебсокетов */
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){

  if(type == WS_EVT_CONNECT){
 
    Serial.println("К вебсокетам подключился клиент");
    client->text(getOut("Вы успешно подключены"));
    
 
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Клиент отключился");
  }
}

void init() {
    /* Служебная инициализация */
    pinMode(attentionPin, OUTPUT);
    Serial.begin(115200);

    /* Инициализация ленты */
    strip.begin();
    strip.setBrightness(Brightness);
    strip.clear();
    strip.show();

    /* Подключение к WIFI */
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(5000);
        digitalWrite(attentionPin, HIGH);
        Serial.println("Подключение к Wi-Fi..");
    }
    digitalWrite(attentionPin, LOW);
    Serial.println(WiFi.localIP());

    /* Инициализация сокетов */
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);
    server.begin();

    /* Устанавливаем основной урл запроса */
    http.begin("http://home.pavel.one/index.php");
}


void checkOTA() {
  if ((mls % 30000) == 0) {
    int httpCode = http.GET();
    if(httpCode > 0) {
      if(httpCode == HTTP_CODE_OK) {
          remote_v = http.getString().toFloat();
          Serial.println(remote_v);
      }
    }

    if (remote_v != version)
    {
      Serial.println("Надо обновляться");
      execOTA();
    }
  }
}