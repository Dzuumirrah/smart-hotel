#pragma once
#include <Arduino.h>

void initMotor(uint8_t dirPin, uint8_t pwmPin);
void openCurtain();
void closeCurtain();
void stopCurtain();
