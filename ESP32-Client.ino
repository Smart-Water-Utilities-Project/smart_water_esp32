#include <WiFi.h>
#include "oled.h"
#include "config.h"
#include "websocket.h"

WebSocketHandler websocket;
SSD1306 oled;

String data_buffer;

void setup() {
  Serial.begin(115200);
  oled.init();
  websocket.init();
}


void loop() {
  
  bool result = websocket.send("Test Message");
  if (!result) {
    websocket.server_connect();
    websocket.server_handshake();
  }
  data_buffer = websocket.listen();
  
  // wait to fully let the client disconnect
  delay(500);
  Serial.println("[DEBUG] Loop end here.");
}
