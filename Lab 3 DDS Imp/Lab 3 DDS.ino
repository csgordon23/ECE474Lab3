void sleep_474_DDS(int t){
  waitTimes[t_curr] = t;
  TaskListDDS[t_curr].state = STATE_SLEEPING;
}

void start_function(void(*functionPTR)()) {
  functionPTR();
}

void task1_DDS(void *p) {
  if(millis() % 1000 > 250){
    PORTL &= ~LED_REG_BIT;
  } else {
    PORTL ^= LED_REG_BIT;
    sleep_474_DDS(750);
  }
}

void task2_DDS(void *p) {
  static unsigned long previousSpeakerTime;
  static int melodyIndex;

  if(millis() - previousSpeakerTime > NOTE_PERIOD){
    previousSpeakerTime = millis();
    OCR4A = freqConv(melody[melodyIndex]);
    melodyIndex ++;
  }

  if(melodyIndex == (sizeof(melody) / sizeof(int))){
    melodyIndex = 0;
    OCR4A = 0;
    sleep_474_DDS(4000);
  }

}

void task_self_quit() {
  TaskListDDS[t_curr].state = STATE_DEAD;
  deadTasks[t_curr] = TaskListDDS[t_curr].ftpr;
  TaskListDDS[t_curr].ftpr = NULL;
}


void taskRevive() {
  TaskListDDS[t_curr].state = STATE_READY;
}