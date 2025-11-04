#include "module_sensor_BH1750.h"

BH1750 lightMeter;

void initSensorBH1750(uint8_t sda, uint8_t scl) {
  #if defined(PROJECT_GALIH_ESP)
  Wire.begin(sda, scl);
  #endif
  #if defined(PROJECT_GALIH_UNO)
  Wire.begin();
  #endif
  lightMeter.begin();
}

float readLightLevel() {
  return lightMeter.readLightLevel();
}