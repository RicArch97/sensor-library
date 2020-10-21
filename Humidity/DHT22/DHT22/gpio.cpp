/*
    GPIO util functions for DHT22 Sensor library.
    Created by Ricardo Steijn, Oktober 10, 2020.
    Last update on Oktober 13, 2020.
*/

#include "gpio.h"
#include "timer.h"

unsigned long checkGpio(REG_SIZE pin, REG_SIZE state, unsigned long timeout) {
    // Check if a pin is in a certain state (for a certain amount of time).
    // Set up a timer to the given timeout.
    Timer pulse(timeout);
    // measure the time after the GPIO was measured high.
    pulse.start();
    while(READ_GPIO(pin) == state) {
        if (pulse.finished()) return 0;
    }
    return pulse.getElapsedTime();
}