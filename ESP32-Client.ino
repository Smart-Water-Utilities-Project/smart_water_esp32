#include "oled.h"
#include "config.h"
#include "websocket.h"
#include "waterflow.h"
#include "Temperature.h"

Temperature ds18b20;
WebSocketHandler websocket;
WaterflowHandler waterflow;

String data_buffer;
int last_send = millis();

void setup() {
  Serial.begin(115200);
  oled.init();
  ds18b20.init();
  waterflow.init();
  websocket.init();
}

void loop() {
  oled.ensure();
  websocket.wifi_ensure(500);
  websocket.server_ensure(500);
  if (millis() - last_send >= 1000) {
    float temperature = ds18b20.get();
    double water_flow = waterflow.get();
    bool result = websocket.send(String(water_flow));
    oled.set_waterflow(water_flow);
    oled.set_temperature(temperature);
    last_send = millis();
  }
  delay(10);
}

/*
流量 水溫 ºC 升 時
*/