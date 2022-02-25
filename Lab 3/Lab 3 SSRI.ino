

void sleep_474(int t){
  TaskListSSRI[t_curr].delay = t;
  TaskListSSRI[t_curr].state = STATE_SLEEPING;
}

void demo2SSRI() {
  static int t_curr = 0;
  SSRIInteruptFlag = 0;
  TaskListSSRI[0].ftpr = &task1;
  TaskListSSRI[1].ftpr = &task2;
  TaskListSSRI[0].state = STATE_READY;
  TaskListSSRI[1].state = STATE_READY;
  TaskListSSRI[0].delay = -1;
  TaskListSSRI[1].delay = -1;
  TaskListSSRI[2].ftpr = &delayReduction;
  TaskListSSRI[2].state = STATE_READY;
  TaskListSSRI[2].delay = NULL;
  TaskListSSRI[3].ftpr  = NULL;
  // int i = 0;
  while(1){
    while(TaskListSSRI[t_curr].ftpr != NULL){
      if(TaskListSSRI[t_curr].state == STATE_READY){
        TaskListSSRI[t_curr].state = STATE_RUNNING;
        //TaskListSSRI[t_curr].ftpr;
        *TaskListSSRI[t_curr].ftpr;
      }
      t_curr ++;
    }
    t_curr = 0;
  }
}


void delayReduction(){
  int i = 0;
  int j = 0;
  while(SSRIInteruptFlag == 0){
    j = j + 1;//Delay time until ready
  }
  while(TaskListSSRI[i].ftpr != NULL){
    TaskListSSRI[i].delay -= 2;
    i++;
    SSRIInteruptFlag = 0;
  }
}