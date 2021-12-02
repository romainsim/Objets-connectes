#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include "Web.h"
const char* ssid = "SIMON";
unsigned int reading;

WebServer server(80);

class Bargraph {
  public:
    Bargraph(int, int, int);
    void Clear();
    void SetValue(unsigned int);
    void Inc();
    void Dec();
    void SetMode(unsigned int);
  private:
    int _val, CLK, AB, modeB;
    int _maxval;
};

Bargraph::Bargraph (int clk, int ab, int m) {
  CLK = clk;
  AB = ab;
  _maxval = 8;
  modeB = m;
  pinMode(clk, OUTPUT);
  pinMode(ab, OUTPUT);
}

void Bargraph::Clear () {
  digitalWrite(AB, LOW);
  for (int i = 0; i < _maxval; i++) {
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }
}

void Bargraph::SetValue(unsigned int value) {
  if (value <= _maxval && value >= 0) { // on vérifie que x n'est pas supérieur à 8
    Clear(); // on met à 0 le bargraph
    _val = value; // on met à jour la valeur du bargraph
    if (modeB == 0) {
      digitalWrite(AB, HIGH);
      for (int i = 0; i <= _val; i++) {
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
      }
    } 
    else if (modeB == 1) {
      digitalWrite(AB, HIGH);
      digitalWrite(CLK, HIGH);
      digitalWrite(CLK, LOW);
  
      digitalWrite(AB, LOW);
      for (int i = 0; i < _val; i++) {
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
      }
    }
  }
}

void Bargraph::Inc() {
  if (_val + 1 <= _maxval) {
    _val++;
    SetValue(_val);
  }
}

void Bargraph::Dec() {
  if (_val - 1 >= 0) {
    _val--;
    SetValue(_val);
  }
}

void Bargraph::SetMode(unsigned int modes) {
  modeB = modes;
  Clear();
  SetValue(_val);
}

void handleRoot() {
  server.send(200, "text/html", mainPage);
}

void handleNotFound(){ // Callback declenché en cas de navigation vers une cible inconnue
    server.send(404, "text/plain","404: Not found");
}

Bargraph bargraph(13, 12, 0);
void setup() {
  Wire.begin(); // On ne specifie pas les broches, alors broches par defaut SCL=22, SDA=23
  Serial.begin(115200);

  Serial.print("Setting AP (Access Point)...");
  WiFi.softAP(ssid);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    server.on("/", handleRoot); // si navigation vers (page daccueil) appel du callback handleRoot
    server.on("/readValeur", readValeur);
    server.onNotFound(handleNotFound); // si navigation vers cible inconnue, appel du callback handlenotFound
    server.begin(); // Initialisation du serveur web
    Serial.println("Serveur web actif");
}

void readValeur() {
  Wire.beginTransmission(112); // initialisation d’une transmission vers le module à l’adresse #112 (0x70)
  Wire.write(byte(0x00)); // on specifie que l’on envoie une commande
  Wire.write(byte(0x51)); // commande 0x52 : effectuer une mesure du temps de vol (duree jusqu’à reception de l’echo)
  Wire.endTransmission(); // fin transmission
  delay(70); // Il est suggèré dans la doc d’attendre au moins 65 millisecondes que la mesure se fasse
  Wire.beginTransmission(112); // initialisation d’une transmission vers le module à l’adresse #112 (0x70)
  Wire.write(byte(0x02)); // on souhaite lire le registre correspondant à echo #1 (0x02)
  Wire.endTransmission(); // fin transmission
  Wire.requestFrom(112, 2); // demande de lecture : 2 octets de la part du module esclave #112
  
  if (2 <= Wire.available()) // 2 octets ont été reçus
  {
    reading = Wire.read(); // recuperation de l’octet MSB
    reading = reading << 8; // on décale
    reading |= Wire.read(); // recupération de l’octet LSB et on combine
    Serial.print(reading); // affichage valeur
    Serial.println("cm"); // affichage unite
  }
  int nb_led = 0;
  if (reading > 100) {
    bargraph.Clear();
  } else {
    nb_led = 10 - (0.1 * reading);
    if (nb_led == 0)
      nb_led++;
    else if (nb_led == 9)
      nb_led--;
    bargraph.SetValue(nb_led);
  }
  Serial.println(nb_led);
  String s = (String) (nb_led+1);
  server.send(200, "text/plain", s);
}

void loop() {
  server.handleClient();
}
