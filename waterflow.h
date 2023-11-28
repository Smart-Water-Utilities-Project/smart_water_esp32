#include "config.h"
#include "logger.h"

#ifndef waterflow_include
#define waterflow_include

class WaterflowHandler {
  public:
    void init(void);
    float get(void);
    float last_value;

    // Measures flow meter pulses
    static int flow_counter;

    // Interrupt function
    static void add_flow(void) {
      flow_counter += 1;
    };

  private:
    unsigned int l_hour;           // Calculated litres/hour
    unsigned long cloopTime;
    const int readpin = (int) PIN_WATERFLOW;
};

int WaterflowHandler::flow_counter = 0;

void WaterflowHandler::init(void) {
  pinMode(readpin ,INPUT);
  attachInterrupt(readpin, add_flow, RISING); // Setup Interrupt
  sei(); // Enable interrupt, see http://arduino.cc/en/Reference/attachInterrupt
  cloopTime = millis();

  return;
}

float WaterflowHandler::get(void) {
  /* 
    Consider to every request won't exactly step on 1000ms
    The code below will calcuate the precise frequency
  */
  float flow_frequency = (float) flow_counter / (millis()-cloopTime) * 1000;
  WATERFLOW_LOGD("count=%d, freq=%0.2f (%dms)", flow_counter, flow_frequency, (millis()-cloopTime));

  /*
    Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min. (Results in +/- 3% range)
    (Pulse frequency x 60 min) / 7.5Q = flow rate in L/hour, divide by 2 due to unknown reason
  */
  // last_value = (flow_frequency * 60 / 7.5);     // Old formula       ref: https://www.rajguruelectronics.com/Product/1668/YF-S201%2012%20Water%20Flow%20Hall%20Sensor.pdf
  // last_value = (flow_frequency * 60 / 7.5 / 2); // Strange formula   ref: self-test
  // last_value = flow_frequency * 7.285 + 2.3725; // Desmos formula    ref: http://www.mantech.co.za/datasheets/products/yf-s201_sea.pdf
  last_value = flow_frequency * 7.285;
  WATERFLOW_LOGD("%0.2f L/hour", last_value);

  flow_counter = 0;   // Reset Counter
  cloopTime = millis(); // Updates cloopTime

  return last_value;
}

#endif