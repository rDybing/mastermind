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

bool browseCol(state_t &s){
  if(getRight() && s.bPos < 4){
    s.bPos++;
    return true;        
  }
  if(getLeft() && s.bPos > 0){
    s.bPos--;
    return true;
  }
  return false;
}

void editCol(state_t &s){
  // set player LED colour
  bool change = true;
  while(!getSelect()){
    if(change){
      updateLEDs(s);
      change = false;
    }
    if(getRight() && s.ledCol[0][s.bPos] < 5){
      s.ledCol[0][s.bPos]++;
      change = true;
    }
    if(getLeft() && s.ledCol[0][s.bPos] > 0){
      s.ledCol[0][s.bPos]--;
      change = true;
    }
  }
}

void endGameWin(){
  // lightshow when winning
}

void endGameLose(){
  // lightshow when loosing
}

