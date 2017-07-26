#ifndef Sensor_h
#define Sensor_h

#include <SoftwareSerial.h>
#include "DHT.h"

// Establish Serial Port 4  (RX, TX)
SoftwareSerial Serial4 (13, 12);
SoftwareSerial Serial5 (69, 68);

// Read the gas density command /Don't change the order
unsigned char hexdata[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; 

// DHT Temperature and Humidity Sensor
#define DHTPIN 4        
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
      Serial.print("Reading WT... ");

      //Send command for single reading
      Serial1.print('R');
      Serial1.print('\r');

      //Get reading
      String WTreadString = Serial1.readStringUntil(13);
      waterTemp = WTreadString.toFloat();
      WTreadString = "";
           
      //Get response. Prints *OK if read.
      WTreadString = Serial1.readStringUntil(13);      
      if (WTreadString != "*OK"){ Serial.println("Failed to read water temperature!"); }
      else{ Serial.println(WTreadString); }
      WTreadString = ""; 
         
      return waterTemp;
    }

//READ DATA FROM CONDUCTIVITY SENSOR (EC)
    float readEC(){
      Serial.print("Reading EC... ");

      //Send command for single reading
      Serial2.print('R');
      Serial2.print('\r');

      //Get reading
      String ECreadString = Serial2.readStringUntil(13);
      conductivity = ECreadString.toFloat();
      conductivity += conductivity * .208;
      ECreadString = "";

      //prints *OK if read.
      ECreadString = Serial2.readStringUntil(13);          
      if (ECreadString != "*OK"){ Serial.println("Failed to read conductivity!"); }
      else{ Serial.println(ECreadString); }
      ECreadString = "";   
      
      return conductivity;
    }

//READ DATA FROM PH SENSOR (PH)
    float readPH() {   
      Serial.print("Reading PH... ");
      
      /*//Send command for single reading
      Serial3.print('R');
      Serial3.print('\r');

      //Get reading
      String PHreadString = Serial3.readStringUntil(13);
      pH = PHreadString.toFloat();
      PHreadString = "";
           
      //prints *OK if read
      PHreadString = Serial3.readStringUntil(13);
      if (PHreadString != "*OK"){ Serial.println("Failed to read pH level!"); }
      else{ Serial.println(PHreadString); }
      PHreadString = "";*/

      float intercept = 13.720;
      float slope = -3.838;

      //Analog reading is currently twice what it should be.  If this changes, remove division by 2
      float count = analogRead(A0) / 2;
      float voltage = count / 1023 * 5.0;
      
      pH = intercept + voltage * slope;

      if (isnan(par))  { Serial.println("Failed to read PH level!"); }
      else             { Serial.println("*OK"); }

      //Troubleshooting pH calibration.  Uncomment to view voltage readings
      //Serial.print("Analog Value: ");
      //Serial.println(count);
      //Serial.print("Voltage: ");
      //Serial.println(voltage);
      //Serial.print("pH: ");
      //Serial.println(pH);
      
      return pH;
    }

//READ DATA FROM DISSOLVED OXYGEN SENSOR (DO)
    float readDO() {
      //Activate software serial port 4
      Serial4.listen();
      Serial.print("Reading DO... ");

      //Send command for single reading
      Serial4.print('R');
      Serial4.print('\r');

      //Get reading
      String DOreadString = Serial4.readStringUntil(13);
      oxygen = DOreadString.toFloat();
      DOreadString = "";   

      //prints *OK if read.
      DOreadString = Serial4.readStringUntil(13);
      if (DOreadString != "*OK"){ Serial.println("Failed to read dissolved oxygen!"); }
      else{ Serial.println(DOreadString); }
      DOreadString = "";
      
      return oxygen;
    }

//READ DATA FROM HUMIDITY SENSOR (HM)
    float readHM() {
      Serial.print("Reading HM... ");
      humidity = dht.readHumidity();
      if (isnan(humidity)) {
        Serial.println("Failed to read humidity!");
      }
      else{
        Serial.println("*OK");
      }
      return humidity;
    }

//READ DATA FROM AIR TEMPERATURE SENSOR (AT)
    float readAT() {
      Serial.print("Reading AT... ");
      airTemp = dht.readTemperature(true);
      if (isnan(airTemp)) {
        Serial.println("Failed to read air temperature!");
      }
      else{
        Serial.println("*OK");
      }
      return airTemp;
    }
 
//READ DATA FROM CARBON DIOXIDE SENSOR (CB)
    long readCB() {
      //Activate software serial port 5
      Serial5.listen();
      Serial.print("Reading CB... ");
      
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
      if (isnan(carbon)) { Serial.println("Failed to read carbon dioxide!"); }
      else               { Serial.println("*OK"); }
      
      return carbon;  
    } 

//READ DATA FROM FLOW RATE SENSOR (PR)
    float readPR(){
      Serial.print("Reading PR... ");   
      /*float intercept = 0;
      float slope = 430;
    
      float count = analogRead(A0);
      float voltage = count / 1023 * 5.0;
      par = intercept + voltage * slope;*/

      if (par==0)  { Serial.println("Failed to read PAR level!"); }
      else             { Serial.println("*OK"); }
      
      return par;
    }

//READ DATA FROM WATER LEVEL SENSOR (WL)
    float readWL(){ 
      Serial.print("Reading WL... ");
      waterLevel = analogRead(SENSORPIN);
      waterLevel = (1023 / waterLevel) - 1;
      waterLevel = SERIESRESISTOR / waterLevel;
      
      if (isnan(waterLevel))  { Serial.println("Failed to read water level!"); }
      else                    { Serial.println("*OK"); }
      
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
      Serial.println(" F");
    }

//PRINT EC DATA TO SERIAL MONITOR
    void printEC (){     
      Serial.print("Conductivity: ");
      Serial.print("\t\t");
      Serial.print(conductivity);
      Serial.println(" microSiemens/cm");
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
      Serial.println(" F");
    }

//PRINT CB DATA TO SERIAL MONITOR
    void printCB() {
      Serial.print("CO2 Concentration: ");
      Serial.print("\t");
      Serial.print(carbon);
      Serial.println(" ppm"); 
    }

//PRINT PR DATA TO SERIAL MONITOR
    void printPR() {
      Serial.print("PAR Level: ");
      Serial.print("\t\t");
      Serial.print(par);
      Serial.println(" micromoles/m*s"); 
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
