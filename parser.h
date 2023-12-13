#include "logger.h"
#include <ArduinoJson.h>

class Parser {
  public :
    String processRequest(char*, float*, float*, float*);

  private:
    int client_id;
    String timestamp;
    String acknowledge();
    String packData(float*, float*, float*, String);
};

String Parser::acknowledge() {
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

String Parser::packData(float* temp, float* flow, float* dist, String time) {
  return R"rawliteral(
    {
      "op": 4,
      "d": {
        "wt": )rawliteral" + String(*temp) + R"rawliteral(,
        "wf": )rawliteral" + String(*flow) + R"rawliteral(,
        "wd": )rawliteral" + String(*dist) + R"rawliteral(,
        "ts": )rawliteral" + time + R"rawliteral(
      }
    }
  )rawliteral";
}

String Parser::processRequest(char* context, float* temp, float* flow, float* distance) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, context);

  // Test if parsing succeeds.
  if (error) {
    LOGE("PARSER", "Unable to handle the json from sever.");
    return "";
  }

  short int op_code = doc["op"];

  switch(op_code) {
    case 1: // Hello
      client_id = doc["d"]["id"];
      LOGD("PARSER", "Get client id from server: %d", client_id);
      return acknowledge();

    case 3:
      LOGD("PARSER", "Get data requests from server");
      timestamp = doc["d"]["ts"].as<String>();
      return packData(temp, flow, distance, timestamp);
    
    default:
      LOGD("PARSER", "Got invaild op_code from server");
      return "";
  }

}