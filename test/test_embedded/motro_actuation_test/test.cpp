#include <Arduino.h>
#include <ESP32Servo.h>

Servo leftLeg;
Servo rightLeg;
Servo leftFoot;
Servo rightFoot;

int minUs = 500;
#define PIN_LG 14 // servo[4] !!Choose your ESP32 pin!!
#define PIN_LF 26 // servo[2] !!Choose your ESP32 pin!!

#define PIN_RG 25 // servo[5] !!Choose your ESP32 pin!!
#define PIN_RF 27 // servo[3] !!Choose your ESP32 pin!!
const int neutralPosition = 90;
int maxUs = 2400;
void setup(){
  Serial.begin(115200);

delay(1000);
  leftLeg.attach(PIN_LG);   // GPIO for left leg
  rightLeg.attach(PIN_RG);  // GPIO for right leg
  leftFoot.attach(PIN_LF);  // GPIO for left foot
  rightFoot.attach(PIN_RF); // GPIO for right foot

Serial.printf("Servo init pos %d\n\r", rightFoot.read()); 
  // Set servos to neutral position
  leftLeg.write(neutralPosition);
  rightLeg.write(neutralPosition);
  leftFoot.write(neutralPosition);
  rightFoot.write(neutralPosition);
Serial.printf("Servo init pos %d\n\r", rightFoot.read()); 


}
void loop(){
rightFoot.write(90);
// leftFoot.write(90);
// delay(1000);
// // delay(5);
// // }
// // for ( int i = 0 ; i< 180 ; i ++){
// // myservx.write(i);
// // delay(10);
// // }
delay(500);
rightFoot.write(0);
// myserv3.write(0);
delay(500);
rightFoot.write(180);
// myserv3.write(180);
delay(500);

}

