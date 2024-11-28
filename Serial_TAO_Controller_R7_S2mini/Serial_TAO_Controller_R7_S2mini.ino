#define SERIAL_BUFFER_SIZE 2048
char buf[SERIAL_BUFFER_SIZE];                              // Serial Receive buffer - max size is channel led count info

#include <elapsedMillis.h>
#include <FastLED.h>

elapsedMillis BlinkTimer;  
int BlinkMode;
elapsedMillis BlinkModeTimeoutTimer;


#define FirmwareVersionMajor 1  
#define FirmwareVersionMinor 4  
#define LedPin LED_BUILTIN

#define NUM_LEDS    1024
#define COLOR_ORDER GRB
#define NUM_STRIPS 8
CRGB ledStrip[NUM_STRIPS][NUM_LEDS];

byte receivedByte;
uint16_t numberOfStrips = 8;
uint16_t stripLength;
uint16_t NumLedArray[8] = {1024,1024,1024,1024,1024,1024,1024,1024};
byte brightnessArray[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

byte StripNum;
uint16_t firstLed;
uint16_t numberOfLeds;
int ColorData;
int buffercount;

void setup() {   
  
  Serial.begin(2000000);
  Serial.setDebugOutput(false);
  /*FastLED.addLeds<WS2812, 7, GRB>(ledStrip[0], NumLedArray[0]);
  FastLED.addLeds<WS2812, 9, GRB>(ledStrip[1], NumLedArray[1]);
  FastLED.addLeds<WS2812, 11, GRB>(ledStrip[2], NumLedArray[2]);
  FastLED.addLeds<WS2812, 12, GRB>(ledStrip[3], NumLedArray[3]);
  FastLED.addLeds<WS2812, 35, GRB>(ledStrip[4], NumLedArray[4]);
  FastLED.addLeds<WS2812, 33, GRB>(ledStrip[5], NumLedArray[5]);
  FastLED.addLeds<WS2812, 18, GRB>(ledStrip[6], NumLedArray[6]);
  FastLED.addLeds<WS2812, 16, GRB>(ledStrip[7], NumLedArray[7]);
  
  */


  pinMode(LedPin,OUTPUT); 
  
  SetBlinkMode(0);
  
}
void loop() 
{
  if (Serial.available()) 
  {
    receivedByte = Serial.read();  
    DOFcommandReceive();                // put board into DOF control Mode
    SetBlinkMode(1);
  } 
  Blink();
}




