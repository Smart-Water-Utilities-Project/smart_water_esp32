class WaterflowHandler {
  public:
    void init(void);
    double get_waterflow(void);

  private:
    volatile int  flow_frequency;  // Measures flow meter pulses
    unsigned int  l_hour;          // Calculated litres/hour                      
    unsigned char flowmeter = 12;  // Flow Meter Pin number
    unsigned long currentTime, cloopTime;
    const int readpin = (int) pin_waterflow;
    const int upload_peroid = (int) delay_waterflow;
    unsigned long startTime; 

    void add_flow(); // Interruot function
};

void WaterflowHandler::init() {
  pinMode(readpin ,INPUT);
  startTime = millis();
  attachInterrupt(readpin, this->add_flow, RISING); // Setup Interrupt
  sei(); // Enable interrupt, see http://arduino.cc/en/Reference/attachInterrupt
  currentTime = millis();
  cloopTime = currentTime;
};

double WaterflowHandler::get_waterflow() {
  cloopTime = currentTime;              // Updates cloopTime
  // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min. (Results in +/- 3% range)
  l_hour = ((double) flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flow rate in L/hour 
  flow_frequency = 0;                   // Reset Counter
  Serial.print(l_hour, DEC);            // Print litres/hour
  Serial.println(" L/hour");

  oled.set_waterflow(String(l_hour));
  return l_hour;
};

void WaterflowHandler::add_flow() {flow_frequency++;}
