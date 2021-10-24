const unsigned char TABLE[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,0x67};
const unsigned char BTN = 23;
const unsigned int EMPTY_MASK = 0xFF;
int count = 0;

void setup() {
 for(byte i = 12; i <= 18; i++) {
    pinMode(i,OUTPUT);
 }
 pinMode(BTN, INPUT_PULLUP);
}

void loop() {
  
  
  if(digitalRead(BTN) == LOW) {
    
    count++;
    if(count > 9) count = 0;
    delay(300);
  }

  REG_WRITE(GPIO_OUT_W1TS_REG, EMPTY_MASK << 12);
    REG_WRITE(GPIO_OUT_W1TC_REG, TABLE[count] << 12);

 
}
