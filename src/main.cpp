#include <Arduino.h>
#include "project-control.h" // Include the main project controller

// Pilih project yang mau di-compile
// Project defines are now in platformio.ini

// The project is now selected in platformio.ini using "build_flags"
// for the chosen environment.

// The project-control.h file will now include your project-fazla.h,
// which contains your setup_fazla() and loop_fazla() functions.

void setup() {
  // Call the setup function for the selected project
  #if defined(PROJECT_FAZLA)
    setup_fazla();
  #elif defined(PROJECT_HILMAN)
    setup_hilman();
  #elif defined(PROJECT_GALIH)
    setup_galih();
  #elif defined(PROJECT_MERLIN)
    setup_merlin();
  #else
    // Default setup behavior
    Serial.begin(115200);
    Serial.println("No project selected.");
  #endif
}

void loop() {
  // Call the loop function for the selected project
  #if defined(PROJECT_FAZLA)
    loop_fazla();
  #elif defined(PROJECT_HILMAN)
    loop_hilman();
  #elif defined(PROJECT_GALIH)
    loop_galih();
  #elif defined(PROJECT_MERLIN)
    loop_merlin();
  #else
    delay(1000);
  #endif
}