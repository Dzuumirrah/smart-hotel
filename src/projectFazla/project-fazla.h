#pragma once // Modern alternative to header guards

// --- REQUIRED LIBRARIES ---
#include <Arduino.h>
#include <Keypad.h>
#include <ESP32Servo.h>

// --- CONSTANTS ---
const int NUM_FLOORS = 3; // You have 3 limit switches
const long DOOR_OPEN_TIME_MS = 3000; // 3 seconds
const int DOOR_OBSTRUCT_DIST_CM = 10; // 10cm for ultrasonic sensor


// DC Motor Driver (e.g., L298N)
const int MOTOR_PIN_UP = 26;   // Pin to turn motor to go UP
const int MOTOR_PIN_DOWN = 25; // Pin to turn motor to go DOWN

// Limit Switches (3 switches for 3 floors)
const int LIMIT_SWITCH_FLOOR_1 = 33;
const int LIMIT_SWITCH_FLOOR_2 = 32;
const int LIMIT_SWITCH_FLOOR_3 = 35;
const int limitSwitchPins[NUM_FLOORS] = {LIMIT_SWITCH_FLOOR_1, LIMIT_SWITCH_FLOOR_2, LIMIT_SWITCH_FLOOR_3};

// Servo (Door)
const int SERVO_PIN = 13;
const int SERVO_ANGLE_OPEN = 90;
const int SERVO_ANGLE_CLOSED = 0;

// Ultrasonic Sensor (Door Sensor)
const int ULTRASONIC_TRIG_PIN = 12;
const int ULTRASONIC_ECHO_PIN = 14;

// Keypad (4x3)
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 3;
char keypadKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'} 
};
byte keypadRowPins[KEYPAD_ROWS] = {19, 18, 5, 17};
byte keypadColPins[KEYPAD_COLS] = {16, 4, 2};


// --- GLOBAL OBJECTS ---
Servo doorServo;
Keypad customKeypad = Keypad(makeKeymap(keypadKeys), keypadRowPins, keypadColPins, KEYPAD_ROWS, KEYPAD_COLS);

// --- LIFT STATE VARIABLES ---

enum class LiftState {
  IDLE,
  MOVING_UP,
  MOVING_DOWN,
  SERVICING_FLOOR // Door is open
};

LiftState currentLiftState = LiftState::IDLE; 
int currentFloor = 1;
LiftState directionPreference = LiftState::MOVING_UP; 
unsigned long doorTimer = 0;

bool destinations[NUM_FLOORS + 1] = {false};

void stopLift() {
  digitalWrite(MOTOR_PIN_UP, LOW);
  digitalWrite(MOTOR_PIN_DOWN, LOW);
  Serial.println("Lift stopped.");
}

void moveLiftUp() {
  digitalWrite(MOTOR_PIN_DOWN, LOW);
  digitalWrite(MOTOR_PIN_UP, HIGH);
  Serial.println("Lift moving UP.");
}

void moveLiftDown() {
  digitalWrite(MOTOR_PIN_UP, LOW);
  digitalWrite(MOTOR_PIN_DOWN, HIGH);
  Serial.println("Lift moving DOWN.");
}

void openDoor() {
  doorServo.write(SERVO_ANGLE_OPEN);
  Serial.printf("Door opening at floor %d\n", currentFloor);
}

void closeDoor() {
  doorServo.write(SERVO_ANGLE_CLOSED);
  Serial.println("Door closing.");
}

long getDoorSensorDistance() {
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

bool isDoorObstructed() {
  return getDoorSensorDistance() < DOOR_OBSTRUCT_DIST_CM;
}

bool updateCurrentFloor() {
  int oldFloor = currentFloor;
  for (int i = 0; i < NUM_FLOORS; i++) {
    if (digitalRead(limitSwitchPins[i]) == LOW) {
      int floorNum = i + 1; 
      if (currentFloor != floorNum) {
          currentFloor = floorNum;
          Serial.printf("Arrived at floor %d\n", currentFloor);
          return true; // Floor has changed
      }
      return false; 
    }
  }
  return false;
}

int findNextTarget() {
  if (destinations[currentFloor]) {
    return currentFloor;
  }

  // Scan Up
  if (directionPreference == LiftState::MOVING_UP) {
    for (int f = currentFloor + 1; f <= NUM_FLOORS; f++) {
      if (destinations[f]) return f;
    }
    //Scan Down
    for (int f = currentFloor - 1; f >= 1; f--) {
      if (destinations[f]) {
        directionPreference = LiftState::MOVING_DOWN; // Change preference
        return f;
      }
    }
  } 
  else { // directionPreference == LiftState::MOVING_DOWN
    // Scan Down
    for (int f = currentFloor - 1; f >= 1; f--) {
      if (destinations[f]) return f;
    }
    // Scan Up
    for (int f = currentFloor + 1; f <= NUM_FLOORS; f++) {
      if (destinations[f]) {
        directionPreference = LiftState::MOVING_UP; // Change preference
        return f;
      }
    }
  }

  return -1; // No requests
}

void checkKeypad() {
  char key = customKeypad.getKey();
  if (key) {
    Serial.printf("Key pressed: %c\n", key);
    if (key >= '1' && key <= '0' + NUM_FLOORS) {
      int floorPressed = key - '0'; // Convert char to int
      destinations[floorPressed] = true;
      Serial.printf("Request added for floor %d\n", floorPressed);
    }
  }
}

void updateLiftFSM() {
  
  updateCurrentFloor();

  int nextTarget = -1;


  switch (currentLiftState) {
    case LiftState::IDLE:
      nextTarget = findNextTarget();

      if (nextTarget == -1) {
        return; // No requests.
      }

      if (nextTarget > currentFloor) {
        currentLiftState = LiftState::MOVING_UP;
        moveLiftUp();
      } else if (nextTarget < currentFloor) {
        currentLiftState = LiftState::MOVING_DOWN;
        moveLiftDown();
      } else { // nextTarget == currentFloor
        destinations[currentFloor] = false; 
        currentLiftState = LiftState::SERVICING_FLOOR;
        openDoor();
        doorTimer = millis(); 
      }
      break;

    case LiftState::MOVING_UP:
      if (destinations[currentFloor]) {
        stopLift();
        destinations[currentFloor] = false; 
        currentLiftState = LiftState::SERVICING_FLOOR;
        openDoor();
        doorTimer = millis(); 
      }
      else if (currentFloor == NUM_FLOORS) {
        stopLift();
        currentLiftState = LiftState::IDLE; // Re-evaluate
        directionPreference = LiftState::MOVING_DOWN; 
      }
      break;

    case LiftState::MOVING_DOWN:
      if (destinations[currentFloor]) {
        stopLift();
        destinations[currentFloor] = false; 
        currentLiftState = LiftState::SERVICING_FLOOR;
        openDoor();
        doorTimer = millis(); 
      }
      else if (currentFloor == 1) {
        stopLift();
        currentLiftState = LiftState::IDLE; // Re-evaluate
        directionPreference = LiftState::MOVING_UP; 
      }
      break;

    case LiftState::SERVICING_FLOOR:
      if (isDoorObstructed()) {
        doorTimer = millis(); // Reset the timer if blocked
      }

      if (millis() - doorTimer > DOOR_OPEN_TIME_MS) {
        closeDoor();
        currentLiftState = LiftState::IDLE; // Go back to idle
        Serial.println("Door closed, returning to IDLE.");
      }
      break;
  }
}



void setup_fazla() {
  Serial.begin(115200);
  Serial.println("Booting PROJECT_FAZLA: Lift Control");

  pinMode(MOTOR_PIN_UP, OUTPUT);
  pinMode(MOTOR_PIN_DOWN, OUTPUT);
  stopLift(); 

  for (int i = 0; i < NUM_FLOORS; i++) {
    pinMode(limitSwitchPins[i], INPUT_PULLUP);
  }

  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  doorServo.attach(SERVO_PIN);
  closeDoor();

  Serial.println("Finding current floor...");
  while (updateCurrentFloor() == false) {
    moveLiftDown();
    delay(100); 
  }
  stopLift();
  Serial.printf("Lift homed. Current floor: %d\n", currentFloor);
  
  // --- FIX 1 (continued): Use scoped enum value ---
  currentLiftState = LiftState::IDLE;
}

void loop_fazla() {
  checkKeypad();
  updateLiftFSM();
  delay(50);
}

