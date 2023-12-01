#include "config.h"
#include <OneWire.h>
#include <DallasTemperature.h>

class Temperature {
  public:
    void init(void);
    float ensure(void);

    float last_value;
  
  private:
    OneWire oneWire;
    DallasTemperature DS18B20;

};

void Temperature::init(void) {
  oneWire = OneWire((int) PIN_TEMPERATURE);
  DS18B20 = DallasTemperature(&oneWire);
}

float Temperature::ensure(void) {
  DS18B20.requestTemperatures(); 
  last_value = DS18B20.getTempCByIndex(0);
  return last_value;
}