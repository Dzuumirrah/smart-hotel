#pragma once

// --- STANDARD LIBRARIES ---
#include <Arduino.h>
#include <Keypad.h>

// --- BOARD-SPECIFIC LIBRARIES ---
#if defined(ARDUINO_ARCH_ESP32)
  #include <ESP32Servo.h>
#elif defined(ARDUINO_ARCH_AVR)
  #include <Servo.h>
#else
  #error "This board architecture is not supported!"
#endif


/*
  Initializes Serial and all the pins, then home in to floor 1
 */
void setup_fazla();

/*
  Checks keypad for new destinations
  and updates the lift's Finite State Machine (FSM) to process
  movement, door operations, and state changes.
 */
void loop_fazla();

/*
  Stops the lift  
*/
void stopLift();

/*
  Moves the lift up
*/
void moveLiftUp();

/*
  Moves the lift down
*/
void moveLiftDown();

/*
  Opens the lift door
*/
void openDoor();

/**
  Closes the lift door
 */
void closeDoor();

/*
  Obtains ultrasonic sensor reading
 */
long getDoorSensorDistance();

/*
  Check if door is blocked by using ultrasonic reading
*/
bool isDoorObstructed();

/*
  Updates the `currentFloor` variable by reading the micro limit switches.
*/
bool updateCurrentFloor();

/*
  Scans the destination queue to find the next target, have directional priority.
*/
int findNextTarget();

/*
  Checks for keypad input
 */
void checkKeypad();

/*
  The core Finite State Machine (FSM) for the lift.
  The lift's states are: (IDLE, MOVING_UP, MOVING_DOWN, SERVICING_FLOOR).
 */
void updateLiftFSM();
