#include "oled.h"
#include "config.h"
#include "parser.h"

#include <WiFi.h>
#include <ArduinoWebsockets.h>

using namespace websockets;

class WebSocket {
  public:
    WiFiClient wifiClient;
    WebsocketsClient webSocketClient;
    
    void init(void (*value)(String) = &null_func);
    bool send(String);
    void listen(void);
    
    void wifiEnsure(int);
    void wifiConnect(void);

    void serverEnsure(int, bool);
    bool serverConnect(void);

    void (*callback)(String);
    static void null_func(String data) {};

  private:
    bool wifi_status = false;
    void wifiConnectIcon(int);
    unsigned long int wifi_last_check = millis();

    bool server_status = false;
    unsigned long int server_last_check = millis();

    
};

#if skip_websocket
void WebSocket::init(void) {}
bool WebSocket::send(String) {return true;}
void WebSocket::listen(void) {}
void WebSocket::wifiEnsure(int) {}
void WebSocket::wifiConnect(void) {}
void WebSocket::serverEnsure(int) {}
bool WebSocket::serverConnect(void) {return true;}
void WebSocket::wifiConnectIcon(int) {}

#else

void WebSocket::init(void (*value)(String)) {
  callback = value;
  wifiConnect();
  serverConnect();
  listen();
  return;
}

bool WebSocket::send(String str_buffer) {
  if (!(webSocketClient.available())) return false;
  oled.showUpload(200);
  webSocketClient.send(str_buffer);
  return true;
}

void WebSocket::listen() {
  webSocketClient.onMessage([&](WebsocketsMessage message){
    oled.showDownload(200);
    String got_message = message.data();
    LOGD("WEBSOCKET", "Got Message: %s", got_message.c_str());
    callback(got_message.c_str());
  });
}

void WebSocket::wifiEnsure(int interval=-1) {
  if (interval > 0 && (millis() - wifi_last_check) <= interval) return;
  bool current_status = WiFi.status() == WL_CONNECTED;
  if (!current_status) { wifiConnect(); }
  
  if (wifi_status^current_status) {
    oled.drawWifi((current_status) ? gImage_wifi_connected : gImage_server_failure);
  }
  wifi_status = current_status;

  return;
}

void WebSocket::wifiConnect() {
  // Connect to a WiFi network
  LOGI("WIFI", "Connecting to %s...", WIFI_SSID);
  WiFi.begin((char*)WIFI_SSID, (char*)WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    // delay(250);
    oled.wifiAnimation(250);
  }

  LOGI("WIFI", "WiFi connected, IP: %s", WiFi.localIP().toString().c_str());
  oled.drawWifi(gImage_wifi_connected);
}

void WebSocket::serverEnsure(int interval=-1, bool until_success=false) {
  if (interval > 0 && (millis() - server_last_check) <= interval) return;
  bool current_status = webSocketClient.available();
  if (server_status^current_status) {
    if (!current_status) {
      LOGI("WEBSOCKET", "Connection lost.");
      webSocketClient.close();
    }
    oled.clearUpload(true);
    oled.clearDownload(true);
    oled.drawServer((current_status) ? gImage_server_connected : gImage_server_failure);
  }
  
  if (!current_status) { 
    current_status = serverConnect();
    if (!current_status && until_success) serverEnsure();
  }
  server_status = current_status;
  webSocketClient.poll();
  return;
}

bool WebSocket::serverConnect(void) {
  // Connect to the websocket server
  LOGI("WEBSOCKET", "Connecting to : %s", WS_SERVER_HOST);
  oled.drawServer(gImage_server_connecting);

  if (webSocketClient.connect(WS_SERVER_HOST, WS_SERVER_PORT, WS_SERVER_PATH)) {
    LOGI("WEBSOCKET", "Connection established.");
    oled.drawServer(gImage_server_connected);
    return true;
  } 
  
  LOGE("WEBSOCKET", "Connection failed.");
  oled.drawServer(gImage_server_failure);
  delay(1000);
  return false;
}

#endif