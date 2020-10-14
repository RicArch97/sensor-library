/*
    DHT22 Sensor library.
    Created by Ricardo Steijn (date)

    This library supports a very efficient temperature and humidity reading.
    This library supports an accurate calibration function.
*/

#include "dht22.h"
#include "gpio.h"

DHT22::DHT22(REG_SIZE pin) {
    // Initialize.
    this->dataPin = pin;
}

void DHT22::setup() {
    // setup the sensor
    pinMode(this->dataPin, INPUT_PULLUP);
    this->lastRead = millis() - 2000;
}

float DHT22::getTemperature() {
    // Return the last read temperature.
    uint32_t currenttime = millis();
    if ((currenttime - this->lastRead) > 2000) {
        readWire();
        this->lastRead = currenttime;
    }
    return this->temperature;
}

float DHT22::getHumidity() {
    // Return the last read humidity.
    uint32_t currenttime = millis();
    if ((currenttime - this->lastRead) > 2000) {
        readWire();
        this->lastRead = currenttime;
    }
    return this->humidity;
}

void DHT22::calibrate(float minValue, float maxValue) {
    // Calibrate the sensor to operate between given voltage levels.
}

void DHT22::readWire() {
    // Data communication with the DHT22 using "1-Wire" protocol.
    // Using Aosong Electronics DHT22 datasheet.
    uint8_t sensorData[5];
    //if (!readTimer.finished()) return;

    /*
    Reading from the wire
    Step 1: Set data line low for (at least) 1ms, and wait 20-40 us.
    Step 2: Sensor responds with a low signal for 80 us, then with a high level signal for 80 us.
    Step 3: Sensor sends 40 bits to the MCU, every bit's transmission starts with low voltage level for 50us.
            The high level signal duration respresents the bit, ~28 us = 0, ~70 us = 1
    */
    // Step 1
    pinMode(this->dataPin, OUTPUT);
    writeGpio(this->dataPin, 0);
    delayMicroseconds(1050);
    pinMode(this->dataPin, INPUT_PULLUP);
    delayMicroseconds(40);

    // Step 2
    checkGpio(this->dataPin, LOW);
    checkGpio(this->dataPin, HIGH);

    // Step 3
    // The micros() function disables interrupts while measuring time. 
    unsigned long measurements[40];
    for (int i = 0; i < 40; i++) {
        checkGpio(this->dataPin, LOW);
        measurements[i] = checkGpio(this->dataPin, HIGH);
    }

    // Check all the values.
    for (int i = 0; i < 40; i++) {
        // Timeout (0)
        if (measurements[i] == 0) {
            Serial.println("Failed to read from sensor");
            return;
        }
        // write bits to the byte array in storage. shift each iteraction.
        sensorData[i/8] <<= 1;
        // pulse should be ~70us, but this can be different for every microcontroller.
        if (measurements[i] > 50) sensorData[i/8] |= 1;
        // pulse should be ~28us, but this can be different for every microcontroller.
        else if (measurements[i] > 10 && measurements[i] < 40) continue;
        else return;
    }

    // Check if the sum matches, and if the value is 8 bits.
    // Parse the temperature and humidity from the data.
    if (sensorData[4] == ((sensorData[0] + sensorData[1] + sensorData[2] + sensorData[3]) & 0b11111111)) {
        // create a floating point value (16 bit) by combining int and decimal data.
        float hum = ((uint16_t)sensorData[0]) << 8 | sensorData[1];
        this->humidity = hum * 0.1;
        // unsigned bytes (negative values) range from -127 to 127
        float temp = ((uint16_t)(sensorData[2] & 0b01111111)) << 8 | sensorData[3];
        temp *= 0.1;
        // if bit 8 is high it must be positive
        if (sensorData[2] & 0b10000000) temp *= -1;
        this->temperature = temp;
    }
    else {
        Serial.println("Checksum failed");
        return;        
    }
}