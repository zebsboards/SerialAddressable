void DOFcommandReceive(void)
{
  switch (receivedByte) 
  {
    case 'L':
    //Set length of strips on per strip basis to reduce overhead
      SetLedStripLength(); 
    break;
    case 'E':
    //Set the virtual strip length used in calculations based on NumberofLedsPerChannel variable - set to longest string amount in system
      ReceiveStripLength();
    break;
    case 'B':
    //Set the Brightness value for each strip
    ReceiveBrightnessSettings();
    break;
    case 'F':
    //Fill strip area with color
      Fill();
    break;
    case 'R':
    //receive data for strips
      ReceiveData();
    break;
    case 'O':
    //output data on strip
      OutputData();
    break;
    case 'P':
    //output data on strip
      OutputData();
    break;   
    case 'C':
    //Clears all previously received led data
      ClearAllLedData();
    break;
    case 'V':     
    //Send the firmware version
      SendVersion();  
    break;
    case 'M':
    //Get max number of leds per strip  
      SendMaxNumberOfLeds();
    break;
    case 'S':
    //Show the received settings - debug usage through serial monitor
      ShowStripLengths();
    break;
    default:
    // unknown comman, Send NACK (N)
      Nack();
    break;
  }
}
