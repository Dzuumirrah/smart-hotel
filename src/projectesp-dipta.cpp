#ifdef ARDUINO_ARCH_ESP32   // <---- Tambahkan ini di paling atas

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define trig1 5
#define echo1 18
#define trig2 17
#define echo2 16
#define trig3 4
#define echo3 0

int jarak1, jarak2, jarak3;
int slotPenuh = 0;
int totalSlot = 3;

long bacaJarak(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long durasi = pulseIn(echoPin, HIGH, 30000);
  long jarak = durasi * 0.034 / 2;
  return jarak;
}

void setup_dipta() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo3, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Sistem Parkir");
  lcd.setCursor(0, 1);
  lcd.print("Hotel Basement");
  delay(2000);
  lcd.clear();
}

void loop_dipta() {
  jarak1 = bacaJarak(trig1, echo1);
  jarak2 = bacaJarak(trig2, echo2);
  jarak3 = bacaJarak(trig3, echo3);

  slotPenuh = 0;
  if (jarak1 > 0 && jarak1 < 10) slotPenuh++;
  if (jarak2 > 0 && jarak2 < 10) slotPenuh++;
  if (jarak3 > 0 && jarak3 < 10) slotPenuh++;

  int slotKosong = totalSlot - slotPenuh;

  lcd.setCursor(0, 0);
  lcd.print("Penuh: ");
  lcd.print(slotPenuh);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("Kosong: ");
  lcd.print(slotKosong);
  lcd.print("   ");

  Serial.println("=== Status Parkir ===");
  Serial.print("Slot 1: "); Serial.print(jarak1); Serial.println(" cm");
  Serial.print("Slot 2: "); Serial.print(jarak2); Serial.println(" cm");
  Serial.print("Slot 3: "); Serial.print(jarak3); Serial.println(" cm");
  Serial.print("Penuh: "); Serial.println(slotPenuh);
  Serial.print("Kosong: "); Serial.println(slotKosong);
  Serial.println("=====================");
  Serial.println();

  delay(1000);
}

#endif   // <---- Tutup di paling bawah
