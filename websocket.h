#include "config.h"
#include <WebSocketClient.h>

class WebSocketHandler {
  public:
    WiFiClient wifiClient;
    WebSocketClient webSocketClient;
    
    void init(void);
    void wifi_connect(void);
    void server_connect(void);
    void server_handshake(void);

    String listen(void);
    bool send(String);
};

void WebSocketHandler::init(void) {
  wifi_connect();
  server_connect();
  server_handshake();
  return;
};

void WebSocketHandler::server_handshake(void) {
  // Handshake with the server
  webSocketClient.path = (char*) Config::server_path.c_str();
  webSocketClient.host = (char*) (String(Config::server_host) + ":" + String(Config::server_port)).c_str();
  if (webSocketClient.handshake(wifiClient)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed, restart in 5 seconds");
    delay(5000);
    ESP.restart();
  }
};

void WebSocketHandler::server_connect(void) {
  // Connect to the websocket server
  if (wifiClient.connect(Config::server_host.c_str(), (int&)Config::server_port)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed, restart in 5 seconds");
    delay(5000);
    ESP.restart();
  }
};

void WebSocketHandler::wifi_connect() {
  // Connect to a WiFi network
  Serial.print("\nConnecting to" + String(Config::ssid) + "\n");
  WiFi.begin((char*)Config::ssid,(char*)Config::pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected with IP: " + String(WiFi.localIP()) + "\n");
  
};

String WebSocketHandler::listen() {
  String str_buffer;
  if (!(wifiClient.connected())) return "";
  webSocketClient.getData(str_buffer);
  
  Serial.println("Received data: " + String(str_buffer));
  return str_buffer;
}

bool WebSocketHandler::send(String str_buffer) {
  if (!(wifiClient.connected())) return false;
  webSocketClient.sendData(str_buffer);
  return true;
}
