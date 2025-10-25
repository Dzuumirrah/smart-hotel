#include <Arduino.h>

// Pilih project yang mau di-compile
// #define PROJECT_HILMAN
// #define PROJECT_MERLIN
#define PROJECT_GALIH
// #define PROJECT_GALIH2
// #define PROJECT_FARREL
// #define PROJECT_UNKNOWN

#include "project-control.h"

void setup() {
  #if defined(PROJECT_GALIH)
  setupProjectGalih();
  #elif defined(PROJECT_GALIH2)
  setupProjectGalih2();
  #endif
}

void loop() {
  #if defined(PROJECT_GALIH)
  loopProjectGalih();
  #elif defined(PROJECT_GALIH2)
  loopProjectGalih2();
  #endif
}
