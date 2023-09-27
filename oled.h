#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

const unsigned char gImage_server_connected[32] = { /* 0X20,0X01,0X10,0X00,0X10,0X00, */
0X00,0X00,0XFE,0X7F,0X02,0X40,0X02,0X50,0X02,0X48,0X02,0X44,0X0A,0X42,0X12,0X41,
0XA2,0X40,0X42,0X40,0X02,0X40,0XFE,0X7F,0X80,0X01,0X80,0X01,0XF0,0X0F,0X00,0X00,
};

const unsigned char gImage_server_not_connected[32] = { /* 0X20,0X01,0X10,0X00,0X10,0X00, */
0X00,0X00,0XFE,0X7F,0X02,0X40,0X12,0X48,0X22,0X44,0X42,0X42,0X82,0X41,0X42,0X42,
0X22,0X44,0X12,0X48,0X02,0X40,0XFE,0X7F,0X80,0X01,0X80,0X01,0XF0,0X0F,0X00,0X00,
};

const unsigned char gImage_wifi_connect[32] = { /* 0X20,0X01,0X10,0X00,0X10,0X00, */
0X00,0X00,0X1E,0X0F,0X02,0X08,0XBA,0X0B,0X0A,0X0A,0XE8,0X02,0XE0,0X00,0XE8,0X00,
0X4A,0X40,0X5A,0X20,0X42,0X11,0X5E,0X0A,0X40,0X04,0X40,0X00,0XF8,0X03,0X00,0X00,
};

const unsigned char gImage_wifi_not_connect[32] = { /* 0X20,0X01,0X10,0X00,0X10,0X00, */
0X00,0X00,0X1E,0X0F,0X02,0X08,0XBA,0X0B,0X0A,0X0A,0XE8,0X02,0XE0,0X00,0XE8,0X00,
0X4A,0X44,0X5A,0X28,0X42,0X10,0X5E,0X28,0X40,0X44,0X40,0X00,0XF8,0X03,0X00,0X00,
};

const unsigned char gImage_wifi_connecting_0[32] = { /* 0X20,0X01,0X10,0X00,0X10,0X00, */
0X00,0X00,0X1E,0X0F,0X02,0X08,0XBA,0X0B,0X0A,0X0A,0XE8,0X02,0XE0,0X00,0XE8,0X08,
0X4A,0X14,0X5A,0X2A,0X42,0X55,0X5E,0X2A,0X40,0X14,0X40,0X08,0XF8,0X03,0X00,0X00,
};

const unsigned char gImage_wifi_connecting_1[32] = { /* 0X20,0X01,0X10,0X00,0X10,0X00, */
0X00,0X00,0X1E,0X0F,0X02,0X08,0XBA,0X0B,0X0A,0X0A,0XE8,0X02,0XE0,0X00,0XE8,0X00,
0X4A,0X00,0X5A,0X00,0X42,0X00,0X5E,0X00,0X40,0X00,0X40,0X00,0XF8,0X03,0X00,0X00,
};


U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

class SSD1306 {
  public:
    void init();
    void sendBuffer();
    void clearBuffer();
    void println(String);
    
  private:
    int yPos = 14;
};

void SSD1306::init() {
  u8g2.begin();
  u8g2.enableUTF8Print();  // 啟動 UTF8 支援
  u8g2.drawXBM(96, 0, 16, 16, gImage_server_not_connected);
  u8g2.drawXBM(112, 0, 16, 16, gImage_wifi_not_connect);
  u8g2.sendBuffer();
  return;
}

void SSD1306::println (String context) {
  u8g2.setFont(u8g2_font_unifont_t_chinese1); // 使用 chinese1字型檔
  u8g2.setCursor(0, yPos); //自動換行
  u8g2.print(context);
  yPos += 16;
  return;
}

void SSD1306::sendBuffer () {  //顯示暫存器內容
  u8g2.sendBuffer();
  return;
}

void SSD1306::clearBuffer () { //清除暫存器內容
  yPos = 14;
  u8g2.clearBuffer();
  return;
}
