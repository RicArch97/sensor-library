/*
    GPIO reading & writing for DHT22 Sensor library.
    Created by Ricardo Steijn, Oktober 10, 2020.
    Last update on Oktober 13, 2020.
*/

#ifndef GPIO_H
#define GPIO_H

#include <Arduino.h>

#if defined(__AVR__)
#define REG_SIZE uint8_t

#elif defined(ARDUINO_ARCH_ESP8266)
#define REG_SIZE uint32_t

#elif defined(ARDUINO_ARCH_ESP32)
#define REG_SIZE uint32_t

#endif

REG_SIZE readGpio(REG_SIZE pin);
void writeGpio(REG_SIZE pin, REG_SIZE bit);
unsigned long checkGpio(REG_SIZE pin, REG_SIZE state, unsigned long timeout = 100);

#endif