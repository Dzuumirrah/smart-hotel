#include "project-galih.h"
#include "modules/actuator_motor/module_motor.h"
#include "modules/sensor_BH1750/module_sensor_BH1750.h"
#include "modules/sensor_PIR/module_sensor_PIR.h"

void setupProjectGalih() {
  #if USE_SERIAL
  Serial.begin(SERIAL_BAUD);
  delay(1000);
  Serial.println(F("=== Smart Curtain (Galih) ==="));
  #endif

  #if USE_SENSOR_BH1750
  initSensorBH1750(SENSOR_BH1750_SDA, SENSOR_BH1750_SCL);
  Serial.println(F("BH1750 initialized"));
  #endif

  #if USE_SENSOR_PIR
  initSensorPIR(SENSOR_PIR_PIN);
  Serial.println(F("PIR initialized"));
  #endif

  initMotor(MOTOR_DIR_PIN, MOTOR_PWM_PIN);
  Serial.println(F("Motor initialized"));
}

void loopProjectGalih() {
  float lux = readLightLevel();
  bool motion = readSensorPIR();

  Serial.print("Lux: "); Serial.print(lux);
  Serial.print(" | Motion: "); Serial.println(motion);

  if (lux > 500 && motion)
    openCurtain();
  else if (lux < 100)
    closeCurtain();
  else
    stopCurtain();

  delay(500);
}
