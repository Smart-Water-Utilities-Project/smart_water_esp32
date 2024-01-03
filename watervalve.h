#include "oled.h"
#include "config.h"

#ifndef WATERVALVE_INCLUDE
#define WATERVALVE_INCLUDE

class WaterValve {
  public:
    void init() {
      pinMode(PIN_WATERVALVE, OUTPUT);
      digitalWrite(PIN_WATERBUMP, HIGH);
    };

    void set(bool state) {
      if (state) { digitalWrite(PIN_WATERVALVE, HIGH); }
      else { digitalWrite(PIN_WATERVALVE, LOW); }
      oled.drawVavle(state);

      return;
    };
    
};

WaterValve watervalve;

#endif