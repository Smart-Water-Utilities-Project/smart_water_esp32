#include <WiFi.h>
#include "config.h"
#include "websocket.h"

WebSocketHandler websocket;

String data_buffer;

void setup() {
  Serial.begin(115200);
  websocket.init();
}


void loop() {
  
  websocket.send("Test Message");
  data_buffer = websocket.listen();
  
  // wait to fully let the client disconnect
  delay(3000);
  
}
