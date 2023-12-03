#include <Servo.h>


#define WM 6      // Weapon motor
Servo weaponESC;
int pos = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  weaponESC.attach(WM);
  weaponESC.writeMicroseconds(1300);
  delay(3000);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  // digitalWrite(LED_BUILTIN, HIGH);
  // weaponESC.writeMicroseconds(1400);
  // delay(2000);
  // digitalWrite(LED_BUILTIN, LOW);
  // weaponESC.writeMicroseconds(1300);
  // delay(2000);

  for (pos = 1300; pos <= 1400; pos += 25) {
    weaponESC.writeMicroseconds(pos);
    delay(500);
  }
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);

  for (pos = 1400; pos <= 1800; pos += 25) {
    weaponESC.writeMicroseconds(pos);
    delay(100);
  }

  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);
  digitalWrite(LED_BUILTIN, HIGH);
}
