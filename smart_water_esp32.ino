#include "config.h"
#include "logger.h"

#include "oled.h"
#include "waterbump.h"
#include "websocket.h"
#include "waterflow.h"
#include "waterlevel.h"
#include "temperature.h"

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

Parser parser;
WaterBump waterbump;
WebSocket websocket;
Waterflow waterflow;
Temperature ds18b20;
WaterLevel waterlevel;

int last_send = millis();

void setup() {
  Serial.begin(115200);
  oled.init();
  ds18b20.init();
  waterflow.init();
  waterlevel.init();
  waterbump.init();
  
  xTaskCreatePinnedToCore(clientHandle, "WebClient", 6144, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
}

void loop() {
  oled.ensure();
  oled.sendBuffer();

  if (millis() - last_send >= 1000) {
    ds18b20.ensure();
    waterflow.ensure();
    waterlevel.ensure();
    
    waterbump.ensure(&waterlevel);
    oled.drawLevel(waterlevel.lastCm);
    oled.drawWaterflow(waterflow.last_value);
    oled.drawTemperature(ds18b20.last_value);
    
    last_send = millis();
  }
  
}

void clientHandle(void *params) {
  websocket.init(&callback);
  while(1) {
    websocket.wifiEnsure(500);
    websocket.serverEnsure(500);
  }
}

void callback(String data) {
  float* temp = &(ds18b20.last_value);
  float* flow = &(waterflow.last_value);
  float* dist = &(waterlevel.lastCm);
  char* context = (char*) data.c_str();
  websocket.send(parser.processRequest(context, temp, flow, dist));
  LOGD("WEBSOCKET", "Response sent.");
  // Serial.println(response);
}

/* For chinses font generator
流量 水溫 °C 升 時 
*/