long f1,f2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  f1 = 1;
  f2 = 1;
}

void loop() {
  // put your main code here, to run repeatedly:
  long nb;
  Serial.setTimeout(5000); // attente de 5sec max pour Serial.parseInt, puis on passe à la suite
  do {
    nb = Serial.parseInt(); // Serial.parseInt detecte les caractères qui ne sont pas des chiffres
    if (Serial.read() == '\n') // si le caractère est un retour à la ligne (touche ‘‘entrée’’)
      break; // alors on quitte la boucle
  } while(Serial.available() > 0); // tant qu’il y a des caractères dans le buffer
  Serial.println(nb);
  f1 = 1;
  f2 = 1;
  long value = 0;
  for(int i = 0; i < nb; i++) {
    value = f1 + f2;
    f2 = f1;
    f1 = value;
    Serial.println(value);
    delay(500); 
  }
}
