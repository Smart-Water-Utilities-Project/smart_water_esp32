#include <OneWire.h>
#include <DallasTemperature.h>

class Temperature {
  public:
    float get(void);
    void init(void);

    float last_value;
  
  private:
    OneWire oneWire;
    DallasTemperature DS18B20;

};

void Temperature::init(void) {
  oneWire = OneWire((int) PIN_TEMPERATURE);
  DS18B20 = DallasTemperature(&oneWire);
}

float Temperature::get(void) {
  DS18B20.requestTemperatures(); 
  last_value = DS18B20.getTempCByIndex(0);
  return last_value;
}