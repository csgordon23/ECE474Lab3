void displayDigits(){
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      digitalWrite(displays[j], LOW);
    }
    digitalWrite(LATCHPIN, LOW);
    if(bitRead(controller[4],i)==1){
      shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, controller[i]);
    }else{
      shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, digits[controller[i]]);
    }
    digitalWrite(LATCHPIN, HIGH);
    digitalWrite(displays[i], HIGH);
  }
  for(int j = 0; j < 4; j++){
    digitalWrite(displays[j], LOW);
  }
}

void increment() {
  bool incrementValue = true;
  for (int i = 0; i < 4; i++){
    int val = int(controller[i]);
    if (incrementValue == true) {
      val++;
      incrementValue = false;
      if (val > 9) {
          controller[i] = 0;
          incrementValue = true;
      } else {
          controller[i] = byte(val);
      }
    }
  }
}                                                 

void displayDigitsReversed(){
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      digitalWrite(displays[j], LOW);
    }
    digitalWrite(LATCHPIN, LOW);
    if(bitRead(controller[4],i)==1){
      shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, controller[i]);
    }else{
      shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, reversedigits[controller[i]]);
    }
    digitalWrite(LATCHPIN, HIGH);
    digitalWrite(displays[i], HIGH);
  }
  for(int j = 0; j < 4; j++){
    digitalWrite(displays[j], LOW);
  }
}
