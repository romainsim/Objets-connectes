class Bargraph {
  public:
    Bargraph(int, int, int);
    void Clear();
    void SetValue(unsigned int);
    void Inc();
    void Dec();
    void SetMode(unsigned int);
  private:
    int _val, CLK, AB, modeB;
    int _maxval;
};

Bargraph::Bargraph (int clk, int ab, int m) {
  CLK = clk;
  AB = ab;
  _maxval = 8;
  modeB = m;
  pinMode(clk, OUTPUT);
  pinMode(ab, OUTPUT);
}

void Bargraph::Clear () {
  digitalWrite(AB, LOW);
  for (int i = 0; i < _maxval; i++) {
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }
}

void Bargraph::SetValue(unsigned int value) {
  if (value <= _maxval && value >= 0) { // on vérifie que x n'est pas supérieur à 8
    Clear(); // on met à 0 le bargraph
    _val = value; // on met à jour la valeur du bargraph
    if (modeB == 0) {
      digitalWrite(AB, HIGH);
      for (int i = 0; i <= _val; i++) {
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
      }
    } 
    else if (modeB == 1) {
      digitalWrite(AB, HIGH);
      digitalWrite(CLK, HIGH);
      digitalWrite(CLK, LOW);
  
      digitalWrite(AB, LOW);
      for (int i = 0; i < _val; i++) {
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
      }
    }
  }
}

void Bargraph::Inc() {
  if (_val + 1 <= _maxval) {
    _val++;
    SetValue(_val);
  }
}

void Bargraph::Dec() {
  if (_val - 1 >= 0) {
    _val--;
    SetValue(_val);
  }
}

void Bargraph::SetMode(unsigned int modes) {
  modeB = modes;
  Clear();
  SetValue(_val);
}

/*void BargraphClear() {
  digitalWrite(AB, LOW);
  for (int i = 0; i < 8; i++) {
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }
}

void SetValue(unsigned char value) {
  int _val = value - '0';
  BargraphClear();
  digitalWrite(AB, HIGH);
  digitalWrite(CLK, HIGH);
  digitalWrite(CLK, LOW);
  
  digitalWrite(AB, LOW);
  for (int i = 0; i < _val; i++) {
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }
}*/

Bargraph bargraph(13,12,1);
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  bargraph.SetMode(1);
  for (int i = 0; i < 8; i++) {
    bargraph.SetValue(i);
    delay(100);
  }

  for (int i = 7; i >= 0; i--) {
    bargraph.SetValue(i);
    delay(100);
  }

  bargraph.SetMode(0);
  for (int i = 0; i < 8; i++) {
    bargraph.SetValue(i);
    delay(100);
  }

  for (int i = 7; i >= 0; i--) {
    bargraph.SetValue(i);
    delay(100);
  }
}
