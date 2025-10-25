#include "project-galih2.h"

void setupProjectGalih2() {
  Serial.begin(SERIAL_BAUD);
  pinMode(MOTOR_DIR_PIN, OUTPUT);
  pinMode(MOTOR_PWM_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  
  // inisialisasi sensor BH1750 misalnya
}

void loopProjectGalih2() {
  // logika utama loop
}
