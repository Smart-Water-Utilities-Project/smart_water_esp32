#include <OneWire.h>
#include <DallasTemperature.h>

class Temperature {
  public:
    float get(void);
    void init(void);
  
  private:
    OneWire oneWire;
    DallasTemperature DS18B20;

};

void Temperature::init(void) {
  const int read_pin = (int) pin_temperature;
  oneWire = OneWire(read_pin);
  DS18B20 = DallasTemperature(&oneWire);
}

float Temperature::get(void) {
  DS18B20.requestTemperatures(); 
  return DS18B20.getTempCByIndex(0);
}