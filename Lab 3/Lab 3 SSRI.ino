

void sleep_474(int t){
  TaskListSSRI[t_curr].delay = t;
  TaskListSSRI[t_curr].state = STATE_SLEEPING;
}

void task1_SSRI(void *p);
void task2_SSRI(void *p);


void demo2SSRI() {
  sFlag = 0;
  TaskListSSRI[0].ftpr = (void *) task1_SSRI;
  TaskListSSRI[0].state = STATE_READY;
  TaskListSSRI[0].delay = -1;
  TaskListSSRI[1].ftpr = (void *) task2_SSRI;
  TaskListSSRI[1].state = STATE_READY;
  TaskListSSRI[1].delay = -1;
  TaskListSSRI[2].ftpr = &schedule_sync;
  TaskListSSRI[2].state = STATE_READY;
  TaskListSSRI[2].delay = NULL;
  TaskListSSRI[3].ftpr  = NULL;
  while(1){
    currentTime = millis();
    while(TaskListSSRI[t_curr].ftpr != NULL){
      if(TaskListSSRI[t_curr].state == STATE_READY && TaskListSSRI[t_curr].state != STATE_RUNNING){
        TaskListSSRI[t_curr].state = STATE_RUNNING;
        start_function(TaskListSSRI[t_curr].ftpr);
      }
      t_curr ++;
      
    }
    t_curr = 0;
    TaskListSSRI[2].state = STATE_READY;//Ready to do schedule_sync
  }
}

void schedule_sync(){
  // Serial.println("Start of schedule_sync");
  int i = 0;
  int j = 0;
  while(sFlag == PENDING){
    j = j + 1;//Delay time until ready
  }
  while(TaskListSSRI[i].ftpr != NULL){
    if(TaskListSSRI[i].state == STATE_SLEEPING){
      TaskListSSRI[i].delay -= 2;
    }
    if(TaskListSSRI[i].delay <= 0){
      TaskListSSRI[i].state = STATE_READY;
    }
    i++;
    sFlag = PENDING;
  }
  // Serial.print("Task 2 delay: ");
  // Serial.println(TaskListSSRI[1].delay);
  sleep_474(0);
}

void task1_SSRI(void *p) {
  // Serial.println("Start of Task1");
  if(currentTime % 1000 < 250){
    PORTL |= LED_REG_BIT;
    sleep_474(250);
  } else {
    PORTL &= ~(LED_REG_BIT);
    sleep_474(750);
  }
}

/**
 * @brief Play melody array, then stop noise for 4 seconds, then repeat.
 * 
 * Task relies on SRRI scheduler.
 * 
 */
void task2_SSRI(void *p) {
  static unsigned long previousSpeakerTime;
  static int melodyIndex;
  static int spkrState = 1;//1 for playing, 0 for off
  // Serial.print("Start of Task2\n");

  if(spkrState && currentTime - previousSpeakerTime > NOTE_PERIOD){
    // Serial.print("Chaning note\n");
    previousSpeakerTime = currentTime;
    OCR4A = freqConv(melody[melodyIndex]);
    melodyIndex ++;
    if(melodyIndex == (sizeof(melody) / sizeof(int) + 1)){
      spkrState = 0;
      melodyIndex = 0;
      // Serial.print("Long Sleep\n");
    }
  } else if (!spkrState && currentTime - previousSpeakerTime > SPEAKER_OFF){
    spkrState = 1;
  }

  sleep_474(100);
}