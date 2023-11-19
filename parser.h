#include "logger.h"
#include <ArduinoJson.h>

class WebsocketAPI {
  public :
    String process_request(char*, float, float);

  private:
    int client_id;
    String timestamp;
    String acknowledge();
    String pack_data(float, float, String);
};

String WebsocketAPI::acknowledge() {
  return 
  R"rawliteral(
    {
      "op": 2,
      "d": {
        "dt": "sensor"
      }
    }
  )rawliteral";
}

String WebsocketAPI::pack_data(float temp, float flow, String time) {
  return R"rawliteral(
    {
      "op": 4,
      "d": {
        "wt": )rawliteral" + String(temp) + R"rawliteral(,
        "wf": )rawliteral" + String(flow) + R"rawliteral(,
        "ts": )rawliteral" + time + R"rawliteral(
      }
    }
  )rawliteral";
}

String WebsocketAPI::process_request(char* context, float temp, float flow) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, context);

  // Test if parsing succeeds.
  if (error) {
    WEBSOCKET_LOGE("Unable to handle the json from sever.");
    return "";
  }

  short int op_code = doc["op"];

  switch(op_code) {
    case 1: // Hello
      client_id = doc["d"]["id"];
      WEBSOCKET_LOGD("Get client id from server: %d", client_id);
      return acknowledge();

    case 3:
      WEBSOCKET_LOGD("Get data requests from server");
      timestamp = doc["d"]["ts"].as<String>();
      return pack_data(temp, flow, timestamp);
    
    default:
      WEBSOCKET_LOGE("Got invaild op_code from server");
      return "";
  }

}