#include "oled.h"
#include "config.h"
#include "websocket.h"
#include "waterflow.h"


WebSocketHandler websocket;
WaterflowHandler waterflow;

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
    double water_flow_result = waterflow.get_waterflow();
    bool result = websocket.send(String(water_flow_result));
    last_send = millis();
  }
}


/*
流量
*/
