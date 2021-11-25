#include <WiFiUdp.h>
// #include <NTPClient.h> // si requete NTP
#include <HTTPClient.h>
#include <SPI.h>
#include <PubSubClient.h>
const char *networkName = "OCPM"; // SSID de votre access point Wifi
const char* mqtt_server = "192.168.199.188";
const char R = 21;
const char G = 17;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if ((char)payload[0] == 'r') {
    digitalWrite(G, 0);
    digitalWrite(R, 1);
  } else if ((char)payload[0] == 'v') {
    digitalWrite(G, 1);
    digitalWrite(R, 0);
  } else {
    digitalWrite(G, 1);
    digitalWrite(R, 1);
  }
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("meteo/test","test");
      // ... and resubscribe
      client.subscribe("inTopic");
      client.subscribe("meteo/vigilance");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
    Serial.begin(115200);
    analogSetPinAttenuation(A3, ADC_0db);
    //DO NOT TOUCH
    // This is here to force the ESP32 to reset the WiFi and initialise correctly.
    WiFi.disconnect(true);
    delay(1000);
    WiFi.mode(WIFI_STA);
    delay(1000);
    // End silly stuff !!
    // connect to Access Point with password
    //WiFi.begin(networkName, networkPswd);
    // connect to Open Access Point without password
    WiFi.begin(networkName);
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

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    pinMode(R, OUTPUT);
    pinMode(G, OUTPUT);

    digitalWrite(G, 1);
}

float convert2Temp(int val) {
    float value = (float) val/1088;
    float result = (value - 0.25) / (0.028);
    return (result);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  /*int lum = analogRead(A3);
  float temp = convert2Temp(analogRead(A4));
  char luminosity[5];
  sprintf(luminosity, "%d", lum);
  char temperature[10];
  sprintf(temperature, "%f", temp);
  client.publish("meteo/tp2/simon/temperature",temperature);
  client.publish("meteo/tp2/simon/luminosity",luminosity);
  Serial.print("Temperature :");
  Serial.print(temperature);
  Serial.print(" Luminosite : ");
  Serial.println(luminosity);
  delay(5000);*/
}
