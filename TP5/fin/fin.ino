#include <ESP32Servo.h>
#include <analogWrite.h>
//#include <tone.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>
#include <WiFi.h>
#include <WebServer.h>
const char* ssid = "binome4"; // Mettre le ssid de votre réseau Wifi
const char* password = "azerty"; // Mettre le mot de passe de votre réseau Wifi
WebServer server(80); // Creation dun objet de type serveur HTTP

int degree = 130;
Servo myServo;
bool isManualMode = false;

const int ECART = 300;
const int SERVO_PIN = 25;
const int TOUCH_1 = 14;
const int TOUCH_2 = 15;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  analogSetPinAttenuation(A3, ADC_0db);
  analogSetPinAttenuation(A4, ADC_0db);
  myServo.attach(SERVO_PIN);
  myServo.write(degree);


  delay(1000);
  Serial.print("Setting AP (Access Point)...");
  //WiFi.softAP(ssid, password); // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.on("/", handleRoot); // si navigation vers (page daccueil) appel du callback handleRoot
  server.on("/mode_auto", handleModeAuto);
  server.onNotFound(handleNotFound); // si navigation vers cible inconnue, appel du callback handlenotFound
  server.begin(); // Initialisation du serveur web
  Serial.println("Serveur web actif");
}


void loop() {
   digitalWrite(LED_BUILTIN, !isManualMode);
   
  server.handleClient();
  Serial.print("Mode : ");
  Serial.println(isManualMode ? "MANUEL" : "AUTOMATIQUE");
    
    
  int touchRead1 = touchRead(TOUCH_1);
  int touchRead2 = touchRead(TOUCH_2);
  Serial.println(" touch1 et touch2 : ");
  Serial.println(touchRead1);
  Serial.println(touchRead2);
  Serial.println("###");
  
  if((touchRead1 < 50 || touchRead2 < 50) && isManualMode == false) {
    isManualMode=true;
    delay(1000);
  }
  
 if(isManualMode == true) {
   if(touchRead1 < 50  && degree < 170 && degree > 90) {
    myServo.write(degree++);
    delay(200);
  }

  if(touchRead2 < 50  && degree < 170 && degree > 90) {
     myServo.write(degree--);
    delay(200);
  }
 } else {
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
  delay(2000);
 }
  
 
  
}

void handleRoot(){ // Callback declenché en cas de navigation vers la page dacceuil (root)
  // Syntaxe décriture pour être compatible avec le C++ / Arduino
  // String page = " xxxxxxxx ";
  // page += " xxxxx ";
  // etc ...
  String page = "<!DOCTYPE html>"; // Début page HTML
  page += "<head>";
  page += " <title>Serveur ESP32</title>";
  page += " <meta http-equiv=refresh name=viewport content=width=device-width, initial-scale=1 charset=UTF-8/>";
  page += "</head>";
  page += "<body lang=fr>";
  page += " <h1>Mon Serveur</h1>";
  page += " <p>Ce serveur est hébergé sur un ESP32</p>";
  page += " <a href=mode_auto>Cliquez ici pour passer en mode automatique</a>";
  page += "</body>";
  page += "</html>"; // Fin page HTML
  server.send(200, "text/html", page); // Envoi de la page HTML
}

void handleNotFound(){ // Callback declenché en cas de navigation vers une cible inconnue
  server.send(404, "text/plain","404: Not found");
}

void handleModeAuto() {
  isManualMode = false;
  degree = 130;
  myServo.write(degree);
 
}
