void initialize(timer_t &t){
  t.oldTime = millis();
  t.newTime = t.oldTime;
  t.intervalMS = 1000;
}

void resetState(state_t &s){
  s.tries = 0;
  s.mode = edit;
  s.bPos = 0;
  s.modeSwitch = false;
}

void resetLEDs(state_t &s){
  for(int i = 0; i < 4; i++){
    s.ledCol[0][i] = 0;
    s.ledCol[1][i] = 0;
  }
}

void browseCol(state_t &s){
  if(getRight() && s.bPos < 4){
    s.bPos++;        
  }
  if(getLeft() && s.bPos > 0){
    s.bPos--;
  }
}

void editCol(state_t &s){
  // set player LED colour
  while(!getSelect()){
    if(getRight() && s.ledCol[0][s.bPos] < 5){
      s.ledCol[0][s.bPos]++;
    }
    if(getLeft() && s.ledCol[0][s.bPos] > 0){
      s.ledCol[0][s.bPos]--;
    }
  updateLEDs(s);
  }
}

void endGameWin(){
  // lightshow when winning
}

void endGameLose(){
  // lightshow when loosing
}

