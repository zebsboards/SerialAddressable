void DOFcommandReceive(void)
{
  switch (receivedByte) {
      case 'L':
        //Set length of strips
        SetLedStripLength(); 
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
        //set Night Mode 
        nightMode = !nightMode;
        Serial.println(nightMode);
        break;    
      default:
        // no unknown commands allowed. Send NACK (N)
        Nack();
     break;
    }
}
