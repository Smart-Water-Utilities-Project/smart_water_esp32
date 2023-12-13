#include "config.h"
#include "logger.h"

#ifndef waterlevel_include
#define waterlevel_include

class WaterLevel {
  public:
    void init();
    void ensure();
    void setTarget(float);
    float dynamicRange = 0.1;
    float targetPercent = 0.9;
    float lastCm, lastInch;

  private:
    int duration;
    int getDuration();
    float convertCm(int);
    float convertInch(int);
    
};

int WaterLevel::getDuration() { 
  // 傳送脈波讓超音波感應器開始測距
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);     // 給 Trig 高電位，持續 10微秒
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  return pulseIn(PIN_ECHO, HIGH);   // 將收到高電位時的時間回傳
}

void WaterLevel::init() {
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_TRIG, OUTPUT); // 定義超音波感應器的輸入及輸出
  return;
}

float WaterLevel::convertCm(int duration) {
  return (duration/2) / 29.1;
}

float WaterLevel::convertInch(int duration) { 
  return (duration/2) / 74;
}

void WaterLevel::ensure() {
  duration = getDuration();
  lastCm = convertCm(duration);
  lastInch = convertInch(duration);
  LOGD("WATERLEVL", "Current distance: %0.2fcm", lastCm);
  return;
}

void WaterLevel::setTarget(float target) {
  targetPercent = (target+dynamicRange > 1) ? target-dynamicRange : target;
  targetPercent = (target-dynamicRange < 0) ? dynamicRange : target;
  return;
}

#endif