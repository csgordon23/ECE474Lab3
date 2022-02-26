/**
 *  @file Lab_3_7segment.c
 *  
 *  This file contains methods needed to the 7-segment display to function
 *
 */

/**
 * @brief Utilizes the a shift register to illuminate segments of the 7-segment display
 * 
 * This function loops through each of the digits on the 7-segment display and shifts
 * in bit values in the order of LSBFIRST. These values are found in the digits array
 * and the current digit needed to be displayed is found in the controllers array.
 * If task3_en, controller[5] is used, If task4_en controller2[5] is used, and if task5_en
 * contoller3[5] is used. Only one may be enabled at a time.
 * 
 */
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
    } else if(!task3_en && !task4_en && task5_en) {
      if(bitRead(controller3[4],i)==1){
        shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, controller3[i]);
      }else{
        shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, digits[controller3[i]]);
      }
    }

    digitalWrite(LATCHPIN, HIGH);
    digitalWrite(displays[i], HIGH);
  }
  for(int j = 0; j < 4; j++){
    digitalWrite(displays[j], LOW);
  }
}

/**
 * @brief This function increments of decrements the current count that will be displayed on the 7-segment display
 * 
 * These values are found in the digits array, If task3_en, controller[5] is updated, If task4_en controller2[5] is updated, 
 * and if task5_en contoller3[5] is updated. Only one may be enabled at a time. If task4_en or task5_en is true, then the 
 * function will behave differently and decrement the count from 4 second to 0 or 3 seconds to 0 respectively.
 * 
 */
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
  } else if(!task3_en && !task4_en && task5_en){
    for (int i = 0; i < 4; i++){
      int val = int(controller3[i]);
      if (alterCount == true) {
        val--;
        alterCount = false;
        if (val < 0) {
          controller3[i] = 9;
          alterCount = true;
        } else {
          controller3[i] = byte(val);
        }
      }
    }
  }
  if((controller2[0] == 0) && (controller2[1] == 0) && (controller2[2] == 0) && (controller2[3] == 0)){
    viewMode = false;
  } else if((controller3[0] == 0) && (controller3[1] == 0) && (controller3[2] == 0) && (controller3[3] == 0)){
    viewMode = false;
  }
  
}                                                 

/**
 * @brief This function is called in task4 and task5
 * 
 * These values are found in the digits array, If task3_en, controller[5] is updated, If task4_en controller2[5] is updated, 
 * and if task5_en contoller3[5] is updated. Only one may be enabled at a time. If task4_en or task5_en is true, then the 
 * function will behave differently and decrement the count from 4 second to 0 or 3 seconds to 0 respectively.
 * 
 */
void freqDisplay(){
  static unsigned long displayTime;
  static int num;
    if(millis() - displayTime > NOTE_PERIOD * 2){
      displayTime = millis();
      if(num == 0){
        controller2[0] = 3;
        controller2[1] = 9;
        controller2[2] = 2;
        controller2[3] = 0;
        num++;        
      } else if (num == 1){
        controller2[0] = 9;
        controller2[1] = 2;
        controller2[2] = 3;
        controller2[3] = 0;
        num++; 
      } else if (num == 2){
        controller2[0] = 1;
        controller2[1] = 6;
        controller2[2] = 2;
        controller2[3] = 0;
        num++;
      } else if (num == 3){
        controller2[0] = 0;
        controller2[1] = 3;
        controller2[2] = 1;
        controller2[3] = 0;
        num++;
      } else if (num == 4){
        controller2[0] = 6;
        controller2[1] = 9;
        controller2[2] = 1;
        controller2[3] = 0;
        num++;
        viewMode = true;
      }
    }
    
    if(num >= 5) {
      num = 0;
      controller2[0] = 0;
      controller2[1] = 4;
      controller2[2] = 0;
      controller2[3] = 0;
    }
} 
