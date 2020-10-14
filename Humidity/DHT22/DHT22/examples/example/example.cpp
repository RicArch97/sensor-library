/*
    Example for DHT22 Sensor library.
    Created by Ricardo Steijn, Oktober 7, 2020.
    Last update on Oktober 14, 2020.
*/

#include "dht22.h"

DHT22 dht22(4);

void setup() {
    Serial.begin(115200);
    dht22.setup();
}

void loop() {
    delay(2000);
    Serial.println(dht22.getHumidity());
    Serial.println(dht22.getTemperature());
}