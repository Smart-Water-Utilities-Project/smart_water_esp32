#include <WiFi.h>
#include <ArduinoWebsockets.h>

using namespace websockets;

class WebSocketHandler {
  public:
    WiFiClient wifiClient;
    WebsocketsClient webSocketClient;
    
    void init(void);
    bool send(String);
    void listen(void);
    
    void wifi_ensure(int);
    void wifi_connect(void);

    void server_ensure(int);
    bool server_connect(void);

  private:
    bool wifi_status = false;
    void wifi_connect_icon(int);
    bool wifi_connect_animation = false;
    unsigned long int wifi_last_check = millis();

    bool server_status = false;
    unsigned long int server_last_check = millis();
};

void WebSocketHandler::init(void) {
  wifi_connect();
  server_connect();
  listen();
  
  return;
}

bool WebSocketHandler::send(String str_buffer) {
  if (!(webSocketClient.available())) return false;
  webSocketClient.send(str_buffer);
  return true;
}

void WebSocketHandler::listen() {
  webSocketClient.onMessage([&](WebsocketsMessage message){
    Serial.print("Got Message: ");
    Serial.println(message.data());
  });
}

void WebSocketHandler::wifi_ensure(int interval=-1) {
  if (interval > 0 && (millis() - wifi_last_check) <= interval) return;
  bool current_status = WiFi.status();
  if (!current_status) { wifi_connect(); }
  
  if (wifi_status^current_status) {
    u8g2.drawXBM(112, 0, 16, 16, 
      (current_status) ? gImage_wifi_connect : gImage_wifi_not_connect);
    u8g2.sendBuffer();
  }
  wifi_status = current_status;

  return;
}

void WebSocketHandler::wifi_connect() {
  // Connect to a WiFi network
  Serial.print("\n[Wifi] Connecting to " + String(wifi_ssid));
  WiFi.begin((char*)wifi_ssid,(char*)wifi_pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    wifi_connect_icon(250);
  }

  Serial.println("\n[Wifi] WiFi connected successfully, IP: " + WiFi.localIP().toString());
  u8g2.drawXBM(112, 0, 16, 16, gImage_wifi_connect);
  u8g2.sendBuffer();
}

void WebSocketHandler::server_ensure(int interval=-1) {
  if (interval > 0 && (millis() - server_last_check) <= interval) return;
  bool current_status = webSocketClient.available();
  if (server_status^current_status) {
    u8g2.drawXBM(96, 0, 16, 16, 
      (current_status) ? gImage_server_connected : gImage_server_not_connected);
    u8g2.sendBuffer();
  }
  
  if (!current_status) { server_connect(); }
  server_status = current_status;
  webSocketClient.poll();
  return;
}

bool WebSocketHandler::server_connect(void) {
  // Connect to the websocket server
  Serial.println("[WiFi-Client] Attempting to connect to host: " + (char) *ws_server_host);
  if (webSocketClient.connect(ws_server_host, ws_server_port, ws_server_path)) {
    Serial.println("[WiFi-Client] The host has been connected successfully");
    u8g2.drawXBM(96, 0, 16, 16, gImage_server_connected);
    u8g2.sendBuffer();
  } else {
    Serial.println("[WiFi-Client] Failed to connect to the host, retry in a seconds");
    server_ensure();
  }
}

/*void WebSocketHandler::socket_ensure(int interval=-1) {
  if (0) {
    u8g2.drawXBM(80, 0, 16, 16, gImage_handshake_successful);
    u8g2.sendBuffer();
    return;
  } 
  u8g2.drawXBM(80, 0, 16, 16, gImage_handshake_failed);
  u8g2.sendBuffer();
  delay(1000);
  socket_handshake();
}

void WebSocketHandler::socket_handshake(void) {
  // Handshake with the server
  webSocketClient.path = (char*) ws_server_path;
  webSocketClient.host = (char*) (String(ws_server_host) + ":" + String(ws_server_port)).c_str();
  if (webSocketClient.handshake(wifiClient)) {
    Serial.println("[WebSocket] Handshake successful");
    u8g2.drawXBM(80, 0, 16, 16, gImage_handshake_successful);
    u8g2.sendBuffer();
    return;
  } 
  Serial.println("[WebSocket] Handshake failed, retry in a second");
  u8g2.drawXBM(80, 0, 16, 16, gImage_handshake_failed);
  u8g2.sendBuffer();
  delay(5000);
  socket_handshake();
  
}*/

void WebSocketHandler::wifi_connect_icon(int interval) {
  wifi_connect_animation = !wifi_connect_animation;
  u8g2.drawXBM(112, 0, 16, 16, 
    wifi_connect_animation ? gImage_wifi_connecting_0 : gImage_wifi_connecting_1);
  u8g2.sendBuffer();
  delay(interval);
  return;
}
