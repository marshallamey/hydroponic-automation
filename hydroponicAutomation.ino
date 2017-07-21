/*
This is a program to automate and control the parameters of a hydroponic garden.
Code was designed to be run with an Arduino Mega 2560
The project was completed by research students at San Antonio College
*****ATLAS SCIENTIFIC SENSORS WILL REQUIRE A POINTER AND A COMMAND FOR SERIAL COMMUNICATION*****
The pointers designate which sensor receives a command.
For commands, please refer to Atlas Scientific Datasheet in the GitHub Repository.
DIRECTIONS: Type the pointer using the serial monitor and press enter.  Type your command and press enter.
Make sure to use Carriage Return only
Baud rate is 9600 bps
*/


//LIBRARIES
//#include <ArduinoUnit.h>
#include <SoftwareSerial.h>  
#include <SPI.h>
#include <WiFi101.h>         
#include <Adafruit_RGBLCDShield.h>
#include "ThingSpeak.h"
#include "Sensor.h"
#include "Motor.h"
//#include "Menu.h"
#include "DHT.h"  


// TIMING VARIABLES
long readDataInterval = 30000;
long printDataInterval = 5000;
unsigned long currentMillis;
unsigned long previousMillis01 = 0;
unsigned long previousMillis02 = 0;

//WIFI VARIABLES  
char ssid[] = "CoC Student Center";    //  your network SSID (name) 
char pass[] = "sanantonio210";   // your network password
int status = WL_IDLE_STATUS;

char thingSpeakAddress[] = "api.thingspeak.com";
unsigned long myChannelNumber = 298095;
String myWriteAPIKey = "F8Q6F3XFDI3QDLUY";


//INITIALIZERS
int lcdPageNumber = 0;
SENSOR Sensor;
MOTOR Motor;
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
WiFiServer server(80);
WiFiClient client;


/*************************************************************************************
SETUP AND LOOP FUNCTIONS
*************************************************************************************/


void setup() {
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   HYDROPONIC   ");
  lcd.setCursor(0,1);
  lcd.print("   SYSTEM  ON   ");
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial4.begin(9600);
  Serial5.begin(9600);
 
  Motor.initializePins();
  dht.begin();
  delay(1500);
   
  //Connect to WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Wifi Network: ");
    Serial.println(ssid);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" CONNECTING TO  ");
    lcd.setCursor(0,1);
    lcd.print("      WIFI      ");
    status = WiFi.begin(ssid, pass);
    delay(7000);   // WAIT FOR CONNECTION
  }

  Serial.println("You're connected to the network");
  lcd.clear();
  lcd.print(" WIFI CONNECTED ");
  printCurrentNet();
  printWifiData();
  delay(1500);
  
  Serial.println("Initializing data collection...");
  Serial.println();
  lcd.clear();
  lcd.print("Collecting data...");
  readData();
  //ThingSpeak.begin(client);
}


void loop() {
  currentMillis = millis();

  if (Serial.available() > 0) { sendSensorCommand(); } 
  
  if (currentMillis - previousMillis01 > readDataInterval) {
    previousMillis01 = currentMillis; 
    readData();
    printToInternet();
    //monitorSolution();
  }  

  if (currentMillis - previousMillis02 > printDataInterval) {
    previousMillis02 = currentMillis; 
    printToLCD();
  }  
}


/******************************************************************************
MEMBER FUNCTIONS
******************************************************************************/


//SEND A COMMAND TO AN ATLAS SCIENTIFIC SENSOR
  void sendSensorCommand() {
    String whichSensorString = Serial.readStringUntil(13);
    Serial.print("Connecting to: ");
    Serial.println(whichSensorString);
    Sensor.getCommand();
    
    if      (whichSensorString == "WT") { Sensor.WTsendCommand(); }
    else if (whichSensorString == "EC") { Sensor.ECsendCommand(); }
    else if (whichSensorString == "PH") { Sensor.PHsendCommand(); }
    else if (whichSensorString == "DO") { Sensor.DOsendCommand(); }  
    
    Serial.println("Sending command");  
  }


//READ DATA FROM ALL SENSORS
  void readData() {
    Serial.println("Reading sensors... ");
    Sensor.readWT();
    Sensor.readEC();
    Sensor.readPH();
    Sensor.readDO();
    Sensor.readHM();
    Sensor.readAT();
    Sensor.readCB();
    Sensor.readPR();
    //Sensor.readWL();
    Serial.println("Reading Complete.");
    Serial.println();
  }


//MONITOR PARAMETERS OF SOLUTION
  void monitorSolution() {
    //**VARIABLE PARAMETERS CAN BE CHANGED HERE**//
    float WT_high = 80.0;
    float WT_low = 70.0;
    float EC_high = 2200;
    float EC_low = 1800;
    float PH_high = 6.6;
    float PH_low = 5.8;
    float DO_high = 30;
    float DO_low = 12;  
    float HM_high = 80.0;
    float HM_low = 40.0;
    float AT_high = 80.0;
    float AT_low = 50.0;
    float CB_high;
    float CB_low;
    float PR_high;
    float PR_low;
    float WL_high;
    float WL_low;
      
    //if (Sensor.getWaterTemp() < WT_low) { Motor.raiseWaterTemp(); }
    //if (Sensor.getWaterTemp() > WT_high) { Motor.lowerWaterTemp(); }
    //if (Sensor.getConductivity() < EC_low) { Motor.raiseConductivity(); }
    //if (Sensor.getConductivity() > EC_high) { Motor.lowerConductivity(); }  
    //if (Sensor.getPH() < PH_low) { Motor.raisePH(); }
    //if (Sensor.getPH() > PH_high) { Motor.lowerPH(); }  
    //if (Sensor.getOxygen() < DO_low) { Motor.raiseOxygenLevel(); }
    //if (Sensor.getOxygen() > DO_high) { Motor.lowerOxygenLevel(); }
    //if (Sensor.getHumidity() < HM_low) { Motor.raiseOxygenLevel(); }
    //if (Sensor.getHumidity() > HM_high) { Motor.lowerOxygenLevel(); }
    //if (Sensor.getAirTemp() < AT_low) { Motor.raiseAirTemp(); }
    //if (Sensor.getAirTemp() > AT_high) { Motor.lowerAirTemp(); }
    //if (Sensor.getCarbon() < CB_low) { Motor.raiseCarbon(); }
    //if (Sensor.getCarbon() > CB_high) { Motor.lowerCarbon(); }
    //if (Sensor.getPar() < FR_low) { Motor.raiseFlowRate(); }
    //if (Sensor.getPar() > FR_high) { Motor.lowerFlowRate(); }
    //if (Sensor.getWaterLevel() < WL_low) { Motor.raiseFlowRate(); }
    //if (Sensor.getWaterLevel() > WL_high) { Motor.lowerFlowRate(); }
  }


/******************************************************************************
PRINT FUNCTIONS
******************************************************************************/


//PRINT SSID OF NETWORK CONNECTED TO
  void printCurrentNet() {
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
  }


//PRINT WIFI SHIELD'S IP ADDRESS
  void printWifiData() {  
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
  }


//PRINT DATA TO SERIAL MONITORS
  void printToSerial(){
    Serial.println("Printing data...");
    Sensor.printWT();
    Sensor.printEC();
    Sensor.printPH();
    Sensor.printDO();
    Sensor.printHM();
    Sensor.printAT();
    Sensor.printCB();
    Sensor.printPR();
    //Sensor.printWL();
    Serial.println();
  }


//PRINT DATA TO INTERNET
  void printToInternet() {
      String Readings = "field1= " + String(Sensor.getWaterTemp()) + "&field2= " + String(Sensor.getConductivity()) 
    + "&field3= " + String(Sensor.getPH()) + "&field4= " + String(Sensor.getOxygen())
    + "&field5= " + String(Sensor.getAirTemp()) + "&field6= " + String(Sensor.getHumidity());

      
      
      if (client.connect(thingSpeakAddress, 80)) {
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: " + myWriteAPIKey + "\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(Readings.length());
        client.print("\n\n");
        client.print(Readings);
        //lastConnectionTime = millis();
        client.stop();

      if (client.connected()) {
        Serial.println("Connecting to ThingSpeak...");
        Serial.println();
        }
      }else {
        // if you couldn't make a connection:
        Serial.println("connection failed, reset in progress...");
        delay(1000);
        asm volatile ("  jmp 0");
      }    


    
//    Serial.println("Sending data to ThingSpeak...");
//    ThingSpeak.setField(1,Sensor.getWaterTemp());
//    ThingSpeak.setField(2,Sensor.getConductivity());
//    ThingSpeak.setField(3,Sensor.getPH());
//    ThingSpeak.setField(4,Sensor.getOxygen());
//    ThingSpeak.setField(5,Sensor.getAirTemp());
//    ThingSpeak.setField(6,Sensor.getHumidity());
//    ThingSpeak.setField(7,Sensor.getCarbon());
//    ThingSpeak.setField(8,Sensor.getPar());
//    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); 
//    Serial.println(); 
//    ThingSpeak.getHttpResponce();
    }


//PRINT DATA TO LCD DISPLAY IN TWO SEPARATE PAGES
  void printToLCD() {
    if (lcdPageNumber == 0) {
      printPageNumber0();
      lcdPageNumber = 1;
    }
    else {
      printPageNumber1();
      lcdPageNumber = 0;
    }
  }


//PRINT FIRST PAGE TO LCD
  void printPageNumber0() {
    int wtemp = Sensor.getWaterTemp();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WT ");
    lcd.print(wtemp);
    lcd.print("F  ");
    lcd.setCursor(9, 0);
    lcd.print("DO ");
    lcd.print(Sensor.getOxygen());
    lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print("PH ");
    lcd.print(Sensor.getPH());
    lcd.print(" ");
    lcd.setCursor(9, 1);
    lcd.print("EC ");
    lcd.print(Sensor.getConductivity());
    lcd.print("");
  }


//PRINT SECOND PAGE TO LCD
  void printPageNumber1() {
    int atemp = Sensor.getAirTemp();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AT ");
    lcd.print(atemp);
    lcd.print("F  ");
    lcd.setCursor(9, 0);
    lcd.print("HM ");
    lcd.print(Sensor.getHumidity());
    lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print("CB ");
    lcd.print(Sensor.getCarbon());
    lcd.print(" ");
    lcd.setCursor(9, 1);
    lcd.print("PR ");
    lcd.print(Sensor.getPar());
    lcd.print("");
  }
