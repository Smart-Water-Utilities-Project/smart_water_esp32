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
  oled.show_upload(200);
  webSocketClient.send(str_buffer);
  return true;
}

void WebSocketHandler::listen() {
  webSocketClient.onMessage([&](WebsocketsMessage message){
    oled.show_download(200);
    Serial.print("Got Message: ");
    Serial.println(message.data());
  });
}

void WebSocketHandler::wifi_ensure(int interval=-1) {
  if (interval > 0 && (millis() - wifi_last_check) <= interval) return;
  bool current_status = WiFi.status();
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
    Serial.print(".");
    wifi_connect_icon(250);
  }

  Serial.println("\n[Wifi] WiFi connected successfully, IP: " + WiFi.localIP().toString());
  oled.draw_wifi(gImage_wifi_connected);
}

void WebSocketHandler::server_ensure(int interval=-1) {
  if (interval > 0 && (millis() - server_last_check) <= interval) return;
  bool current_status = webSocketClient.available();
  if (server_status^current_status) {
    if (!current_status) Serial.println("[Websocket] Connection lost from the server");
    oled.clear_upload();
    oled.clear_download();
    oled.draw_server((current_status) ? gImage_server_connected : gImage_server_failure);
  }
  
  if (!current_status) { server_connect(); }
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
  } else {
    Serial.println("[Websocket] Failed to connect to the host, retry in a seconds");
    oled.draw_server(gImage_server_failure);
    delay(1000);
    server_ensure();
  }
}

void WebSocketHandler::wifi_connect_icon(int interval) {
  delay(interval);
  wifi_connect_animation = !wifi_connect_animation;
  oled.draw_wifi(wifi_connect_animation ? gImage_wifi_connecting : gImage_wifi_blank);
  return;
}
