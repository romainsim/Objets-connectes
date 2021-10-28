#include <ESP32Servo.h>
#include <analogWrite.h>
//#include <tone.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

Servo myServo;
const int servoPin = 25;

void setup() {
  Serial.begin(115200);
  
  analogSetPinAttenuation(A3, ADC_0db);
  analogSetPinAttenuation(A4, ADC_0db);
  myServo.attach(25);
}

void loop() {
  int valA3 = analogRead(A3);
  int valA4 = analogRead(A4);
  Serial.print(" Luminosite capteur 1 : ");
  Serial.println(valA3);
   Serial.print(" Luminosite capteur 2 : ");
  Serial.println(valA4);


  delay(2000);
  for(int degree = 0; degree <= 180; degree++) {
    myServo.write(degree);
    delay(20);
  }
  for(int degree = 180; degree >= 0; degree--) {
    myServo.write(degree);
    delay(20);
  }

}
