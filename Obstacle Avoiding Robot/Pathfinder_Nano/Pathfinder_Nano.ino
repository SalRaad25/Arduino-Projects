#include <AFMotor.h>
#include <Servo.h>
#define BUZZ_PIN A0
#define TRIG_PIN A1
#define ECHO_PIN A2
#define M1A 2
#define M1B 3
#define M2A 4
#define M2B 5
#define M1S 9
#define M2S 10
#define LEDB_PIN 6
#define LEDG_PIN 7
#define LEDR_PIN 8
//#define DCRMOFF

Servo SER;

int Search(void) {                   
  float Duration = 0.0;               
  float CM = 0.0;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite (TRIG_PIN, LOW);
  Duration = pulseIn (ECHO_PIN, HIGH);
  CM = (Duration/58.8);
  return CM;
}

int RightDistance, LeftDistance;
float Distance = 0.00;

void setup() {
  pinMode (BUZZ_PIN, OUTPUT);  
  pinMode (TRIG_PIN, OUTPUT);
  pinMode (ECHO_PIN, INPUT);
  pinMode (LEDB_PIN, OUTPUT);
  pinMode (LEDG_PIN, OUTPUT);
  pinMode (LEDR_PIN, OUTPUT);
  pinMode (M1A, OUTPUT);
  pinMode (M1B, OUTPUT);
  pinMode (M2A, OUTPUT);
  pinMode (M2B, OUTPUT);
  SER.attach (10);
}

void loop() {
  SER.write(80);
  delay(100);
  Distance = Search();
  if (Distance < 30) {
    digitalWrite (BUZZ_PIN, HIGH);
    digitalWrite (LEDB_PIN, LOW);
    digitalWrite (LEDG_PIN, LOW);
    digitalWrite (LEDR_PIN, HIGH);
    //set motor speed to low
    analogWrite(M1S, 100);
    analogWrite(M2S, 100);
    Serial.println("Turn Around!");
    
    ChangePath();
    
  }
  else if ((Distance >= 30) && (Distance <60)) {
    digitalWrite (BUZZ_PIN, LOW);
    digitalWrite (LEDB_PIN, HIGH);
    digitalWrite (LEDG_PIN, LOW);
    digitalWrite (LEDR_PIN, LOW);
    //set motor speed medium
    analogWrite(M1S, 150);
    analogWrite(M2S, 150);
    Serial.println("Mid Speed!");
    Forward();
  }
  else if ((Distance >= 60) && (Distance < 90)) {
    digitalWrite (BUZZ_PIN, LOW);
    digitalWrite (LEDB_PIN, LOW);
    digitalWrite (LEDG_PIN, HIGH);
    digitalWrite (LEDR_PIN, LOW);
    analogWrite(M1S, 200);
    analogWrite(M2S, 200);
    Serial.println("High Speed!");
    Forward ();
  } else{
    digitalWrite (BUZZ_PIN, LOW);
    digitalWrite (LEDB_PIN, HIGH);
    digitalWrite (LEDG_PIN, HIGH);
    digitalWrite (LEDR_PIN, HIGH);
    //set motor speed to high
    analogWrite(M1S, 255);
    analogWrite(M2S, 255);
    Serial.println("Full Speed!");
    Forward();
  }
}

void ChangePath() {
  Stop();
  Backward();
  SER.write(12);
  delay(500);
  RightDistance = Search();
  delay(500);
  SER.write(160);
  delay(1000);
  LeftDistance = Search();
  delay(500);
  SER.write(80);
  delay(500);
  CompareDistance();
}

void CompareDistance() {
  if (RightDistance > LeftDistance) {
    TurnRight();
  }
  else if (LeftDistance > RightDistance) {
    TurnLeft();
  } else {
    TurnAround();  
  }
}

void Forward() {
  //set both motors to run forward
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);
}

void Backward() {
  //set both motors to run back
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
  delay(500);
}

void TurnRight() {
  //m1 run backward
  //m2 run forward
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);
  //calibrate speed of m1 w/ DCMROFF
  analogWrite(M1S, 125);
  delay(300);
}

void TurnLeft() {
  //m1 run forwward
  //m2 run backward
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
  //calibrate speed of m2 w/ DCMROFF
  analogWrite(M2S, 125);
  delay(300);
}

void TurnAround() {
  //m1 run forward
  //m2 run backward
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
  //calibrate speed of motor 2
  analogWrite(M2S, 125);
  delay(700);
}

void Stop() {
  //m1 stop
  //m2 stop
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, LOW);
  delay(100);
}
