#include "oled.h"
#include "config.h"
#include "waterlevel.h"

class WaterBump {
  public:
    void init();
    void ensure(WaterLevel*);
    
};

void WaterBump::init() {
  pinMode(PIN_WATERBUMP, OUTPUT);
}

void WaterBump::ensure(WaterLevel* waterlevel) {
  int _state = digitalRead(PIN_WATERBUMP);
  float _range = waterlevel->dynamicRange;
  float _target = waterlevel->targetPercent;
  float _percent = (MAX_HEIFHT - (waterlevel->lastCm - OFFSET_HEIGHT)) / MAX_HEIFHT;
  Serial.printf("State=%d, range=%0.2f, target=%0.2f, percent=%0.2f\n", _state, _range, _target, _percent);
  
  // If bump is currently on and currnet percent is more than target + range
  if (_state && _percent > _target+_range) {
    digitalWrite(PIN_WATERBUMP, LOW);
    oled.drawVavle(LOW);
  } 
  // If bump is currently off and currnet percent is lower than target - range
  else if (!_state && _percent < _target-_range) {
    digitalWrite(PIN_WATERBUMP, HIGH);
    oled.drawVavle(HIGH);
  }

  return;
}