#include <Arduino.h>
#include "project-control.h" // Include the main project controller

void setup() {
  #if defined(PROJECT_DIPTA)
    setup_dipta();
  #elif defined(PROJECT_FAZLA)
    setup_fazla();
  #elif defined(PROJECT_HILMAN)
    setup_hilman();
  #elif defined(PROJECT_GALIH)
    setup_galih();
  #else
    Serial.begin(115200);
    Serial.println("No project selected!");
  #endif
}

void loop() {
  #if defined(PROJECT_DIPTA)
    loop_dipta();
  #elif defined(PROJECT_FAZLA)
    loop_fazla();
  #elif defined(PROJECT_HILMAN)
    loop_hilman();
  #elif defined(PROJECT_GALIH)
    loop_galih();
  #else
    delay(1000);
  #endif
}
