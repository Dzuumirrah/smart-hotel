#pragma once

void initSerial(int baud) {
  #if USE_SERIAL
    Serial.begin(baud);
  #endif
}