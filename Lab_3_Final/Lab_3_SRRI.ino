/**
 *  @file Lab_3_SRRI.c
 *  
 *  This file contains methods needed for the SRRI to function
 *
 */

/**
 * @brief Puts the function that called it to sleep state for t ms
 * 
 * Assigns t to delay of the TCB entry for the function where the sleep was called
 * Sets the state in the TCB to sleeping
 * 
 * @param t time(ms) for the function to sleep
 */
void sleep_474(int t){
  TaskListSSRI[t_curr].delay = t;
  TaskListSSRI[t_curr].state = STATE_SLEEPING;
}

void task1_SSRI(void *p); ///
void task2_SSRI(void *p); ///
void task3_SSRI(void *p); ///

/**
 * @brief Demo 2 for Lab 3, SRRI for task 1 and 2
 * 
 * Implements a SRRI scheduler for task 1 and 2.
 * 
 */
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

/**
 * @brief Demo 4 for Lab 3, SRRI for tasks 1 2 and 3
 * 
 * Sets up and runs an SRRI scheduler for tasks 1 2 and 3
 * 
 */
void demo4SSRI() {
  sFlag = 0;
  TaskListSSRI[0].ftpr = (void *) task1_SSRI;
  TaskListSSRI[0].state = STATE_READY;
  TaskListSSRI[0].delay = -1;
  TaskListSSRI[1].ftpr = (void *) task2_SSRI;
  TaskListSSRI[1].state = STATE_READY;
  TaskListSSRI[1].delay = -1;
  TaskListSSRI[2].ftpr = (void *) task3_SSRI;
  TaskListSSRI[2].state = STATE_READY;
  TaskListSSRI[2].delay = -1;
  TaskListSSRI[3].ftpr = &schedule_sync;
  TaskListSSRI[3].state = STATE_READY;
  TaskListSSRI[3].delay = NULL;
  TaskListSSRI[4].ftpr  = NULL;
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
    TaskListSSRI[3].state = STATE_READY;//Ready to do schedule_sync
  }
}

/**
 * @brief Schedule Synchronizer for the SRRI schedulers
 * 
 * Synchronizes the schedule on a period of 2ms. Also stalls any extra time away when other tasks are not running.
 * 
 */
void schedule_sync(){
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
  sleep_474(0);
}

/**
 * @brief Task1 in SRRI form.
 * 
 * Flashes LED on for 250ms then off for 750ms
 * 
 * @param p leave empty, for start_function function
 */
void task1_SSRI(void *p) {
  if(currentTime % 1000 < LED_ON_TIME){
    PORTL |= LED_REG_BIT;
    sleep_474(LED_ON_TIME);
  } else {
    PORTL &= ~(LED_REG_BIT);
    sleep_474(LED_OFF_TIME);
  }
}

/**
 * @brief Play melody array, then stop noise for 4 seconds, then repeat.
 * 
 * Task relies on SRRI scheduler.
 *
 * @param p leave empty, for start_function function 
 */
void task2_SSRI(void *p) {
  static unsigned long previousSpeakerTime;
  static int melodyIndex;
  static int spkrState = STATE_ON;
  // Serial.print("Start of Task2\n");

  if(spkrState && currentTime - previousSpeakerTime > NOTE_PERIOD){
    // Serial.print("Chaning note\n");
    previousSpeakerTime = currentTime;
    OCR4A = freqConv(melody[melodyIndex]);
    melodyIndex ++;
    if(melodyIndex == (sizeof(melody) / sizeof(int))){
      spkrState = STATE_OFF;
      melodyIndex = 0;
      // Serial.print("Long Sleep\n");
    }
  } else if (!spkrState && (currentTime - previousSpeakerTime > SPEAKER_OFF)){
    spkrState = STATE_ON;
    
  }
  sleep_474(100);
  
}

void task3_SSRI(void *p){
  static unsigned long segmentTime;
    displayDigits();
    if(currentTime - segmentTime > COUNTER_INCREMENT){
      segmentTime = currentTime;
      increment();
    }
    sleep_474(50);
}
