#ifndef PROJECT_CONTROL_H
#define PROJECT_CONTROL_H

#if defined(PROJECT_HILMAN)
#include "project-hilman.h"
#include "control.cpp"

#elif defined(PROJECT_MERLIN)
#include "project-merlin.h"

#elif defined(PROJECT_GALIH)
#include "project-galih.h"

    #define TARGET_ESP32
    #define USE_SENSOR_BH1750  1
    #define USE_SENSOR_PIR     1
    #define USE_IO             1
    #define USE_SERIAL         1

    #define SENSOR_BH1750_SDA  21
    #define SENSOR_BH1750_SCL  22
    #define SENSOR_PIR_PIN     15

    #define MOTOR_DIR_PIN      18
    #define MOTOR_PWM_PIN      19
    #define LIMIT_OPEN_PIN     32
    #define LIMIT_CLOSE_PIN    33

    #define SERIAL_BAUD        115200

#elif defined(PROJECT_GALIH2)
#include "project-galih2.h"
    #define TARGET_UNO
    #define USE_SENSOR_BH1750  1
    #define USE_SENSOR_PIR     1
    #define USE_IO             1
    #define USE_SERIAL         1

    // Pin untuk Arduino Uno
    #define SENSOR_BH1750_SDA  A4
    #define SENSOR_BH1750_SCL  A5
    #define SENSOR_PIR_PIN     7

    #define MOTOR_DIR_PIN      8
    #define MOTOR_PWM_PIN      9
    #define LIMIT_OPEN_PIN     10
    #define LIMIT_CLOSE_PIN    11

    #define SERIAL_BAUD        9600


#elif defined(PROJECT_FARREL)
#include "project-farrel.h"

#else
#error "No project defined. Please define one of PROJECT_HILMAN, PROJECT_MERLIN, PROJECT_GALIH, or PROJECT_FARREL."
#endif

#endif // PROJECT_CONTROL_H
