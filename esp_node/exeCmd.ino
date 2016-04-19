void serialClear(){
  for (unsigned int i=0; i<sizeof(serial_incmd); i++) {
    serial_incmd[i] = 0;
  }
  serial_incmd[0] = 32;
  serial_incmd_idx = 0;
}

void serialCore(){
  while (client.available()) {
    serial_inbyte = client.read();
    if (serial_incmd_idx >= sizeof(serial_incmd)){
      serial_incmd_idx = 0;
    }
    if (serial_inbyte == 10 ||serial_inbyte == 13 ) {
      serialExecute(serial_incmd);
      serialClear();
    }
    else{
      serial_incmd[serial_incmd_idx++] = serial_inbyte;
    }
  }
}

void serialNextIndex(char *&serial_incmd){
  while (*serial_incmd != ' ') serial_incmd++;
  while (isspace(*serial_incmd)) serial_incmd++;
}

byte testChar(byte vstup){
  if((vstup>=ASCII_NUM_START)and(vstup<=ASCII_NUM_END)){
      return 1;
  }else if((vstup>=ASCII_L_ALPHA_START)and(vstup<=ASCII_L_ALPHA_END)){
      return 2;
  }else if((vstup>=ASCII_U_ALPHA_START)and(vstup<=ASCII_U_ALPHA_END)){
      return 3;
  }
  return 0;
}

void serialExecute(char *serial_incmd){

  if((debug_mode==1)or(debug_mode==2)or(debug_mode==255)){
      Serial.print(F("SERIAL_EXECUTE: "));
      Serial.println(serial_incmd);
  }

  boolean isOK;
  int InTmp2, InTmp3, InTmp4, InTmp0, InTmp1;
  byte InTmpB0, InTmpB1, InTmpB2, InTmpB3;

  isOK = HIGH;

  if((testChar(serial_incmd[0])==2)||(testChar(serial_incmd[0])==3)){
    if(serial_incmd[1]==' '){
    }else{
      return;
    }
  }else{
    return;
  }

  switch (serial_incmd[0]){


  case 'c':
    serialNextIndex(serial_incmd);
    if (atoi(serial_incmd) >= 0 && atoi(serial_incmd) <= 255){
      InTmp0 = atoi(serial_incmd);
      if(InTmp0==0){
        clrStrip();
      }
      if(InTmp0==1){
        strip.show();
      }
    }
    break;

  case 's':
    serialNextIndex(serial_incmd);
    if (atoi(serial_incmd) >= 0 && atoi(serial_incmd) <= 255){
      InTmp0 = atoi(serial_incmd);
    }else{
      isOK = LOW;
    }
    serialNextIndex(serial_incmd);
    if (atoi(serial_incmd) >= 0 && atoi(serial_incmd) <= 255){
      InTmp1 = atoi(serial_incmd);
    }else{
      isOK = LOW;
    }
    serialNextIndex(serial_incmd);
    if (atoi(serial_incmd) >= 0 && atoi(serial_incmd) <= 255){
      InTmp2 = atoi(serial_incmd);
    }else{
      isOK = LOW;
    }
    serialNextIndex(serial_incmd);
    if (atoi(serial_incmd) >= 0 && atoi(serial_incmd) <= 255){
      InTmp3 = atoi(serial_incmd);
    }else{
      isOK = LOW;
    }
    if(isOK==HIGH){
      strip.setPixelColor(InTmp0, strip.Color(InTmp1, InTmp2, InTmp3));
    }
    break;

  case 'r':
    serialNextIndex(serial_incmd);
    if (atoi(serial_incmd) >= 0 && atoi(serial_incmd) <= 255){
      InTmp0 = atoi(serial_incmd);
    }else{
      isOK = LOW;
    }
    serialNextIndex(serial_incmd);
    if (atoi(serial_incmd) >= 0 && atoi(serial_incmd) <= 255){
      InTmp1 = atoi(serial_incmd);
    }else{
      isOK = LOW;
    }
    serialNextIndex(serial_incmd);
    if (atoi(serial_incmd) >= 0 && atoi(serial_incmd) <= 255){
      InTmp2 = atoi(serial_incmd);
    }else{
      isOK = LOW;
    }
    serialNextIndex(serial_incmd);
    if (atoi(serial_incmd) >= 0 && atoi(serial_incmd) <= 255){
      InTmp3 = atoi(serial_incmd);
    }else{
      isOK = LOW;
    }
    serialNextIndex(serial_incmd);
    if (atoi(serial_incmd) >= 0 && atoi(serial_incmd) <= 255){
      InTmp4 = atoi(serial_incmd);
    }else{
      isOK = LOW;
    }
    if(isOK==HIGH){
      ipRange(InTmp0,InTmp1,InTmp2,InTmp3,InTmp4);
    }
    break;

  case 'l':
    serialNextIndex(serial_incmd);
    if (atoi(serial_incmd) >= 0 && atoi(serial_incmd) <= 255){
      InTmp0 = atoi(serial_incmd);
    }else{
      isOK = LOW;
    }
    serialNextIndex(serial_incmd);
    if (atoi(serial_incmd) >= 0 && atoi(serial_incmd) <= 255){
      InTmp1 = atoi(serial_incmd);
    }else{
      isOK = LOW;
    }
    serialNextIndex(serial_incmd);
    if (atoi(serial_incmd) >= 0 && atoi(serial_incmd) <= 255){
      InTmp2 = atoi(serial_incmd);
    }else{
      isOK = LOW;
    }
    serialNextIndex(serial_incmd);
    if (atoi(serial_incmd) >= 0 && atoi(serial_incmd) <= 255){
      InTmp3 = atoi(serial_incmd);
    }else{
      isOK = LOW;
    }
    if(isOK==HIGH){
      ipLevel(InTmp0,InTmp1,InTmp2,InTmp3);
    }
    break;

  }
}

