// --- PROJECT HEADER ---
#include "project-merlin.h"

// --- GLOBAL OBJECT DEFINITIONS ---

/**
 * @brief Definition of the global DHT object.
 * @details Initializes the dht object using the pins defined in the header.
 */
DHT dht(DHTPIN, DHTTYPE);


// ---------------------
// FUNGSI SETUP & LOOP
// ---------------------

void setup_merlin() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial
  
  dht.begin();
  pinMode(MQ2_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Make sure buzzer is off

  Serial.println("====================================");
  Serial.print("Sistem Deteksi Asap & Suhu - ");
  Serial.println(BOARD_NAME);
  Serial.println("====================================");
}

void loop_merlin() {
  // Reading sensors
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();
  int mq2Value = analogRead(MQ2_PIN);

  // Check if readings failed (common with DHT11)
  if (isnan(suhu) || isnan(kelembapan)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  // Print values
  Serial.print("Suhu: "); Serial.print(suhu); Serial.print(" *C | ");
  Serial.print("Kelembapan: "); Serial.print(kelembapan); Serial.print(" % | ");
  Serial.print("Asap (Analog): "); Serial.println(mq2Value);

  // --- Alarm Logic ---
  // Thresholds can be adjusted as needed
  // Note: ESP32 ADC (default) is 0-4095, Uno is 0-1023.
  // A simple value check might need refinement, but 500 is a low-end
  // threshold that should work for detecting significant smoke on both.
  bool bahayaAsap = mq2Value > 500;  
  bool bahayaSuhu = suhu > 35.0;     // 35°C (95°F) is a reasonable alert temp

  if (bahayaAsap && bahayaSuhu) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("!!! BAHAYA! Asap dan suhu tinggi terdeteksi!");
  } else if (bahayaAsap) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("--- PERINGATAN! Asap terdeteksi!");
  } else if (bahayaSuhu) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("--- PERINGATAN! Suhu tinggi terdeteksi!");
  }
  else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(2000); // Wait 2 seconds between readings
}
