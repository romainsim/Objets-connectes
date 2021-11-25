#include <WiFi.h>
#include <WebServer.h>
const char* ssid = "SIMON"; // Mettre le ssid de votre réseau Wifi
const char* password = "azerty"; // Mettre le mot de passe de votre réseau Wifi
WebServer server(80); // Creation dun objet de type serveur HTTP

  void handleRoot(){ // Callback declenché en cas de navigation vers la page dacceuil (root)
    const char PROGMEM readValeurHTML[] = R"(
                <!DOCTYPE html>
          <html lang="en">
          <head>
              <meta charset="UTF-8">
              <title>TEA</title>
          </head>
          <body>
          <p>Luminosité :</p>
          <div>
            <input type="range" name="rangeInput" min="0" max="4095" id="myRange">
          </div>
          <div>
            <input type="text" id="mtText" value="2000">
          </div>
          <p>Température :</p>
          <div>
            <input type="range" name="rangeTemp" min="0" max="4095" id="rangeTemp">
          </div>
          <div>
            <input type="text" id="tempText" value="2000">
          </div>
          </body>
          </html>
          <script>
            const input = document.querySelector('#mtText');
            const range = document.querySelector('#myRange');
            const tempI = document.querySelector('#rangeTemp');
            const tempT = document.querySelector('#tempText');
          
            const refresh = async () => {
              const value = (await getData()).split(';');
              input.value = value[0];
              range.value = value[0];
            tempI.value = value[1];
            tempT.value = value[1];
            };
          
            const getData = async () => {
              const res = await fetch('readValeur');
              return res.text();
            };
          
            setInterval(refresh, 1000);
          </script>
    )";
    server.send(200, "text/html", readValeurHTML); // Envoi de la page HTML
  }

  void handleAnalog() {
    const char PROGMEM analogHTML[] = R"(
              <html>
          <head>
            <meta charset="UTF-8" />
            <meta name="viewport" content="width=device-width, initial-scale=1" />
          </head>
          <style>
            body {
              display: flex;
              flex-direction: column;
              align-items: center;
              justify-content: center;
              width: 100vw;
              height: 100vh;
              margin: 0;
              padding: 0;
              background-color: rgba(0, 0, 0, 0.7);
            }
            #needle, #needle2 {
              width: 140px;
              height: 5px;
              background-color: red;
              transition: transform 1s ease;
              transform-origin: 100% 50%;
              border-bottom-left-radius: 100%;
              border-top-left-radius: 100%;
              margin: auto auto 0 auto;
              transform: translateX(-50%);
            }
        
            .circle {
              border-top-left-radius: 9999px;
              border-top-right-radius: 9999px;
              display: flex;
              width: 300px;
              height: 150px;
              background-color: black;
            }
        
            #input, #input2 {
              background-color: black;
              color: white;
              border: 0;
              width: 300px;
              text-align: center;
              padding: 9px;
              outline: 0;
              font-weight: bold;
              border-top: 2px solid rgba(255, 255, 255, 0.2);
            }
          </style>
          <body>
            <div class="circle">
              <div id="needle"></div>
            </div>
            <input id="input" value="0" type="text" />
          
          <div class="circle">
              <div id="needle2"></div>
            </div>
            <input id="input2" value="0" type="text" />
        
            <script>
              const input = document.querySelector('#input');
              const needle = document.querySelector('#needle');
            const tempI = document.querySelector('#input2');
            const tempT = document.querySelector('#needle2');
        
              const refresh = async () => {
                const value = (await getData()).split(';');
                input.value = value[0];
                needle.style.transform = `translateX(-50%) rotate(${
                  (value[0] * 180) / 4095
                }deg)`;
            input2.value = value[1];
                needle2.style.transform = `translateX(-50%) rotate(${
                  (value[1] * 180) / 50
                }deg)`;
              };
        
              const getData = async () => {
                const res = await fetch('readValeur');
                return res.text();
              };
        
        
              setInterval(refresh, 1000);
            </script>
          </body>
        </html>
    )";
    server.send(200, "text/html", analogHTML); // Envoi de la page HTML
  }
  
  void handleNotFound(){ // Callback declenché en cas de navigation vers une cible inconnue
    server.send(404, "text/plain","404: Not found");
  }

  void setup() {
    Serial.begin(115200);
    analogSetPinAttenuation(A3, ADC_0db); // eventuellement, pour le capteur de luminosite
    // ne rien faire sur l’entrée A4 : 1V = valeur 1088
    delay(1000);
    Serial.print("Setting AP (Access Point)...");
    WiFi.softAP(ssid, password); // Remove the password parameter, if you want the AP (Access Point) to be open
    //WiFi.softAP(ssid);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    server.on("/", handleRoot); // si navigation vers (page daccueil) appel du callback handleRoot
    server.on("/analog", handleAnalog);
    server.on("/readValeur", readValeur);
    server.onNotFound(handleNotFound); // si navigation vers cible inconnue, appel du callback handlenotFound
    server.begin(); // Initialisation du serveur web
    Serial.println("Serveur web actif");
  }
  float convert2Temp(int val) {
    float value = (float) val/1088;
    float result = (value - 0.25) / (0.028);
    return (result);
  }

  void readValeur() {
    String s = (String)analogRead(A3) + ";" + (String)convert2Temp(analogRead(A4));// ici effectuer la mesure, et formater la mesure comme une chaine de caractère s
    server.send(200, "text/plain", s); // renvoie chaine de caratère s comme texte pur
  }

  void loop() {
    int valA3 = analogRead(A3); // resultat de la conversion A->N, correspondant a la tension sur A3
    int valA4 = analogRead(A4); // resultat de la conversion A->N, correspondant a la tension sur A4
    float temp = convert2Temp(valA4);
    Serial.print("Temperature :");
    Serial.print(temp);
    Serial.print(" Luminosite : ");
    Serial.println(valA3);
    delay(1000);
    server.handleClient();
  }
