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

// void displayDigits(){
//   for(int i = 0; i < 4; i++){
//     for(int j = 0; j < 4; j++){
//       digitalWrite(displays[j], LOW);
//     }
//     digitalWrite(LATCHPIN, LOW);
//     if(task3_en && !task4_en){
//       if(bitRead(controller[4],i)==1){
//         shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, controller[i]);
//       }else{
//         shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, digits[controller[i]]);
//       }
//     } else if(!task3_en && task4_en) {
//       if(bitRead(controller2[4],i)==1){
//         shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, controller2[i]);
//       }else{
//         shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, digits[controller2[i]]);
//       }
//     }

//     digitalWrite(LATCHPIN, HIGH);
//     digitalWrite(displays[i], HIGH);
//   }
//   for(int j = 0; j < 4; j++){
//     digitalWrite(displays[j], LOW);
//   }
// }

// void increment() {
//   bool alterCount = true;
//   if(task3_en && !task4_en){
//     for (int i = 0; i < 4; i++){
//       int val = int(controller[i]);
//       if (alterCount == true) {
//         val++;
//         alterCount = false;
//         if (val > 9) {
//           controller[i] = 0;
//           alterCount = true;
//         } else {
//           controller[i] = byte(val);
//         }
//       }
//     }
//   } else if(!task3_en && task4_en){
//     for (int i = 0; i < 4; i++){
//       int val = int(controller2[i]);
//       if (alterCount == true) {
//         val--;
//         alterCount = false;
//         if (val < 0) {
//           controller2[i] = 9;
//           alterCount = true;
//         } else {
//           controller2[i] = byte(val);
//         }
//       }
//     }
//   }

//   if((controller2[0] == 0) && (controller2[1] == 0) && (controller2[2] == 0) && (controller2[3] == 0)){
//     viewMode = !viewMode;
//   }
  
// }                                                 

// void freqDisplay(){
//   static unsigned long displayTime;
//   bool finish = false;
//   static int num;
//     if(!finish) {
//       if(currentTime - displayTime > NOTE_PERIOD * 3){
//         displayTime = currentTime;
//         if(num == 0){
//           controller2[0] = 3;
//           controller2[1] = 9;
//           controller2[2] = 2;
//           controller2[3] = 0;        
//         } else if (num == 1){
//           controller2[0] = 9;
//           controller2[1] = 2;
//           controller2[2] = 3;
//           controller2[3] = 0; 
//         } else if (num == 2){
//           controller2[0] = 1;
//           controller2[1] = 6;
//           controller2[2] = 2;
//           controller2[3] = 0; 
//         } else if (num == 3){
//           controller2[0] = 0;
//           controller2[1] = 3;
//           controller2[2] = 1;
//           controller2[3] = 0; 
//         } else if (num == 4){
//           controller2[0] = 6;
//           controller2[1] = 9;
//           controller2[2] = 1;
//           controller2[3] = 0; 
//           finish = true;
//         }
//         num++;
//       }
//     }else {
//       num = 0;
//       controller2[0] = 0;
//       controller2[1] = 4;
//       controller2[2] = 0;
//       controller2[3] = 0;
//       viewMode = true;
//     }

// } 

void task5(){
  static unsigned long previousChange = 0;
  static int currentState = 0;
  if(task5_en){
    task1_en = 1;//Task 1 runs all the time

    if(currentState == 0 ) {
      if(currentTime - previousChange > MUSIC_PLAYS * NOTE_PERIOD * (sizeof(melody) / sizeof(int))) {
        task2_en = 0;
        currentState ++;
        previousChange = currentTime;
        //Turn on count down here
      } else {
        task2_en = 1;
      }
    }

    if(currentState == 1 && currentTime - previousChange > COUNT_DOWN){
      currentState ++;
      previousChange = currentTime;
      task2_en = 1;
    }

    if(currentState == 2 && currentTime - previousChange > NOTE_PERIOD * (sizeof(melody) / sizeof(int))){
      task2_en = 0;
      currentState ++;
      previousChange = currentTime;
      //Draw smiley here
    }

    if(currentState == 3 && currentTime - previousChange > SMILE_TIME){
      //Turn off smile
      currentState ++;
    }


  }

}