#pragma once
#include <modules/infrared/module_infrared.h>
#include <modules/led/module_led.h>
#include <modules/pir/module_pir.h>
#include <modules/serial/module_serial.h>

// Deklarasi fungsi inisialisasi dari modul-modul
void initSerial(int baud);
void initSensorPIR(int pin);
void initSensorInfrared(int pin);
void initLED(int pin);