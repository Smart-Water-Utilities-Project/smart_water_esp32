#include "config.h"
#include "logger.h"

int flow_frequency = 0;  // Measures flow meter pulses

// Interrupt function
void add_flow(void) { flow_frequency++; };

class WaterflowHandler {
  public:
    void init(void);
    float get(void);
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

float WaterflowHandler::get (void) {
  cloopTime = currentTime;              // Updates cloopTime
  // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min. (Results in +/- 3% range)
  // (Pulse frequency x 60 min) / 7.5Q = flow rate in L/hour, divide by 2 due to unknown reason
  last_value = ((float) flow_frequency * 60 / 7.5 / 2); 
  flow_frequency = 0;                   // Reset Counter
  WATERFLOW_LOGD("%d L/hour", last_value);

  return last_value;
}
