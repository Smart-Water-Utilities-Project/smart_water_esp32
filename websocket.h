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
    unsigned long int wifi_last_check = millis();

    bool server_status = false;
    unsigned long int server_last_check = millis();
};

#if skip_websocket
void WebSocketHandler::init(void) {}
bool WebSocketHandler::send(String) {return true;}
void WebSocketHandler::listen(void) {}
void WebSocketHandler::wifi_ensure(int) {}
void WebSocketHandler::wifi_connect(void) {}
void WebSocketHandler::server_ensure(int) {}
bool WebSocketHandler::server_connect(void) {return true;}
void WebSocketHandler::wifi_connect_icon(int) {}

#else
void WebSocketHandler::init(void) {
  wifi_connect();
  server_connect();
  listen();
  
  return;
}

bool WebSocketHandler::send(String str_buffer) {
  if (!(webSocketClient.available())) return false;
  oled.show_upload(200);
  webSocketClient.send(str_buffer);
  return true;
}

void WebSocketHandler::listen() {
  webSocketClient.onMessage([&](WebsocketsMessage message){
    oled.show_download(200);
    Serial.println("Got Message: " + message.data());
  });
}

void WebSocketHandler::wifi_ensure(int interval=-1) {
  if (interval > 0 && (millis() - wifi_last_check) <= interval) return;
  bool current_status = WiFi.status() == WL_CONNECTED;
  if (!current_status) { wifi_connect(); }
  
  if (wifi_status^current_status) {
    oled.draw_wifi((current_status) ? gImage_wifi_connected : gImage_server_failure);
  }
  wifi_status = current_status;

  return;
}

void WebSocketHandler::wifi_connect() {
  // Connect to a WiFi network
  Serial.print("\n[Wifi] Connecting to " + String(wifi_ssid));
  WiFi.begin((char*)wifi_ssid,(char*)wifi_pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
    oled.wifi_animation(0);
  }

  Serial.println("\n[Wifi] WiFi connected successfully, IP: " + WiFi.localIP().toString());
  oled.draw_wifi(gImage_wifi_connected);
}

void WebSocketHandler::server_ensure(int interval=-1) {
  if (interval > 0 && (millis() - server_last_check) <= interval) return;
  bool current_status = webSocketClient.available();
  if (server_status^current_status) {
    if (!current_status) {
      Serial.println("[Websocket] Connection lost from the server");
      webSocketClient.close();
    }
    oled.clear_upload(true);
    oled.clear_download(true);
    oled.draw_server((current_status) ? gImage_server_connected : gImage_server_failure);
  }
  
  if (!current_status) { 
    current_status = server_connect();
    if (!current_status) server_ensure();
  }
  server_status = current_status;
  webSocketClient.poll();
  return;
}

bool WebSocketHandler::server_connect(void) {
  // Connect to the websocket server
  Serial.println("[Websocket] Attempting to connect to host: " + String(ws_server_host));
  oled.draw_server(gImage_server_connecting);

  if (webSocketClient.connect(ws_server_host, ws_server_port, ws_server_path)) {
    Serial.println("[Websocket] The host has been connected successfully");
    oled.draw_server(gImage_server_connected);
    return true;
  } 
  
  Serial.println("[Websocket] Failed to connect to the host, retry in a seconds");
  oled.draw_server(gImage_server_failure);
  delay(1000);
  return false;
}

#endif