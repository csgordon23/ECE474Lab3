#define DIG1REG              DDB4
#define DIG1                 10
#define DIG2REG              DDB5
#define DIG2                 11
#define DIG3REG              DDB6
#define DIG3                 12
#define DIG4REG              DDB7
#define DIG4                 13
#define DATAREG              0x08
#define DATAPIN              5
#define LATCHREG             0x20
#define LATCHPIN             3
#define CLOCKREG             0x10
#define CLOCKPIN             2
#define COUNTER_INCREMENT    100

unsigned long int currentTime;

byte digits[10]= {0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xE6};
byte reversedigits[10]= {0xE6,0xFE,0xE0, 0xBE, 0xB6, 0x66, 0xF2, 0xDA, 0x60, 0xFC};
byte displays[4] = {DIG1, DIG2, DIG3, DIG4};
byte controller[5] = {0,0,0,0,0};
byte controller2[5] = {9,9,9,9,0};

bool viewMode = false;

void setup() {
    DDRB |= DIG1REG | DIG2REG | DIG3REG | DIG4REG;
    DDRE |= DATAREG | LATCHREG | CLOCKREG;
}

void loop() {
  currentTime = millis();
  task3();
  //task4();
}

void task4(){
  static unsigned long segmentTime;
  displayDigitsReversed();
  if(currentTime - segmentTime > COUNTER_INCREMENT) {
    segmentTime = currentTime;
    increment();
  }
}

void task3(){
  static unsigned long segmentTime;
  displayDigits();
  if(currentTime - segmentTime > COUNTER_INCREMENT){
    segmentTime = currentTime;
    increment();
  } 
}
