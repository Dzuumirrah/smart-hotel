// --- PROJECT HEADER ---
#include "project-fazla.h" 

// --- CONSTANTS ---
const int NUM_FLOORS = 3;
const long DOOR_OPEN_TIME_MS = 3000; 
const int DOOR_OBSTRUCT_DIST_CM = 10; 

// --- BOARD-SPECIFIC PIN DEFINITIONS ---

#if defined(ARDUINO_ARCH_ESP32)
  // --- ESP32 PINS ---
  const int MOTOR_PIN_UP = 26;   // Pin to turn motor to go UP
  const int MOTOR_PIN_DOWN = 25; // Pin to turn motor to go DOWN

  // Micro Limit Switches (3 switches for 3 floors)
  const int LIMIT_SWITCH_FLOOR_1 = 33;
  const int LIMIT_SWITCH_FLOOR_2 = 32;
  const int LIMIT_SWITCH_FLOOR_3 = 35;

  // Servo (Door)
  const int SERVO_PIN = 13;

  // Ultrasonic Sensor (Door Sensor)
  const int ULTRASONIC_TRIG_PIN = 12;
  const int ULTRASONIC_ECHO_PIN = 14;

  // Keypad (4x3)
  const byte KEYPAD_ROWS = 4;
  const byte KEYPAD_COLS = 3;
  byte keypadRowPins[KEYPAD_ROWS] = {19, 18, 5, 17};
  byte keypadColPins[KEYPAD_COLS] = {16, 4, 2};

#elif defined(ARDUINO_ARCH_AVR)
  // --- ARDUINO UNO PINS ---
  const int MOTOR_PIN_UP = 4;   // Pin to turn motor to go UP (e.g., L298N IN1)
  const int MOTOR_PIN_DOWN = 5; // Pin to turn motor to go DOWN (e.g., L298N IN2)

  // Micro Limit Switches (3 switches for 3 floors)
  const int LIMIT_SWITCH_FLOOR_1 = A0; // Use Analog pins as Digital
  const int LIMIT_SWITCH_FLOOR_2 = A1;
  const int LIMIT_SWITCH_FLOOR_3 = A2;

  // Servo (Door)
  const int SERVO_PIN = 9; 

  // Ultrasonic Sensor (Door Sensor)
  const int ULTRASONIC_TRIG_PIN = 12;
  const int ULTRASONIC_ECHO_PIN = 13;

  // Keypad (4x3)
  const byte KEYPAD_ROWS = 4;
  const byte KEYPAD_COLS = 3;
  byte keypadRowPins[KEYPAD_ROWS] = {8, 7, 6, 3};
  byte keypadColPins[KEYPAD_COLS] = {2, 10, 11};
#endif


const int limitSwitchPins[NUM_FLOORS] = {LIMIT_SWITCH_FLOOR_1, LIMIT_SWITCH_FLOOR_2, LIMIT_SWITCH_FLOOR_3};
const int SERVO_ANGLE_OPEN = 90;
const int SERVO_ANGLE_CLOSED = 0;
char keypadKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'} 
};


Servo doorServo;
Keypad customKeypad = Keypad(makeKeymap(keypadKeys), keypadRowPins, keypadColPins, KEYPAD_ROWS, KEYPAD_COLS);


enum class LiftState {
  IDLE,
  MOVING_UP,
  MOVING_DOWN,
  SERVICING_FLOOR 
};

LiftState currentLiftState = LiftState::IDLE; 
int currentFloor = 1; 
LiftState directionPreference = LiftState::MOVING_UP; 
unsigned long doorTimer = 0;

bool destinations[NUM_FLOORS + 1] = {false, false, false, false};


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
  #if defined(ARDUINO_ARCH_ESP32)
    Serial.printf("Door opening at floor %d\n", currentFloor);
  #else
    Serial.print("Door opening at floor ");
    Serial.println(currentFloor);
  #endif
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

  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH, 25000); 
  
  if (duration == 0) {
      return 999; // Return a large value if timeout occurs
  }
  
  return duration * 0.034 / 2;
}

bool isDoorObstructed() {
  return getDoorSensorDistance() < DOOR_OBSTRUCT_DIST_CM;
}

bool updateCurrentFloor() {
  // int oldFloor = currentFloor; // Unused variable
  for (int i = 0; i < NUM_FLOORS; i++) {
    // Limit switches are INPUT_PULLUP, so LOW means pressed
    if (digitalRead(limitSwitchPins[i]) == LOW) {
      int floorNum = i + 1; // Array index 0 is floor 1
      if (currentFloor != floorNum) {
          currentFloor = floorNum;
          #if defined(ARDUINO_ARCH_ESP32)
            Serial.printf("Arrived at floor %d\n", currentFloor);
          #else
            Serial.print("Arrived at floor ");
            Serial.println(currentFloor);
          #endif
          return true; // Floor has changed
      }
      return false; // Still on the same floor
    }
  }
  return false; // Between floors
}

int findNextTarget() {
  // Always service the current floor first if requested
  if (destinations[currentFloor]) {
    return currentFloor;
  }

  // --- Scan based on preference ---
  if (directionPreference == LiftState::MOVING_UP) {
    // 1. Scan UP
    for (int f = currentFloor + 1; f <= NUM_FLOORS; f++) {
      if (destinations[f]) return f;
    }
    // 2. Scan DOWN
    for (int f = currentFloor - 1; f >= 1; f--) {
      if (destinations[f]) {
        directionPreference = LiftState::MOVING_DOWN; // Change preference
        return f;
      }
    }
  } 
  else { // directionPreference == LiftState::MOVING_DOWN
    // 1. Scan DOWN
    for (int f = currentFloor - 1; f >= 1; f--) {
      if (destinations[f]) return f;
    }
    // 2. Scan UP
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
    #if defined(ARDUINO_ARCH_ESP32)
      Serial.printf("Key pressed: %c\n", key);
    #else
      Serial.print("Key pressed: ");
      Serial.println(key);
    #endif
    if (key >= '1' && key <= '0' + NUM_FLOORS) {
      int floorPressed = key - '0'; // Convert char to int
      destinations[floorPressed] = true;
      #if defined(ARDUINO_ARCH_ESP32)
        Serial.printf("Request added for floor %d\n", floorPressed);
      #else
        Serial.print("Request added for floor ");
        Serial.println(floorPressed);
      #endif
    }
  }
}

void updateLiftFSM() {
  bool floorChanged = updateCurrentFloor();

  int nextTarget = -1;

  switch (currentLiftState) {
    case LiftState::IDLE:
      nextTarget = findNextTarget();

      if (nextTarget == -1) {
        return; // No requests. Stay IDLE.
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
      if (floorChanged && destinations[currentFloor]) {
        stopLift();
        destinations[currentFloor] = false; 
        currentLiftState = LiftState::SERVICING_FLOOR;
        openDoor();
        doorTimer = millis(); 
      }
      else if (currentFloor == NUM_FLOORS) { // Reached the top
        stopLift();
        currentLiftState = LiftState::IDLE; // Re-evaluate
        directionPreference = LiftState::MOVING_DOWN; 
      }
      break;

    case LiftState::MOVING_DOWN:
      if (floorChanged && destinations[currentFloor]) {
        stopLift();
        destinations[currentFloor] = false; 
        currentLiftState = LiftState::SERVICING_FLOOR;
        openDoor();
        doorTimer = millis(); 
      }
      else if (currentFloor == 1) { // Reached the bottom
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
  while (!Serial);
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

  Serial.println("Finding current floor (homing)...");
  // Home the lift: move down until we hit floor 1
  bool homed = false;
  for (int i = 0; i < NUM_FLOORS; i++) {
      if (digitalRead(limitSwitchPins[i]) == LOW) {
          currentFloor = i + 1;
          #if defined(ARDUINO_ARCH_ESP32)
            Serial.printf("Already at a floor: %d\n", currentFloor);
          #else
            Serial.print("Already at a floor: ");
            Serial.println(currentFloor);
          #endif
          homed = true;
          break;
      }
  }
  
  if (!homed) {
      moveLiftDown();
      while (digitalRead(LIMIT_SWITCH_FLOOR_1) == HIGH) {
        // Wait until floor 1 switch is pressed
        delay(10);
      }
  }

  stopLift();
  currentFloor = 1;
  #if defined(ARDUINO_ARCH_ESP32)
    Serial.printf("Lift homed. Current floor: %d\n", currentFloor);
  #else
    Serial.print("Lift homed. Current floor: ");
    Serial.println(currentFloor);
  #endif
  
  currentLiftState = LiftState::IDLE;
}

void loop_fazla() {
  checkKeypad();
  updateLiftFSM();
  delay(50); 
}

