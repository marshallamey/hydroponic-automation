#ifndef Conductivity_h
#define Conductivity_h

class Conductivity {

  private:
    String commandString;
    String sendStringEC;
    String readStringEC;
    float conductivity;

  public:

    //SENDS COMMAND TO EC SENSOR
    void sendCommand (){

      Serial2.print(commandString);
      Serial2.print('\r');
      Serial.print("Response from EC: ");

      sendStringEC = Serial2.readStringUntil(13);
      Serial.println(sendStringEC);
      sendStringEC = "";

      sendStringEC = Serial2.readStringUntil(13);
      Serial.println(sendStringEC);
      Serial.println();
      sendStringEC = "";

      sendStringEC = Serial2.readStringUntil(13);
      sendStringEC = "";

    }

    //READS CONDUCTIVITY DATA
    float readData(){

       Serial2.print('R');
       Serial2.print('\r');

       readStringEC = Serial2.readStringUntil(13);
       conductivity = readStringEC.toFloat();
       readStringEC = "";

       readStringEC = Serial2.readStringUntil(13);
       Serial.print("Reading EC:  ");
       Serial.print(readStringEC);
       Serial.print("\t");
       readStringEC = "";

       Serial.print("Conductivity: ");
       Serial.print("\t\t");
       Serial.print(conductivity);
       Serial.println(" (micro)S");
    }

};

#endif
