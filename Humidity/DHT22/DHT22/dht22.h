/*
    DHT22 Sensor library.
    Created by Ricardo Steijn, Oktober 7, 2020.
    Last update on Oktober 13, 2020.

    This library supports a very efficient temperature and humidity reading.
    This library supports an accurate calibration function.
*/

#ifndef DHT22_H
#define DHT22_H

#include <Arduino.h>
#include "gpio.h"

class DHT22 {
public:
    DHT22(REG_SIZE pin);
    void setup();
    float getTemperature();
    float getHumidity();
    void calibrate(float minValue, float maxValue);
private:
    REG_SIZE dataPin;
    float temperature;
    float humidity;
    uint32_t lastRead;
    void readWire();
};

#endif