/****************************************************************************************************************************************************
*    Working Libraries Used
*    - Adaruit NeoPixel 1.12.3
*    - elapsedMillis    1.0.6
*
*    Working Board File Used
*    - ESP32  - Espressive Systems 2.0.14
*    - ESP8266 - Espressive System 3.1.2
*
*    Wemos (Lolin S2 mini)
*    Pins used for output - 7,9,11,12,35,33,18,16  (TAO compatibility)
*    Pins used for output - 7,9,11,12,35,33,18,16,2,4,6,13,10,8 (14 out Serial Controller)
*    Wemos (Lolin D1 mini)
*    Pins used for output - 14,12,13,15,5,4,0  (TAO compatibility)
*
*    Enumerates in DOF as ZBSerial
*
***************************************************************************************************************************************************/
#define SERIAL_BUFFER_SIZE 1024
#include <elapsedMillis.h>
#include <Adafruit_NeoPixel.h>
#define FirmwareVersionMajor 2  
#define FirmwareVersionMinor 0  
#define LedPin LED_BUILTIN  
#define TotalMaxLeds 1024
#define NumberOfStrips 14
elapsedMillis BlinkTimer;  
int BlinkMode;
elapsedMillis BlinkModeTimeoutTimer;

uint32_t Green = 0xFF0000;
uint32_t Red =   0x00FF00;
uint32_t Blue =  0x0000FF;
uint32_t White = 0xFFFFFF; 
uint32_t Black = 0x000000;

uint16_t stripLength; 
int8_t StripsInUse;
int nightMode = 0;
int setPixeloff = 0;

//int Pins[] = {7,9,11,12,35,33,18,16}; // 8 output code
int Pins[] = {7,9,11,12,35,33,18,16,2,4,6,13,10,8}; // 14 output code

Adafruit_NeoPixel ledStrip[] = {    
  Adafruit_NeoPixel(1, Pins[0], NEO_RGB + NEO_KHZ800),                                  // (NumLeds, Pin, LEDtype + Speed)
  Adafruit_NeoPixel(2, Pins[1], NEO_RGB + NEO_KHZ800),
  Adafruit_NeoPixel(3, Pins[2], NEO_RGB + NEO_KHZ800),
  Adafruit_NeoPixel(4, Pins[3], NEO_RGB + NEO_KHZ800),
  Adafruit_NeoPixel(5, Pins[4], NEO_RGB + NEO_KHZ800),
  Adafruit_NeoPixel(6, Pins[5], NEO_RGB + NEO_KHZ800),
  Adafruit_NeoPixel(7, Pins[6], NEO_RGB + NEO_KHZ800),
  Adafruit_NeoPixel(8, Pins[7], NEO_RGB + NEO_KHZ800),    
  Adafruit_NeoPixel(9, Pins[8], NEO_RGB + NEO_KHZ800),    
  Adafruit_NeoPixel(10, Pins[9], NEO_RGB + NEO_KHZ800),  
  Adafruit_NeoPixel(11, Pins[10], NEO_RGB + NEO_KHZ800),  
  Adafruit_NeoPixel(12, Pins[11], NEO_RGB + NEO_KHZ800),
  Adafruit_NeoPixel(13, Pins[12], NEO_RGB + NEO_KHZ800),
  Adafruit_NeoPixel(14, Pins[13], NEO_RGB + NEO_KHZ800)
};

Adafruit_NeoPixel QualityStrip = Adafruit_NeoPixel(38, 7, NEO_RGB + NEO_KHZ800);   //  1-38 leds used on mainboard test, mainboard pin (7)

void setup() {   
  Serial.begin(2000000);
  pinMode(LedPin,OUTPUT); 
  SetBlinkMode(0);
  while(Serial.available() > 0) {int clrBuffer = Serial.read();}                                              // ensure empty buffer before starting loop
  for (int i=0; i<NumberOfStrips; i++) { ledStrip[i].begin();  ledStrip[i].setBrightness(255); ledStrip[i].clear();  ledStrip[i].show(); }    // turn off all outputs
}
void loop() 
{
  if (Serial.available()) {
    byte receivedByte = Serial.read();  
    switch (receivedByte) 
    {
      case 'V':                               // Send the firmware version
        SendVersion();  
        break;
      case 'M':                               // Send max number of leds per strip  
        SendMaxNumberOfLeds();
        break;
      case 'L':                               // Set length of strips
        SetLedStripLength(); 
        break;
      case 'C':                               // Clears all previously received led data
        ClearAllLedData();
        break;
      case 'B':                               // Send the firmware version
        ReceiveNumberOfStrips();  
        break;  
      case 'R':                               // Receive data for strips
        ReceiveData();
        break;
      case 'O':                               // Output data on strip        
        OutputData();
        break;   
      case 'S':                               // Set Night Mode  (TAO)
        nightMode = !nightMode;
        break; 
      case 'Q':                               // Quality Control Test (TAO) 
        QualityTest();
        break; 
      case 'U':
        Serial.println("S2");                   // Configuration Utility Board Verification
      break;  
      case 'T':
        ReceiveConfigTestData();
      break; 
      case 'W':
        OutputTestData();
      break;
      case 'X':
        ClearAllTestData();
      break;   
      case 'Y':
        TestChannels();
      break;   
      case 'Z':
        Serial.println(StripsInUse);
        break; 
      default:                                  // no unknown commands allowed. Send NACK (N)
        Serial.write("N");
        break;
    }
    SetBlinkMode(1);
  } 
  Blink();
}
/******************************************************* Sends the max number of leds per strip *********************************************************************/
void SendMaxNumberOfLeds() 
{
  byte B = TotalMaxLeds >> 8;
  Serial.write(B);
  B = TotalMaxLeds & 255;
  Serial.write(B);
  Serial.write("A");
}
/************************************************************* Sends the firmware version ***************************************************************************/
void SendVersion() 
{
  Serial.write(FirmwareVersionMajor);
  Serial.write(FirmwareVersionMinor);
  Serial.write('A');
}
/****************************** Sets the length of the longest connected ledstrip. Length is restricted to the max number of allowed leds ***************************/
void SetLedStripLength() 
{
  stripLength = ReceiveWord();
  Serial.write('A');
}
/****************************** Sets the Number of Strips being Used by DOF as set out in cabinet.xml file ***************************/
void ReceiveNumberOfStrips() 
{
  StripsInUse = Serial.read();
  Serial.write('A');
}
/**************************************************************** Receives data for the ledstrips *******************************************************************/
void ReceiveData() 
{
  uint16_t firstLed = ReceiveWord();
  uint16_t numberOfLeds = ReceiveWord();
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
  Serial.write('A');
}
/********************************************************** Clears the data for all configured leds ****************************************************************/
void  ClearAllLedData() 
{
  for (int j=0; j<NumberOfStrips; j++) 
  {
    for (uint16_t i=0; i<ledStrip[j].numPixels(); i++) 
    {
      ledStrip[j].setPixelColor(i, 0);
    }
    ledStrip[j].show();
  }
  Serial.write('A'); 
}
/********************************************************** Outputs the data in the ram to the ledstrips ************************************************************/
void OutputData() 
{
  for (int i=0; i < StripsInUse; i++) 
  {
    ledStrip[i].show();
  }
  Serial.write('A');
}
/****************************************************** Receives a word value. High byte first, low byte second *****************************************************/
uint16_t ReceiveWord() 
{
  while(!Serial.available()) {};
  uint16_t wordValue=Serial.read()<<8; 
  while(!Serial.available()) {};
  wordValue=wordValue|Serial.read();
  return wordValue;
}
/****************************************************************** Receives 3 bytes of color data ******************************************************************/
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
/******************************************************* Sets the mode for the blinking of the led ******************************************************************/
void SetBlinkMode(int Mode) 
{
  BlinkMode = Mode;
  BlinkModeTimeoutTimer = 0;
}
/******************************************************* Controls the blinking of the led ***************************************************************************/
void Blink() 
{
  switch(BlinkMode) 
  {
    case 0:
      //Blinkmode 0 is only active after the start of the Teensy until the first command is received.
      if(BlinkTimer<1500) {
        digitalWrite(LedPin,0);
      } else if(BlinkTimer<1600) {
        digitalWrite(LedPin,1);
      } else {
        BlinkTimer=0;
        digitalWrite(LedPin,0);
      }
    break;
    case 1:
      //Blinkmode 1 is activated when the Teensy receives a command
      //Mode expires 500ms after the last command has been received resp. mode has been set
      if(BlinkTimer>30) {
        BlinkTimer=0;
        digitalWrite(LedPin,!digitalRead(LedPin));
      }
      if(BlinkModeTimeoutTimer>500) {
        SetBlinkMode(2);
      }
    break;   
    case 2:
      //Blinkmode 2 is active while the Teensy is waiting for more commands
      if(BlinkTimer<1500) {
        digitalWrite(LedPin,0);
      } else if(BlinkTimer<1600) {
        digitalWrite(LedPin,1);
      } else if(BlinkTimer<1700) {
        digitalWrite(LedPin,0);
      } else if(BlinkTimer<1800) {
        digitalWrite(LedPin,1);
      }else {
        BlinkTimer=0;
        digitalWrite(LedPin,0);
      }
      default:
      //This should never be active
      //The code is only here to make it easier to determine if a wrong Blinkcode has been set
      if(BlinkTimer>2000) {
        BlinkTimer=0;
        digitalWrite(LedPin,!digitalRead(LedPin));
      }
    break;
  }
}