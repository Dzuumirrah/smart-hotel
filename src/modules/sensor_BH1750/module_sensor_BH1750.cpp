#include "module_sensor_BH1750.h"

BH1750 lightMeter;

void initSensorBH1750(uint8_t sda, uint8_t scl) {
  Wire.begin(sda, scl);
  lightMeter.begin();
}

float readLightLevel() {
  return lightMeter.readLightLevel();
}
