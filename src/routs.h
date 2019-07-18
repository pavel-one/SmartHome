void setColor (AsyncWebServerRequest *request) {
    /* Получение и установка переменных */
    hexColor = request->getParam("color", true)->value();
    red = request->getParam("red", true)->value().toInt();
    green = request->getParam("green", true)->value().toInt();
    blue = request->getParam("blue", true)->value().toInt();
    Brightness = new_brightness = request->getParam("brightness", true)->value().toInt();
    led_type = request->getParam("type", true)->value().toInt();
    strip.setBrightness(Brightness);

    if (led_type == 0) {
      fillAll();
    }
    if (led_type == 1) {
      fillHalf();
    }
    if (led_type == 2) {
      fillHalf(false);
    }
    if (led_type == 3) {
      frequency = request->getParam("period", true)->value().toInt();
      strobo_start = true;
    }

    ws.textAll(getOut("Обновление цвета "+String(red)+"|"+String(green)+"|"+String(blue)));
    request->redirect("/");

}

void routs() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      http2.begin("http://home.pavel.one/template/index.php");
      http2.GET();
      request->send(200, "text/html", http2.getString());
  });

  server.on("/api/setcolor", HTTP_POST, setColor);
}