// get if button have been pressed
bool getSelect(){
  if(bSel.update() && bSel.read() == LOW){    
    return true;
  } else {
    return false;
  }
}

bool getLeft(){
  if(bLft.update() && bLft.read() == LOW){    
    return true;
  } else {
    return false;
  }
}

bool getRight(){
  if(bRgt.update() && bRgt.read() == LOW){    
    return true;
  } else {
    return false;
  }
}

void updateLEDs(state_t &s){
  byte strSize = 120;
  char strOut[strSize];
  snprintf(strOut, strSize, "bPos: %d || ledCol - 1: %d 2:%d 3: %d 4: %d\n", s.bPos, s.ledCol[0][0], s.ledCol[0][1], s.ledCol[0][2], s.ledCol[0][3]);
  Serial.print(strOut);  
}

