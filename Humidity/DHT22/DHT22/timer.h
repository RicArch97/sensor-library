/*
  timer.cpp - Class to create and manage timers.
  Created by Ricardo Steijn, October 6, 2020.
*/

#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
public:
    Timer(unsigned long microseconds);
    void start();
    bool finished();
    bool isRunning();
    unsigned long getElapsedTime();
private:
    unsigned long timeToElapse;
    unsigned long startTime;
    bool isStarted = false;
};

#endif