#ifdef ARDUINO_ARCH_AVR   

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Konfigurasi pin sensor ultrasonik
#define trig1 4
#define echo1 5
#define trig2 6
#define echo2 8
#define trig3 9
#define echo3 12

int jarak1, jarak2, jarak3;
int slotPenuh = 0;
int totalSlot = 3;

long bacaJarak(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long durasi = pulseIn(echoPin, HIGH);
  long jarak = durasi * 0.034 / 2;
  return jarak;
}

void setup_dipta() {
  lcd.init();
  lcd.backlight();

  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo3, INPUT);

  Serial.begin(9600);
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
  if (jarak1 < 10) slotPenuh++;
  if (jarak2 < 10) slotPenuh++;
  if (jarak3 < 10) slotPenuh++;

  int slotKosong = totalSlot - slotPenuh;

  lcd.setCursor(0, 0);
  lcd.print("Penuh: ");
  lcd.print(slotPenuh);
  lcd.print("  ");

  lcd.setCursor(0, 1);
  lcd.print("Kosong: ");
  lcd.print(slotKosong);
  lcd.print("  ");

  Serial.print("Slot 1: "); Serial.print(jarak1); Serial.print(" cm | ");
  Serial.print("Slot 2: "); Serial.print(jarak2); Serial.print(" cm | ");
  Serial.print("Slot 3: "); Serial.print(jarak3); Serial.println(" cm");

  delay(1000);
}

#endif
