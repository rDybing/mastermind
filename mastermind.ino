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

byte ledColours[6]{
  1, // red
  2, // green
  3, // yellow
  4, // blue  
  5, // magenta
  6  // cyan
};

typedef struct timer_t{
  uint32_t oldTime;
  uint32_t newTime;
  uint32_t intervalMS;
};

typedef struct state_t{
  byte tries;
  modeSel mode;
  byte bPos;
  byte ledCol[2][4];
  bool modeSwitch;
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
  
  timer_t timer;
  state_t state;
  initialize(timer);
  resetState(state);
  
  while(true){
    resetState(state);
    resetLEDs(state);
    gameLoop(state);
  }
}

void gameLoop(state_t &s){
  bool change = true;
  while(s.mode != restart){
    switch(s.mode){
    case edit:
      Serial.println("mode: edit");
      while(!s.modeSwitch){
        if(change){
          updateLEDs(s);
          change = false;
        }        
        change = browseCol(s);        
        if(getSelect()){
          if(s.bPos == 4){
            s.modeSwitch = true;
          } else {
            editCol(s);
          }
        }
      }
      s.mode = browse;
      s.modeSwitch = false;
      break;
    case browse:
      Serial.println("mode: browse");
      while(!s.modeSwitch){
        // do stuff
      }
      break;
    case commit:
      Serial.println("mode: commit");
      while(!s.modeSwitch){
        // do stuff
      }
      break;      
    }
    if(s.tries == 0xF){
      endGameLose();
      s.mode = restart;
    }
  }
}

