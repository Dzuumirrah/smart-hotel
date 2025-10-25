#pragma once
#include <Wire.h>
#include <BH1750.h>

void initSensorBH1750(uint8_t sda, uint8_t scl);
float readLightLevel();
