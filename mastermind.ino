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

const byte CC = 4;

const byte srData = 11;
const byte srLatch = 12;
const byte srClock = 13;
const byte srCC[CC] = {2, 3, 4, 5};
const byte bPinSel = 6;
const byte bPinLft = 7;
const byte bPinRgt = 8;

typedef struct timer_t{
  uint32_t oldTime;
  uint32_t newTime;
  uint32_t intervalMS;
};

Bounce bSel = Bounce();
Bounce bLft = Bounce();
Bounce bRgt = Bounce();


void setup(){
  pinMode(srData, OUTPUT);
  pinMode(srLatch, OUTPUT);
  pinMode(srClock, OUTPUT);
  for(int i = 0; i < CC; i++){
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
/* contrary to popular paradigm, this will not
 * be allowed to loop, as I am going more
 * structured in my approach to this project.*/
}
