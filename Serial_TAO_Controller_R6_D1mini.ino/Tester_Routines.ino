void SingleTest (void)
{
      Serial.print("Channel ");Serial.print(TESTVAL[0]); Serial.print(" Using ");
      switch (TESTVAL[1]){
        case 0:
          Serial.print("RED at ");Serial.print(TESTVAL[2]); Serial.println("%");
          Hue = Red;
          break;
        case 1:
          Serial.print("GREEN at ");Serial.print(TESTVAL[2]); Serial.println("%");
          Hue = Green;
          break;
        case 2:
          Serial.print("BLUE at ");Serial.print(TESTVAL[2]); Serial.println("%");
          Hue = Blue;
          break;
        default:
          Serial.print("WHITE at ");Serial.print(TESTVAL[2]); Serial.println("%");
          Hue = White;
          break;
      }
      Serial.println(""); Serial.println("Flashing Six(6) Times");
      
      ledStrip[(TESTVAL[0]-1)].setBrightness(TESTVAL[2]);
      
      for(int a = 0; a < Cycle; a++){                                             
         for (int b = 0; b < LedNum[(TESTVAL[0]-1)]; b++)
         {
            ledStrip[(TESTVAL[0]-1)].setPixelColor(b,Hue); 
         }
         ledStrip[(TESTVAL[0]-1)].show();
         delay(delayVal);
         for (int b = 0; b < LedNum[(TESTVAL[0]-1)]; b++)
         {
            ledStrip[(TESTVAL[0]-1)].setPixelColor(b,Black); 
         } 
         ledStrip[(TESTVAL[0]-1)].show();
         delay(delayVal);
      }
      ledStrip[(TESTVAL[0]-1)].setBrightness(100);
      Serial.println("Test Complete."); 
      delay(250);
}


void ColorWipe (void) 
{
 SetHueValue();
 Serial.println("Processing Color Fill . . . .");
 for (int a = 0; a < NumberOfStrips; a++)
 {
    ledStrip[a].setBrightness(TESTVAL[1]);
    for(uint16_t i = 0; i < LedNum[a]; i++) 
    {
       ledStrip[a].setPixelColor(i, Hue);
       ledStrip[a].show();
       delay(2);
    }
 }
 Serial.println("Preparing to Clear Outputs . . . .");    
 for (int a = 0; a < NumberOfStrips; a++)
 {
    for(uint16_t i = 0; i < LedNum[a]; i++) 
    {
       ledStrip[a].setPixelColor(i, Black);
       ledStrip[a].show();
       delay(5);
    }
 }
 Serial.println("Test Complete."); 
 delay(500); 
}

void theaterChase (void) 
{
  SetHueValue();
  
  for (int j=0; j<40; j++)                                              //do 10 cycles of chasing
  {  
        for (int q=0; q < 3; q++) 
        {
           for (int a = 0; a < NumberOfStrips; a++)
           { 
               ledStrip[a].setBrightness(TESTVAL[1]);
               for (uint16_t i=0; i < LedNum[a]; i=i+3) 
               {
                   ledStrip[a].setPixelColor(i+q, Hue);                  //turn every third pixel on
               }
           }  
           for (int a = 0; a < NumberOfStrips; a++)
           {
               ledStrip[a].show();
           }    
           delay(50);
           for (int a = 0; a < NumberOfStrips; a++)
           {
               for (uint16_t i=0; i < LedNum[a]; i=i+3) 
               {
                   ledStrip[a].setPixelColor(i+q, Black);        //turn every third pixel off
               }
           }
        }
   }
   Serial.println("Test Complete."); 
   delay(500);
}

 
void theaterChaseRainbow (void) 
{
  for (int j=0; j < 256; j++)                                    // cycle all 256 colors in the wheel
  {
    for (int q=0; q < 3; q++) 
    {
      for (int a= 0; a < NumberOfStrips; a++)
      { 
        ledStrip[a].setBrightness(TESTVAL[0]);
        for (uint16_t i=0; i < LedNum[a]; i=i+3) 
        {
          ledStrip[a].setPixelColor(i+q, Wheel( (i+j) % 255));           //turn every third pixel on
        }
      }
      for (int a= 0; a < NumberOfStrips; a++)
      {   
        ledStrip[a].show();
      }
      delay(50);
      for (int a= 0; a < NumberOfStrips; a++)
      { 
         for (uint16_t i=0; i < LedNum[a]; i=i+3) 
         {
           ledStrip[a].setPixelColor(i+q, 0);        //turn every third pixel off
         }
      }
    }
  }
  Serial.println("Test Complete."); 
  delay(500);
}

void AllOn (void)
{
 SetHueValue();
 Serial.println("Processing Color Fill . . . .");
 delay(1500);
 Serial.println("Disabling Watchdog Timer . . . .");
 delay(1500);
 Serial.println("Calculating (This May Take Some Time)");
 delay(1500);
 for (int a = 0; a < NumberOfStrips; a++)
 {
    //ESP.wdtDisable();
    ledStrip[a].setBrightness(TESTVAL[1]);
    for(uint16_t i = 0; i < LedNum[a]; i++) 
    {
       ledStrip[a].setPixelColor(i, Hue);
       ledStrip[a].show();
       delay(1);
    }
    
 }
 delay(1000);
 Serial.println("Preparing to Clear Outputs . . . .");  
 for (int a = 0; a < NumberOfStrips; a++)
 {
    //ESP.wdtDisable();
    for(uint16_t i = 0; i < LedNum[a]; i++) 
    {
       ledStrip[a].setPixelColor(i, Black);
       ledStrip[a].show();
       delay(1);
    }
    
 } 
 //ESP.wdtEnable(0);
 Serial.println("Test Complete. WDT Enabled."); 
 delay(1000); 
}


uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return ledStrip[wheelLed].Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return ledStrip[wheelLed].Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ledStrip[wheelLed].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void ClearOutputs (void) 
{
  for(int a = 0; a < NumberOfStrips; a++){                                                           // Clear all of the outputs
    ledStrip[a].Color(0,0,0); 
    ledStrip[a].show();
  }
  
}

void SetHueValue (void)                                                                        // Assign the color value recieved to an int val 
{
  switch (TESTVAL[0]){
        case 0:
          Serial.print("RED at  ");Serial.print(TESTVAL[1]); Serial.println("%");
          Hue = Red;
          break;
        case 1:
          Serial.print("GREEN at  ");Serial.print(TESTVAL[1]); Serial.println("%");
          Hue = Green;
          break;
        case 2:
          Serial.print("BLUE at  ");Serial.print(TESTVAL[1]); Serial.println("%");
          Hue = Blue;
          break;
        default:
          Serial.print("WHITE at  ");Serial.print(TESTVAL[1]); Serial.println("%");
          Hue = White;
          break;
  }
}

void RestoreBrightness (void)                                                                  // Reset brightness level to 100% 
{
  for (int a = 0; a < NumberOfStrips; a++)
   {
      ledStrip[a].setBrightness(100);  
   }
}

