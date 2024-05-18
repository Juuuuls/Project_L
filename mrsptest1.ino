#include "IR_remote.h"
#include "keymap.h"

IRremote ir(3);
bool x = ir.getIrKey(ir.getCode(), 1);



// movement
void forward() {
  digitalWrite(2, LOW);
  analogWrite(5, 150);
  digitalWrite(4, LOW);
  analogWrite(6, 150);
}
void down() {
  digitalWrite(2, HIGH);
  analogWrite(5, 150);
  digitalWrite(4, HIGH);
  analogWrite(6, 150);
}
void left() {
  digitalWrite(2, HIGH);
  analogWrite(5, 50);
  digitalWrite(4, LOW);
  analogWrite(6, 50);
  delay(200);
}
void right() {
  digitalWrite(2, LOW);
  analogWrite(5, 50);
  digitalWrite(4, HIGH);
  analogWrite(6, 50);
  delay(200);
}
void stop() {
  digitalWrite(2, LOW);
  analogWrite(5, 0);
  digitalWrite(4, LOW);
  analogWrite(6, 0);
}

//functions

void RC() {
  if (ir.getIrKey(ir.getCode(), 1) == IR_KEYCODE_UP) {
    forward();
    delay(100);
    stop();
  } else if (ir.getIrKey(ir.getCode(), 1) == IR_KEYCODE_DOWN) {
    down();
    delay(100);
    stop();


  } else if (ir.getIrKey(ir.getCode(), 1) == IR_KEYCODE_LEFT) {
    left();
    delay(100);
    stop();

  } else if (ir.getIrKey(ir.getCode(), 1) == IR_KEYCODE_RIGHT) {
    right();
    delay(100);
    stop();

  } else if (ir.getIrKey(ir.getCode(), 1) == IR_KEYCODE_OK) {
    stop();
  }
}

void ObsDetection() {
  left();
}
void lineFollower() {
  right();
}


void setup() {
  IRremote ir(3);

  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  if (ir.getIrKey(ir.getCode(), 1) == IR_KEYCODE_UP) {
    forward();
    delay(100);
    stop();
  } else if (ir.getIrKey(ir.getCode(), 1) == IR_KEYCODE_DOWN) {
    down();
    delay(100);
    stop();


  } else if (ir.getIrKey(ir.getCode(), 1) == IR_KEYCODE_LEFT) {
    left();
    delay(100);
    stop();

  } else if (ir.getIrKey(ir.getCode(), 1) == IR_KEYCODE_RIGHT) {
    right();
    delay(100);
    stop();

  } else if (ir.getIrKey(ir.getCode(), 1) == IR_KEYCODE_OK) {
    stop();

  } else if (ir.getIrKey(ir.getCode(), 1) == IR_KEYCODE_STAR) {
    stop();

  } else if (ir.getIrKey(ir.getCode(), 1) == IR_KEYCODE_POUND) {
    stop();

  } else if (ir.getIrKey(ir.getCode(), 1) == IR_KEYCODE_1) {
    ObsDetection();

  } else if (ir.getIrKey(ir.getCode(), 1) == IR_KEYCODE_2) {
    lineFollower();
  }
}
