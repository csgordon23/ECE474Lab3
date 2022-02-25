void start_function(void(*functionPTR())) {
  functionPTR();
}

// void task1_DDS(void *p) {
//   if(millis() % 1000 > 250){
//     PORTL &= ~(LED_REG_BIT);
//   } else {
//     PORTL |= LED_REG_BIT;
//     sleepfunc(750);
//   }
// }

// void task2_DDS(void *p) {
//   static unsigned long previousSpeakerTime;
//   static int currentState = STATE_ON;//1=>Music playing. 0=> No sound
//   static int melodyIndex;

//   if(task2_en){
//     if(currentState){//Speaker is playing Music
//       if(currentTime - previousSpeakerTime > NOTE_PERIOD){
//         previousSpeakerTime = currentTime;
//         OCR4A = freqConv(melody[melodyIndex]);
//         melodyIndex ++;
//         if(melodyIndex == (sizeof(melody) / sizeof(int))){
//           melodyIndex = 0;
//           currentState = STATE_OFF;//Turn off speaker
//         }
//       }
//     } else {//Speaker is not making sound
//       if(currentTime - previousSpeakerTime > SPEAKER_OFF){
//         previousSpeakerTime = currentTime;
//         currentState = STATE_ON;
//       } 
//     }
// }