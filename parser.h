#include <ArduinoJson.h>

class WebsocketAPI {
  public :
    String process_request(char*, float, float);

  private:
    int client_id;
    String acknowledge();
    String data_return(float, float);

};

String WebsocketAPI::acknowledge() {
  return R"rawliteral(.
  {
    \"op\": 2,
    \"d\": {
      \"dt\": \"sensor\"
    }
  })rawliteral";
}

String WebsocketAPI::data_return(float temp, float flow) {
  return R"rawliteral(
    {
      \"op\": 4,
      \"d\": {
        \"wt\": \")rawliteral" + String(temp) + R"rawliteral(\",
        \"wf\": \")rawliteral" + String(flow) + R"rawliteral(\"
      }
    })rawliteral";
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
      client_id = doc["data"]["cid"];
      WEBSOCKET_LOGD("Get client id from server: %d", client_id);
      return acknowledge();

    case 3:
      WEBSOCKET_LOGD("Get data requests from server");
      return data_return(temp, flow);
    
    default:
      WEBSOCKET_LOGE("Got invaild op_code from server");
      return "";
  }

}

