#include <WiFi.h>
#include "parser.h"
#include <ArduinoWebsockets.h>

using namespace websockets;

class WebSocketHandler {
  public:
    WiFiClient wifiClient;
    WebsocketsClient webSocketClient;
    
    void init(void (*value)(String) = &null_func);
    bool send(String);
    void listen(void);
    
    void wifi_ensure(int);
    void wifi_connect(void);

    void server_ensure(int, bool);
    bool server_connect(void);

    void (*callback)(String);
    static void null_func(String data) {};

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

void WebSocketHandler::init(void (*value)(String)) {
  callback = value;
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
    String got_message = message.data();
    WEBSOCKET_LOGD("Got Message: %s", got_message.c_str());
    callback(got_message.c_str());
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
  WEBSOCKET_LOGI("Connecting to %s...", WIFI_SSID);
  WiFi.begin((char*)WIFI_SSID,(char*)WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    oled.wifi_animation(0);
  }

  WEBSOCKET_LOGI("WiFi connected, IP: %s", WiFi.localIP().toString().c_str());
  oled.draw_wifi(gImage_wifi_connected);
}

void WebSocketHandler::server_ensure(int interval=-1, bool until_success=false) {
  if (interval > 0 && (millis() - server_last_check) <= interval) return;
  bool current_status = webSocketClient.available();
  if (server_status^current_status) {
    if (!current_status) {
      WEBSOCKET_LOGI("Connection lost.");
      webSocketClient.close();
    }
    oled.clear_upload(true);
    oled.clear_download(true);
    oled.draw_server((current_status) ? gImage_server_connected : gImage_server_failure);
  }
  
  if (!current_status) { 
    current_status = server_connect();
    if (!current_status && until_success) server_ensure();
  }
  server_status = current_status;
  webSocketClient.poll();
  return;
}

bool WebSocketHandler::server_connect(void) {
  // Connect to the websocket server
  WEBSOCKET_LOGI("Connecting to the host: %s", WS_SERVER_HOST);
  oled.draw_server(gImage_server_connecting);

  if (webSocketClient.connect(WS_SERVER_HOST, WS_SERVER_PORT, WS_SERVER_PATH)) {
    WEBSOCKET_LOGI("Connection established.");
    oled.draw_server(gImage_server_connected);
    return true;
  } 
  
  WEBSOCKET_LOGE("Connection failed.");
  oled.draw_server(gImage_server_failure);
  delay(1000);
  return false;
}

#endif