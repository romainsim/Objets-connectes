#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
const char * networkName = "iPhone de Romain";
const char * networkPswd = "password";
const char * ntpAdress = "80.67.184.129";  // OVH NTP server ntp.midway.ovh
const int ntpOffset = 7200;                // UTC+2 summer time (3600 x 2). Set to 3600 if winter time
const unsigned int ntpInterval = 60*1000;  // refresh interval
WiFiUDP ntpUDP;
// instanciation d'un objet de type NTPClient
NTPClient timeClient(ntpUDP, ntpAdress, ntpOffset, ntpInterval);
const unsigned char TABLE[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,0x67};
const unsigned int EMPTY_MASK = 0xFF;

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
  
    // si tout ce passe bien, le serveur NTP renvoie la date et l'heure et l'horloge interne de l'ESP32 est reglée.
  for(byte i = 12; i <= 18; i++) {
    pinMode(i,OUTPUT);
  }
}

void loop() {
  esp_sleep_enable_timer_wakeup(800000);
  int seconds = timeClient.getSeconds()%10;
  REG_WRITE(GPIO_OUT_W1TC_REG, TABLE[seconds] << 12); // met à 0 seulement les bons segments du chiffre pour les allumer
  delay(200);
  REG_WRITE(GPIO_OUT_W1TS_REG, EMPTY_MASK << 12); // met tout à 1 donc efface tous les segments
  int ret = esp_light_sleep_start(); 
}
