#pragma once
#include <Arduino.h>

// ================================
// Project: Galih - Smart Curtain
// ================================

#define TARGET_ESP32
#define USE_SENSOR_BH1750   1
#define USE_SENSOR_PIR      1
#define USE_IO              1
#define USE_SERIAL          1

// Pin configuration
#define SENSOR_BH1750_SDA   21
#define SENSOR_BH1750_SCL   22
#define SENSOR_PIR_PIN      15

#define MOTOR_DIR_PIN       18
#define MOTOR_PWM_PIN       19
#define LIMIT_OPEN_PIN      32
#define LIMIT_CLOSE_PIN     33

#define SERIAL_BAUD         115200

// Function declarations
void setupProjectGalih();
void loopProjectGalih();
