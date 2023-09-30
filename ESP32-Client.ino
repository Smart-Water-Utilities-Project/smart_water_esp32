#include "oled.h"
#include "config.h"
#include "websocket.h"

SSD1306 oled;
WebSocketHandler websocket;

String data_buffer;

void setup() {
  Serial.begin(115200);
  oled.init();
  websocket.init();
}


void loop() {
  websocket.wifi_ensure(500);
  websocket.server_ensure(500);
  bool result = websocket.send("Test Message");
  delay(500);
}
