void task1() {
  static unsigned long previousLEDToggle;
  static int currentState = STATE_ON; //1 for on, 0 for off

  if(task1_en){
    if(currentState){//LED is currently on
      if(currentTime - previousLEDToggle > LED_ON_TIME){
        currentState = STATE_OFF;
        previousLEDToggle = currentTime;
        PORTL &= ~LED_REG_BIT;
      }
    } else {//LED is currently off
      if(currentTime - previousLEDToggle > LED_OFF_TIME){
        currentState = STATE_ON;
        previousLEDToggle = currentTime;
        PORTL ^= LED_REG_BIT;
      }
    }
  }
}

/**
 * @brief Play melody array, then stop noise for 4 seconds, then repeat.
 * 
 * Task relies on taskB_en signal being true, either set by the scheduler or setup.
 * 
 */
void task2() {
  static unsigned long previousSpeakerTime;
  static int currentState = STATE_ON;//1=>Music playing. 0=> No sound
  static int melodyIndex;

  if(task2_en){
    if(currentState){//Speaker is playing Music
      if(currentTime - previousSpeakerTime > NOTE_PERIOD){
        previousSpeakerTime = currentTime;
        OCR4A = freqConv(melody[melodyIndex]);
        melodyIndex ++;
        if(melodyIndex == (sizeof(melody) / sizeof(int))){
          melodyIndex = 0;
          currentState = STATE_OFF;//Turn off speaker
        }
      }
    } else {//Speaker is not making sound
      if(currentTime - previousSpeakerTime > SPEAKER_OFF){
        previousSpeakerTime = currentTime;
        currentState = STATE_ON;
      } 
    }
  }


}