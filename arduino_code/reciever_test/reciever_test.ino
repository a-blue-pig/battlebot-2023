/*
Steven Ortega
ME366J
Fall 2023

Code to test pwm reciever inputs. Recieves commands from a RC reciever
and prints the values.

Last Updated 11.11.2023
*/


// Define Pins
#define CH1 3     // reciever inputs
#define CH2 11

// pin Interrupt stuff
// ch1
volatile long startTime1 = 0;
volatile long currentTime1 = 0;
volatile long pulses1 = 0;
int ch1Val = 0;

// ch2
volatile long startTime2 = 0;
volatile long currentTime2 = 0;
volatile long pulses2 = 0;
int ch2Val = 0;


void setup() {
  Serial.begin(9600);

  // Setup pins to read channels
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  attachInterrupt(digitalPinToInterrupt(CH1), pulseTimer1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CH2), pulseTimer2, CHANGE);
}

void loop() {
  // only save pulses less than 2000 microseconds
  if (pulses1 < 2000){
    ch1Val = pulses1;
  }
  if (pulses2 < 2000){
    ch2Val = pulses2;
  }

  
  Serial.print(ch1Val);
  Serial.print("   ");
  Serial.println(ch2Val);
  
}

// read signal pulse on ch1
void pulseTimer1 (){
  currentTime1 = micros();
  if (currentTime1 > startTime1){
    pulses1 = currentTime1 - startTime1;
    startTime1 = currentTime1;
  }
}

// read signal pulse on ch2
void pulseTimer2 (){
  currentTime2 = micros();
  if (currentTime2 > startTime2){
    pulses2 = currentTime2 - startTime2;
    startTime2 = currentTime2;
  }
}
