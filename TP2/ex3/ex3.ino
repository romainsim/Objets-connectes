#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
const char * networkName = "iPhone de Romain";
const char * networkPswd = "password";
const char * ntpAdress = "80.67.184.129";  // OVH NTP server ntp.midway.ovh
const int ntpOffset = 7200;                // UTC+2 summer time (3600 x 2). Set to 3600 if winter time
const unsigned int ntpInterval = 60*1000;  // refresh interval
int cumul;
int cumul1, cumul2, cumul3;
int value;
int moyenne;

WiFiUDP ntpUDP;
// instanciation d'un objet de type NTPClient
NTPClient timeClient(ntpUDP, ntpAdress, ntpOffset, ntpInterval);

void setup() {
   Serial.begin(115200);
   //  DO NOT TOUCH
   //  This is here to force the ESP32 to reset the WiFi and initialise correctly.
   WiFi.disconnect(true);
   delay(1000);
   WiFi.mode(WIFI_STA);
   delay(1000);
   //  End WiFi init
   // connect to Access Point
   WiFi.begin(networkName, networkPswd);
   while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        long signalStrength = WiFi.RSSI(); 
        Serial.printf("RSSi: %ld dBm\n",signalStrength);  // U can use printf with the Serial class !!!
    }
    Serial.printf("WiFi connected, output NTP request\n");
    timeClient.begin();
    timeClient.update();    // force refresh
  value = 0;
  cumul1 = 0;
  cumul2 = 0;
  cumul3 = 0;
    // si tout ce passe bien, le serveur NTP renvoie la date et l'heure et l'horloge interne de l'ESP32 est reglée.
}

void loop() {
  String formattedTime = timeClient.getFormattedTime();
  Serial.println(formattedTime);
  delay(1000); // ajouter le code pour la mesure et la moyenne glissante
  
  cumul = 0;
  moyenne = 0;
  for(int count =0; count < 8 ; count++) // do 8 measures, compute average to reduce noise
  {
    value = analogRead(A2);
    delay(1); // short delay because A-D conversion need time
    cumul = cumul + value;
  }

  cumul = cumul >> 3; // c'est la moyenne, décallage de 3 donc divise par 2^3 = 8 car on a 8 valeurs
  moyenne = (cumul+cumul1+cumul2+cumul3)/4;
  Serial.print(cumul);
  Serial.print(";");
  Serial.print(cumul1);
  Serial.print(";");
  Serial.print(cumul2);
  Serial.print(";");
  Serial.print(cumul3);
  Serial.print(";");
  Serial.println(moyenne);
  cumul3 = cumul2;
  cumul2 = cumul1;
  cumul1 = cumul;
  
  delay(1000);
}
