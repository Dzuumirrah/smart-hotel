#include <Arduino.h>
#include <stdint.h>
#include <modules/infrared/module_infrared.h>
#include <modules/led/module_led.h>
#include <modules/pir/module_pir.h>
#include <modules/serial/module_serial.h>
// ========================
// Pilih salah satu project dengan cara uncomment
// ========================
//#define PROJECT_1    //esp
#define PROJECT_2  //uno

// ========================
// Konfigurasi berdasarkan project yang dipilih
// ========================
#if defined(PROJECT_1)  // ESP32: PIR, Infrared, LED, Serial
  #define TARGET_ESP32
  #define USE_SENSOR_PIR        1
  #define USE_SENSOR_INFRARED   1
  #define USE_LED               1
  #define USE_SERIAL            1
  #define SENSOR_PIR_PIN        12    // GPIO 12 pada ESP32
  #define SENSOR_INFRARED_PIN   13    // GPIO 13 pada ESP32
  #define LED_PIN               2     // GPIO 2 pada ESP32
  #define SERIAL_BAUD           115200

#elif defined(PROJECT_2)  // UNO: PIR, Infrared, LED, Serial
  #define TARGET_UNO
  #define USE_SENSOR_PIR        1
  #define USE_SENSOR_INFRARED   1
  #define USE_LED               1
  #define USE_SERIAL            1
  #define SENSOR_PIR_PIN        2     // Digital pin 2 pada Arduino UNO
  #define SENSOR_INFRARED_PIN   3     // Digital pin 3 pada Arduino UNO
  #define LED_PIN               13    // Digital pin 13 pada Arduino UNO
  #define SERIAL_BAUD           9600
#endif

// ========================
// Pastikan salah satu project dipilih
// ========================
#if !defined(PROJECT_1) && !defined(PROJECT_2)
  #error "Pilih salah satu project: PROJECT_1 atau PROJECT_2"
#endif


// ========================
// Program utama
// ========================

void setup_farrel() {
  // Inisialisasi Serial
  #if USE_SERIAL
    initSerial(SERIAL_BAUD);
    delay(2000);
    Serial.println(F("=== System Booting ==="));
  #endif

  // Inisialisasi Sensor PIR
  #if USE_SENSOR_PIR
    initSensorPIR(SENSOR_PIR_PIN);
    #if USE_SERIAL
      Serial.println(F("PIR Sensor initialized"));
    #endif
  #endif

  // Inisialisasi Sensor Infrared
  #if USE_SENSOR_INFRARED
    initSensorInfrared(SENSOR_INFRARED_PIN);
    #if USE_SERIAL
      Serial.println(F("Infrared Sensor initialized"));
    #endif
  #endif

  // Inisialisasi LED
  #if USE_LED
    initLED(LED_PIN);
    #if USE_SERIAL
      Serial.println(F("LED initialized"));
    #endif
  #endif
}

void loop_farrel() {
  static unsigned long lastMotionTime = 0;    // waktu terakhir ada gerakan
  const unsigned long timeout = 300000;       // 5 menit = 300.000 ms
  static bool lampState = false;              // status lampu (false = mati)

  int pirState = digitalRead(SENSOR_PIR_PIN); // baca sensor PIR

  if (pirState == HIGH) {
    // Ada gerakan → orang masuk / masih di kamar
    lastMotionTime = millis();  // reset waktu
    if (!lampState) {
      digitalWrite(LED_PIN, HIGH);  // nyalakan lampu
      lampState = true;
      #if USE_SERIAL
        Serial.println(F("Gerakan terdeteksi! Lampu menyala."));
      #endif
    }
  } 
  else {
    // Tidak ada gerakan
    if (lampState && (millis() - lastMotionTime >= timeout)) {
      digitalWrite(LED_PIN, LOW);  // matikan lampu
      lampState = false;
      #if USE_SERIAL
        Serial.println(F("Tidak ada gerakan selama 5 menit. Lampu mati."));
      #endif
    }
  }

  delay(500); // jeda pembacaan sensor agar stabil
}