/*
    GPIO reading & writing for DHT22 Sensor library.
    Created by Ricardo Steijn, Oktober 10, 2020.
    Last update on Oktober 28, 2020.
*/

#ifndef GPIO_H
#define GPIO_H

#include <Arduino.h>

#if defined(__AVR__)
// Most Arduino boards
#define REG_SIZE                                uint8_t
#define PORT_REG_IN(pin)                        (portInputRegister(digitalPinToPort(pin)))
#define PORT_REG_OUT(pin)                       (portOutputRegister(digitalPinToPort(pin)))  
#define BITMASK(pin)                            (digitalPinToBitMask(pin))
#define READ_GPIO(portRegIn, bitMask)           (((*(portRegIn)) & (bitMask)) ? 1 : 0)        
#define WRITE_GPIO_LOW(portRegOut, bitMask)     ((*(portRegOut)) &= ~(bitMask))
#define WRITE_GPIO_HIGH(portRegOut, bitMask)    ((*(portRegOut)) |= (bitMask))

#elif defined(ARDUINO_ARCH_ESP8266)
// ESP8266 has pre defined registers 
// GPI for read input level, GPO for write output level, GPOC for high, GPOS for low
#define REG_SIZE                                uint32_t
#define PORT_REG_IN(pin)                        (0)
#define PORT_REG_OUT(pin)                       (0)
#define BITMASK(pin)                            (1 << pin)
#define READ_GPIO(portRegIn, bitMask)           ((GPI & (bitMask)) ? 1 : 0)
#define WRITE_GPIO_LOW(portRegOut, bitMask)     (GPOC = (bitMask))
#define WRITE_GPIO_HIGH(portRegOut, bitMask)    (GPOS = (bitMask))

#elif defined(ARDUINO_ARCH_ESP32)
// ESP32 also has pre defined registers
// GPIO.in for inputs, GPIO.out for outputs, with w1tc for 0 and w1ts for 1
#define REG_SIZE                                uint32_t
#define PORT_REG_IN(pin)                        ((volatile uint32_t*)(((((pin) > 31) ? 1 : 0)) ? GPIO_IN1_REG : GPIO_IN_REG))
#define PORT_REG_OUT(pin)                       ((volatile uint32_t*)(((((pin) > 31) ? 1 : 0)) ? GPIO_OUT1_REG : GPIO_OUT_REG))
#define BITMASK(pin)                            (1UL << (((pin) > 31) ? ((pin) - 32) : (pin)))
#define READ_GPIO(portRegIn, bitMask)           (((*(portRegIn)) & (bitMask)) ? 1 : 0)  
#define WRITE_GPIO_LOW(portRegOut, bitMask)     ((*(portRegOut)) &= ~(bitMask))
#define WRITE_GPIO_HIGH(portRegOut, bitMask)    ((*(portRegOut)) |= (bitMask))

#else
// Use standard digitalRead and digitalWrite.
#define REG_SIZE                                int
#define PORT_REG_IN(pin)                        (0)
#define PORT_REG_OUT(pin)                       (0)
#define BITMASK(pin)                            (pin)
#define READ_GPIO(portRegIn, bitMask)           digitalRead((bitMask))
#define WRITE_GPIO_LOW(portRegOut, bitMask)     digitalWrite((bitMask), LOW)
#define WRITE_GPIO_HIGH(portRegOut, bitMask)    digitalWrite((bitMask), HIGH)
#endif

// Util methods.
static inline __attribute__((always_inline))
unsigned long checkGpio(volatile REG_SIZE *portReg, REG_SIZE bitMask, REG_SIZE state, unsigned long timeout = 1000000L) {
    // Check if a pin is in a certain state (for a certain amount of time).
    // Basically the same as pulseInLong but then with own direct read function.
    unsigned long startMicros = micros();
    
    // wait for the pulse to start
	while (READ_GPIO(portReg, bitMask) != state) {
        if (micros() - startMicros > timeout) return 0;
    }
    
    unsigned long start = micros();
    // wait for the pulse to stop
    while (READ_GPIO(portReg, bitMask) == state) {
        if (micros() - startMicros > timeout) return 0;
    }
    
    return micros() - start;
}

#endif