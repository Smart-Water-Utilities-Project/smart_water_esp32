#include "oled.h"
#include "config.h"
#include "websocket.h"


WebSocketHandler websocket;

String data_buffer;
int last_send = millis();

void setup() {
  Serial.begin(115200);
  oled.init();
  websocket.init();
}


void loop() {
  oled.ensure();
  websocket.wifi_ensure(500);
  websocket.server_ensure(500);
  if (millis() - last_send >= 1000) {
    bool result = websocket.send("Test Message");
    last_send = millis();
  }
}
