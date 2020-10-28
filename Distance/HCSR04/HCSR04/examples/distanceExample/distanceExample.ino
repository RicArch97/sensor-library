#include "hcsr04.h"

HCSR04 distance(8, 9);

void setup() {
  Serial.begin(115200);

  // Setup pins, start timer.
  distance.start();
}

void loop() {
  // Distance to object.
  Serial.println(distance.getDistance());
}