#pragma once


void initSensorInfrared(int pin) {
  #if USE_SENSOR_INFRARED
    pinMode(pin, INPUT);
  #endif
}