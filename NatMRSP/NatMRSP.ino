#include "IR_remote.h"
#include "keymap.h"
#include <Servo.h>
#include <Ultrasonic.h>

// Define ultrasonic sensor pins
#define FTRIGGER_PIN 12 // white wire
#define FECHO_PIN 13 // yellow wire

// Left sensor
#define LTRIGGER_PIN 8 // orange wire
#define LECHO_PIN 9 // yellow wire

// Right sensor
#define RTRIGGER_PIN 11 // brown+
#define RECHO_PIN 7  // violet wire

// Global variables
IRremote ir(3);

int state = 0;
const int STATE_RC = 0;
const int STATE_LF = 1;
const int STATE_OA = 2;

Servo myservo;
int pos = 40; // Set the initial position to 40

void setup() {
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  Serial.begin(9600);
  myservo.attach(10); 
  myservo.write(pos); // Initialize the servo to the neutral position
}

void loop() {
  int keycode = ir.getIrKey(ir.getCode(), 1);

  // Change state
  if (keycode == IR_KEYCODE_1) {
    state = STATE_RC;
  } else if (keycode == IR_KEYCODE_2) {
    state = STATE_LF;
  } else if (keycode == IR_KEYCODE_3) {
    state = STATE_OA;
  }

  // Function according to state (if-else)
  if (state == STATE_RC) {
    RC(keycode);
  } else if (state == STATE_LF) {
    LineFollower();
  } else if (state == STATE_OA) {
    ObsDetection();
  }
}

void RC(int keycode) {
  if (keycode == IR_KEYCODE_UP) {
    forward();
    delay(100);
    stop();
  } else if (keycode == IR_KEYCODE_DOWN) {
    reverse();
    delay(100);
    stop();
  } else if (keycode == IR_KEYCODE_LEFT) {
    left();
    delay(100);
    stop();
  } else if (keycode == IR_KEYCODE_RIGHT) {
    right();
    delay(100);
    stop();
  } else if (keycode == IR_KEYCODE_OK) {
    stop();
  } else if (keycode == IR_KEYCODE_STAR) {
    pos = 40; // Use 40 as the opening position
    myservo.write(pos);
  } else if (keycode == IR_KEYCODE_POUND) {
    pos = 5; // Use 5 as the closing position
    myservo.write(pos);
  } 
}

void LineFollower() {
  left();
}

void ObsDetection() {
  right();
}

// Movement functions
void forward() {
  digitalWrite(2, LOW);
  analogWrite(5, 70);
  digitalWrite(4, LOW);
  analogWrite(6, 70);
}

void reverse() {
  digitalWrite(2, HIGH);
  analogWrite(5, 70);
  digitalWrite(4, HIGH);
  analogWrite(6, 70);
}

void right() {
  digitalWrite(2, HIGH);
  analogWrite(5, 75);
  digitalWrite(4, LOW);
  analogWrite(6, 75);
}

void left() {
  digitalWrite(2, LOW);
  analogWrite(5, 75);
  digitalWrite(4, HIGH);
  analogWrite(6, 75);
}

void stop() {
  digitalWrite(2, LOW);
  analogWrite(5, 0);
  digitalWrite(4, LOW);
  analogWrite(6, 0);
}
