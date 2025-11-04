#include "module_pir.h"

static uint8_t pirPin;

void initSensorPIR(uint8_t pin) {
  #if USE_SENSOR_PIR
    pinMode(pin, INPUT);
  #endif
}


bool readSensorPIR() {
  return digitalRead(pirPin);
}