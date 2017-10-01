/*  Arduino Mastermind Game
 *  
 *  CC-BY Roy Dybing February-March 2017
 *  
 *  Writeup and schematics at:
 *  dybings.blogspot.no
 *  
 *  Source code:
 *  https://github.com/rDybing/mastermind
 */

#include <Bounce2.h>

const byte srData = 11;
const byte srLatch = 12;
const byte srClock = 13;
const byte srCC[4] = {2, 3, 4, 5};
const byte bPinSel = 8;
const byte bPinLft = 7;
const byte bPinRgt = 6; 

enum modeSel {edit, browse, commit, restart};


byte ledColors[6]{
  B00000001, // red
  B00000010, // green
  B00000011, // yellow
  B00000100, // blue  
  B00000101, // magenta
  B00000110  // cyan
};

typedef struct ledByte_t{
  byte player;  // bit 0-2
  byte code;    // bit 3-5
  bool tries;   // bit 6
  bool mode;    // bit 7
  byte output;  // all 8 bits
};

typedef struct ledOut_t{
  byte resLeds[4];
  byte tryLeds[4];
  byte plaLeds[4];
};

typedef struct timer_t{
  uint32_t oldTime;
  uint32_t newTime;
  uint32_t intervalMS;
};

typedef struct state_t{
  byte tries;
  byte bPos;
  byte hPos;
  bool modeSwitch;
  byte blinkLed[4]; // 0: off || 1: slow || 2: fast || 3: solid
  modeSel mode;
};

Bounce bSel = Bounce();
Bounce bLft = Bounce();
Bounce bRgt = Bounce();

// ******************************** Arduino Functions *********************

void setup(){  
  pinMode(srData, OUTPUT);
  pinMode(srLatch, OUTPUT);
  pinMode(srClock, OUTPUT);
  for(int i = 0; i < 4; i++){
    pinMode(srCC[i], OUTPUT);
  }
  pinMode(bPinSel,  INPUT_PULLUP);
  pinMode(bPinLft,  INPUT_PULLUP);
  pinMode(bPinRgt,  INPUT_PULLUP);
  bSel.attach(bPinSel);
  bLft.attach(bPinLft);
  bRgt.attach(bPinRgt);

  Serial.begin(9600);
}

void loop(){  
  ledOut_t history[0xF];
  timer_t timer;
  state_t state;
  initialize(timer, 1000);
  resetState(state);
  
  while(true){
    resetState(state);
    resetLEDs(history);
    gameLoop(state, history);
  }
}

// ******************************** Main Logic ****************************

void gameLoop(state_t &s, ledOut_t *h){    
  while(s.mode != restart){
    switch(s.mode){
    case edit:
      Serial.println("mode: edit");
      s.bPos = 0;
      s.hPos = s.tries; 
      modeEdit(s, h);
      s.mode = (s.tries > 0) ? browse : commit;
      s.modeSwitch = false;
      break;
    case browse:
      Serial.println("mode: browse");
      modeBrowse(s, h);
      s.modeSwitch = false;
      break;
    case commit:
      Serial.println("mode: commit");
      modeCommit(s, h);
      s.modeSwitch = false;
      break;      
    }
    if(s.tries == 0xF){
      endGameLose();
      s.mode = restart;
    }
  }
}

void modeEdit(state_t &s, ledOut_t *h){
  bool change = true;
    
  while(!s.modeSwitch){
    if(change){
      updateLEDs(s, h);
      change = false;
    }        
    change = browseColumn(s);        
    if(getButton(bSel)){
      if(s.bPos == 4){
        s.modeSwitch = true;
      } else {
        editColor(s, h);
      }
    }
  }
}

void modeBrowse(state_t &s, ledOut_t *h){  
  bool change = true;
  ledOut_t ht;
  s.hPos = s.tries;
  
  while(!s.modeSwitch){
    if(change){
      updateLEDs(s, h);
      change = false;
    }
    if(getButton(bLft)){
      if(s.hPos > 0){
        s.hPos--;
        change = true;        
      }
    }
    if(getButton(bRgt)){
      if(s.hPos < s.tries){
        s.hPos++;
        change = true;
      }
    }
    if(getButton(bSel)){
      if(s.hPos != s.tries){
        s.mode = edit;
        s.modeSwitch = true;
      } else {
        s.mode = commit;
        s.modeSwitch = true;
      }         
    }
  }
}

void modeCommit(state_t &s, ledOut_t *h){
  while(!s.modeSwitch){
    if(getButton(bRgt) && s.tries > 0){
      s.mode = browse;
      s.modeSwitch = true;
    }
    if(getButton(bSel)){
      // get result
      s.tries++;
      newTry(s.tries, h);
      s.mode = edit;
      s.modeSwitch = true;
    }
  }
}

// ******************************** In/Out ********************************

// Set column to edit
bool browseColumn(state_t &s){  
  if(getButton(bRgt) && s.bPos < 4){
    s.bPos++;
    return true;        
  }
  if(getButton(bLft) && s.bPos > 0){
    s.bPos--;
    return true;
  }
  return false;
}

// Set player color choice on selected column
void editColor(state_t &s, ledOut_t *edit){
  bool change = true;
  
  while(!getButton(bSel)){
    if(change){
      updateLEDs(s, edit);
      change = false;
    }
    if(getButton(bRgt) && edit[s.tries].plaLeds[s.bPos] < 5){
      edit[s.tries].plaLeds[s.bPos]++;
      change = true;
    }
    if(getButton(bLft) && edit[s.tries].plaLeds[s.bPos] > 0){
      edit[s.tries].plaLeds[s.bPos]--;
      change = true;
    }
  }
}

// get if button have been pressed
bool getButton(Bounce b){  
  if(b.update() && b.read() == LOW){    
    return true;
  }
  return false;
}

void updateLEDs(state_t &s, ledOut_t *out){  
  byte strSize = 60;
  char strPlayerOut[strSize];
  char strResultOut[strSize];
  byte t = s.hPos;

  snprintf(strResultOut, strSize, "Result: Try: %02d * ledCol[1] - %d *  %d *  %d *  %d\n", 
    t, out[t].resLeds[0], out[t].resLeds[1], out[t].resLeds[2], out[t].resLeds[3]); 
  snprintf(strPlayerOut, strSize, "Player: bPos: %d * ledCol[0] - %d *  %d *  %d *  %d\n", 
    s.bPos, out[t].plaLeds[0], out[t].plaLeds[1], out[t].plaLeds[2], out[t].plaLeds[3]);
  Serial.print(strResultOut);
  Serial.print(strPlayerOut);  
}

// ******************************** Chores ********************************

void initialize(timer_t &t, uint32_t freq){  
  t.oldTime = millis();
  t.newTime = t.oldTime;
  t.intervalMS = freq;
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