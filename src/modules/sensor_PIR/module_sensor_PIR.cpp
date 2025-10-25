#include "module_sensor_PIR.h"

static uint8_t pirPin;

void initSensorPIR(uint8_t pin) {
  pirPin = pin;
  pinMode(pirPin, INPUT);
}

bool readSensorPIR() {
  return digitalRead(pirPin);
}
