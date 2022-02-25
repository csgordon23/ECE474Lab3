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
//*******************************
#define STATE_DEAD          3
//*******************************

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

//**********************************
#define NTASKS               10

//**********************************

unsigned long currentTime;

bool task1_en, task2_en, task3_en, task4_en, task5_en;
bool viewMode = true;
bool DDSMode = true;
bool smile = false;

int SSRIInteruptFlag;
//**********************
int melodyPlayCount = 0;
//**********************
//Holds the melody for strange encounters theme
int melody[] = {293, 0, 329, 0, 261, 0, 130, 0, 196, 0};
byte digits[14]= {0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xE6, 0x4A, 0x90, 0x90, 0x26};
byte reversedigits[10]= {0xE6,0xFE,0xE0, 0xBE, 0xB6, 0x66, 0xF2, 0xDA, 0x60, 0xFC};
byte displays[4] = {DIG1, DIG2, DIG3, DIG4};
byte controller[5] = {0,0,0,0,0};
byte controller2[5] = {0,4,0,0,0};
//*************************
byte controller3[5] = {0,3,0,0,0};
//*************************

void task1();
void task2();
void task3();
void task5();
//********************
void task1_DDS(void *p);
void task2_DDS(void *p);
void task4_DDS(void *p);
void task5_DDS(void *p);
void task5_2_DDS(void *p);
void showSmile(void *p);
void task_self_quit();
void start_function(void (*functionPTR)());
//********************
void displayDigits();
void increment();
void freqDisplay();


struct TCBSSRI;

typedef struct TCBSSRI{
  void (*ftpr) (int *p);
  unsigned short int state;
  unsigned int delay;
};

struct TCBSSRI TaskListSSRI[10];//Create TCB list

//*******************************
struct TCBDDS;

typedef struct TCBDDS {
    void (*ftpr)(void *p);
    unsigned short int state; 
    unsigned long int ID; 
    char* taskName; 
    unsigned int runtimes;
    unsigned int delay;
};

TCBDDS TaskListDDS[NTASKS];
void (*readyTasks[NTASKS])(void *p);
void (*deadTasks[NTASKS])(void *p);
int waitTimes[] = {0, 0, 0, 0 ,0};
int taskStates[] = {0, 0, 0, 0, 0};

//*******************************

int t_curr;//Points to active task in the TaskList

ISR(TIMER1_COMPA_vect){
    SSRIInteruptFlag = 1;
    //*******************************
    if(DDSMode){
      for(int i = 0; i < 5; i++){
        if(waitTimes[i] > 2) {
          waitTimes[i] -= 2;
        } else {
          waitTimes[i] = 0;
          if(TaskListDDS[i].state != STATE_DEAD){
            TaskListDDS[i].state = STATE_READY;
          }
        } 
      }
      SSRIInteruptFlag = 0;
    }
    //********************************
  }


void setup() {
  Serial.begin(9600);

  t_curr = 0;
  
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

  TCCR1A = 0;      //Reset entire TCCR1A register
  TCCR1B = 0;     //Reset entire TCCR1B register
  TCCR1A |= 0b01000000;
  TCCR1B |= 0b00001100;//Prescalar - 256
  TCNT1 = 0; 
  TIMSK1 |= B00000010;//enabling interrupts
  OCR1A = 125; // 16,0000,000 / 256 / 500Hz = 125 (500Hz because need interrupt tick every 2ms)

  // Setting output mode for all pins
  DDRH |= 1 << OC4A_PIN;

  task1_en = 1;
  task2_en = 1;

  //Part 3 Setups
    DDRB |= DIG1REG | DIG2REG | DIG3REG | DIG4REG;
    DDRE |= DATAREG | LATCHREG | CLOCKREG;
    task4_en = 0;
    task3_en = 0;
    task5_en = 1;
  //Demo 3 DDS Scheduler*************
  // TaskListDDS[0].ftpr = task1_DDS;
  // TaskListDDS[0].state = STATE_READY;
  // TaskListDDS[0].ID = 1;
  // TaskListDDS[0].taskName = "LED";
  // TaskListDDS[0].runtimes = 0;
  // TaskListDDS[0].delay = -1;

  // TaskListDDS[1].ftpr = task2_DDS;
  // TaskListDDS[1].state = STATE_READY;
  // TaskListDDS[1].ID = 2;
  // TaskListDDS[1].taskName = "Tone";
  // TaskListDDS[1].runtimes = 0;
  // TaskListDDS[0].delay = -1;

  // TaskListDDS[2].ftpr = NULL;
  //********************************

  //Demo 5 DDS Scheduler************
  // TaskListDDS[0].ftpr = task4_DDS;
  // TaskListDDS[0].state = STATE_READY;
  // TaskListDDS[0].ID = 1;
  // TaskListDDS[0].taskName = "Segs";
  // TaskListDDS[0].runtimes = 0;
  // TaskListDDS[0].delay = -1;

  // TaskListDDS[1].ftpr = task2_DDS;
  // TaskListDDS[1].state = STATE_DEAD;
  // TaskListDDS[1].ID = 2;
  // TaskListDDS[1].taskName = "Tone";
  // TaskListDDS[1].runtimes = 0;
  // TaskListDDS[0].delay = -1;

  // TaskListDDS[2].ftpr = NULL;
  //**********************************


  //Demo 6 DDS Scheduler**************
  TaskListDDS[0].ftpr = task5_DDS;
  TaskListDDS[0].state = STATE_READY;
  TaskListDDS[0].ID = 1;
  TaskListDDS[0].taskName = "Segs";
  TaskListDDS[0].runtimes = 0;
  TaskListDDS[0].delay = -1;

  TaskListDDS[1].ftpr = task1_DDS;
  TaskListDDS[1].state = STATE_READY;
  TaskListDDS[1].ID = 2;
  TaskListDDS[1].taskName = "LED";
  TaskListDDS[1].runtimes = 0;
  TaskListDDS[1].delay = -1;

  TaskListDDS[2].ftpr = task5_2_DDS;
  TaskListDDS[2].state = STATE_READY;
  TaskListDDS[2].ID = 3;
  TaskListDDS[2].taskName = "Tone";
  TaskListDDS[2].runtimes = 0;
  TaskListDDS[2].delay = -1;

  TaskListDDS[3].ftpr = task5_3_DDS;
  TaskListDDS[3].state = STATE_DEAD;
  TaskListDDS[3].ID = 4;
  TaskListDDS[3].taskName = "countdown";
  TaskListDDS[3].runtimes = 0;
  TaskListDDS[3].delay = -1;
  
  TaskListDDS[4].ftpr = showSmile;
  TaskListDDS[4].state = STATE_DEAD;
  TaskListDDS[4].ID = 4;
  TaskListDDS[4].taskName = "smile";
  TaskListDDS[4].runtimes = 0;
  TaskListDDS[4].delay = -1;

  TaskListDDS[5].ftpr = NULL;
  //*********************************
}

void loop() {
  currentTime = millis();

  //For DDS scheduler
  //**********************************************
  for(t_curr = 0; t_curr < NTASKS; t_curr++) {
   if((TaskListDDS[t_curr].ftpr != NULL) && (TaskListDDS[t_curr].state != STATE_SLEEPING) && (TaskListDDS[t_curr].state != STATE_DEAD)) {
     TaskListDDS[t_curr].state = STATE_RUNNING;
     start_function(TaskListDDS[t_curr].ftpr);   
    }
  }
  //**********************************************
}

double freqConv (int inputFreq){
  float divdend = 2 * 1 * inputFreq;
  float divsor = 16000000;
  float result = divsor / divdend;
  return (double) (result - 1.0);
}



