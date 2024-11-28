void QualityTest()
{
QualityStrip.setBrightness(50);
QualityStrip.show(); // Initialize all pixels to 'off'

  for (int f=0; f<10; f++)
  {
    
    colorWipe(QualityStrip.Color(255, 255, 255), 10); // Red
    colorWipe(QualityStrip.Color(255, 255, 255), 10); // Green
    colorWipe(QualityStrip.Color(255, 255, 255), 10); // Blue
    colorWipe(QualityStrip.Color(0, 0, 0), 10); // Blue
  } 
} 
void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<QualityStrip.numPixels(); i++) 
  {
    QualityStrip.setPixelColor(i, c);
    QualityStrip.show();
    delay(wait);
  }
}

void ReceiveConfigTestData(void)
{
  // { 1st Led, NumberOfLeds, Color, Brightness, stripLength }
  uint16_t firstLed = ReceiveWord();
  uint16_t numberOfLeds = ReceiveWord();
  uint32_t ColorData = ReceiveColorData();
  byte Brightness = Serial.read();
  stripLength = ReceiveWord();
  
  uint16_t currentStrip = firstLed / stripLength;
  uint16_t currentStripNumPixels = ledStrip[currentStrip].numPixels();
  uint16_t currentStripFirstLed = firstLed % stripLength;
  if (currentStripFirstLed + numberOfLeds > currentStripNumPixels) 
  {
    ledStrip[currentStrip].updateLength(currentStripFirstLed + numberOfLeds);
    currentStripNumPixels = ledStrip[currentStrip].numPixels();
  }
  for (uint16_t i=0; i < numberOfLeds; i++) 
  {
    if (i < currentStripNumPixels) 
    {
      ledStrip[currentStrip].setPixelColor(currentStripFirstLed + i, ColorData);
    }
  }
  ledStrip[currentStrip].setBrightness(Brightness);
  OutputTestData();
}

void TestChannels (void)
{
  uint16_t firstLed = ReceiveWord();
  uint16_t numberOfLeds = ReceiveWord();
  uint32_t ColorData = ReceiveColorData();
  byte Brightness = Serial.read();
  uint16_t currentStrip = ReceiveWord();
  
  if (numberOfLeds > ledStrip[currentStrip - 1].numPixels())
  {
    ledStrip[currentStrip - 1].updateLength(numberOfLeds);
  }
  for (int a = 0; a < numberOfLeds; a++ )
  {
    ledStrip[currentStrip - 1].setPixelColor(a, ColorData);
  }
  ledStrip[currentStrip - 1].setBrightness(Brightness);
  OutputTestData();
}

void OutputTestData() 
{
  for (int i = 0; i < NumberOfStrips; i++) 
  {
    ledStrip[i].show();
  }
  Serial.println("A");
}

void  ClearAllTestData() 
{
  for (int j=0; j<NumberOfStrips; j++) 
  {
    for (uint16_t i=0; i<ledStrip[j].numPixels(); i++) 
    {
      ledStrip[j].setPixelColor(i, 0);
    }
    ledStrip[j].show();
  }
  Serial.println("A"); 
}
