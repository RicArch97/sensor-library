/*
    GPIO reading & writing for DHT22 Sensor library.
    Created by Ricardo Steijn, Oktober 10, 2020.
    Last update on Oktober 13, 2020.
*/

#include "gpio.h"
#include "timer.h"

REG_SIZE readGpio(REG_SIZE pin) {
    // Read directly from the GPIO pins, otherwise use digitalRead.
#ifdef __AVR__
    // Arduino boards
    volatile uint8_t *portReg = portInputRegister(digitalPinToPort(pin));
    uint8_t bitMask = digitalPinToBitMask(pin);
    return *portReg & bitMask ? 1 : 0;
#endif

#ifdef ARDUINO_ARCH_ESP8266
    // ESP8266 Boards
    // pre defined registers (GPI for read input level)
    uint32_t bitMask = 1 << pin;
    return GPI & bitMask ? 1 : 0;
#endif

#ifdef ARDUINO_ARCH_ESP32
    // ESP32 Boards
    // GPIO numbers in range from 0 - 31
    if (pin > 0 && pin < 32) {
        return (GPIO.in >> pin) & 0x1 ? 1 : 0;
    }
#endif
    // use standard digitalRead
    return digitalRead(pin);
}

void writeGpio(REG_SIZE pin, REG_SIZE bit) {
    // Write directly to the GPIO pins, otherwise use digitalWrite.
#ifdef __AVR__
    // Arduino boards
    volatile uint8_t *portReg = portOutputRegister(digitalPinToPort(pin));
    uint8_t bitMask = digitalPinToBitMask(pin);
    if (bit == 1) *portReg &= ~bitMask;
    else if (bit == 0) *portReg |= bitMask;
    else return;
#endif

#ifdef ARDUINO_ARCH_ESP8266
    // ESP8266 Boards
    // pre defined registers (GPO for write output level, GPOC for high, GPOS for low)
    uint32_t bitMask = 1 << pin;
    if (bit == 1) GPOS = bitMask;
    else if (bit == 0) GPOC = bitMask;
    else return;
#endif

#ifdef ARDUINO_ARCH_ESP32
    // ESP32 Boards
    // GPIO numbers in range from 0 - 31
    if (pin > 0 && pin < 32) {
        if (bit == 1) GPIO.out_w1ts = (uint32_t)1 << pin;
        else if (bit == 0) GPIO.out_w1tc = (uint32_t)1 << pin;
        else return;
    }
#endif
    // use standard digitalWrite
    digitalWrite(pin, bit);
}

unsigned long checkGpio(REG_SIZE pin, REG_SIZE state, unsigned long timeout) {
    // Check if a pin is in a certain state (for a certain amount of time).
    // Set up a timer to the given timeout.
    Timer pulse(timeout);
    // measure the time after the GPIO was measured high.
    pulse.start();
    while(readGpio(pin) == state) {
        if (pulse.finished()) return 0;
    }
    return pulse.getElapsedTime();
}