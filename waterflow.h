class WaterflowHandler {
  public: 
    int readpin = (int) waterflowPin;
    int cutTime = 1000;
    int currentPulse;
    int numPulse =0;
    int startTime, duration;
    int lastpulse = 0;

    void get_waterflow(void);
    void init(void);
};

void WaterflowHandler::init() {
  pinMode(readpin ,INPUT);
  startTime = millis();
};

void WaterflowHandler::get_waterflow() {
  duration = millis() - startTime;
  currentPulse = digitalRead(readpin);
  if (lastpulse != currentPulse) {
    lastpulse = currentPulse;
    numPulse+= 1;
    
  }

  if(duration >= cutTime){
    Serial.println(numPulse);
    numPulse =0;
    startTime = millis();
  }
};
