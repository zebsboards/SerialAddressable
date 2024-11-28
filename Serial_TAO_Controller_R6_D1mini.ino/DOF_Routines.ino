//Sets the mode for the blinking of the led
void SetBlinkMode(int Mode) 
{
  BlinkMode = Mode;
  BlinkModeTimeoutTimer = 0;
}


//Controls the blinking of the led
void Blink() 
{
  switch(BlinkMode) 
  {
    case 0:
      //Blinkmode 0 is only active after the start of the Teensy until the first command is received.
      if(BlinkTimer<1500) 
      {
        digitalWrite(LedPin,0);
      } 
      else if(BlinkTimer<1600) 
      {
        digitalWrite(LedPin,1);
      } 
      else 
      {
        BlinkTimer=0;
        digitalWrite(LedPin,0);
      }
    break;
    case 1:
      //Blinkmode 1 is activated when the Teensy receives a command
      //Mode expires 500ms after the last command has been received resp. mode has been set
      if(BlinkTimer>30) 
      {
        BlinkTimer=0;
        digitalWrite(LedPin,!digitalRead(LedPin));
      }
      if(BlinkModeTimeoutTimer>500) 
      {
        SetBlinkMode(2);
      }
    break;   
    case 2:
      //Blinkmode 2 is active while the Teensy is waiting for more commands
      if(BlinkTimer<1500) 
      {
        digitalWrite(LedPin,0);
      } 
      else if(BlinkTimer<1600) 
      {
        digitalWrite(LedPin,1);
      } 
      else if(BlinkTimer<1700) 
      {
        digitalWrite(LedPin,0);
      } 
      else if(BlinkTimer<1800) 
      {
        digitalWrite(LedPin,1);
      }
      else 
      {
        BlinkTimer=0;
        digitalWrite(LedPin,0);
      }
      default:
      //This should never be active
      //The code is only here to make it easier to determine if a wrong Blinkcode has been set
      if(BlinkTimer>2000) 
      {
        BlinkTimer=0;
        digitalWrite(LedPin,!digitalRead(LedPin));
      }
      break;
  }
}

//Outputs the data in the ram to the ledstrips
void OutputData() 
{
  for (int i=0; i<NumberOfStrips; i++) 
  {
    ledStrip[i].show();
  }
  Ack();
}


//Sets the length of the longest connected ledstrip. Length is restricted to the max number of allowed leds
void SetLedStripLength() 
{
   stripLength = ReceiveWord();
   Ack();
}



//Fills the given area of a ledstrip with a color
void Fill() 
{
  word firstLed = ReceiveWord();
  word numberOfLeds = ReceiveWord();
  uint32_t ColorData = ReceiveColorData();
  word currentStrip = firstLed / stripLength;
  word currentStripNumPixels = ledStrip[currentStrip].numPixels();
  word currentStripFirstLed = firstLed % stripLength;
  

  if (currentStripFirstLed + numberOfLeds > currentStripNumPixels)                 //Fills the given area of a ledstrip with a color
  {
    ledStrip[currentStrip].updateLength(currentStripFirstLed + numberOfLeds);
    currentStripNumPixels = ledStrip[currentStrip].numPixels();
    for (word i=0; i < numberOfLeds; i++) 
    {
      if (i < currentStripNumPixels) 
      {
        switch (nightMode) 
        {
          case 1:
            if ((currentStrip < 1) && ((currentStripFirstLed + i) > 12)) 
            {
             setPixeloff = 1;
            } 
             break;
          default:
             setPixeloff = 0;
             break;
         }
         ledStrip[currentStrip].setPixelColor(currentStripFirstLed + i, ColorData);
      }
    }
    Ack();
  }
  else                                   //Number of the first led or the number of leds to receive is outside the allowed range
  {
    Nack();
  }
}


//Receives data for the ledstrips
void ReceiveData() 
{
  word firstLed = ReceiveWord();
  word numberOfLeds = ReceiveWord();
  word currentStrip = firstLed / stripLength;
  word currentStripNumPixels = ledStrip[currentStrip].numPixels();
  word currentStripFirstLed = firstLed % stripLength;
  

  if (currentStripFirstLed + numberOfLeds > currentStripNumPixels) 
  {
    ledStrip[currentStrip].updateLength(currentStripFirstLed + numberOfLeds);
    currentStripNumPixels = ledStrip[currentStrip].numPixels();
    for (word i=0; i < numberOfLeds; i++) 
    {
      if (i < currentStripNumPixels) 
      {
        switch (nightMode) 
        {
          case 1:
            if ((currentStrip < 1) && ((currentStripFirstLed + i) > 12))
            {
             setPixeloff = 1;
            } 
            break;
          default:
            setPixeloff = 0;
            break;
         }
         ledStrip[currentStrip].setPixelColor(currentStripFirstLed + i, ReceiveColorData());
       }
     }
     Ack();
  }
  else                                   //Number of the first led or the number of leds to receive is outside the allowed range
  {
     Nack();  
  }
}



//Clears the data for all configured leds
void  ClearAllLedData() 
{
  for (int j=0; j<NumberOfStrips; j++) 
  {
     for (word i=0; i<ledStrip[j].numPixels(); i++) 
     {
        ledStrip[j].setPixelColor(i, 0);
     }
     ledStrip[j].show();
  }
  Ack();   
}

//Sends the firmware version
void SendVersion() 
{
  Serial.write(FirmwareVersionMajor);
  Serial.write(FirmwareVersionMinor);
  Ack();
}

//Sends the max number of leds per strip
void SendMaxNumberOfLeds() 
{
  byte B = TotalMaxLeds >> 8;
  Serial.write(B);
  B = TotalMaxLeds & 255;
  Serial.write(B);
  Ack();
}

void Ack (void)
{
  Serial.write('A');
}

void Nack (void)
{
  Serial.write('N');
}

//Receives 3 bytes of color data.
uint32_t ReceiveColorData() 
{
  while(!Serial.available()) {};
  uint32_t colorValue=Serial.read(); 
  while(!Serial.available()) {};
  colorValue=(colorValue<<8)|Serial.read();
  while(!Serial.available()) {};
  colorValue=(colorValue<<8)|Serial.read();
  if (setPixeloff != 0) 
  {
    colorValue = Black;
    setPixeloff= 0;
  }
  return colorValue;
}

//Receives a word value. High byte first, low byte second
word ReceiveWord() 
{
  while(!Serial.available()) {};
  word wordValue=Serial.read()<<8; 
  while(!Serial.available()) {};
  wordValue=wordValue|Serial.read();
  return wordValue;
}
