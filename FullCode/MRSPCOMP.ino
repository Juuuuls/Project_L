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

#define LEFT_IR_PIN A1
#define CENTER_IR_PIN A3
#define RIGHT_IR_PIN A2


Ultrasonic sonarF(FTRIGGER_PIN, FECHO_PIN);
Ultrasonic sonarL(LTRIGGER_PIN, LECHO_PIN);
Ultrasonic sonarR(RTRIGGER_PIN, RECHO_PIN);


IRremote ir(3);

// Global variables
//remote state
int state = 0; 
const int STATE_RC = 0;
const int STATE_LF = 1;
const int STATE_OA = 2;
bool lastDirectionLeft = false;

// Global variables
float DistanceF;
float DistanceL;
float DistanceR;

// Distance threshold for obstacle detection
int distance = 80;
int distanceS = 60;

// Servo variable
Servo myservo;
int pos = 0;

unsigned long previousMillis = 0;
const long interval = 100;

void setup() {
  myservo.attach(10);  // attaches the servo on pin 10 to the servo object

  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(CENTER_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);
  pinMode(2, OUTPUT);
   pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  Serial.begin(9600);
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
    forwardRC();
    delay(100);
    stop();
  } else if (keycode == IR_KEYCODE_DOWN) {
    reverseRC();
    delay(100);
    stop();
  } else if (keycode == IR_KEYCODE_LEFT) {
    rightRC();
    delay(100);
    stop();
  } else if (keycode == IR_KEYCODE_RIGHT) {
    leftRC();
    delay(100);
    stop();
  } else if (keycode == IR_KEYCODE_OK) {
    stop();
  } else if (keycode == IR_KEYCODE_STAR) {
    pos = 110;
    myservo.write(pos);
  } else if (keycode == IR_KEYCODE_POUND) {
    pos = 50;
    myservo.write(pos);
  } 
}

void ObsDetection() {
  int iterations = 7; // Number of measurements for median filtering
  pos = 50;
  myservo.write(pos);

  // Get sensor data
  DistanceF = sonarF.distanceRead(iterations);
  DistanceL = sonarL.distanceRead(iterations);
  DistanceR = sonarR.distanceRead(iterations);

  navigate();
}

void navigate() {
  int Repeated_FL = 0;
  int Repeated_FR = 0;

  if (DistanceF <= distance) {
    Serial.println("F");
    stop();
    delay(100);

    if (DistanceL < distanceS) {
      if (DistanceF < distance) {
        reverse();
      }
      Repeated_FL += 1; // Correct increment
      if (Repeated_FL == 2) {
        if (DistanceR > DistanceF || DistanceR > DistanceL) {
          right(); // Brute right
          delay(1000);
          Repeated_FL = 0;
        } else {
          left();
        }
      } else {
        left();  // Right sensors detect obstacles, turn left
        delay(1000);
      }
    } else {
      Repeated_FL = 0;
      Serial.println("Left");
      right();  // If only the front sensor detects an obstacle, turn right
      delay(1000);
    }
  } else {
    forward();  // If no obstacles detected in front, move forward
  }
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
      forwardLF();
    } else if (leftIRValue == LOW && rightIRValue == HIGH) {
      rightLF();
    } else if (leftIRValue == HIGH && rightIRValue == LOW) {
      leftLF();
    } else {
      forwardLF(); // You might want to use stop(); here depending on your intended behavior
    }
  }
}

// Movement functions
void forward() {
  digitalWrite(2, LOW);
  analogWrite(5, 100);
  digitalWrite(4, LOW);
  analogWrite(6, 100);
}

void reverse() {
  digitalWrite(2, HIGH);
  analogWrite(5, 64);
  digitalWrite(4, HIGH);
  analogWrite(6, 64);
}

void left() {
  digitalWrite(2, HIGH);
  analogWrite(5, 70);
  digitalWrite(4, LOW);
  analogWrite(6, 70);
  
}

void right() {
  digitalWrite(2, LOW);
  analogWrite(5, 70);
  digitalWrite(4, HIGH);
  analogWrite(6, 70);
  
}

void stop() {
  digitalWrite(2, LOW);
  analogWrite(5, 0);
  digitalWrite(4, LOW);
  analogWrite(6, 0);
}

// Movement functions RC

void forwardRC() {
  digitalWrite(2, LOW);
  analogWrite(5, 200);
  digitalWrite(4, LOW);
  analogWrite(6, 200);
}

void reverseRC() {
  digitalWrite(2, HIGH);
  analogWrite(5, 100);
  digitalWrite(4, HIGH);
  analogWrite(6, 100);
}

void leftRC() {
  digitalWrite(2, HIGH);
  analogWrite(5, 170);
  digitalWrite(4, LOW);
  analogWrite(6, 170);
  
}

void rightRC() {
  digitalWrite(2, LOW);
  analogWrite(5, 170);
  digitalWrite(4, HIGH);
  analogWrite(6, 170);
  
}

// Movement functions LF

void forwardLF() {
  digitalWrite(2, LOW);
  analogWrite(5, 54);
  digitalWrite(4, LOW);
  analogWrite(6, 54);
}

void reverseLF() {
  digitalWrite(2, HIGH);
  analogWrite(5, 64);
  digitalWrite(4, HIGH);
  analogWrite(6, 64);
}

void leftLF() {
  digitalWrite(2, HIGH);
  analogWrite(5, 64);
  digitalWrite(4, LOW);
  analogWrite(6, 64);
  
}

void rightLF() {
  digitalWrite(2, LOW);
  analogWrite(5, 64);
  digitalWrite(4, HIGH);
  analogWrite(6, 64);
  
}

