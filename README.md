# battlebot-2023
BattleBot Project for ME366J

Course Completed in FALL 2023.

## Project Materials
### Microcontroller
Project uses an Arduino Nano Every as the microcontroller.
Requires the following packages/board packages:
1. Servo.
2. Arduino megaAVR Boards.

### Drivetrain
Differential Drive with 2 Motors.
Motor Controller is an L298N.

### Weapon
Weapon is a vertical spinner, made of Aluminum.
It is powered by a 30A ESC with a 2212 2200Kv Motor on a pulley with a 
2.3:1 gear ratio to add torque.

### Controller
Uses a generic RC car controller with 4 Channels.

### Power
Powered using a 3S 1300mAh Lipo.

## Usage/Controls
* The speed channel controls forward and backward movement.
* The steering channel allows for pivoting in place.
    * When combined with speed, it drives like a normal car.
* Channel 3 activates/deactivates the weapon.
* Channel 4 is a software E-Stop.


## Notes about the project
In this project we utilized various engineering practices to create a
battle bot from scratch. Our budget was $300 and we were given ~ 4 weeks
to procure and build our battlebot.


Based on the component we bought we realized our selection of RC
controller had a lot of noise and would not always send signals when
channel 3 and 4 were pressed. As a result I had to get creative in how to
determine if the signal was a true on or true off for the weapon activatiaon.


Overall this project was a strong learning experience and allowed me to
try out kiCAD when drawing our schematics and designing a custom PCB.
The PCB was ultimately made by hand using a protoboard and wires, but was
a unique experience.
