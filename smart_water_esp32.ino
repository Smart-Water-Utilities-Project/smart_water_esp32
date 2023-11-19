#include "config.h"
#include "logger.h"

#include "oled.h"
#include "websocket.h"
#include "waterflow.h"
#include "temperature.h"

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

WebsocketAPI api;
Temperature ds18b20;
WebSocketHandler websocket;
WaterflowHandler waterflow;
String data_buffer, response;

int last_send = millis();
float temperature_result, waterflow_result;

void setup() {
  Serial.begin(115200);
  oled.init();
  ds18b20.init();
  waterflow.init();
  xTaskCreatePinnedToCore(webclient_handle, "WebClient", 6144, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  // websocket.init(&callback);
}

void loop() {
  oled.ensure();
  // websocket.wifi_ensure(500);
  // websocket.server_ensure(500);
  if (millis() - last_send >= 1000) {
    temperature_result = ds18b20.get();
    waterflow_result = waterflow.get();
    oled.set_waterflow(waterflow_result);
    oled.set_temperature(temperature_result);
    last_send = millis();
    delay(10);
  }
}

void webclient_handle(void *params) {
  websocket.init(&callback);
  while(1) {
    websocket.wifi_ensure(500);
    websocket.server_ensure(500);
  }
}

void callback(String data) {
  float temp = ds18b20.last_value;
  float flow =  waterflow.last_value;
  char* context = (char*) data.c_str();
  response = api.process_request(context, temp, flow);
  websocket.send(response);
  WEBSOCKET_LOGI("Response sent.");
  // Serial.println(response);
}

/* For chinses font generator
流量 水溫 °C 升 時 
*/