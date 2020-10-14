/*
  timer.cpp - Class to create and manage timers.
  Created by Ricardo Steijn, October 6, 2020.
  Last update on Oktober 12, 2020.
*/

#include "timer.h"

Timer::Timer(unsigned long microseconds) {
    // Create a new timer.
    this->timeToElapse = microseconds;
}

void Timer::start() {
    // start the timer.
    this->startTime = micros();
    this->isStarted = true;
}

bool Timer::finished() {
    // return if the timer has finished.
    if (!(this->isStarted)) return false;
    if ((micros() - this->startTime) >= this->timeToElapse) {
        this->isStarted = false;
        return true;
    }
    return false;
}
unsigned long Timer::getElapsedTime() {
    // get the elapsed time for the timer.
    return micros() - this->startTime;
}

bool Timer::isRunning() {
    // return if the timer is currently running.
    return this->isStarted;
}