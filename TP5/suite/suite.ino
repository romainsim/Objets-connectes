#include <ESP32Servo.h>
#include <analogWrite.h>
//#include <tone.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

bool isManualMode = false;
int degree = 0;
Servo myServo;

const int SERVO_PIN = 25;
const int TOUCH_1 = 14;
const int TOUCH_2 = 15;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  analogSetPinAttenuation(A3, ADC_0db);
  analogSetPinAttenuation(A4, ADC_0db);
  myServo.attach(SERVO_PIN);
}

void loop() {
  digitalWrite(LED_BUILTIN, isManualMode);

  int touchRead1 = touchRead(TOUCH_1);
  int touchRead2 = touchRead(TOUCH_2);
  Serial.println(" touch1 et touch2 : ");
  Serial.println(touchRead1);
  Serial.println(touchRead2);
  Serial.println("###");
  
  if((touchRead1 < 50 || touchRead2 < 50) && isManualMode == false) {
    isManualMode=true;
    myServo.write(0);
    delay(1000);
  }
  
  if(touchRead1 < 50) {
    myServo.write(degree++);
    delay(20);
  }

  if(touchRead2 < 50) {
     myServo.write(degree--);
    delay(20);
  }
  int valA3 = analogRead(A3);
  int valA4 = analogRead(A4);
  Serial.print(" Luminosite capteur 1 : ");
  Serial.println(valA3);
  Serial.print(" Luminosite capteur 2 : ");
  Serial.println(valA4);
 

}
