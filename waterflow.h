#include "config.h"
#include "logger.h"

volatile int flow_frequency = 0;  // Measures flow meter pulses

void add_flow(void) { // Interrupt function
  flow_frequency++;
}; 
    
class WaterflowHandler {
  public:
    void init(void);
    double get(void);
    float last_value;
  private:
    unsigned int l_hour;           // Calculated litres/hour
    unsigned long currentTime, cloopTime;
    const int readpin = (int) PIN_WATERFLOW;
};

void WaterflowHandler::init (void) {
  pinMode(readpin ,INPUT);

  attachInterrupt(readpin, add_flow, RISING); // Setup Interrupt
  sei(); // Enable interrupt, see http://arduino.cc/en/Reference/attachInterrupt
  currentTime = millis();
  cloopTime = currentTime;

  return;
}

double WaterflowHandler::get (void) {
  cloopTime = currentTime;              // Updates cloopTime
  // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min. (Results in +/- 3% range)
  last_value = ((float) flow_frequency * 60 / 7.5 / 2); // (Pulse frequency x 60 min) / 7.5Q = flow rate in L/hour 
  flow_frequency = 0;                   // Reset Counter
  WATERFLOW_LOGD("%d L/hour", last_value);

  return last_value;
}
