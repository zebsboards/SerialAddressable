void TesterCommandReceive(void)
{
  if (Serial.available()) {
    
        receivedByte = Serial.read(); 
        switch (receivedByte) {
          case 'N':
            ReceiveTestData();
            Serial.println("LED Channel Info As Follows"); Serial.println("");
            for(int a = 0; a < NumberOfStrips; a++){
              LedNum[a] = TESTVAL[a];
              Serial.print("Channel   "); Serial.print(a+1); Serial.print("   "); Serial.print("S2 Mini  Pin# "); 
              if (Pins[a] < 10)
              {
                 Serial.print(" "); Serial.print(Pins[a]); Serial.print("   "); Serial.println(LedNum[a]);
              }
              else
              {
                 Serial.print(Pins[a]); Serial.print("   "); Serial.println(LedNum[a]); 
              }
            }
            for (int a = 0; a < NumberOfStrips; a++)
            { 
               ledStrip[a].updateLength(LedNum[a]);
            }
            receivedByte = NULL;
            break;
         case 'C':
            ReceiveTestData();
            SingleTest();
            ClearOutputs();
            Serial.println("Ack");
            receivedByte = NULL;      
            break;  
         case 'R':
            ReceiveTestData();
            ColorWipe();
            ClearOutputs();
            RestoreBrightness();
            Serial.println("Ack");
            receivedByte = NULL; 
            break; 
         case 'F':
            ReceiveTestData();
            theaterChaseRainbow();
            ClearOutputs();
            RestoreBrightness();
            Serial.println("Ack");
            receivedByte = NULL;      
            break;  
         case 'T':
            ReceiveTestData();
            theaterChase();
            ClearOutputs();
            RestoreBrightness();
            Serial.println("Ack");
            receivedByte = NULL;
            break;
         case 'E':
            ReceiveTestData();
            Serial.println("Preparing LED Channels");
            Serial.println("");Serial.println("Please Stand By");
            AllOn();
            RestoreBrightness();
            receivedByte = NULL;
            Serial.println("Ack");
            break; 
         case 'Q':
            Serial.end();
            Serial.begin(115200);
            delay(100);
            testerUtilityMode = 0;              // put board into DOF Control Mode 
            receivedByte = NULL;
            break;     
         case 'V':
            delay(500);
            Serial.println("D1");
            receivedByte = NULL;
            break; 
         case 'Z':
            ClearOutputs();
            receivedByte = NULL;
            break;              
         default:
            if (Serial.available() > 0) {
               Serial.readBytesUntil('\n',buf,SERIAL_BUFFER_SIZE);
            }
            receivedByte = NULL; 
            break;    
        }
    }   
    SetBlinkMode(1); 
}


void ReceiveTestData (void){
  
  delay(200); 
  while (Serial.available()!= 0) {
     {
      byte n = Serial.available();                                                 //byte 'n' is the number of bytes in the buffer   
      if (n != 0){                                                                // if 'n' is not a 0 then there is data available to be read
        byte numBytesReceived = Serial.readBytesUntil('\n',buf,SERIAL_BUFFER_SIZE);
        buf[numBytesReceived] = '\0';
      }
     } 
     char*next;                                                                    //pointer to substring (token)
     next = strtok(buf, ",");                                                      //splitting string by "," and get 1st substring token
     TESTVAL[0] = atoi(next);                                                      //assign the 1st value
     
     for (int a=1; a < NumberOfStrips; a++)
     {
        next = strtok(NULL, ",");                                                  //assign remaining values to next in order using the ',' token to delimit the string chunks
        TESTVAL[a] = atoi(next);                                                   //assign the single value from the string as an integer value in the temporary array
     }
 }
}
