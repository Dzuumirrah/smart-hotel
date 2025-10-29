#pragma once

void initSensorPIR(int pin) {
  #if USE_SENSOR_PIR
    pinMode(pin, INPUT);
  #endif
}