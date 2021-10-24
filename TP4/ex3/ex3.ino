#include <WiFiUdp.h>
#include <NTPClient.h> // si requete NTP
#include <HTTPClient.h>
const char *networkName = "iPhone de Romain"; // SSID de votre access point Wifi locale = 192.168.199.188
const char *networkPswd = "password";
const char * ntpAdress = "80.67.184.129"; // si reseau local mettre adresse locale
const int ntpOffset = 7200;                // UTC+2 summer time (3600 x 2). Set to 3600 if winter time
const unsigned int ntpInterval = 60*1000;  // refresh interval
WiFiUDP ntpUDP;
// instanciation d'un objet de type NTPClient
NTPClient timeClient(ntpUDP, ntpAdress, ntpOffset, ntpInterval);

void setup()
{
  Serial.begin(115200);
  //DO NOT TOUCH
  // This is here to force the ESP32 to reset the WiFi and initialise correctly.
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  delay(1000);
  // End silly stuff !!
  // connect to Access Point with password
  WiFi.begin(networkName, networkPswd);
  // connect to Open Access Point without password
  //WiFi.begin(networkName);
  Serial.printf("Try connexion to %s\n", networkName);
  long signalStrength;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    signalStrength = WiFi.RSSI();
    Serial.printf("RSSi: %ld dBm\n",signalStrength);
  }
  // now connected to WiFi, got IP from DHCP
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  timeClient.update();
  analogSetPinAttenuation(A3, ADC_0db);
  esp_sleep_enable_timer_wakeup(1000000); 
}

float convert2Temp(int val) {
  // a écrire, à la fin TP. Pour l’instant, la valeur est transmise sans modification
  float v = (float)val/1088;
  float t = 0;
  t = (33-10*v)/(3.4*v);
  return t;
}

void loop()
{
  //String request = "http://192.168.199.188/collecte/collecte.php"; // ou alors collecte.php
  String request2 = "https://tp5.deno.dev/"; // ou alors collecte.php
  
  HTTPClient http;
  String nom = "?name=SIMON";
  //request = request + nom;
  request2 = request2 + nom;
  
  String formattedTime = timeClient.getFormattedTime();
  //request = request + "&time=" + formattedTime;
  request2 = request2 + "&time=" + formattedTime;

  int valA3 = analogRead(A3); // resultat de la conversion A->N, correspondant a la tension sur A3
  int valA4 = analogRead(A4); // resultat de la conversion A->N, correspondant a la tension sur A4
  float temp = convert2Temp(valA4);
  String valeurs = "&temp=" + String(temp) + "&lum=" + String(valA3);
 // request = request + valeurs;
  request2 = request2 + valeurs;
  //Serial.println(request);
  // mettre ici votre code pour completer la chaine de caractères request, rajoutez au moins name=NNNNNNN
  
  //http.begin(request);
  //int httpResponseCode = http.GET(); // send request
  //Serial.println(httpResponseCode);
  http.begin(request2);
  int httpResponseCode = http.GET(); // send request
  delay(1000);
  esp_deep_sleep_start();
}
