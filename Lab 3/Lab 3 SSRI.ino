

void sleep_474(int t){
  TaskListSSRI[t_curr].delay = t;
  TaskListSSRI[t_curr].state = STATE_SLEEPING;
}

void task1_SSRI(void *p);
void task2_SSRI(void *p);


void demo2SSRI() {
  // static int t_curr = 0;
  SSRIInteruptFlag = 0;
  TaskListSSRI[0].ftpr = (void *) task1_SSRI;
  // TaskListSSRI[1].ftpr = (void *) task2_SSRI;
  TaskListSSRI[0].state = STATE_READY;
  // TaskListSSRI[1].state = STATE_READY;
  TaskListSSRI[0].delay = -1;
  // TaskListSSRI[1].delay = -1;
  TaskListSSRI[1].ftpr = &delayReduction;
  TaskListSSRI[1].state = STATE_READY;
  TaskListSSRI[1].delay = NULL;
  TaskListSSRI[2].ftpr  = NULL;
  while(1){
    while(TaskListSSRI[t_curr].ftpr != NULL){
      if(TaskListSSRI[t_curr].state == STATE_READY && TaskListSSRI[t_curr].state != STATE_RUNNING){
        // Serial.print("\n Testing a Task");
        TaskListSSRI[t_curr].state = STATE_RUNNING;
        start_function(TaskListSSRI[t_curr].ftpr);
      }
      t_curr ++;
      // Serial.print("\nt_curr has been incremented");
      
    }
    t_curr = 0;
    TaskListSSRI[1].state = STATE_READY;
    // Serial.print("\n End of Testing for this period");
  }
}

void delayReduction(){
  int i = 0;
  int j = 0;
  while(SSRIInteruptFlag == 0){
    j = j + 1;//Delay time until ready
  }
  while(TaskListSSRI[i].ftpr != NULL){
    // Serial.print("\nChecking function ");
    // Serial.print(i);
    // Serial.print("\nState is ");
    // Serial.print(TaskListSSRI[i].state);
    // Serial.print("\n");
    if(TaskListSSRI[i].state == STATE_SLEEPING){
      TaskListSSRI[i].delay -= 2;
      // Serial.print("Delay is:");
      // Serial.print(TaskListSSRI[i].delay);
    }


    if(TaskListSSRI[i].delay <= 0){
      TaskListSSRI[i].state = STATE_READY;
    }
    i++;
    SSRIInteruptFlag = 0;
  }
  // Serial.print("\nEnd of DelayReduction\n");
  sleep_474(0);
}

void task1_SSRI(void *p) {
  if(millis() % 1000 < 250){
    // Serial.print("Turning on LED\n");
    PORTL |= LED_REG_BIT;
    sleep_474(248);
  } else {
    // Serial.print("Turning off LED\n");
    PORTL &= ~(LED_REG_BIT);
    sleep_474(748);
  }

  // sleep_474(50);
}

/**
 * @brief Play melody array, then stop noise for 4 seconds, then repeat.
 * 
 * Task relies on taskB_en signal being true, either set by the scheduler or setup.
 * 
 */
void task2_SSRI(void *p) {
  static unsigned long previousSpeakerTime;
  static int melodyIndex;

  if(currentTime - previousSpeakerTime > NOTE_PERIOD){
    previousSpeakerTime = currentTime;
    OCR4A = freqConv(melody[melodyIndex]);
    melodyIndex ++;

    if(melodyIndex == (sizeof(melody) / sizeof(int))){
      melodyIndex = 0;
      OCR4A = 0;
      sleep_474(4000);
    }
  }



}