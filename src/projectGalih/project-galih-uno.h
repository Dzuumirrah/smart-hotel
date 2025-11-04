#pragma once
#include <Arduino.h>
#include "modules/actuator_motor/module_motor.h"
#include "modules/sensor_BH1750/module_sensor_BH1750.h"
#include "modules/pir/module_pir.h"

// =========================
// Konfigurasi Umum Project Galih 2 (Arduino Uno)
// =========================
#define SERIAL_BAUD 9600

// Pin Motor
#define MOTOR_DIR_PIN 5
#define MOTOR_PWM_PIN 6

// Pin Sensor
#define PIR_PIN 7
#define BH1750_ADDR 0x23
#define SENSOR_BH1750_SDA A4
#define SENSOR_BH1750_SCL A5

// =========================
// Fungsi setup dan loop
// =========================
void setupProjectGalih2();
void loopProjectGalih2();
