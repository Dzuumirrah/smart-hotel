#include "module_motor.h"

static uint8_t _dirPin, _pwmPin;

void initMotor(uint8_t dirPin, uint8_t pwmPin) {
  _dirPin = dirPin;
  _pwmPin = pwmPin;
  pinMode(_dirPin, OUTPUT);
  pinMode(_pwmPin, OUTPUT);
  stopCurtain();
}

void openCurtain() {
  digitalWrite(_dirPin, HIGH);
  analogWrite(_pwmPin, 200);  // speed (0–255)
}

void closeCurtain() {
  digitalWrite(_dirPin, LOW);
  analogWrite(_pwmPin, 200);
}

void stopCurtain() {
  analogWrite(_pwmPin, 0);
}
