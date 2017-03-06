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

