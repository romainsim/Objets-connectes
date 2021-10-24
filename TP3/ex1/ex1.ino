const unsigned char TABLE[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,0x67};
const unsigned int EMPTY_MASK = 0xFF;

void setup() {
 for(byte i = 12; i <= 18; i++) {
    pinMode(i,OUTPUT);
 }
}

void loop() {

  for(byte i = 0; i < 10; i++) {
    REG_WRITE(GPIO_OUT_W1TS_REG, EMPTY_MASK << 12);
    REG_WRITE(GPIO_OUT_W1TC_REG, TABLE[i] << 12);
    delay(1000);
  }
 
}
