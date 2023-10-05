volatile int flow_frequency = 0;  // Measures flow meter pulses

void add_flow(void) { // Interrupt function
  flow_frequency++;
}; 
    
class WaterflowHandler {
  public:
    void init(void) {
      pinMode(readpin ,INPUT);

      attachInterrupt(digitalPinToInterrupt(readpin), add_flow, RISING); // Setup Interrupt
      sei(); // Enable interrupt, see http://arduino.cc/en/Reference/attachInterrupt
      currentTime = millis();
      cloopTime = currentTime;
    };
    
    double get_waterflow(void) {
      cloopTime = currentTime;              // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min. (Results in +/- 3% range)
      l_hour = ((double) flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flow rate in L/hour 
      flow_frequency = 0;                   // Reset Counter
      Serial.println(String(l_hour) + " L/hour");            // Print litres/hour
    
      oled.set_waterflow(l_hour);
      return l_hour;
    };

  private:
    unsigned int l_hour;           // Calculated litres/hour
    unsigned char flowmeter = 12;  // Flow Meter Pin number
    unsigned long currentTime, cloopTime;
    const int readpin = (int) pin_waterflow;
};
