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
    }

    digitalWrite(LATCHPIN, HIGH);
    digitalWrite(displays[i], HIGH);
  }
  for(int j = 0; j < 4; j++){
    digitalWrite(displays[j], LOW);
  }
}

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
  }

  if((controller2[0] == 0) && (controller2[1] == 0) && (controller2[2] == 0) && (controller2[3] == 0)){
    viewMode = !viewMode;
  }
  
}                                                 

void freqDisplay(){
  static unsigned long displayTime;
  bool finish = false;
  static int num;
    if(!finish) {
      if(currentTime - displayTime > NOTE_PERIOD * 2){
        displayTime = currentTime;
        if(num == 0){
          controller2[0] = 3;
          controller2[1] = 9;
          controller2[2] = 2;
          controller2[3] = 0;        
        } else if (num == 1){
          controller2[0] = 9;
          controller2[1] = 2;
          controller2[2] = 3;
          controller2[3] = 0; 
        } else if (num == 2){
          controller2[0] = 1;
          controller2[1] = 6;
          controller2[2] = 2;
          controller2[3] = 0; 
        } else if (num == 3){
          controller2[0] = 0;
          controller2[1] = 3;
          controller2[2] = 1;
          controller2[3] = 0; 
        } else if (num == 4){
          controller2[0] = 6;
          controller2[1] = 9;
          controller2[2] = 1;
          controller2[3] = 0; 
          finish = true;
        }
        num++;
      }
    }else {
      num = 0;
      controller2[0] = 0;
      controller2[1] = 4;
      controller2[2] = 0;
      controller2[3] = 0;
      viewMode = true;
    }

} 
