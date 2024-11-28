#define SERIAL_BUFFER_SIZE 2048
char buf[SERIAL_BUFFER_SIZE];                              // Serial Receive buffer - max size is channel led count info

#include <elapsedMillis.h>
#include <Adafruit_NeoPixel.h>

elapsedMillis BlinkTimer;  
int BlinkMode;
elapsedMillis BlinkModeTimeoutTimer;


#define FirmwareVersionMajor 1  
#define FirmwareVersionMinor 4  
#define LedPin LED_BUILTIN
#define TotalMaxLeds 1024
#define NumberOfStrips 7
#define delayVal 750                                // Delay between flashes - Single Test variable
#define Cycle 6                                     // Number of times to loop - Single Test variable

int testerUtilityMode = 0;                          // Flags Tester / DOF Control Mode
int wheelLed;                                       // global variable used in the TheaterChaseRainbow and Wheel functions
int Intensity;
uint32_t Hue;                                       // Placeholder for conversion from received color string to int
uint32_t Green = 0xFF0000;
uint32_t Red =   0x00FF00;
uint32_t Blue =  0x0000FF;
uint32_t White = 0xFFFFFF; 
uint32_t Black = 0x000000;
int nightMode = 0;
int setPixeloff = 0;

word stripLength = 100; 
byte receivedByte;                                  // Initial byte recieved - defines which routine has been called

const int config PROGMEM = NEO_RGB + NEO_KHZ800; 

int TESTVAL[NumberOfStrips];                        // Array to hold the variables sent by the software
int LedNum[NumberOfStrips];                         // Array to hold the number of Leds per channel
int Pins[] = {14,12,13,15,4,0};                      // D1 Array holding the output pin assignments

Adafruit_NeoPixel ledStrip[] = {                    // Array of the 12 channels available for lightstrips 
  Adafruit_NeoPixel(LedNum[0], Pins[0], config),    // LED numbers are placeholders and are updated later
  Adafruit_NeoPixel(LedNum[1], Pins[1], config),    // using the ledStrip[x].updateStringLength method
  Adafruit_NeoPixel(LedNum[2], Pins[2], config),    // after number of Leds per channel has been received
  Adafruit_NeoPixel(LedNum[3], Pins[3], config),
  Adafruit_NeoPixel(LedNum[4], Pins[4], config),
  Adafruit_NeoPixel(LedNum[5], Pins[5], config),
  Adafruit_NeoPixel(LedNum[6], Pins[6], config)
 /* Adafruit_NeoPixel(LedNum[7], Pins[7], config)     // LED numbers are placeholders and are updated later
  Adafruit_NeoPixel(LedNum[8], Pins[8], config),    // using the ledStrip[x].updateStringLength method
  Adafruit_NeoPixel(LedNum[9], Pins[9], config),    // after number of Leds per channel has been received
  Adafruit_NeoPixel(LedNum[10], Pins[10], config),  // LED numbers are placeholders and are updated later
  Adafruit_NeoPixel(LedNum[11], Pins[11], config),  // using the ledStrip[x].updateStringLength method
  Adafruit_NeoPixel(LedNum[12], Pins[12], config),
  Adafruit_NeoPixel(LedNum[13], Pins[13], config)*/
};


void setup() {   
  
  Serial.begin(115200);
  //while (!Serial.available())
  delay(100);
  Serial.println("");

  pinMode(LedPin,OUTPUT); 
  
  SetBlinkMode(0);

  for (int i=0; i<NumberOfStrips; i++) {
    ledStrip[i].begin();
    ledStrip[i].clear();
    ledStrip[i].show();
  }
}
void loop() {

  
  if (Serial.available()) {
    
    receivedByte = Serial.read();  
    switch (receivedByte) {
      case 'U':
        testerUtilityMode = 1;              // put board into Tester Utility Control Mode
        while (testerUtilityMode == 1)
        {
          TesterCommandReceive();
           Blink();
        }
        break;
      default:
        DOFcommandReceive();                // put board into DOF control Mode
        break;  
    }
    
    SetBlinkMode(1);
  } 
  Blink();
}




