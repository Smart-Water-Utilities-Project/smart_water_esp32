# ESP32 Client

## Get started 
A program run on ESP32 that can detect water flow, temperature and control the valve. \
Also can send data to database server via websocket.

## How to use 
1. Download whole project
2. Duplicate file ``config.h.example`` and rename it to ``config.h``
3. Finally open IDE and fill settings properly then flash!

## Division
| Filename    | Programer |
| ----------- | --------- |
| websocket.h | @YFHD-osu |
| waterflow.h | @William  |

## Used Libaray
- [ArduinoJson](https://arduinojson.org/?utm_source=meta&utm_medium=library.properties)
- [ArduinoWebsockets](https://github.com/gilmaimon/ArduinoWebsockets)
- [DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library)
- [OneWire](https://github.com/PaulStoffregen/OneWire)

## Hardware Specs
Tank Empty: ``24`` cm
Tank Full: ``14.5`` cm
Tank Bottom Area: ``16`` cm