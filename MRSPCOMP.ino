#include "IR_remote.h"
#include "keymap.h"
#include <Servo.h>

#define LEFT_IR_PIN A1
#define RIGHT_IR_PIN A2
#define LEFT_DIR_PIN 2
#define RIGHT_DIR_PIN 4
#define LEFT_SPEED_PIN 5
#define RIGHT_SPEED_PIN 6

IRremote ir(3);

// Global variables
int state = 0;  // to keep track of the state
const int STATE_RC = 0;
const int STATE_LF = 1;
const int STATE_OA = 2;

// Servo variable
Servo myservo;
int pos = 0;

// Timing variables for LineFollower
unsigned long previousMillis = 0;
const long interval = 100;  // Interval at which to read sensors (in milliseconds)

void setup() {
  myservo.attach(10);  // attaches the servo on pin 10 to the servo object

  Serial.begin(9600);
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);
  pinMode(LEFT_DIR_PIN, OUTPUT);
  pinMode(RIGHT_DIR_PIN, OUTPUT);
  pinMode(LEFT_SPEED_PIN, OUTPUT);
  pinMode(RIGHT_SPEED_PIN, OUTPUT);
}

void loop() {
  int keycode = ir.getIrKey(ir.getCode(), 1);

  // Change state
  if (keycode == IR_KEYCODE_1) {
    stop();
    state = STATE_RC;
  } else if (keycode == IR_KEYCODE_2) {
    stop();
    state = STATE_LF;
  } else if (keycode == IR_KEYCODE_3) {
    stop();
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
    pos = 110;
    myservo.write(pos);
  } else if (keycode == IR_KEYCODE_POUND) {
    pos = 70;
    myservo.write(pos);
  }
}

void ObsDetection() {
  left();
}

void LineFollower() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Update the timing

    int leftIRValue = digitalRead(LEFT_IR_PIN);
    int rightIRValue = digitalRead(RIGHT_IR_PIN);

    Serial.print("Left_IR_Value:");
    Serial.println(leftIRValue);
    Serial.print("Right_IR_Value:");
    Serial.println(rightIRValue);
    Serial.println("");

    if (leftIRValue == LOW && rightIRValue == LOW) {
      forward();
    } else if (leftIRValue == LOW && rightIRValue == HIGH) {
      right();
    } else if (leftIRValue == HIGH && rightIRValue == LOW) {
      left();
    } else {
      forward();  // You might want to use stop(); here depending on your intended behavior
    }
  }
}

// Movement functions
void move(int left_speed, int right_speed, bool left_dir, bool right_dir) {
  digitalWrite(LEFT_DIR_PIN, left_dir);
  digitalWrite(RIGHT_DIR_PIN, right_dir);
  analogWrite(LEFT_SPEED_PIN, left_speed);
  analogWrite(RIGHT_SPEED_PIN, right_speed);
}

void forward() {
  move(54, 54, LOW, LOW);
}

void reverse() {
  move(54, 54, HIGH, HIGH);
}

void left() {
  move(65, 65, LOW, HIGH);
}

void right() {
  move(65, 65, HIGH, LOW);
}

void stop() {
  move(0, 0, LOW, LOW);
}
