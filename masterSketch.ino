
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

#include <SoftwareSerial.h>           
#include <Adafruit_RGBLCDShield.h>
#include "Sensor.h"
#include "DHT.h"  



// DHT Temperature and Humidity Sensor
//#define DHTPIN 4        
//#define DHTTYPE DHT22   
//DHT dht(DHTPIN, DHTTYPE);

// MOTOR PIN LOCATIONS
int pHup_pin = 30;
int pHdown_pin = 31;
int fertA_pin = 32;
int fertB_pin = 33;
int mixPump_pin = 34;
int stonePump_pin = 35;
int water_pin = 36;


int lcdPageNumber = 0;
unsigned long currentMillis;
unsigned long previousMillis01 = 0;
unsigned long previousMillis02 = 0;
long readInterval = 10000;
long menuScrollTime = 5000;
long pumpInterval = 5000;

Sensor Sensor;

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

/*************************************************************************************
SETUP AND LOOP FUNCTIONS
*************************************************************************************/

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial4.begin(9600);
  
  pinMode(pHup_pin, OUTPUT);
  pinMode(pHdown_pin, OUTPUT);
  pinMode(fertA_pin, OUTPUT);
  pinMode(fertB_pin, OUTPUT);
  pinMode(mixPump_pin, OUTPUT);
  pinMode(stonePump_pin, OUTPUT);
  pinMode(water_pin, OUTPUT);

  digitalWrite(pHup_pin, HIGH);
  digitalWrite(pHdown_pin, HIGH);
  digitalWrite(fertA_pin, HIGH);
  digitalWrite(fertB_pin, HIGH);
  digitalWrite(mixPump_pin, HIGH);
  digitalWrite(stonePump_pin, HIGH);
  digitalWrite(water_pin, HIGH);

  Serial.println("Initializing data collection...");
  Serial.println();
  dht.begin();
  lcd.begin(16, 2);
  lcd.print("Collecting data...");
  uint8_t i = 0;
}

void loop() {
  currentMillis = millis();
  
  if (Serial.available() > 0) {
    sendSensorCommand();
  }
  
  if (currentMillis - previousMillis01 > readInterval) {
    previousMillis01 = currentMillis;
    
    readData();
    printToSerial();
    //printToInternet();
    monitorSolution();
  }

  if (currentMillis - previousMillis02 > menuScrollTime) {
    previousMillis02 = currentMillis;
    
    printToLCD();
  }
}

/******************************************************************************************
SEND FUNCTIONS
******************************************************************************************/

// SEND A COMMAND TO AN ATLAS SCIENTIFIC SENSOR
void sendSensorCommand() {
    whichSensor();
    if (whichSensor() == "WT") { 
      Sensor.getCommand(); 
      Sensor.WTsendCommand(); 
    }
    else if (whichSensor() == "EC") { 
      Sensor.getCommand();
      Sensor.ECsendCommand(); 
    }
    else if (whichSensor() == "PH") { 
      Sensor.getCommand();
      Sensor.PHsendCommand(); 
    }
    else if (whichSensor() == "DO") { 
      Sensor.getCommand();
      Sensor.DOsendCommand(); 
    }  
    Serial.println("Sending command");  
  }

//DETERMINE WHICH SENSOR TO SEND A COMMAND TO
String whichSensor() {
  String whichSensorString = "";
  whichSensorString = Serial.readStringUntil(13);
  Serial.print("Sending to: ");
  Serial.println(whichSensorString);
  return whichSensorString;
}

/*****************************************************************************
READ FUNCTIONS
*****************************************************************************/

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
  Serial.println();
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
void printToInternet() {}

//PRINT DATA TO LCD DISPLAY
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
  lcd.clear();
  lcd.setCursor(0, 0);
  int wtemp = Sensor.getWaterTemp();
  int atemp = Sensor.getAirTemp();
  lcd.print("H20 ");
  lcd.print(wtemp);
  lcd.print("F ");
  lcd.print("AIR ");
  lcd.print(atemp);
  lcd.print("F");
  lcd.setCursor(0, 1);
  lcd.print("PH ");
  lcd.print(Sensor.getPH());
  lcd.print(" ");
  lcd.print("EC ");
  lcd.print(Sensor.getConductivity());
  lcd.print("");
}

//PRINT SECOND PAGE TO LCD
void printPageNumber1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HUMIDITY ");
  lcd.print(Sensor.getHumidity());
  lcd.print(" ");
  lcd.setCursor(0, 1);
  lcd.print("DO ");
  lcd.print(Sensor.getOxygen());
  lcd.print(" ");
  lcd.print("CO2 ");
  lcd.print(Sensor.getCarbon());
  lcd.print("");
}

/***********************************************************************************
MONITOR FUNCTIONS
***********************************************************************************/

//MONITOR PARAMETERS OF SOLUTION
void monitorSolution() {

  //**VARIABLE PARAMETERS CAN BE CHANGED HERE**//
  float WT_high = 80.0;
  float WT_low = 70.0;
  float EC_high = 20000;
  float EC_low = 2000;
  float PH_high = 6.5;
  float PH_low = 5.5;
  float DO_high = 5000;
  float DO_low = 2000;  
  float HM_high = 80.0;
  float HM_low = 40.0;
  float AT_high = 80.0;
  float AT_low = 50.0;
  
  //if (Sensor.getWaterTemp() < WT_low) { raiseWaterTemp(); }
  //if (Sensor.getWaterTemp() > WT_high) { lowerWaterTemp(); }
  if (Sensor.getConductivity() < EC_low) { raiseConductivity(); }
  //if (Sensor.getConductivity() > EC_high) { lowerConductivity(); }  
  if (Sensor.getPH() < PH_low) { raisePH(); }
  if (Sensor.getPH() > PH_high) { lowerPH(); }  
  if (Sensor.getOxygen() < DO_low) { raiseOxygenLevel(); }
  //if (Sensor.getOxygen() > DO_high) { lowerOxygenLevel(); }
  //if (Sensor.getHumidity() < HM_low) { raiseOxygenLevel(); }
  //if (Sensor.getHumidity() > HM_high) { lowerOxygenLevel(); }
  //if (Sensor.getWaterTemp() < WT_low) { raiseWaterTemp(); }
  //if (Sensor.getWaterTemp() > WT_high) { lowerWaterTemp(); }
}

// RAISE WATER TEMPERATURE OF THE NUTRIENT SOLUTION
// void raiseWaterTemp() {}

// LOWER WATER TEMPERATURE OF THE NUTRIENT SOLUTION
// void lowerWaterTemp() {}

//RAISE CONDUCTIVITY OF THE NUTRIENT SOLUTION
void raiseConductivity() {
  digitalWrite(fertA_pin, LOW);
  digitalWrite(fertB_pin, LOW);
  digitalWrite(mixPump_pin, LOW);
  long ecInterval = 0;
  
  while (ecInterval < pumpInterval) {
    ++ecInterval;
  }
  digitalWrite(fertA_pin, HIGH);
  digitalWrite(fertB_pin, HIGH);    
  digitalWrite(mixPump_pin, HIGH);
}

//LOWER CONDUCTIVITY OF THE NUTRIENT SOLUTION
// void lowerConductivity() {}

//RAISE PH OF THE NUTRIENT SOLUTION
  void raisePH() {
  digitalWrite(pHup_pin, LOW);
  digitalWrite(mixPump_pin, LOW);
  long phInterval = millis();
  currentMillis = millis();

  while (currentMillis - phInterval < pumpInterval) { 
    currentMillis = millis();
  }

  digitalWrite(pHup_pin, HIGH);
  digitalWrite(mixPump_pin, HIGH);
  }

//LOWER PH OF THE NUTRIENT SOLUTION
  void lowerPH() {
  digitalWrite(pHdown_pin, LOW);
  digitalWrite(mixPump_pin, LOW);
  long phInterval = 0;
  
  while (phInterval < pumpInterval) {
    ++phInterval;
  }
  digitalWrite(pHdown_pin, HIGH);
  digitalWrite(mixPump_pin, HIGH);
  }

//RAISE OXYGEN LEVEL OF THE NUTRIENT SOLUTION
void raiseOxygenLevel() {
  digitalWrite(stonePump_pin, LOW);
  digitalWrite(mixPump_pin, LOW);
  int doInterval = 0;
  while (doInterval < pumpInterval) {
    ++doInterval;
  }

  digitalWrite(stonePump_pin, HIGH);
  digitalWrite(mixPump_pin, HIGH);
}

// LOWER OXYGEN LEVEL OF THE NUTRIENT SOLUTION
// void lowerOxygenLevel() {}

// RAISE HUMIDITY OF THE ENVIRONMENT
// LOWER HUMIDITY OF THE ENVIRONMENT

// RAISE AIR TEMPERATURE OF THE ENVIRONMENT
// LOWER AIR TEMPERATURE OF THE ENIVRONMENT

// RAISE CARBON DIOXIDE LEVEL OF THE ENVIRONMENT
// LOWER CARBON DIOXIDE LEVEL OF THE ENVIRONMENT

// RAISE FLOW RATE OF THE NUTRIENT SOLUTION
// LOWER FLOW RATE OF THE NUTRIENT SOLUTION


