#if defined(PROJECT_GALIH_UNO)
#include "project-galih-uno.h"

void setupProjectGalih2() {
  Serial.begin(SERIAL_BAUD);
  initMotor(MOTOR_DIR_PIN, MOTOR_PWM_PIN);
  initSensorPIR(PIR_PIN);
  initSensorBH1750(SENSOR_BH1750_SDA, SENSOR_BH1750_SCL);

  Serial.println("Project Galih 2 initialized.");
}

void loopProjectGalih2() {
  bool motion = readSensorPIR();
  float lux = readLightLevel();

  Serial.print("PIR: "); Serial.print(motion);
  Serial.print(" | Light: "); Serial.println(lux);

  if (motion && lux < 100) {
    openCurtain();  // misal motor buka gorden
  } else {
    closeCurtain(); // atau stopCurtain();
  }

  delay(500);
}
#endif