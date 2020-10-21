/*
    GPIO reading & writing for DHT22 Sensor library.
    Created by Ricardo Steijn, Oktober 10, 2020.
    Last update on Oktober 21, 2020.
*/

#ifndef GPIO_H
#define GPIO_H

#include <Arduino.h>

#if defined(__AVR__)
// Most Arduino boards
#define REG_SIZE                            uint8_t
#define PORT_REG_IN(pin)                    (portInputRegister(digitalPinToPort(pin)))
#define PORT_REG_OUT(pin)                   (portOutputRegister(digitalPinToPort(pin)))  
#define BITMASK(pin)                        (digitalPinToBitMask(pin))
#define READ_GPIO(pin)                      (((*(PORT_REG_IN(pin))) & (BITMASK(pin))) ? 1 : 0)
#define WRITE_GPIO_LOW(pin)                 ((*(PORT_REG_OUT(pin))) &= ~(BITMASK(pin)))
#define WRITE_GPIO_HIGH(pin)                ((*(PORT_REG_OUT(pin))) |= (BITMASK(pin)))

#elif defined(ARDUINO_ARCH_ESP8266)
// ESP8266 has pre defined registers 
// GPI for read input level, GPO for write output level, GPOC for high, GPOS for low
#define REG_SIZE                            uint32_t
#define BITMASK(pin)                        (1 << pin)
#define READ_GPIO(pin)                      ((GPI & (BITMASK(pin))) ? 1 : 0)
#define WRITE_GPIO_LOW(pin)                 (GPOC = (BITMASK(pin)))
#define WRITE_GPIO_HIGH(pin)                (GPOS = (BITMASK(pin)))

#elif defined(ARDUINO_ARCH_ESP32)
// ESP32 also has pre defined registers
// GPIO.in for inputs, GPIO.out for outputs, with w1tc for 0 and w1ts for 1
#define REG_SIZE                            uint32_t
#define READ_GPIO(pin)                      (((GPIO.in >> (pin)) & 0x1))
#define WRITE_GPIO_LOW(pin)                 (GPIO.out_w1tc = ((uint32_t)1 << (pin)))
#define WRITE_GPIO_HIGH(pin)                (GPIO.out_w1ts = ((uint32_t)1 << (pin)))

#else
// Use standard digitalRead and digitalWrite.
#define REG_SIZE                            unsigned int
#define READ_GPIO(pin)                      digitalRead(pin)
#define WRITE_GPIO_LOW(pin)                 digitalWrite(pin, LOW)
#define WRITE_GPIO_HIGH(pin)                digitalWrite(pin, HIGH)
#endif

// Util methods.
unsigned long checkGpio(REG_SIZE pin, REG_SIZE state, unsigned long timeout = 100);

#endif