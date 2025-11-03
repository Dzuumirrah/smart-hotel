#ifndef PROJECT_DIPTA_H
#define PROJECT_DIPTA_H

#if defined(PROJECT_DIPTA_UNO)
  #include "projectarduino-dipta.cpp"
  void setup_dipta() {
    setup_dipta_uno();
  }
  void loop_dipta() {
    loop_dipta_uno();
  }

#elif defined(PROJECT_DIPTA_ESP)
  #include "projectesp-dipta.cpp"
  void setup_dipta() {
    setup_dipta_esp();
  }
  void loop_dipta() {
    loop_dipta_esp();
  }

#else
  #error "PROJECT_DIPTA defined, but no sub-project is selected."
#endif
#endif