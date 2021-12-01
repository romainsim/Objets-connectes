class Bargraph {
  public:
    Bargraph(int, int);
    void Clear();
    void SetValue(unsigned char);
    void Inc();
    void Dec();
    void Mode(int);
  private:
    int _val, CLK, AB, modeB;
    const int _maxval;
};

Bargraph::Bargraph (int clk, int ab) {
  CLK = clk;
  AB = ab;
  _maxval = 8;
  modeB = 0;
}

void Bargraph::Clear () {
  digitalWrite(AB, LOW);
  for (int i = 0; i < _maxval; i++) {
    digitalWrite(CLK, HIGH);
    delay(50);
    digitalWrite(CLK, LOW);
    delay(50);
  }
}

void Bargraph::SetValue(unsigned char value) {
  int x = value - '0'; // on convertit le char en int
  if (x <= _maxval) { // on vérifie que x n'est pas supérieur à 8
    Clear(); // on met à 0 le bargraph
    _val = x; // on met à jour la valeur du bargraph
    if (modeB == 0) {
      digitalWrite(AB, HIGH);
      for (int i = 0; i < _val; i++) {
        digitalWrite(CLK, HIGH);
        delay(50);
        digitalWrite(CLK, LOW);
        delay(50);
      }
    } 
    else if (modeB == 1) {
      digitalWrite(AB, HIGH);
      for (int i = 0; i < _val; i++) {
        digitalWrite(CLK, HIGH);
        delay(50);
        digitalWrite(CLK, LOW);
        delay(50);
      }
      digitalWrite(AB, LOW);
      for (int i = 0; i < _val-1; i++) {
        digitalWrite(CLK, HIGH);
        delay(50);
        digitalWrite(CLK, LOW);
        delay(50);
      }
    }
  }
}

void Bargraph::Inc() {
  if (_val + 1 <= _maxval) {
    _val++;
    unsigned char v = '0' + _val; // on convertit la valeur (int) en char
    SetValue(v);
  }
}

void Bargraph::Dec() {
  if (_val - 1 >= 0) {
    _val--;
    unsigned char v = '0' + _val; // on convertit la valeur (int) en char
    SetValue(v);
  }
}

void Bargraph::Mode(int modes) {
  modeB = modes;
  Clear();
  unsigned char v = '0' + _val; // on convertit la valeur (int) en char
  SetValue(v);
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
