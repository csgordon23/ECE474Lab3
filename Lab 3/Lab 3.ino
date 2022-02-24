/* University of Washington
 * ECE/CSE 474,  01/25/2022
 *
 *   Christian Gordon
 *   Marc Hernandez
 *
 */

#define STATE_OFF           0
#define STATE_ON            1

//Defines for Part 1
#define LED_EXTERN_PIN      47
#define LED_REG_BIT         0X04
#define LED_ON_TIME         250
#define LED_OFF_TIME        750
//Defines for Part 2       
#define NOTE_PERIOD         200
#define SPEAKER_OFF         4000
#define OC4A_PIN            DDH3

#define STATE_READY         0
#define STATE_RUNNING       1
#define STATE_SLEEPING      2

//Defines for Part 3 and 4
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

unsigned long currentTime;

bool task1_en, task2_en, task3_en, task4_en, task5_en;
bool viewMode = true;

//Holds the melody for strange encounters theme
int melody[] = {293, 0, 329, 0, 261, 0, 130, 0, 196, 0};
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


int t_curr;//Points to active task in the TaskList

void setup() {
  Serial.begin(9600);
  
  //Part 1 Setups
  DDRL |= LED_REG_BIT;

  //Part 2
  // Initialize timer 4 register A & B to 0 to clear values
  TCCR4A = 0; 
  TCCR4B = 0;

  // Stetting registers to proper settings for Part 2
  TCCR4A |= 1 << COM4A0;
  TCCR4B |= 1 << WGM42;
  TCCR4B |= 1 << CS40; //16MHz clock
  OCR4A = 0; // Initially 0 

  // Setting output mode for all pins
  DDRH |= 1 << OC4A_PIN;

  task1_en = 1;
  task2_en = 1;


  //TCBStruct TaskList[10];//Create TCB list

  // TaskList[0].ftpr = task1();
  // TaskList[0].arg_ptr = NULL;
  // TaskList[0].state = STATE_READY;

  //Part 3 Setups
    DDRB |= DIG1REG | DIG2REG | DIG3REG | DIG4REG;
    DDRE |= DATAREG | LATCHREG | CLOCKREG;
    task4_en = 0;
    task3_en = 1;

}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();
  //task1();
  //task2();
  task3();
  //task4();

}

double freqConv (int inputFreq){
  float divdend = 2 * 1 * inputFreq;
  float divsor = 16000000;
  float result = divsor / divdend;
  Serial.print("Result:\n");
  Serial.print(result);
  return (double) (result - 1.0);
}

// typedef struct TCBStruct{
//   void (*ftpr) (void *p);
//   void *arg_ptr;
//   unsigned short int state;
//   unsigned int delay;
// };

// void halt_me(){
//   TaskList[t_curr].state = STATE_READY;
// }

// void start_task(int taskID){
//   TaskList[taskID].state = STATE_RUNNING;
// }

// void delay(int d){
//   TaskList[t_curr].delay = d;
//   TaskList[t_curr].state = STATE_SLEEPING;
// }
