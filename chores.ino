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

void resetLEDs(ledOut_t *h){  
  for(int x = 0; x < 0xF; x++){
    for(int i = 0; i < 4; i++){
      h[x].resLeds[i] = 0;
      h[x].plaLeds[i] = 0;
    }
  }
}

bool browseColumn(state_t &s){  
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

void editColor(state_t &s, ledOut_t *edit){
  bool change = true;
  
  while(!getSelect()){
    if(change){
      updateLEDs(s, edit);
      change = false;
    }
    if(getRight() && edit[s.tries].plaLeds[s.bPos] < 5){
      edit[s.tries].plaLeds[s.bPos]++;
      change = true;
    }
    if(getLeft() && edit[s.tries].plaLeds[s.bPos] > 0){
      edit[s.tries].plaLeds[s.bPos]--;
      change = true;
    }
  }
}

void newTry(byte pos, ledOut_t *h){
  for(byte i = 0; i < 4; i++){
    h[pos].plaLeds[i] = h[pos - 1].plaLeds[i]; 
  }
}

void popHistory(){
  // read from history
}

void endGameWin(){
  // lightshow when winning
}

void endGameLose(){
  // lightshow when loosing
}

