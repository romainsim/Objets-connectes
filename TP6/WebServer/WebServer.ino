#include <WiFi.h>
#include <WebServer.h>
#include "Web.h"
const char* ssid = "SIMON"; // Mettre le ssid de votre réseau Wifi
const char* password = "azerty"; // Mettre le mot de passe de votre réseau Wifi
WebServer server(80); // Creation dun objet de type serveur HTTP
bool etatLED = false;
const char R = 26;
const char B = 25;
const char G = 21;


int freq = 5000;
int ledChannel = 0;
int resolution = 8;

  int GetRfromHTML(char* ColorCode);
  int GetBfromHTML(char* ColorCode);
  int GetGfromHTML(char* ColorCode);
  
  void handleRoot(){ // Callback declenché en cas de navigation vers la page dacceuil (root)
    server.send(200, "text/html", rgb_wheelHTML); // Envoi de la page HTML
  }
  
  void handleNotFound(){ // Callback declenché en cas de navigation vers une cible inconnue
    server.send(404, "text/plain","404: Not found");
  }
  
  void handleOff(){ // Callback declenché en cas de navigation vers la page dacceuil (root)
    etatLED = false;
    server.send(200, "text/html", offHTML); // Envoi de la page HTML
  }

  void handleOn(){ // Callback declenché en cas de navigation vers la page dacceuil (root)
    etatLED = true;
    server.send(200, "text/html", onHTML); // Envoi de la page HTML
  }

  void handleRGBSimple(){ // Callback declenché en cas de navigation vers la page dacceuil (root)
    String valueColor = server.arg("value");
    char* valueC = &valueColor[0];
    Serial.println(valueC);
    dacWrite(R, GetRfromHTML(valueC));
    dacWrite(B, GetGfromHTML(valueC));
    ledcWrite(ledChannel, GetBfromHTML(valueC));
    server.send(200, "text/html", rgb_simpleHTML); // Envoi de la page HTML
  }

  void handleRGBWheel() {
    String valueColor = server.arg("value");
    char* valueC = &valueColor[0];
    Serial.println(valueC);
    dacWrite(R, GetRfromHTML(valueC));
    dacWrite(B, GetGfromHTML(valueC));
    ledcWrite(ledChannel, GetBfromHTML(valueC));
    server.send(200, "text/html", rgb_wheelHTML);
  }
  
  void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    delay(1000);
    Serial.print("Setting AP (Access Point)...");
    WiFi.softAP(ssid, password); // Remove the password parameter, if you want the AP (Access Point) to be open
    //WiFi.softAP(ssid);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    server.on("/", handleRoot); // si navigation vers (page daccueil) appel du callback handleRoot
    server.on("/off.html", handleOff);
    server.on("/on.html", handleOn);
    server.on("/rgb_simple.html", handleRGBSimple);
    server.on("/rgb_wheel.html", handleRGBWheel);
    server.onNotFound(handleNotFound); // si navigation vers cible inconnue, appel du callback handlenotFound
    server.begin(); // Initialisation du serveur web
    Serial.println("Serveur web actif");

    pinMode(R, OUTPUT);
    pinMode(B, OUTPUT);
    ledcSetup(ledChannel, freq, resolution);
    ledcAttachPin(G, ledChannel);
    
    /*for (int i = 0; i <= 255; i++) {
      dacWrite(R, i);
      delay(10);
    }
    delay(200);
    for (int i = 0; i <= 255; i++) {
      dacWrite(B, i);
      delay(10);
    }
    delay(200);
    for (int i = 255; i >= 0; i--) {
      dacWrite(R, i);
      delay(10);
    }
    delay(200);
    for (int i = 255; i >= 0; i--) {
      dacWrite(B, i);
      delay(10);
    }
    delay(200);*/

    digitalWrite(R, 1);
    delay(1000);
    digitalWrite(B, 1);
    delay(1000);
    digitalWrite(R, 0);
    delay(1000);
    ledcWrite(ledChannel, 255);
    delay(1000);
    digitalWrite(B, 0);
    delay(1000);
    digitalWrite(R, 1);
    delay(1000);
    ledcWrite(ledChannel, 0);
    delay(1000);
    digitalWrite(R, 0);

    char * color = "FF0000";
    Serial.println(GetRfromHTML(color));
    Serial.println(GetBfromHTML(color));
    Serial.println(GetGfromHTML(color));
  }
  
  void loop() {
    digitalWrite(LED_BUILTIN, etatLED);
    server.handleClient(); // Attente de demande du client
  }

  int GetRfromHTML(char* ColorCode) {
    char color[2];
    
    if (ColorCode[0] == 35) {
      color[0] = ColorCode[1];
      color[1] = ColorCode[2];
    } else {
      color[0] = ColorCode[0];
      color[1] = ColorCode[1];
    }

    int number = strtol(color, NULL, 16);
    return number;
  }

  int GetBfromHTML(char* ColorCode) {
    char color[2];
    
    if (ColorCode[0] == 35) {
      color[0] = ColorCode[3];
      color[1] = ColorCode[4];
    } else {
      color[0] = ColorCode[2];
      color[1] = ColorCode[3];
    }

    int number = strtol(color, NULL, 16);
    return number;
  }

  int GetGfromHTML(char* ColorCode) {
    char color[2];
    
    if (ColorCode[0] == 35) {
      color[0] = ColorCode[5];
      color[1] = ColorCode[6];
    } else {
      color[0] = ColorCode[4];
      color[1] = ColorCode[5];
    }

    int number = strtol(color, NULL, 16);
    return number;
  }

 
