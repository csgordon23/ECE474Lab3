/**
 *  @file Lab_3_DDS.c
 *  
 *  This file contains methods needed for the DDS to function
 *
 */

/**
 * @brief Puts the function that called it to sleep state for t ms
 * 
 * Assigns t to delay of the TCB entry for the function where the sleep was called
 * Sets the state in the TCB to sleeping. This sleep method was used specifically
 * for DDS implementation.
 * 
 * @param t time(ms) for the function to sleep
 */
void sleep_474_DDS(int t){
  waitTimes[t_curr] = t;
  TaskListDDS[t_curr].state = STATE_SLEEPING;
}

/**
 * @brief Calls the next function in the task list
 * 
 * This function utilizes void(*functionPTR)() to call a function from the 
 * task list array that is initialized on startup.
 * 
 * @param void(*functionPTR)() a pointer to the next function in the TaskList
 */
void start_function(void(*functionPTR)()) {
  functionPTR();
}

/**
 * @brief Calls the DDS version of Task 1 LED on for 250ms off for 750
 * 
 * This function is the DDS version of Task 1 used in the task involving
 * function pointers and ISR inturrupts and can be added to the active 
 * task list in the startup method.
 * 
 * @param p leave empty, for start_function function
 */
void task1_DDS(void *p) {
  if(millis() % 1000 > 250){
    PORTL &= ~LED_REG_BIT;
  } else {
    PORTL ^= LED_REG_BIT;
    sleep_474_DDS(750);
  }
}

/**
 * @brief Calls the DDS version of Task 2, play melody array, then sleep for 4 seconds, then repeat.
 * 
 * This function is the DDS version of Task 2 used in the task involving
 * function pointers and ISR inturrupts and can be added to the active 
 * task list in the startup method.
 * 
 * @param p leave empty, for start_function function
 */
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

/**
 * @brief Calls the function of task 4, and task 2. Plan melody, sleep and countdown, repeat.
 * 
 * This function is the DDS function for Task 4 used in the task involving
 * function pointers and ISR inturrupts and can be added to the active 
 * task list in the startup method. This method requires that task4_en be true
 * and calls task2_DDS within the method. This also utilizes the same functions
 * found in task 3.
 * 
 * @param p leave empty, for start_function function
 */
void task4_DDS(void *p){
  static unsigned long segmentTime;
  displayDigits();
  if(millis() - segmentTime > COUNTER_INCREMENT) {
    segmentTime = millis();
    if(viewMode){
      increment();
    } else {
      TaskListDDS[1].state = STATE_READY;
      freqDisplay();
    }
  }
}

/**
 * @brief Calls the function of task 5, task 5_2, task_5_3, and showSmile. Play melody twice, sleep and countdown, play melody once, and smile (no repeat) with LED blinking indefinetly.
 * 
 * This function is the DDS function for Task 5 used in the task involving
 * function pointers and ISR inturrupts and can be added to the active 
 * task list in the startup method. This method requires that task5_en be true
 * and calls several other tasks within the method. This also utilizes the same functions
 * found in task 3. 
 * 
 * @param p leave empty, for start_function function
 */
void task5_DDS(void *p){
  static unsigned long segmentTime;
  static unsigned long smileTime;

  if(smile){
    TaskListDDS[4].state = STATE_READY;
    displayDigits();
    if(millis() - segmentTime > COUNTER_INCREMENT) {
      segmentTime = millis();
    }

    if(millis() - smileTime > 11000){
      smileTime = millis();
      smile = false;
    }
  }
}

/**
 * @brief Calls the function of task 5_2. Plays melody twice, sleep for 3 seconds, play melody once and quit
 * 
 * This function is the DDS function for Task 5_2 used in the task involving
 * function pointers and ISR inturrupts and can be added to the active 
 * task list in the startup method. This method requires that task5_en be true
 * and controls the behavior of the speaker, after playing twice in a row the
 * function is slept and only plays once more.
 * 
 * @param p leave empty, for start_function function
 */
void task5_2_DDS(void *p) {
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
    melodyPlayCount += 1;
    if((melodyPlayCount == 2) || (melodyPlayCount == 3)){
      sleep_474_DDS(3000);
      TaskListDDS[3].state = STATE_READY;
      if(melodyPlayCount == 3){
        smile = true;
        task_self_quit();
      }
    }
  }
}

/**
 * @brief Calls the function of task 5_3. begins a 3 second countdown on 7-segment display once and quits
 * 
 * This function is the DDS function for Task 5_3 used in the task involving
 * function pointers and ISR inturrupts and can be added to the active 
 * task list in the startup method. This method requires that task5_en be true
 * and controls the behavior of the 7-segment display by utilizing functions used
 * in task 3 
 * 
 * @param p leave empty, for start_function function
 */
void task5_3_DDS(void *p) {
  static unsigned long segmentTime;
  displayDigits();
  if(millis() - segmentTime > COUNTER_INCREMENT) {
    segmentTime = millis();
    if(viewMode){
      increment();
    } else {
      task_self_quit();
    }
  }
}


/**
 * @brief Calls the function of showSmile displays a smile on the 7 segment display for 2 seconds once then quits
 * 
 * This function is the DDS function for showSmile used in the task involving
 * function pointers and ISR inturrupts and can be added to the active 
 * task list in the startup method. This method requires that task5_en be true
 * and controls the behavior of the 7-segment display by setting the digits to
 * display a smile with values found in an array.
 * 
 * @param p leave empty, for start_function function
 */
void showSmile(void *p) {
  controller3[0] = 10;
  controller3[1] = 11;
  controller3[2] = 12;
  controller3[3] = 13;
  if(!smile){
    task_self_quit();
  }
}

/**
 * @brief Calls the function of task_self_quit which removes the active task from the task list
 * 
 * This function is called when we want the active task to remove itself from the
 * list of active tasks and place it in the deadList.
 * 
 */
void task_self_quit() {
  TaskListDDS[t_curr].state = STATE_DEAD;
  deadTasks[t_curr] = TaskListDDS[t_curr].ftpr;
  TaskListDDS[t_curr].ftpr = NULL;
}