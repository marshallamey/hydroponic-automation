
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
#include "Menu.h"
#include "DHT.h"  

// TIMING VARIABLES
long readDataInterval = 30000;
unsigned long currentMillis;
unsigned long previousMillis01 = 0;
unsigned long previousMillis02 = 0;

//WIFI VARIABLES  
char ssid[] = "CoC Student Center";    //  your network SSID (name) 
char pass[] = "sanantonio210";   // your network password
int status = WL_IDLE_STATUS;
unsigned long myChannelNumber = 298095;
const char * myWriteAPIKey = "F8Q6F3XFDI3QDLUY";

/*
//LCD MENU VARIABLES
int currentMenuItem = 0;
//int lastState = 0;
unsigned long lastButtonPress = 0;
int lcdPageNumber = 0;
int lcdIdleTime = 10 * 1000;
long menuScrollTime = 5 * 1000;
enum operatingState {HOME, MENU, RIGHT, SENSORS, MOTORS, NUTRIENT_A, NUTRIENT_B, PH_UP, PH_DOWN};                      
*/

//SENSOR Sensor;
//MOTOR Motor;
MENU Menu;
//Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
//operatingState opState = HOME;
WiFiClient  client;

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
  Motor.initializePins();
  dht.begin();
  delay(1500);
   
  //CONNECT TO WIFI
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
  ThingSpeak.begin(client);
}

void loop() {
  currentMillis = millis();

  if (Serial.available() > 0) { sendSensorCommand(); } 
  
  if (currentMillis - previousMillis01 > readDataInterval) {
    previousMillis01 = currentMillis; 
    readData();
    printToInternet();
    //printToSerial();
    //monitorSolution();
  }  
  Menu.lcdMenu();
}

// print the SSID of the network you're attached to:
  void printCurrentNet() {
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
  }

// print your WiFi shield's IP address:
  void printWifiData() {  
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
  }

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
    //Sensor.readCB();
    //Sensor.readFR();
    Serial.println("Reading Complete.");

  }

/******************************************************************************
PRINT FUNCTIONS
******************************************************************************/

//PRINT DATA TO SERIAL MONITORS
  void printToSerial(){
    Serial.println("Printing data...");
    Sensor.printWT();
    Sensor.printEC();
    Sensor.printPH();
    Sensor.printDO();
    Sensor.printHM();
    Sensor.printAT();
    //Sensor.printCB();
    //Sensor.printFR();
    Serial.println();
  }

//PRINT DATA TO INTERNET
  void printToInternet() {
    Serial.println("Sending data to ThingSpeak...");
    ThingSpeak.setField(1,Sensor.getWaterTemp());
    ThingSpeak.setField(2,Sensor.getConductivity());
    ThingSpeak.setField(3,Sensor.getPH());
    ThingSpeak.setField(4,Sensor.getOxygen());
    ThingSpeak.setField(5,Sensor.getAirTemp());
    ThingSpeak.setField(6,Sensor.getHumidity());
    ThingSpeak.setField(7,Sensor.getCarbon());
    ThingSpeak.setField(8,Sensor.getFlowRate());
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); 
    //Serial.println(); 
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
  float FR_high;
  float FR_low;
  
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
  //if (Sensor.getFlowRate() < FR_low) { Motor.raiseFlowRate(); }
  //if (Sensor.getFlowRate() > FR_high) { Motor.lowerFlowRate(); }
}
