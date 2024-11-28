//Sets the mode for the blinking of the led
void SetBlinkMode(int Mode) 
{
  BlinkMode = Mode;
  BlinkModeTimeoutTimer = 0;
}


//Controls the blinking of the led
void Blink(void) 
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
    break;
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

//Sets the length of the longest connected ledstrip. Length is restricted to the max number of allowed leds
void SetLedStripLength(void) 
{
  for (int a = 0; a < numberOfStrips; a++)
  {
    NumLedArray[a] = ReceiveWord();
  }
  Ack();
}


void ReceiveBrightnessSettings(void)
{
  
  for (int a = 0; a < numberOfStrips; a++)
  {
    while (!Serial.available()) {};
    brightnessArray[a] = Serial.read();
  }
  FastLED.addLeds<WS2812, 7, GRB>(ledStrip[0], NumLedArray[0]).setCorrection(CRGB(brightnessArray[0],brightnessArray[0],brightnessArray[0]));
  FastLED.addLeds<WS2812, 9, GRB>(ledStrip[1], NumLedArray[1]).setCorrection(CRGB(brightnessArray[1],brightnessArray[1],brightnessArray[1]));
  FastLED.addLeds<WS2812, 11, GRB>(ledStrip[2], NumLedArray[2]).setCorrection(CRGB(brightnessArray[2],brightnessArray[2],brightnessArray[2]));
  FastLED.addLeds<WS2812, 12, GRB>(ledStrip[3], NumLedArray[3]).setCorrection(CRGB(brightnessArray[3],brightnessArray[3],brightnessArray[3]));
  FastLED.addLeds<WS2812, 35, GRB>(ledStrip[4], NumLedArray[4]).setCorrection(CRGB(brightnessArray[4],brightnessArray[4],brightnessArray[4]));
  FastLED.addLeds<WS2812, 33, GRB>(ledStrip[5], NumLedArray[5]).setCorrection(CRGB(brightnessArray[5],brightnessArray[5],brightnessArray[5]));
  FastLED.addLeds<WS2812, 18, GRB>(ledStrip[6], NumLedArray[6]).setCorrection(CRGB(brightnessArray[6],brightnessArray[6],brightnessArray[6]));
  FastLED.addLeds<WS2812, 17, GRB>(ledStrip[7], NumLedArray[7]).setCorrection(CRGB(brightnessArray[7],brightnessArray[7],brightnessArray[7]));
  Ack();
}


// Receives the length of the longest strip in the system
void ReceiveStripLength (void)
{
  stripLength = ReceiveWord();
  Ack();
}

void ShowStripLengths(void)
{
  Serial.println("Strip Lengths:");
  for (int a = 0; a < numberOfStrips; a++)
   {
     Serial.print(" # of LEDs "); Serial.print(NumLedArray[a]); Serial.print(" Brightness "); Serial.println(brightnessArray[a]);
   }
  Serial.print("StripLength Received:   "); Serial.println(stripLength);
  Serial.print("Strip Number Received   "); Serial.println(StripNum);
  Serial.print("Start LED Received:     "); Serial.println(firstLed);
  Serial.print("Number LEDs Received:   "); Serial.println(numberOfLeds);
  Serial.print("Color Data Received:    "); Serial.println(ColorData, HEX);
  Serial.print("Excess Bytes in Buffer: "); Serial.println(buffercount);
}


//Receives data for the ledstrips
void ReceiveData(void) 
{
  StripNum = Serial.read();
  firstLed = ReceiveWord();
  numberOfLeds = ReceiveWord();
  ColorData = receiveColorData();
  //uint16_t currentStrip = firstLed / stripLength;
  //uint16_t currentStripNumPixels = NumLedArray[currentStrip];
  //uint16_t currentStripFirstLed = firstLed % stripLength;
  
  if (firstLed + numberOfLeds > NumLedArray[StripNum]) 
  {
    Nack();    //Number of the first led or the number of leds to receive is outside the allowed range
  }
  else
  { 
    uint16_t ledOffset;
    for (byte a = 0; a < StripNum; a++)
    {
      ledOffset = ledOffset + NumLedArray[a];
    }
    for (uint16_t i = firstLed - ledOffset; i < numberOfLeds; i++) 
    {
      if (i < NumLedArray[StripNum]) 
      {
        ledStrip[StripNum][i] = CRGB(ColorData);
      }
    }
    Ack();
  } 
  /* if (currentStripFirstLed + numberOfLeds > currentStripNumPixels) 
  {
    NumLedArray[currentStrip] = (currentStripFirstLed + numberOfLeds);
    currentStripNumPixels = NumLedArray[currentStrip];
    for (uint16_t i=0; i < numberOfLeds; i++) 
    {
      if (i < currentStripNumPixels) 
      {
        ledStrip[currentStrip][currentStripFirstLed + i] = CRGB(receiveColorData());
      }
    }
    Ack();
  }
  else                                   //Number of the first led or the number of leds to receive is outside the allowed range
  {
    Nack();  
  }*/
}


//Fills the given area of a ledstrip with a color
void Fill(void) 
{
  uint16_t firstLed = ReceiveWord();
  uint16_t numberOfLeds = ReceiveWord();
  uint16_t currentStrip = firstLed / stripLength;
  uint16_t currentStripNumPixels = NumLedArray[currentStrip];
  uint16_t currentStripFirstLed = firstLed % stripLength;
    
  if (currentStripFirstLed + numberOfLeds > currentStripNumPixels) 
  {
    NumLedArray[currentStrip] = (currentStripFirstLed + numberOfLeds);
    currentStripNumPixels = NumLedArray[currentStrip];
    for (uint16_t i=0; i < numberOfLeds; i++) 
    {
      if (i < currentStripNumPixels) 
      {
        ledStrip[currentStrip][currentStripFirstLed + i] = CRGB(receiveColorData());
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
void  ClearAllLedData(void) 
{
  FastLED.clear();
  Ack();
}


//Outputs the data in the ram to the ledstrips
void OutputData(void) 
{
  Ack();
  FastLED.show();
}


//Sends the firmware version
void SendVersion(void) 
{
  Serial.write(FirmwareVersionMajor);
  Serial.write(FirmwareVersionMinor);
  Ack();
}

//Sends the max number of leds per strip
void SendMaxNumberOfLeds(void) 
{
  byte B = NUM_LEDS >> 8;
  Serial.write(B);
  B = NUM_LEDS & 0xFF;
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
int receiveColorData() 
{
  while (!Serial.available()) {};
  int colorValue = Serial.read();
  while (!Serial.available()) {};
  colorValue = (colorValue << 8) | Serial.read();
  while (!Serial.available()) {};
  colorValue = (colorValue << 8) | Serial.read();
  byte clrBuff;
 /* while(Serial.available() > 0)
  {
    clrBuff = Serial.read();
    buffercount++;
  }*/
  return colorValue;
}


//Receives a word value. High byte first, low byte second
uint16_t ReceiveWord() 
{
  while(!Serial.available()) {};
  uint16_t wordValue=Serial.read() & 0xFF; 
  while(!Serial.available()) {};
  wordValue = wordValue | (Serial.read()<<8) & 0xFFFF;
  return wordValue;
}