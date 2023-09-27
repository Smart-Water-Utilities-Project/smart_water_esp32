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
    Serial.println("[WebSocket] Handshake successful");
  } else {
    Serial.println("[WebSocket] Handshake failed, retry in a second");
    delay(1000);
    server_connect();
    server_handshake();
  }
};

void WebSocketHandler::server_connect(void) {
  // Connect to the websocket server
  Serial.println("[WebSocket] Attempting to connect to host: " + Config::server_host);
  if (wifiClient.connect(Config::server_host.c_str(), (int&)Config::server_port)) {
    Serial.println("[WebSocket] The host has been connected successfully");
  } else {
    Serial.println("[WebSocket] Failed to connect to the host, retry in a seconds");
    delay(1000);
    server_connect();
  }
};

void WebSocketHandler::wifi_connect() {
  // Connect to a WiFi network
  Serial.print("\n[Wifi] Connecting to " + String(Config::ssid));
  WiFi.begin((char*)Config::ssid,(char*)Config::pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    u8g2.drawXBM(112, 0, 16, 16, gImage_wifi_connecting_0);
    u8g2.sendBuffer();
    delay(250);
    u8g2.drawXBM(112, 0, 16, 16, gImage_wifi_connecting_1);
    u8g2.sendBuffer();
    delay(250);
    Serial.print(".");
  }

  Serial.println("\n[Wifi] WiFi connected successfully, IP: " + WiFi.localIP().toString());
  u8g2.drawXBM(112, 0, 16, 16, gImage_wifi_connect);
  u8g2.sendBuffer();
};

String WebSocketHandler::listen() {
  String str_buffer;
  if (!(wifiClient.connected())) {
    Serial.println("[DEBUG] Data listen failed.");
    return "";
  }
  webSocketClient.getData(str_buffer);
  
  Serial.println("Received data: " + String(str_buffer));
  return str_buffer;
}

bool WebSocketHandler::send(String str_buffer) {
  if (!(wifiClient.connected())) return false;
  webSocketClient.sendData(str_buffer);
  return true;
}
