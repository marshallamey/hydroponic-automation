#ifndef Sensor_h
#define Sensor_h

#include <SoftwareSerial.h>
#include "DHT.h"

// Establish Serial Port 4  (RX, TX)
SoftwareSerial Serial4 (13, 12);
SoftwareSerial Serial5 (69, 68);
SoftwareSerial Serial6 (65, 64);

// Read the gas density command /Don't change the order
unsigned char hexdata[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; 

// DHT Temperature and Humidity Sensor
#define DHTPIN 53        
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);

// E-TAPE LIQUID LEVEL SENSOR
#define SERIESRESISTOR 560    
#define SENSORPIN A4 


class SENSOR {
  private:

    String commandString;
    float waterTemp;
    float conductivity;
    float pH;
    float oxygen;
    float humidity;
    float airTemp;
    long carbon;
    float par;
    float waterLevel;

  public:

//CONSTRUCTOR
    SENSOR(){}

/************************************************************************************************
MEMBER FUNCTIONS    
************************************************************************************************/

//GET A COMMAND FROM THE USER
    String getCommand() {
      boolean commandStringComplete = false;
      Serial.print("Enter a command: ");

      while (commandStringComplete == false) {
        commandString = Serial.readStringUntil(13);
        if (commandString.length() > 0) {
          commandStringComplete = true;
        }
      }
      Serial.println(commandString);
      return commandString;
    }

/*************************************************************************************************
SEND FUNCTIONS
**************/

//SEND COMMAND TO WATER TEMPERATURE SENSOR (WT)
    void WTsendCommand() {
      Serial1.print(commandString);
      Serial1.print('\r');
      Serial.print("Response from WT: ");

      String WTsendString = Serial1.readStringUntil(13);
      Serial.println(WTsendString);
      WTsendString = "";

      WTsendString = Serial1.readStringUntil(13);
      Serial.println(WTsendString);
      Serial.println();
      WTsendString = "";

      WTsendString = Serial1.readStringUntil(13);
      WTsendString = "";
      commandString = "";
    }

//SEND COMMAND TO CONDUCTIVITY SENSOR (EC)
    void ECsendCommand(){

      Serial2.print(commandString);
      Serial2.print('\r');
      Serial.print("Response from EC: ");

      String ECsendString = Serial2.readStringUntil(13);
      Serial.println(ECsendString);
      ECsendString = "";

      ECsendString = Serial2.readStringUntil(13);
      Serial.println(ECsendString);
      Serial.println();
      ECsendString = "";

      ECsendString = Serial2.readStringUntil(13);
      ECsendString = "";
      commandString = "";
    }

//SEND COMMAND TO PH SENSOR (PH)
    void PHsendCommand() {
      Serial3.print(commandString);
      Serial3.print('\r');
      Serial.print("Response from PH: ");

      String PHsendString = Serial3.readStringUntil(13);
      Serial.println(PHsendString);
      PHsendString = "";

      PHsendString = Serial3.readStringUntil(13);
      Serial.println(PHsendString);
      Serial.println();
      PHsendString = "";

      PHsendString = Serial3.readStringUntil(13);
      PHsendString = "";
      commandString = "";
    }

//SEND COMMAND TO DISSOLVED OXYGEN SENSOR (DO)  
    void DOsendCommand() {
      Serial4.listen();
      Serial4.print(commandString);
      Serial4.print('\r');
      Serial.print("Response from DO: ");
    
      String DOsendString = Serial4.readStringUntil(13);
      Serial.println(DOsendString);
      DOsendString = "";
    
      DOsendString = Serial4.readStringUntil(13);
      Serial.println(DOsendString);
      Serial.println();
      DOsendString = "";
    
      DOsendString = Serial4.readStringUntil(13);
      DOsendString = "";
      commandString = "";
    }
    
/************************************************************************************************
READ FUNCTIONS
**************/

//READ DATA FROM WATER TEMPERATURE SENSOR (WT)
    float readWT() {
      Serial1.print('R');
      Serial1.print('\r');

      String WTreadString = Serial1.readStringUntil(13);
      waterTemp = WTreadString.toFloat();
      waterTemp = waterTemp*1.8 + 32;
      WTreadString = "";
      WTreadString = Serial1.readStringUntil(13);
      
      //prints *OK if read. prints *ER if error
      Serial.print("Reading WT: ");
      Serial.println(WTreadString);
      WTreadString = "";
      return waterTemp;
    }

//READ DATA FROM CONDUCTIVITY SENSOR (EC)
    float readEC(){
      Serial2.print('R');
      Serial2.print('\r');
      
      String ECreadString = Serial2.readStringUntil(13);
      conductivity = ECreadString.toFloat();
      ECreadString = "";
      ECreadString = Serial2.readStringUntil(13); 
      
      //prints *OK if read. prints *ER if error
      Serial.print("Reading EC:  ");
      Serial.println(ECreadString);

      ECreadString = "";      
      return conductivity;
    }

//READ DATA FROM PH SENSOR (PH)
    float readPH() {   
      Serial3.print('R');
      Serial3.print('\r');
      
      String PHreadString = Serial3.readStringUntil(13);
      pH = PHreadString.toFloat();
      PHreadString = "";
      PHreadString = Serial3.readStringUntil(13);
      
      //prints *OK if read. prints *ER if error
      Serial.print("Reading PH:  ");
      Serial.println(PHreadString);
      PHreadString = "";
      return pH;
    }

//READ DATA FROM DISSOLVED OXYGEN SENSOR (DO)
    float readDO() {
      Serial4.listen();
      Serial4.print('R');
      Serial4.print('\r');
    
      String DOreadString = Serial4.readStringUntil(13);
      oxygen = DOreadString.toFloat();
      DOreadString = "";   
      DOreadString = Serial4.readStringUntil(13);

      //prints *OK if read. prints *ER if error
      Serial.print("Reading DO:  ");
      Serial.println(DOreadString);
      DOreadString = "";
      return oxygen;
    }

//READ DATA FROM HUMIDITY SENSOR (HM)
    float readHM() {
      humidity = dht.readHumidity();
      if (isnan(humidity)) {
        Serial.println("Failed to read humidity!");
      }
      return humidity;
    }

//READ DATA FROM AIR TEMPERATURE SENSOR (AT)
    float readAT() {
      airTemp = dht.readTemperature(true);
      if (isnan(airTemp)) {
        Serial.println("Failed to read air temperature!");
      }
      return airTemp;
    }
 
//READ DATA FROM CARBON DIOXIDE SENSOR (CB)
    long readCB() {
      Serial5.listen();
      Serial5.write(hexdata,9);
      delay(500);

      for(int i=0,j=0;i<9;i++){
        if (Serial5.available()>0){
          
          long hi, lo;
          int ch = Serial5.read();
    
          if(i==2){ hi=ch; }   //High concentration
          if(i==3){ lo=ch; }   //Low concentration
          if(i==8){
            carbon = hi*256+lo;  //CO2 concentration     
          }
        }       
      } 
      return carbon;  
    } 

//READ DATA FROM FLOW RATE SENSOR (PR)
  float readPR(){   
    float intercept = 0;
    float slope = 430;
    
    float count = analogRead(A0);
    float voltage = count / 1023 * 5.0; 
    par = intercept + voltage * slope; 
    return par;
  }

//READ DATA FROM WATER LEVEL SENSOR (WL)
  float readWL(){ 
    waterLevel = analogRead(SENSORPIN);
    waterLevel = (1023 / waterLevel) - 1;
    waterLevel = SERIESRESISTOR / waterLevel;
    return waterLevel;
  }
   
/********************************************************************************************
PRINT FUNCTIONS
***************/

//PRINT WT DATA TO SERIAL MONITOR
    void printWT() {
      Serial.print("Water Temperature: ");
      Serial.print("\t");
      Serial.print(waterTemp);
      Serial.println(" *F");
    }

//PRINT EC DATA TO SERIAL MONITOR
    void printEC (){     
      Serial.print("Conductivity: ");
      Serial.print("\t\t");
      Serial.print(conductivity);
      Serial.println(" (micro)S");
    }

//PRINT PH DATA TO SERIAL MONITOR
    void printPH() {
      Serial.print("pH: ");
      Serial.print("\t\t\t");
      Serial.println(pH);
    }

//PRINT DO DATA TO SERIAL MONITOR
    void printDO() {
      Serial.print("Dissolved Oxygen: ");
      Serial.print("\t");
      Serial.print(oxygen);
      Serial.println(" mg/L");
    }
    
//PRINT HM DATA TO SERIAL MONITOR
    void printHM() {
      Serial.print("Humidity: ");
      Serial.print("\t\t");
      Serial.print(humidity);
      Serial.println(" %");
    }

//PRINT AT DATA TO SERIAL MONITOR
    void printAT() {
      Serial.print("Air Temperature: ");
      Serial.print("\t");
      Serial.print(airTemp);
      Serial.println(" *F");
    }

//PRINT CB DATA TO SERIAL MONITOR
    void printCB() {
      Serial.print("CO2 Concentration: ");
      Serial.print("\t");
      Serial.print(carbon);
      Serial.println("ppm"); 
    }

//PRINT PR DATA TO SERIAL MONITOR
    void printPR() {
      Serial.print("PAR Level: ");
      Serial.print("\t");
      Serial.print(par);
      Serial.println("micromoles/m*s"); 
    }

//PRINT WL DATA TO SERIAL MONITOR
    void printWL() {
      Serial.print("Water Level: ");
      Serial.print("\t"); // tab character
      Serial.print(waterLevel);
      Serial.println(" "); 
    }
    
/*************************************************************************************************
ACCESSOR FUNCTIONS
*************************************************************************************************/

//GET WATER TEMPERATURE
    float getWaterTemp(){
      return waterTemp;
    }

//GET CONDUCTIVITY
    float getConductivity(){
      return conductivity;
    }
    
//GET PH
    float getPH(){
      return pH;
    }

//GET DISSOLVED OXYGEN
    float getOxygen(){
      return oxygen;
    }

//GET HUMIDITY
    float getHumidity(){
      return humidity;
    }

//GET AIR TEMPERATURE
    float getAirTemp(){
      return airTemp;
    }

//GET CARBON DIOXIDE
    long getCarbon(){
      return carbon;
    }

//GET PAR LEVEL
    float getPar(){
      return par;
    }

//GET WATER LEVEL
    float getWaterLevel(){
      return waterLevel;
    }
    
};
#endif
