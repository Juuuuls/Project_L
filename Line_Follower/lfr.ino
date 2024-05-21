#include <AFMotor.h>

//defining Sensor pins
#define lefts A0
#define rights A1

//defining motors
AF_DCMotor m1(1, MOTOR12_64KHZ); 
AF_DCMotor m4(4, MOTOR34_64KHZ);


void setup() {
  //Declaring PIN input types
  pinMode(lefts,INPUT);
  pinMode(rights,INPUT);
  //Setting the motor speed
  m1.setSpeed(100);
  m4.setSpeed(100);
  Serial.begin(9600);
}

void loop(){
  //Print values of the sensors to the serial monitor
  Serial.println(analogRead(lefts));
  Serial.println(analogRead(rights));
  //line detected by both sensors
  if(analogRead(lefts)<=500 && analogRead(rights)<=500){
    m1.run(FORWARD);
    m4.run(FORWARD);
  }
  //line detected by left sensor
  else if(analogRead(lefts)<=500 && !analogRead(rights)<=500){
    m1.run(FORWARD);
    m1.setSpeed(110);
    m4.run(BACKWARD);
  }
  //line detected by right sensor
  else if(!analogRead(lefts)<=500 && analogRead(rights)<=500){
    m1.run(BACKWARD);
    m4.setSpeed(110);
    m4.run(FORWARD);
  }
  //line detected by none
  else if(!analogRead(lefts)<=500 && !analogRead(rights)<=500){
    m1.run(FORWARD);
    m4.run(FORWARD);
  }
  
}
