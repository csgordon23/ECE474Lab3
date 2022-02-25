#define MUSIC_PLAYS   2
#define COUNT_DOWN    3000
#define SMILE_TIME    2000


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