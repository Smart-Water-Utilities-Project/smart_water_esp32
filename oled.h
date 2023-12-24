#include "icons.h"
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#ifndef oled_include
#define oled_include
class u8g2: public U8G2_SSD1306_128X64_NONAME_F_HW_I2C {
  public: 
    u8g2(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) :
      U8G2_SSD1306_128X64_NONAME_F_HW_I2C(rotation, reset, clock, data) {};
};

class SSD1306: public u8g2 {
  public:
    SSD1306() : u8g2(U8G2_R0, U8X8_PIN_NONE) {};

    void init();
    void send();
    void clear();
    void ensure();
    void println(String);
    
    void showUpload(int);
    void clearUpload(bool force=false);

    void showDownload(int);
    void clearDownload(bool force=false);

    void drawWifi(const unsigned char*);
    void drawServer(const unsigned char*);

    void wifiAnimation(int);
    void clearArea(int, int, int, int);

    void drawWaterflow(int);
    void drawTemperature(float);
    void drawLevel(float);

    void sendBuffer();

    void drawBump(bool);
    void drawVavle(bool);

    void update(float, float, float);
    
  private:
    int string_pos_y = 14;
    
    int upload_interval;
    int download_interval;
    
    unsigned long last_upload;
    unsigned long last_download;

    bool wifi_animation_state = false;
    unsigned long last_wifi_animation = 0;

    bool sending_buffer = false;
};

void SSD1306::init(void) {
  u8g2::begin();
  u8g2::enableUTF8Print();  // 啟動 UTF8 支援
  drawBump(false);
  drawVavle(false);
  drawWifi(gImage_wifi_blank);
  drawServer(gImage_server_blank);
  println("");
  println("[ Initializing ]");
  println(" Please wait... ");
  sendBuffer();
}

void SSD1306::println(String context) {
  u8g2::setFont(u8g2_font_unifont_t_chinese1); // 使用 chinese1字型檔
  u8g2::drawStr(0, string_pos_y, context.c_str()); //自動換行
  string_pos_y += 16;
  return;
}

void SSD1306::send() {  //顯示暫存器內容
  sendBuffer();
  return;
}

void SSD1306::clear() { //清除暫存器內容
  string_pos_y = 14;
  u8g2::clearBuffer();
  return;
}

void SSD1306::showUpload(int interval) {
  last_upload = millis();
  upload_interval = interval;
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::drawXBM(80, 0, 8, 16, gImage_socket_upload);
  sendBuffer();
}

void SSD1306::clearUpload(bool force) {
  if (!force) {
    if (upload_interval < 0) return;
    if (millis() - last_upload <= upload_interval) return;
  }
  
  upload_interval = -1;
  clearArea(80, 0, 8, 16);
  sendBuffer();
}

void SSD1306::showDownload(int interval) {
  last_download = millis();
  download_interval = interval;
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::drawXBM(88, 0, 8, 16, gImage_socket_download);
  sendBuffer();
}

void SSD1306::clearDownload(bool force) {
  if (!force) {
    if (download_interval < 0) return;
    if (millis() - last_download <= download_interval) return;
  }
  download_interval = -1;
  clearArea(88, 0, 8, 16);
  sendBuffer();
}

void SSD1306::drawServer(const unsigned char* image) {
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::drawXBM(96, 0, 16, 16, (const uint8_t*) image);
  sendBuffer();
}

void SSD1306::drawWifi(const unsigned char* image) {
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::drawXBM(112, 0, 16, 16, (const uint8_t*) image);
  sendBuffer();
}

void SSD1306::wifiAnimation(int interval) {
  if (millis() - last_wifi_animation < interval) return;
  wifi_animation_state = !wifi_animation_state;
  drawWifi(wifi_animation_state ? gImage_wifi_connecting : gImage_wifi_blank);
  last_wifi_animation = millis();
}

void SSD1306::clearArea(int x, int y, int w, int h) {
  u8g2::setFontMode(1);
  u8g2::setDrawColor(0);
  u8g2::drawRBox(x, y, w, h, 0);
}

void SSD1306::drawWaterflow(int context) {
  clearArea(0, 16, 128, 16);
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::setFont(u8g2_font_unifont_t_chinese1);
  u8g2::setCursor(0, 32);
  u8g2::print(String("流量: "+String(context)+"升/時").c_str());
}

void SSD1306::drawTemperature(float context) {
  clearArea(16, 34, 128, 16);
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::setFont(u8g2_font_unifont_t_chinese1);
  u8g2::setCursor(0, 48);
  u8g2::print(String("水溫: "+String(context)+"°C").c_str());
}

void SSD1306::drawLevel(float context) {
  clearArea(34, 52, 128, 16);
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::setFont(u8g2_font_unifont_t_chinese1);
  u8g2::setCursor(0, 64);
  u8g2::print(String("水位: "+String(context)+"cm").c_str());
}

void SSD1306::ensure() {
  clearUpload();
  clearDownload();
  return;
}

void SSD1306::update(float lastCm, float waterFlow, float temperature) {
  oled.drawLevel(lastCm);
  oled.drawWaterflow(waterFlow);
  oled.drawTemperature(temperature);
  oled.sendBuffer();
}

void SSD1306::sendBuffer() {
  if (sending_buffer) return;
  sending_buffer = true;
  u8g2::sendBuffer();
  sending_buffer = false;
  return;
}

void SSD1306::drawBump(bool state) {
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::drawXBM(16, 0, 16, 16, (const uint8_t*) ((state) ? gImage_bump_on : gImage_bump_off));
}

void SSD1306::drawVavle(bool state) {
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::drawXBM(0, 0, 16, 16, (const uint8_t*) ((state) ? gImage_valve_on : gImage_valve_off));
}

SSD1306 oled;

#endif