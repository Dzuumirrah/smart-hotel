#pragma once 

#include <Arduino.h>
#include <stdint.h>


#include <modules/infrared/module_infrared.h>
#include <modules/led/module_led.h>
#include <modules/pir/module_pir.h>
#include <modules/serial/module_serial.h>

/*
   Sistem Hemat Listrik Otomatis - Farrel
   Logika:
   - Lampu (LED) akan menyala jika sensor PIR mendeteksi gerakan.
   - Jika tidak ada gerakan terdeteksi selama 5 menit, lampu akan mati.
   - Sensor inframerah tidak digunakan di loop utama, tapi diinisialisasi.
*/

// ========================
// Konfigurasi Pin & Board
// ========================


#if defined(ARDUINO_ARCH_ESP32)
  #define SENSOR_PIR_PIN        12    // GPIO 12 pada ESP32
  #define SENSOR_INFRARED_PIN   13    // GPIO 13 pada ESP32
  #define LED_PIN               2     // GPIO 2 pada ESP32
  #define SERIAL_BAUD           115200
  #define BOARD_NAME "ESP32"

#elif defined(ARDUINO_ARCH_AVR)
  #define SENSOR_PIR_PIN        2     // Digital pin 2 pada Arduino UNO
  #define SENSOR_INFRARED_PIN   3     // Digital pin 3 pada Arduino UNO
  #define LED_PIN               13    // Digital pin 13 pada Arduino UNO (built-in LED)
  #define SERIAL_BAUD           9600
  #define BOARD_NAME "Arduino UNO"

#else
  #error "Board not supported! Please use ESP32 or Arduino UNO."
#endif

/*
    Initializes Serial, pin for PIR sensor, Infrared sensor, and LED
 */
void setup_farrel();

/**
  1. Reads the PIR sensor.
  2. If motion is detected, turn the LED ON and reset a timer.
  3. If no motion is detected for the duration of a timeout (5 mins),
  turn the LED OFF.
 */
void loop_farrel();
