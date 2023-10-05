#include "icons.h"
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

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
    
    void show_upload(int);
    void clear_upload(bool force=false);

    void show_download(int);
    void clear_download(bool force=false);

    void draw_wifi(const unsigned char*);
    void draw_server(const unsigned char*);

    void wifi_animation(int);
    void clear_area(int, int, int, int);

    void set_waterflow(unsigned int);
    
  private:
    int string_pos_y = 14;
    
    int upload_interval;
    int download_interval;
    
    unsigned long last_upload;
    unsigned long last_download;

    bool wifi_animation_state = false;
    unsigned long last_wifi_animation = 0;
};

void SSD1306::init(void) {
  u8g2::begin();
  u8g2::enableUTF8Print();  // 啟動 UTF8 支援
  u8g2::drawXBM(96, 0, 16, 16, gImage_server_blank);
  u8g2::drawXBM(112, 0, 16, 16, gImage_wifi_blank);
  u8g2::sendBuffer();
}

void SSD1306::println (String context) {
  u8g2::setFont(u8g2_font_unifont_t_chinese1); // 使用 chinese1字型檔
  u8g2::drawStr(0, string_pos_y, context.c_str()); //自動換行
  string_pos_y += 16;
  return;
}

void SSD1306::send() {  //顯示暫存器內容
  u8g2::sendBuffer();
  return;
}

void SSD1306::clear() { //清除暫存器內容
  string_pos_y = 14;
  u8g2::clearBuffer();
  return;
}

void SSD1306::show_upload(int interval) {
  last_upload = millis();
  upload_interval = interval;
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::drawXBM(80, 0, 8, 16, gImage_socket_upload);
  u8g2::sendBuffer();
}

void SSD1306::clear_upload(bool force) {
  if (!force) {
    if (upload_interval < 0) return;
    if (millis() - last_upload <= upload_interval) return;
  }
  
  upload_interval = -1;
  clear_area(80, 0, 8, 16);
  u8g2::sendBuffer();
}

void SSD1306::show_download(int interval) {
  last_download = millis();
  download_interval = interval;
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::drawXBM(88, 0, 8, 16, gImage_socket_download);
  u8g2::sendBuffer();
}

void SSD1306::clear_download(bool force) {
  if (!force) {
    if (download_interval < 0) return;
    if (millis() - last_download <= download_interval) return;
  }
  download_interval = -1;
  clear_area(88, 0, 8, 16);
  u8g2::sendBuffer();
}

void SSD1306::draw_server(const unsigned char* image) {
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::drawXBM(96, 0, 16, 16, (const uint8_t*) image);
  u8g2::sendBuffer();
}

void SSD1306::draw_wifi(const unsigned char* image) {
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::drawXBM(112, 0, 16, 16, (const uint8_t*) image);
  u8g2::sendBuffer();
}

void SSD1306::wifi_animation(int interval) {
  if (millis() - last_wifi_animation < interval) return;
  wifi_animation_state = !wifi_animation_state;
  draw_wifi(wifi_animation_state ? gImage_wifi_connecting : gImage_wifi_blank);
  last_wifi_animation = millis();
}

void SSD1306::clear_area(int x, int y, int w, int h) {
  u8g2::setFontMode(1);
  u8g2::setDrawColor(0);
  u8g2::drawRBox(x, y, w, h, 0);
}

void SSD1306::set_waterflow(unsigned int context) {
  clear_area(0, 16, 128, 16);
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::setFont(u8g2_font_unifont_t_chinese1);
  u8g2::setCursor(0, 32);
  u8g2::print(String("流量: "+String(context)+"升/時").c_str());

  u8g2::sendBuffer();
}

void SSD1306::ensure() {
  clear_upload();
  clear_download();
  return;
}

SSD1306 oled;
