#pragma once


void initLED(int pin) {
  #if USE_LED
    pinMode(pin, OUTPUT);
  #endif
}