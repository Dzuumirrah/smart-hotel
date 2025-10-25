#pragma once
#include <Arduino.h>
#include "modules/sensor_BH1750/module_sensor_BH1750.h"
#include "modules/sensor_PIR/module_sensor_PIR.h"
#include "modules/actuator_motor/module_motor.h"

// =========================
// Konfigurasi Umum Project Galih 2 (Arduino Uno)
// =========================
#define SERIAL_BAUD 9600

// Pin Motor
#define MOTOR_DIR_PIN 5
#define MOTOR_PWM_PIN 6

// Pin Sensor (contoh)
#define PIR_PIN 7
#define BH1750_ADDR 0x23

// =========================
// Fungsi setup dan loop
// =========================
void setupProjectGalih2();
void loopProjectGalih2();
