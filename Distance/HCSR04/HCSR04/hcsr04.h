/*
    HC-SR04 Sensor library.
    Created by Ricardo Steijn, Oktober 20, 2020.

    This library supports sending a soundpulse and waiting for the echo, then calculating the distance based on the elapsed time.
    This library supports setting the sensor as only transmitter (sending pulses) or only receiver (receiving echos).
    This library supports a calibration tool to ensure correct readings.
*/

#ifndef HCSR04_H
#define HCSR04_H

#include <Arduino.h>
#include "gpio.h"

class HCSR04 {
public:
    HCSR04(REG_SIZE trig_pin, REG_SIZE echo_pin);
    void start();
    float getDistance(float temperature = 20, float humidity = 50);
    unsigned long getDuration();
    void triggerPulse();
    void setTransmitter(HCSR04 *transmitter);
    void calibrate(float minValue, float maxValue);
private:
    REG_SIZE trig_pin, echo_pin;
    REG_SIZE bitMaskEcho, bitMaskTrig;
    volatile REG_SIZE *trigOut;
    volatile REG_SIZE *echoIn; 
    unsigned long duration;
    unsigned long lastRead;
    HCSR04 *transmitter = NULL;
    void measurePulse();
    float calculateDistance(float temperature, float humidity);
};

#endif