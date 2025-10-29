#ifndef PROJECT_MERLIN_H
#define PROJECT_MERLIN_H

#include <Arduino.h>
#include <DHT.h>

/* 
   Sistem Deteksi Asap & Suhu - Merlin
   Kompatibel: ESP32 DevKit & Arduino UNO
*/

// ---------------------
// BOARD-SPECIFIC CONFIG
// ---------------------
#if defined(ARDUINO_ARCH_ESP32)
  #define DHTPIN 4          // GPIO4 untuk ESP32
  #define MQ2_PIN 34        // GPIO34 (ADC input only)
  #define BUZZER_PIN 5      // GPIO5
  #define BOARD_NAME "ESP32 DEVKIT"

#elif defined(ARDUINO_AVR_UNO)
  #define DHTPIN 7          // Pin digital 7 untuk DHT11 UNO
  #define MQ2_PIN A0        // Pin analog A0 untuk MQ2
  #define BUZZER_PIN 8      // Pin digital 8 untuk buzzer
  #define BOARD_NAME "ARDUINO UNO"

#else
  #error "Board not supported! Please use ESP32 DevKit or Arduino UNO."
#endif

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// ---------------------
// FUNGSI SETUP & LOOP
// ---------------------
void setup_merlin() {
  Serial.begin(115200);
  dht.begin();
  pinMode(MQ2_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("====================================");
  Serial.print("Sistem Deteksi Asap & Suhu - ");
  Serial.println(BOARD_NAME);
  Serial.println("====================================");
}

void loop_merlin() {
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();
  int mq2Value = analogRead(MQ2_PIN);

  Serial.print("Suhu: "); Serial.print(suhu); Serial.print(" °C | ");
  Serial.print("Kelembapan: "); Serial.print(kelembapan); Serial.print(" % | ");
  Serial.print("Asap: "); Serial.println(mq2Value);

  // Threshold dapat disesuaikan
  bool bahayaAsap = mq2Value > 500;  // Untuk UNO ADC max 1023
  bool bahayaSuhu = suhu > 35;

  if (bahayaAsap && bahayaSuhu) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("⚠️  BAHAYA! Asap dan suhu tinggi terdeteksi!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(2000);
}

#endif
