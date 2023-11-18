/*

*/

// Define Pins
const int ENA = 9;
const int IN1 = 2;
const int IN2 = 3;

const int ENB = 10;
const int IN3 = 4;
const int IN4 = 5;

void setup() {
  Serial.begin(9600);

  // Set Inputs and Outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  while(Serial.available() > 0){
    char direction = Serial.read();
    
    switch (direction){
      case 'w':
        Serial.println("FORWARD");
        set_direction('f');
        break;
      case 'a':
        Serial.println("LEFT");
        set_direction('l');
        break;
      case 's':
        Serial.println("BACKWARD");
        set_direction('b');
        break;
      case 'd':
        Serial.println("RIGHT");
        set_direction('r');
        break;
      default:
        Serial.println("STOP");
        set_direction('s');
        break;
    }
  }
}

void set_direction(char dir) {
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
}