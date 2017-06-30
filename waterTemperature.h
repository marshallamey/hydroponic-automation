#ifndef waterTemperature_h
#define waterTemperature_h

/*
// Define ...
#define xxx 111
#define yyy 222
*/

class WaterTemperature {

  private:
    String commandString;
    String sendStringRTD;
    String readStringRTD;
    float waterTemp;

  public:

  //Sends command to RTD sensor
  void sendCommand() {


      Serial1.print(commandString);
      Serial1.print('\r');
      Serial.print("Response from RTD: ");

      sendStringRTD = Serial1.readStringUntil(13);
      Serial.println(sendStringRTD);
      sendStringRTD = "";

      sendStringRTD = Serial1.readStringUntil(13);
      Serial.println(sendStringRTD);
      Serial.println();
      sendStringRTD = "";

      sendStringRTD = Serial1.readStringUntil(13);
      sendStringRTD = "";
  }

  // Reads water temperature data
  float readData() {


      Serial1.print('R');
      Serial1.print('\r');

      readStringRTD = Serial1.readStringUntil(13);
      waterTemp = readStringRTD.toFloat();
      waterTemp = waterTemp*1.8 + 32;
      readStringRTD = "";

      readStringRTD = Serial1.readStringUntil(13);
      return waterTemp;
  }


  // Prints water temperature data
  void printData() {

    Serial.print("Reading RTD: ");
    Serial.print(readStringRTD);
    Serial.print("\t");
    readStringRTD = "";

    Serial.print("Water Temperature: ");
    Serial.print("\t");
    Serial.print(waterTemp);
    Serial.println(" *F");
  }

  //void raiseWaterTemp() {}

  //void lowerWaterTemp() {}

  // Private variables
};




#endif
