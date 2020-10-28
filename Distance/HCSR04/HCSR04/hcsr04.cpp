/*
    HC-SR04 Sensor library.
    Created by Ricardo Steijn, Oktober 20, 2020.
    Last update on Oktober 28, 2020.

    This library supports sending a soundpulse and waiting for the echo, then calculating the distance based on the elapsed time.
    This library supports setting the sensor as only transmitter (sending pulses) or only receiver (receiving echos).
    This library supports a calibration tool to ensure correct readings.
*/

#include "hcsr04.h"

HCSR04::HCSR04(REG_SIZE trig_pin, REG_SIZE echo_pin) {
    // Initialize.
    this->trig_pin = trig_pin;
    this->echo_pin = echo_pin;
    this->bitMaskEcho = BITMASK(echo_pin);
    this->bitMaskTrig = BITMASK(trig_pin);
    this->trigOut = PORT_REG_OUT(trig_pin);
    this->echoIn = PORT_REG_IN(echo_pin);
}

void HCSR04::start() {
    // Set modes & start timing.
    pinMode(this->trig_pin, OUTPUT);
    pinMode(this->echo_pin, INPUT);
    this->lastRead = micros() - 60;
}

float HCSR04::getDistance(float temperature, float humidity) {
    // Return the distance.
    unsigned long currenttime = micros();
    if ((currenttime - this->lastRead) > 60) {
        measurePulse();
        this->lastRead = currenttime;
    }
    return calculateDistance(temperature, humidity);
}

unsigned long HCSR04::getDuration() {
    // Get the duration of a pulse.
    unsigned long currenttime = micros();
    if ((currenttime - this->lastRead) > 60) {
        measurePulse();
        this->lastRead = currenttime;
    }
    return this->duration;
}

void HCSR04::triggerPulse() {
    // Trigger a pulse.
    WRITE_GPIO_LOW(this->trigOut, this->bitMaskTrig);
    delayMicroseconds(2);
    WRITE_GPIO_HIGH(this->trigOut, this->bitMaskTrig);
    delayMicroseconds(10);
    WRITE_GPIO_LOW(this->trigOut, this->bitMaskTrig);
}

void HCSR04::setTransmitter(HCSR04 *transmitter) {
    // Set the an other HC-SR04 as transmitter, and this one as receiver.
    this->transmitter = transmitter;
}

void HCSR04::calibrate(float minValue, float maxValue) {
    // Calibration tool for HC-SR04.
}

void HCSR04::measurePulse() {
    // Measure how long it took for a pulse to be received.
    if (this->transmitter != NULL) {
        // Set ECHO to high without actually sending a pulse itself.
        // Only works when the transmitter transducer on the module is desoldered or taped off.
        triggerPulse();
        // Make sure the ECHO pin is HIGH.
        // Wait for the pulse to start.
	    while (READ_GPIO(this->echoIn, this->bitMaskEcho) != HIGH);
        // Trigger the pulse from the transmitter HC-SR04.
        // Measure how long it took for the pulse to send out.
        unsigned long triggerDuration = micros();
        this->transmitter->triggerPulse();
        // Measure how long the pin is high, and subtract the trigger duration.
        triggerDuration = micros() - triggerDuration;
        unsigned long time = checkGpio(this->echoIn, this->bitMaskEcho, HIGH);
        if (time == 0) return;  // timeout

        this->duration = time - triggerDuration;
    }
    else {
        // Trigger pulse. This will set ECHO to high.
        triggerPulse();
        // Meaure how long it takes for the pulse to come back.
        unsigned long time = checkGpio(this->echoIn, this->bitMaskEcho, HIGH);
        if (time == 0) return;  // timeout

        this->duration = time;
    }
}

float HCSR04::calculateDistance(float temperature, float humidity) {
    // Calcalute the distance based on elapsed time.
    /*
        Calculate the distance, temperature and humidity taken into account. 
        s(distance) = v(velocity) * t(time) * 0.5
        v(speed) = 331.4ms. This is the velocity of sound, but this depends on temperature & humidity.
        t(time) = to be measured. 
    */
    float soundVelocityMs = 331.4 + (0.6 * temperature) + (0.0124 * humidity);
    float soundVelocityCmUs = (soundVelocityMs * 100) / 1000000;
    // Duration is multiplied by 0.5 because the echo is send out and then back the the sensor.
    return (this->duration * 0.5) * soundVelocityCmUs;
}