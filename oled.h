#include "icons.h"
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

class SSD1306 {
  public:
    void init();
    void send();
    void clear();
    void ensure();
    void println(String);
    
    void show_upload(int);
    void clear_upload(void);

    void show_download(int);
    void clear_download(void);

    void draw_wifi(const unsigned char*);
    void draw_server(const unsigned char*);
    
  private:
    int string_pos_y = 14;
    
    int upload_interval;
    int download_interval;
    unsigned long last_upload;
    unsigned long last_download;

};

void SSD1306::init() {
  u8g2.begin();
  u8g2.enableUTF8Print();  // 啟動 UTF8 支援
  u8g2.drawXBM(96, 0, 16, 16, gImage_server_blank);
  u8g2.drawXBM(112, 0, 16, 16, gImage_wifi_blank);
  u8g2.sendBuffer();
  return;
}

void SSD1306::println (String context) {
  u8g2.setFont(u8g2_font_unifont_t_chinese1); // 使用 chinese1字型檔
  u8g2.drawStr(0, string_pos_y, context.c_str()); //自動換行
  string_pos_y += 16;
  return;
}

void SSD1306::send() {  //顯示暫存器內容
  u8g2.sendBuffer();
  return;
}

void SSD1306::clear() { //清除暫存器內容
  string_pos_y = 14;
  u8g2.clearBuffer();
  return;
}

void SSD1306::ensure() {
  clear_upload();
  clear_download();
  return;
}

void SSD1306::show_upload(int interval) {
  last_upload = millis();
  upload_interval = interval;
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.drawXBM(0, 0, 16, 16, gImage_socket_upload);
  u8g2.sendBuffer();
}

void SSD1306::clear_upload() {
  if (upload_interval < 0) return;
  if (millis() - last_upload <= upload_interval) return;
  upload_interval = -1;
  u8g2.setFontMode(1);
  u8g2.setDrawColor(0);
  u8g2.drawRBox(0, 0, 16, 16, 0);
  u8g2.sendBuffer();
}

void SSD1306::show_download(int interval) {
  last_download = millis();
  download_interval = interval;
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.drawXBM(16, 0, 16, 16, gImage_socket_download);
  u8g2.sendBuffer();
}

void SSD1306::clear_download() {
  if (download_interval < 0) return;
  if (millis() - last_download <= download_interval) return;
  download_interval = -1;
  u8g2.setFontMode(1);
  u8g2.setDrawColor(0);
  u8g2.drawRBox(16, 0, 16, 16, 0);
  u8g2.sendBuffer();
}

void SSD1306::draw_server(const unsigned char* image) {
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.drawXBM(96, 0, 16, 16, (const uint8_t*) image);
  u8g2.sendBuffer();
}

void SSD1306::draw_wifi(const unsigned char* image) {
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.drawXBM(112, 0, 16, 16, (const uint8_t*) image);
  u8g2.sendBuffer();
}

SSD1306 oled;
