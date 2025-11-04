#include "project-farrel.h"


void setup_farrel() {
  // Inisialisasi Serial
  initSerial(SERIAL_BAUD);
  delay(1000); // Give serial time to initialize
  Serial.println(F("================================="));
  Serial.print(F("Booting Project Farrel on "));
  Serial.println(BOARD_NAME);
  Serial.println(F("================================="));

  // Inisialisasi Sensor PIR
  initSensorPIR(SENSOR_PIR_PIN);
  Serial.println(F("PIR Sensor initialized"));

  // Inisialisasi Sensor Infrared
  initSensorInfrared(SENSOR_INFRARED_PIN);
  Serial.println(F("Infrared Sensor initialized"));

  // Inisialisasi LED
  initLED(LED_PIN);
  Serial.println(F("LED initialized"));

  Serial.println(F("System ready. Waiting for motion..."));
}

void loop_farrel() {
  // Static variables persist their values between loop calls
  static unsigned long lastMotionTime = 0;    // Waktu terakhir ada gerakan
  static bool lampState = false;              // Status lampu (false = mati)
  
  const unsigned long TIMEOUT_MS = 300000;    // 5 menit = 300,000 ms
  // --- Untuk testing, gunakan 5 detik ---
  // const unsigned long TIMEOUT_MS = 5000;      // 5 detik

  int pirState = digitalRead(SENSOR_PIR_PIN); // Baca sensor PIR

  if (pirState == HIGH) {
    // Ada gerakan → orang masuk / masih di kamar
    lastMotionTime = millis();  // Reset timer
    if (!lampState) {
      // Nyalakan lampu HANYA jika sebelumnya mati
      digitalWrite(LED_PIN, HIGH);
      lampState = true;
      Serial.println(F("Gerakan terdeteksi! Lampu menyala."));
    }
  } 
  else {
    // Tidak ada gerakan
    if (lampState && (millis() - lastMotionTime >= TIMEOUT_MS)) {
      // Matikan lampu HANYA jika sebelumnya nyala dan timer sudah habis
      digitalWrite(LED_PIN, LOW);
      lampState = false;
      Serial.println(F("Tidak ada gerakan selama 5 menit. Lampu mati."));
    }
  }

  delay(500); // Jeda pembacaan sensor agar stabil
}
