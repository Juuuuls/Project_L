#define LEFT_IR_PIN A1
#define CENTER_IR_PIN A3
#define RIGHT_IR_PIN A2
#define LEFT_DIR_PIN 2
#define RIGHT_DIR_PIN 4
#define LEFT_SPEED_PIN 5
#define RIGHT_SPEED_PIN 6

// Variable to store the last movement direction
bool lastDirectionLeft = false;

void setup() {
  Serial.begin(9600);
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(CENTER_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);
  pinMode(LEFT_DIR_PIN, OUTPUT);
  pinMode(RIGHT_DIR_PIN, OUTPUT);
  pinMode(LEFT_SPEED_PIN, OUTPUT);
  pinMode(RIGHT_SPEED_PIN, OUTPUT);
}

// Function to control movement
void move(int left_speed, int right_speed, bool left_dir, bool right_dir) {
  digitalWrite(LEFT_DIR_PIN, left_dir);
  digitalWrite(RIGHT_DIR_PIN, right_dir);
  analogWrite(LEFT_SPEED_PIN, left_speed);
  analogWrite(RIGHT_SPEED_PIN, right_speed);
}

// Function to move forward
void moveForward() {
  move(64, 64, LOW, LOW); // Adjust speed as needed
}

// Function to turn left
void moveLeft() {
  move(0, 64, LOW, LOW); // Adjust speed as needed
  lastDirectionLeft = true;
}

// Function to turn right
void moveRight() {
  move(64, 0, LOW, LOW); // Adjust speed as needed
  lastDirectionLeft = false;
}

// Function to move backward
void moveBackward() {
  move(60, 60, HIGH, HIGH); // Reverse both motors
  delay(500); // Reverse for 500 milliseconds
}

void loop() {
  // Read sensor values
  int leftIRValue = digitalRead(LEFT_IR_PIN);
  int centerIRValue = digitalRead(CENTER_IR_PIN);
  int rightIRValue = digitalRead(RIGHT_IR_PIN);

  // Line following logic
  if (centerIRValue == HIGH) {
    if (leftIRValue == LOW && rightIRValue == LOW) { // Both left and right sensors off the line
      moveForward();
    } 
    else if (leftIRValue == LOW && rightIRValue == HIGH) { // Right sensor on line
      moveRight();
    } 
    else if (leftIRValue == HIGH && rightIRValue == LOW) { // Left sensor on line
      moveLeft();
    } 
  } 
  else { // Center sensor off the line
    if (leftIRValue == LOW && rightIRValue == LOW) { // Both left and right sensors off the line
      if (lastDirectionLeft) {
        moveRight(); // Backtrack to the right
      } 
      else {
        moveLeft(); // Backtrack to the left
      }
      moveBackward(); // Perform reverse maneuver
    }
    else { // Either left or right sensor on the line
      if (leftIRValue == LOW) { // Left sensor on line
        moveLeft();
      } 
      else if (rightIRValue == LOW) { // Right sensor on line
        moveRight();
      }
    }
  }

  // Debugging output
  Serial.print("Left_IR_Value:");
  Serial.println(leftIRValue);
  Serial.print("Center_IR_Value:");
  Serial.println(centerIRValue);
  Serial.print("Right_IR_Value:");
  Serial.println(rightIRValue);
  Serial.println("");
}
