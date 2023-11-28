#include <Servo.h>


#define WM 6      // Weapon motor
Servo weaponESC;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  weaponESC.attach(WM);
  weaponESC.writeMicroseconds(1300);
  delay(5000);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  weaponESC.writeMicroseconds(1400);
  delay(2000);
  digitalWrite(LED_BUILTIN, LOW);
  weaponESC.writeMicroseconds(1300);
  delay(2000);
}
