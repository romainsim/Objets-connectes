int cumul;
int cumul1, cumul2, cumul3;
int value;
int moyenne;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  value = 0;
  cumul1 = 0;
  cumul2 = 0;
  cumul3 = 0;
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  cumul = 0;
  moyenne = 0;
  for(int count =0; count < 8 ; count++) // do 8 measures, compute average to reduce noise
  {
    value = analogRead(A0);
    delay(1); // short delay because A-D conversion need time
    cumul = cumul + value;
  }

  cumul = cumul >> 3; // c'est la moyenne, décallage de 3 donc divise par 2^3 = 8 car on a 8 valeurs
  moyenne = (cumul+cumul1+cumul2+cumul3)/4;
  if (moyenne < 800)
    digitalWrite(LED_BUILTIN, HIGH);
  else if (moyenne > 1400)
    digitalWrite(LED_BUILTIN, LOW);
  else
    digitalWrite(LED_BUILTIN, LOW);
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
