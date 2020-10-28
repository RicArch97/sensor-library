# DHT22 Sensor Library

Library for the DHT22 sensor. Supports reading the temperature and humidity every 2 seconds & doing automatic calibration of the sensor (coming soon). The library is written based on the [Aosong Electronics datasheet](https://datasheetspdf.com/pdf/792211/Aosong/DHT22/1).

## Requirements

* Microcontroller
* DHT22 sensor
* Jumpwires (optional)
* USB Cable

The library uses the Arduino framework.

## Recommendation

This library was developed, and tested with:

### DHT22

* RobotDyn DHT22 board which contains the ASAIR AM2302 DHT22. (every DHT22 should work fine!)

### Microcontrollers

* Arduino Mega 2560 & Arduino Uno (AVR)
* NodeMCU ESP-12E (ESP8266)
* DoIt ESP32 DevKit V1 (ESP32)

For these microcontrollers, the library currently supports garuanteed direct GPIO reading and writing. This should also work on any other Arduino, ESP8266 & ESP32 boards, however I only own the microcontrollers listed above, so other boards were not tested.

Direct GPIO pins could be faster on slower boards since digitalRead could take a few microseconds which might interfere with the timings. There was no noticable difference between direct GPIO access and digitalRead on ESP32 however.

# Function list

#### DHT22(int pin)
> Create a new DHT22 object, provide the pin number on the microcontroller that the datapin of the DHT22 is connected to.

#### void start()
> Initializes pins and sets up timer.

#### float getTemperature()
> Returns the last saved temperature value read from the sensor. A timer will make sure readings are only requested every 2 seconds. If the 2 seconds has not passed yet, the function returns the last read value.

#### float getHumidity()
> Returns the last saved humidity value read from the sensor. A timer will make sure readings are only requested every 2 seconds. If the 2 seconds has not passed yet, the function returns the last read value.

#### void calibrate()
> COMING SOON!