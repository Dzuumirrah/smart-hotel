#pragma once // Header guard

// --- STANDARD LIBRARIES ---
#include <Arduino.h>
#include <DHT.h>

/* Sistem Deteksi Asap & Suhu - Merlin
   Kompatibel: ESP32 DevKit & Arduino UNO
*/

#if defined(ARDUINO_ARCH_ESP32)
  #define DHTPIN 4          // GPIO4 untuk ESP32
  #define MQ2_PIN 34        // GPIO34 (ADC input only)
  #define BUZZER_PIN 5      // GPIO5
  #define BOARD_NAME "ESP32 DEVKIT"

#elif defined(ARDUINO_ARCH_AVR)
  #define DHTPIN 7          // Pin digital 7 untuk DHT11 UNO
  #define MQ2_PIN A0        // Pin analog A0 untuk MQ2
  #define BUZZER_PIN 8      // Pin digital 8 untuk buzzer
  #define BOARD_NAME "ARDUINO UNO"

#else
  #error "Board not supported! Please use ESP32 DevKit or Arduino UNO."
#endif

#define DHTTYPE DHT11

extern DHT dht;

/*
 Initializes Serial, pin forDHT sensor, MQ2 sensor (INPUT) and Buzzer (OUTPUT),

 */
void setup_merlin();

/*
 reads temperature fromDHT11,
 reads the analog value from the smoke sensor.
 buzz if their values are beyond threshold.
 */
void loop_merlin();
