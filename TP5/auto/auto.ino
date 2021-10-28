#include <ESP32Servo.h>
#include <analogWrite.h>
//#include <tone.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

int degree = 130;
Servo myServo;

const int ECART = 300;
const int SERVO_PIN = 25;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  analogSetPinAttenuation(A3, ADC_0db);
  analogSetPinAttenuation(A4, ADC_0db);
  myServo.attach(SERVO_PIN);
  myServo.write(degree);
}


void loop() {

  int valA3 = analogRead(A3);
  int valA4 = analogRead(A4) - 1000;

  int ecart = valA3 - valA4;

  if(ecart >= -ECART && ecart <= ECART && degree < 170 && degree > 90) {
   
  } else {
    if(ecart < ECART) {
    degree+=10;
    delay(200);
  } else {
    degree-=10;
    delay(200);
  }
  }
  

  myServo.write(degree);
  Serial.print(" Luminosite capteur 1 : ");
  Serial.println(valA3);
  Serial.print(" Luminosite capteur 2 : ");
  Serial.println(valA4);
   Serial.println("##");
    Serial.println(ecart);
 delay(2000);

}
