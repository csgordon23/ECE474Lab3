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
#define NOTE_PERIOD          200

unsigned long int currentTime;
bool task1_en, task2_en, task3_en, task4_en, task5_en;

byte digits[10]= {0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xE6};
byte reversedigits[10]= {0xE6,0xFE,0xE0, 0xBE, 0xB6, 0x66, 0xF2, 0xDA, 0x60, 0xFC};
byte displays[4] = {DIG1, DIG2, DIG3, DIG4};
byte controller[5] = {0,0,0,0,0};
byte controller2[5] = {0,4,0,0,0};

void task3();
void task4();
void displayDigits();
void increment();
void freqDisplay();

bool viewMode = true;

void setup() {
    DDRB |= DIG1REG | DIG2REG | DIG3REG | DIG4REG;
    DDRE |= DATAREG | LATCHREG | CLOCKREG;
    task4_en = 1;
    task3_en = 0;
}

void loop() {
  currentTime = millis();
  task3();
  task4();
}

void task4(){
  static unsigned long segmentTime;
  displayDigits();
  if(currentTime - segmentTime > COUNTER_INCREMENT) {
    segmentTime = currentTime;
    if(viewMode){
      increment();
    } else {
      freqDisplay();
    }
  }
}

void task3(){
  static unsigned long segmentTime;
  if(task3_en){
    displayDigits();
    if(currentTime - segmentTime > COUNTER_INCREMENT){
      segmentTime = currentTime;
      increment();
    } 
  }

}

void displayDigits(){
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      digitalWrite(displays[j], LOW);
    }
    digitalWrite(LATCHPIN, LOW);
    if(task3_en && !task4_en){
      if(bitRead(controller[4],i)==1){
        shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, controller[i]);
      }else{
        shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, digits[controller[i]]);
      }
    } else if(!task3_en && task4_en) {
      if(bitRead(controller2[4],i)==1){
        shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, controller2[i]);
      }else{
        shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, digits[controller2[i]]);
      }
    }

    digitalWrite(LATCHPIN, HIGH);
    digitalWrite(displays[i], HIGH);
  }
  for(int j = 0; j < 4; j++){
    digitalWrite(displays[j], LOW);
  }
}

void increment() {
  bool alterCount = true;
  if(task3_en && !task4_en){
    for (int i = 0; i < 4; i++){
      int val = int(controller[i]);
      if (alterCount == true) {
        val++;
        alterCount = false;
        if (val > 9) {
          controller[i] = 0;
          alterCount = true;
        } else {
          controller[i] = byte(val);
        }
      }
    }
  } else if(!task3_en && task4_en){
    for (int i = 0; i < 4; i++){
      int val = int(controller2[i]);
      if (alterCount == true) {
        val--;
        alterCount = false;
        if (val < 0) {
          controller2[i] = 9;
          alterCount = true;
        } else {
          controller2[i] = byte(val);
        }
      }
    }
  }

  if((controller2[0] == 0) && (controller2[1] == 0) && (controller2[2] == 0) && (controller2[3] == 0)){
    viewMode = !viewMode;
  }
  
}                                                 

void freqDisplay(){
  static unsigned long displayTime;
  bool finish = false;
  static int num;
    if(!finish) {
      if(currentTime - displayTime > NOTE_PERIOD * 3){
        displayTime = currentTime;
        if(num == 0){
          controller2[0] = 3;
          controller2[1] = 9;
          controller2[2] = 2;
          controller2[3] = 0;        
        } else if (num == 1){
          controller2[0] = 9;
          controller2[1] = 2;
          controller2[2] = 3;
          controller2[3] = 0; 
        } else if (num == 2){
          controller2[0] = 1;
          controller2[1] = 6;
          controller2[2] = 2;
          controller2[3] = 0; 
        } else if (num == 3){
          controller2[0] = 0;
          controller2[1] = 3;
          controller2[2] = 1;
          controller2[3] = 0; 
        } else if (num == 4){
          controller2[0] = 6;
          controller2[1] = 9;
          controller2[2] = 1;
          controller2[3] = 0; 
          finish = true;
        }
        num++;
      }
    }else {
      num = 0;
      controller2[0] = 0;
      controller2[1] = 4;
      controller2[2] = 0;
      controller2[3] = 0;
      viewMode = true;
    }

} 
