/*
Steven Ortega
ME366J
Fall 2023

Code for a battlebot. Recieves commands from a RC reciever
and controls the drive train and weapon.

Last Updated 11.11.2023
*/

#include <Servo.h>

// Define Pins
#define CH1 21     // reciever inputs
#define CH2 2
#define CH3 3
#define CH4 11

#define ENA 10     // right side
#define IN1 9
#define IN2 8

#define ENB 5    // left side
#define IN3 7
#define IN4 4

#define WM 6      // Weapon motor
Servo weaponESC;

// pin Interrupt stuff
volatile long startTime1 = 0;       // ch1
volatile long currentTime1 = 0;
volatile long pulses1 = 0;
int ch1Val = 0;

volatile long startTime2 = 0;       // ch2
volatile long currentTime2 = 0;
volatile long pulses2 = 0;
int ch2Val = 0;

volatile long startTime3 = 0;       // ch3
volatile long currentTime3 = 0;
volatile long pulses3 = 0;
int ch3Val = 2000;

volatile long startTime4 = 0;       // ch4
volatile long currentTime4 = 0;
volatile long pulses4 = 0;
int ch4Val = 0;


// pwm value mapping
const int ch1Max = 1960;    // ch1
const int ch1Min = 952;
int ch1Resting = 1468;

const int ch2Max = 1976;    // ch2
const int ch2Min = 1012;
int ch2Resting = 1528;

int deadzone = 50;
int adjAmount = 0;
int pwmLowerLimit = 10;

// Set_Direction()
char prevDir = 's';

// Weapon
int ch3ActiveVal = 1200;
int confirmCount = 0;
bool prevStatusOn = 0;
int minWeaponCount = 10;
int weaponHigh = 1700;    // [us] micro seconds
int weaponLow = weaponLow;
bool weaponOn = 0;
int confirmOffCount = 0;


void setup() {
  Serial.begin(9600);

  // Setup pins to read channels
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  attachInterrupt(digitalPinToInterrupt(CH1), pulse_timer1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CH2), pulse_timer2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CH3), pulse_timer3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CH4), pulse_timer4, CHANGE);

  // Setup pins to control motors
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);
  weaponESC.attach(WM);
  weaponESC.writeMicroseconds(weaponLow);
  delay(3000);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // only save pulses less than 2000 microseconds
  if (pulses1 < 2000){
    ch1Val = pulses1;
  }
  if (pulses2 < 2000){
    ch2Val = pulses2;
  }
  if (pulses3 < 2000){
    ch3Val = pulses3;
  }
  if (pulses4 < 2000){
    ch4Val = pulses4;
  }

  // E-Stop
  if (ch4Val == 1000){
    Serial.println(". E-Stop Activated");
    eStop();
    while (1);
  }

  // reciever_test();
  // Serial.println(ch3Val);

  // Determine if ch3 truly on
  if (ch3Val < ch3ActiveVal) {
    if (prevStatusOn && (confirmCount < minWeaponCount)){
      confirmCount++;
      confirmOffCount = 0;
    }
    prevStatusOn = 1;
  } else {
    confirmOffCount++;
    if (confirmOffCount > 10) {
      confirmCount = 0;
      prevStatusOn = 0;
      confirmOffCount = 0;
    }
  }

  // Weapon Operation
  if (confirmCount == minWeaponCount) {
    // if (!weaponOn){
    //   Serial.println("Weapon Spinning");  
    // }
    // weaponOn = 1;
    weaponESC.writeMicroseconds(weaponHigh);
  } else {
    // if (weaponOn){
    //   Serial.println("Weapon Off");  
    // }
    // weaponOn = 0;
    weaponESC.writeMicroseconds(weaponLow);  
  }

  // Move Forward
  if (ch2Val > (ch2Resting+deadzone)) {
    set_direction('f');
    // Serial.println("forward");

    // Turn Left
    if (ch1Val > (ch1Resting+deadzone)) {
      // slow down turn inner wheel
      adjAmount = (ch1Val - ch1Resting);
      if (adjAmount > (ch2Val-ch2Resting)){
        adjAmount = ch2Val-ch2Resting;
      }
      float pwmOut = map(ch2Val, ch2Resting+deadzone, ch2Max, 0, 255);
      float pwmAdj = abs(map(ch2Val-adjAmount, ch2Resting, ch2Max, pwmLowerLimit, 255));

      // turn left
      analogWrite(ENA, pwmOut);
      analogWrite(ENB, pwmAdj);

      //Serial.println("forward left");

    // Turn. Right
    } else if (ch1Val < (ch1Resting-deadzone)) {
      // slow down turn inner wheel
      adjAmount = (ch1Resting - ch1Val);
      if (adjAmount > (ch2Val-ch2Resting)){
        adjAmount = ch2Val-ch2Resting;
      }
      float pwmOut = map(ch2Val, ch2Resting-deadzone, ch2Max, 0, 255);
      float pwmAdj = abs(map(ch2Val-adjAmount, ch2Resting, ch2Max, pwmLowerLimit, 255));

      // turn right
      analogWrite(ENA, pwmAdj);
      analogWrite(ENB, pwmOut);
      // Serial.println("forward right");
    
    // Only Forward
    } else {
      float pwmOut = map(ch2Val, ch2Resting+deadzone, ch2Max, 0, 255);
      analogWrite(ENA, pwmOut);
      analogWrite(ENB, pwmOut);
    }

  // Move Backward
  } else if (ch2Val < (ch2Resting-deadzone)) {
    set_direction('b');
    // Serial.println("backward");

    // turn backward left
    if (ch1Val > (ch1Resting+deadzone)) {
      adjAmount = (ch1Val - ch1Resting);
      if (adjAmount > (ch2Resting-ch2Val)){
        adjAmount = ch2Resting-ch2Val;
      }
      float pwmOut = map(ch2Val, ch2Min, ch2Resting+deadzone, 255, 0);
      float pwmAdj = abs(map(ch2Val+adjAmount, ch2Min, ch2Resting, 255, pwmLowerLimit));

      // turn backwards left
      analogWrite(ENA, pwmOut);
      analogWrite(ENB, pwmAdj);

      // Serial.println("backward left");

    // turn backwards right
    } else if (ch1Val < (ch1Resting - deadzone)) {
      adjAmount = (ch1Resting - ch1Val);
      if (adjAmount > (ch2Resting-ch2Val)){
        adjAmount = ch2Resting-ch2Val;
      }
      float pwmOut = map(ch2Val, ch2Min, ch2Resting-deadzone, 255, 0);
      float pwmAdj = abs(map(ch2Val+adjAmount, ch2Min, ch2Resting, 255, pwmLowerLimit));

      // turn backwards right
      analogWrite(ENA, pwmAdj);
      analogWrite(ENB, pwmOut);
      // Serial.println("backward right");

    // Only Backward
    } else {
      float pwmOut = map(ch2Val, ch2Min, ch2Resting-deadzone, 255, 0);
      analogWrite(ENA, pwmOut);
      analogWrite(ENB, pwmOut);
    }

  // Pivoting Manuevers
  } else {

    // Pivot CCW
    if (ch1Val > (ch1Resting+deadzone)) {
      set_direction('l');
      float pwmOut = map(ch1Val, ch1Resting+deadzone, ch1Max, pwmLowerLimit, 255);
      analogWrite(ENA, pwmOut);
      analogWrite(ENB, pwmOut);
      // Serial.println("pivot left");

    // Pivot CW
    } else if (ch1Val < (ch1Resting-deadzone)) {
      set_direction('r');
      float pwmOut = map(ch1Val, ch1Min, ch1Resting-deadzone, 255, pwmLowerLimit);
      analogWrite(ENA, pwmOut);
      analogWrite(ENB, pwmOut);
      // Serial.println("pivot right");

    // Do Nothing if within the deadzone on both ch1 and ch2
    } else {
      set_direction('s');
      analogWrite(ENA, 0);
      analogWrite(ENB, 0);
      // Serial.println("nothing");
    }
  }
}

// read signal pulse on ch1
void pulse_timer1 (){
  currentTime1 = micros();
  if (currentTime1 > startTime1){
    pulses1 = currentTime1 - startTime1;
    startTime1 = currentTime1;
  }
}

// read signal pulse on ch2
void pulse_timer2 (){
  currentTime2 = micros();
  if (currentTime2 > startTime2){
    pulses2 = currentTime2 - startTime2;
    startTime2 = currentTime2;
  }
}

// read signal pulse on ch3
void pulse_timer3 () {
  currentTime3 = micros();
  if (currentTime3 > startTime3){
    pulses3 = currentTime3 - startTime3;
    startTime3 = currentTime3;
  }
}

void pulse_timer4 () {
  currentTime4 = micros();
  if (currentTime4 > startTime4){
    pulses4 = currentTime4 - startTime4;
    startTime4 = currentTime4;
  }
}
// set direction of motors
void set_direction(char dir) {
  if (dir == prevDir){
    return;
  }
  switch (dir){
    case 'l':
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);  
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;
    case 'r':
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);  
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;
    case 'f':
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);  
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;
    case 'b':
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);  
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;
    case 's':
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);  
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
    default:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);  
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
  }
  prevDir = dir;
}

void eStop() {
  weaponESC.writeMicroseconds(1300);  
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  set_direction('s');
  digitalWrite(LED_BUILTIN, HIGH);
}

void reciever_test(){
  Serial.print("Ch1: ");
  Serial.print(ch1Val);
  Serial.print(",  Ch2: ");
  Serial.print(ch2Val);
  Serial.print(",  Ch3: ");
  Serial.print(ch3Val);
  Serial.print(",  Ch4: ");
  Serial.println(ch4Val);
}