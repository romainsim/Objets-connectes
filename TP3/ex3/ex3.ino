const unsigned char TABLE[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,0x67};
const unsigned char BTN = 23;
const unsigned int EMPTY_MASK = 0xFF;
int count = 0;
unsigned int temps = 0;
int appuyer = 0;

void setup() {
 for(byte i = 12; i <= 18; i++) {
    pinMode(i,OUTPUT);
 }
 pinMode(BTN, INPUT_PULLUP);
}

void loop() {
  
  temps = millis(); // temps avant appui bouton
  while(digitalRead(BTN) == LOW) { // tant que le bouton est appuyé on ne fait rien
    appuyer = 1; // permet d'assurer que le bouton a été appuyé
  }
  temps = millis() - temps; // compte la durée de pression du bouton.
  if (temps > 1000 && appuyer == 1) { // si la durée est superieure à 1000ms, on reset le compteur
    count = 0;
    delay(300); // empecher les rebonds
  } else if (temps <= 1000 && appuyer == 1) { // sinon on l'incrémente normalement et on verifie qu'il ne dépasse pas 9
    count++;
    if(count > 9) count = 0;
    delay(300); // empecher les rebonds
  }
  appuyer = 0;  
  REG_WRITE(GPIO_OUT_W1TS_REG, EMPTY_MASK << 12); // éteint tous les segments
  REG_WRITE(GPIO_OUT_W1TC_REG, TABLE[count] << 12); // allume les corrects segments par rapport au compteur
}
