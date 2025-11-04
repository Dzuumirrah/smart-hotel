#ifndef PROJECT_GALIH_H
#define PROJECT_GALIH_H

#if defined(PROJECT_GALIH_UNO)
  #include "project-galih-uno.cpp"
  void setup_galih() {
    setupProjectGalih2();
  }
  void loop_galih() {
    loopProjectGalih2();
  }

#elif defined(PROJECT_GALIH_ESP)
  #include "project-galih-esp.cpp"
  void setup_galih() {
    setupProjectGalih();
  }
  void loop_galih() {
    loopProjectGalih();
  }

#else
  #error "PROJECT_GALIH defined, but no sub-project is selected."
#endif
#endif