#define LEFT_IR_PIN A1
#define RIGHT_IR_PIN A2
#define LEFT_DIR_PIN 2
#define RIGHT_DIR_PIN 4
#define LEFT_SPEED_PIN 5
#define RIGHT_SPEED_PIN 6

unsigned long previousMillis = 0;
const long interval = 100; // Interval between sensor checks (in milliseconds)

void setup() {
  Serial.begin(9600);
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);
  pinMode(LEFT_DIR_PIN, OUTPUT);
  pinMode(RIGHT_DIR_PIN, OUTPUT);
  pinMode(LEFT_SPEED_PIN, OUTPUT);
  pinMode(RIGHT_SPEED_PIN, OUTPUT);
}

void move(int left_speed, int right_speed, bool left_dir, bool right_dir) {
  digitalWrite(LEFT_DIR_PIN, left_dir);
  digitalWrite(RIGHT_DIR_PIN, right_dir);
  analogWrite(LEFT_SPEED_PIN, left_speed);
  analogWrite(RIGHT_SPEED_PIN, right_speed);
}

void moveForward() {
  move(54, 54, LOW, LOW); //100
}

void moveLeft() {
  move(65, 65, LOW, HIGH); //80-84-
}

void moveRight() {
  move(65, 65, HIGH, LOW); //100
}

void stop() {
  move(0, 0, HIGH, HIGH); //100
}

void loop() {
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
      moveForward();
    } else if (leftIRValue == LOW && rightIRValue == HIGH) {
      moveRight();
    } else if (leftIRValue == HIGH && rightIRValue == LOW) {
      moveLeft();
    } else {
      moveForward(); // You might want to use stop(); here depending on your intended behavior
    }
  }
}
