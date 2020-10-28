# HC-SR04 Sensor Library

Library for the HC-SR04 ultrasonic distance sensor. Supports reading the distance to an object when facing an object,
also an experimental mode where the transmitter transducer of one HC-SR04 is taped off, and set as the receiver. It will then only listen to echo's send out by other HC-SR04's instead of it's own echo. However this is not very accurate yet and requires tweaking and calibration.

## Requirements

* Microcontroller
* 1 or more HC-SR04 sensors
* Jumpwires (optional)
* USB Cable

# Function list

#### HCSR04(int trig_pin, int echo_pin)
> Create a new HCSR04 object, provide the pin numbers of the trigger and echo pins on the HC-SR04.

#### void start()
> Initializes pins and sets up timer.

#### float getDistance()
> Returns the distance based on the last duration reading. A timer will make sure readings are only requested every 60 microseconds. If the 60 microseconds has not passed yet, the function returns the last read value.

#### float getDuration()
> Returns the last duration reading. A timer will make sure readings are only requested every 60 microseconds. If the 60 microseconds has not passed yet, the function returns the last read value.

#### void triggerPulse()
> Trigger a pulse manually from an other HC-SR04 sensor for example.

#### void setTransmitter(HCSR04 *transmitter)
> Set a HCSR04 object (an other HC-SR04 sensor) as the transmitter for echo's.

#### void calibrate()
> Not yet implemented.