#include "hcsr04.h"

HCSR04 receiver(8, 9);
HCSR04 transmitter(3, 4);

void setup() {
  Serial.begin(115200);

  // Set this HCSR04 as the transmitter.
  receiver.setTransmitter(&transmitter);

  // Setup pins, start timer.
  transmitter.start();
  receiver.start();
}

void loop() {
  // Time of pulse arrival between the two.
  Serial.println(receiver.getDuration());
}