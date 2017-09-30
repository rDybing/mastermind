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
  initialize(timer);
  resetState(state);
  
  while(true){
    resetState(state);
    resetLEDs(history);
    gameLoop(state, history);
  }
}

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
    if(getSelect()){
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
    if(getLeft()){
      if(s.hPos > 0){
        s.hPos--;
        change = true;        
      }
    }
    if(getRight()){
      if(s.hPos < s.tries){
        s.hPos++;
        change = true;
      }
    }
    if(getSelect()){
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
    if(getRight() && s.tries > 0){
      s.mode = browse;
      s.modeSwitch = true;
    }
    if(getSelect()){
      // get result
      s.tries++;
      newTry(s.tries, h);
      s.mode = edit;
      s.modeSwitch = true;
    }
  }
}