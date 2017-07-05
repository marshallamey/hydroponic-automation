
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
#include <SoftwareSerial.h>  
#include <SPI.h>
#include <WiFi101.h>         
#include <Adafruit_RGBLCDShield.h>
#include "ThingSpeak.h"
#include "Sensor.h"
#include "Motor.h"
#include "DHT.h"  

String whichSensorString = "";

// TIMING VARIABLES
long readDataInterval = 20000;
unsigned long currentMillis;
unsigned long previousMillis01 = 0;
unsigned long previousMillis02 = 0;

//WIFI VARIABLES  
char ssid[] = "CoC Student Center";    //  your network SSID (name) 
char pass[] = "sanantonio210";   // your network password
int status = WL_IDLE_STATUS;
unsigned long myChannelNumber = 298095;
const char * myWriteAPIKey = "F8Q6F3XFDI3QDLUY";

//LCD MENU VARIABLES
int currentMenuItem = 0;
//int lastState = 0;
unsigned long lastButtonPress = 0;
int lcdPageNumber = 0;
int lcdIdleTime = 10 * 1000;
long menuScrollTime = 5 * 1000;
enum operatingState {HOME, MENU, RIGHT, SENSORS, MOTORS, NUTRIENT_A, NUTRIENT_B, PH_UP, PH_DOWN};                      

SENSOR Sensor;
MOTOR Motor;
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
operatingState opState = HOME;
WiFiClient  client;

/*************************************************************************************
SETUP AND LOOP FUNCTIONS
*************************************************************************************/

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial4.begin(9600);
  
  Motor.initializePins();
  WiFi.begin(ssid, pass);
  ThingSpeak.begin(client);
  
  Serial.println("Initializing data collection...");
  Serial.println();
  dht.begin();
  lcd.begin(16, 2);
  lcd.print("Collecting data...");
  uint8_t i = 0;
  readData();
}

void loop() {
  currentMillis = millis();
  lcdMenu();

  if (Serial.available() > 0) {
    sendSensorCommand();
  }
  
  if (currentMillis - previousMillis01 > readDataInterval) {
    previousMillis01 = currentMillis; 
    readData();
    printToSerial();
    printToInternet();
    //monitorSolution();
  } 
}


//SEND A COMMAND TO AN ATLAS SCIENTIFIC SENSOR
  void sendSensorCommand() {
    whichSensorString = Serial.readStringUntil(13);
    Serial.print("Connecting to: ");
    Serial.println(whichSensorString);
    Sensor.getCommand();
    
    if (whichSensorString == "WT") { Sensor.WTsendCommand(); }
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
  void printToInternet() {
    ThingSpeak.setField(1,Sensor.getWaterTemp());
    ThingSpeak.setField(2,Sensor.getConductivity());
    ThingSpeak.setField(3,Sensor.getPH());
    ThingSpeak.setField(4,Sensor.getOxygen());
    ThingSpeak.setField(5,Sensor.getAirTemp());
    ThingSpeak.setField(6,Sensor.getHumidity());
    ThingSpeak.setField(7,Sensor.getCarbon());
    ThingSpeak.setField(8,Sensor.getFlowRate());
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);  
    }

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
  if (Sensor.getConductivity() < EC_low) { Motor.raiseConductivity(); }
  //if (Sensor.getConductivity() > EC_high) { Motor.lowerConductivity(); }  
  if (Sensor.getPH() < PH_low) { Motor.raisePH(); }
  if (Sensor.getPH() > PH_high) { Motor.lowerPH(); }  
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

/*************************************************************************************
MENU FUNCTIONS
**************/

//LCD DISPLAY DATA
  void lcdMenu(){
    // wait for button release before changing state
    while(ReadButtons() != 0) {}
    
    switch (opState){
    case HOME:
      HomeScreen();
      break;
    case MENU:
      MainMenu();
      break;
    case SENSORS:
      Sensors();
      break;
    case MOTORS:
      Motors();
      break;   
    case NUTRIENT_A:
      NutrientA();
      break;
    case NUTRIENT_B:
      NutrientB();
      break;
    case PH_UP:
      phUP();
      break;
    case PH_DOWN:
      phDOWN();
      break;   
    }
  }

//WATCH FOR BUTTON PUSH 
  uint8_t ReadButtons(){
    uint8_t buttons = lcd.readButtons();
    if (buttons != 0){
      lastButtonPress = millis();
    }
    return buttons;
  }  

//HOME SCREEN
  void HomeScreen(){
    uint8_t buttons = 0;
    //while(true){
      buttons = ReadButtons();
      if (buttons !=0 ){
        opState = MENU;
        return;   
      }
      else{
        currentMillis = millis();       
        if (currentMillis - previousMillis02 > menuScrollTime) {
        printToLCD();
        previousMillis02 = currentMillis;
        }  
   
      //}
    }
  }

//MAIN MENU SCREEN
  void MainMenu(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      
      //BUTTON ASSIGNMENT
      buttons = ReadButtons();
        if (buttons & BUTTON_UP){
           currentMenuItem = currentMenuItem-1;
           return;
        }
        if (buttons & BUTTON_DOWN){
           currentMenuItem = currentMenuItem+1;
           return;
        }
        if (buttons & BUTTON_LEFT){
           opState = HOME;
           return;
        }    
        if (buttons & BUTTON_RIGHT){
          if(currentMenuItem == 0){
            opState = SENSORS;
            currentMenuItem = 0;
            return;
          }
          if(currentMenuItem == 1){
            opState = MOTORS;
            currentMenuItem = 0;
            return;
          }
        }
        if (buttons & BUTTON_SELECT) {
          opState = HOME;
          currentMenuItem = 0;
          return;  
        }
      
      //SCROLL THROUGH MENU ITEMS
      if(currentMenuItem == 0){
        lcd.setCursor(0, 0);
        lcd.print(">");
        lcd.setCursor(1, 0);
        lcd.print("Sensors");
        lcd.setCursor(1, 1);
        lcd.print("Motors");
        }
      if(currentMenuItem == 1){
        lcd.setCursor(1, 0);
        lcd.print("Sensors");
        lcd.setCursor(0, 1);
        lcd.print(">");
        lcd.print("Motors");
        }

      //RESTART MENU WHEN OUT OF BOUNDS
      if (currentMenuItem < 0) {
        currentMenuItem = 1;
      }
      else if (currentMenuItem > 1) {
        currentMenuItem = 0;
      }
      
      //RETURN TO HOME SCREEN IF IDLE 
      if ((millis() - lastButtonPress) > lcdIdleTime){
        lcd.clear();
        opState = HOME; 
        currentMenuItem = 0;
        return;
      }
    }
    
  }

//VIEW INDIVIDUAL SENSOR READINGS
  void Sensors(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
  
    //BUTTON ASSIGNMENT
    buttons = ReadButtons();
      if (buttons & BUTTON_UP){
         currentMenuItem = currentMenuItem-1;
         return;
      }
      if (buttons & BUTTON_DOWN){
         currentMenuItem = currentMenuItem+1;
         return;
      }
      if (buttons & BUTTON_RIGHT){
         opState = HOME;
         currentMenuItem = 0;
         return;
      }
      if (buttons & BUTTON_LEFT){
         opState = MENU;
         return;
      }
      if (buttons & BUTTON_SELECT) {
          opState = HOME;
          currentMenuItem = 0;
          return;  
        }
  
    //SCROLL THROUGH MENU ITEMS
      if(currentMenuItem == 0){
        lcd.setCursor(0, 0);
        lcd.print("H2O Temperature");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getWaterTemp());
        }
      if(currentMenuItem == 1){
        lcd.setCursor(0, 0);
        lcd.print("Conductivity");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getConductivity());
        }
      if(currentMenuItem == 2){
        lcd.setCursor(0, 0);
        lcd.print("pH");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getPH());
        }
      if(currentMenuItem == 3){
        lcd.setCursor(0, 0);
        lcd.print("Dissolved Oxygen");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getOxygen());
        }
      if(currentMenuItem == 4){
        lcd.setCursor(0, 0);
        lcd.print("Air Temperature ");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getAirTemp());
        }               
      if(currentMenuItem == 5){
        lcd.setCursor(0, 0);
        lcd.print("Humidity ");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getHumidity());
        }      
      if(currentMenuItem == 6){
        lcd.setCursor(0, 0);
        lcd.print("Carbon Dioxide ");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getCarbon());
        }
      if(currentMenuItem == 7){
        lcd.setCursor(0, 0);
        lcd.print("Flow Rate");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getFlowRate());
        }
  
      //RESTART MENU WHEN OUT OF BOUNDS
      if (currentMenuItem < 0) {
        currentMenuItem = 7;
      }
      else if (currentMenuItem > 7) {
        currentMenuItem = 0;
      }
      
      //RETURN TO HOME SCREEN IF IDLE 
      if ((millis() - lastButtonPress) > lcdIdleTime){
        lcd.clear();
        opState = HOME; 
        currentMenuItem = 0;
        return;
      }
    }
  }

//OPERATE MOTORS MANUALLY 
  void Motors(){
    lcd.clear();
    uint8_t buttons = 0;

    while(true){
  
    //BUTTON ASSIGNMENT
    buttons = ReadButtons();
      if (buttons & BUTTON_UP){
         currentMenuItem = currentMenuItem-1;
         return;
      }
      if (buttons & BUTTON_DOWN){
         currentMenuItem = currentMenuItem+1;
         return;
      }
      if (buttons & BUTTON_RIGHT){
        if(currentMenuItem == 0){
          opState = NUTRIENT_A;
          currentMenuItem = 0;
          return;
        }
        if(currentMenuItem == 1){
          opState = NUTRIENT_B;
          currentMenuItem = 0;
          return;
        }
        if(currentMenuItem == 2){
            opState = PH_UP;
            currentMenuItem = 0;
            return;
          }
        if(currentMenuItem == 3){
          opState = PH_DOWN;
          currentMenuItem = 0;
          return;
        }
      }
      if (buttons & BUTTON_LEFT){
         opState = MENU;
         return;
      }
      if (buttons & BUTTON_SELECT) {
          opState = HOME;
          currentMenuItem = 0;
          return;  
        }
  
    //SCROLL THROUGH MENU ITEMS
      if(currentMenuItem == 0){
        lcd.setCursor(0, 0);
        lcd.print("> Nutrient A");
        lcd.setCursor(0, 1);
        lcd.print("Nutrient B");
        }
      if(currentMenuItem == 1){
        lcd.setCursor(0, 0);
        lcd.print("> Nutrient B");
        lcd.setCursor(0, 1);
        lcd.print("pH UP");
        }
      if(currentMenuItem == 2){
        lcd.setCursor(0, 0);
        lcd.print("> pH UP");
        lcd.setCursor(0, 1);
        lcd.print("pH DOWN");
        }
      if(currentMenuItem == 3){
        lcd.setCursor(0, 0);
        lcd.print("> pH DOWN");
        lcd.setCursor(0, 1);
        lcd.print("Nutrient A");
        }
      
      //RESTART MENU WHEN OUT OF BOUNDS
      if (currentMenuItem < 0) {
        currentMenuItem = 3;
      }
      else if (currentMenuItem > 3) {
        currentMenuItem = 0;
      }
      
      //RETURN TO HOME SCREEN IF IDLE 
      if ((millis() - lastButtonPress) > lcdIdleTime){
        lcd.clear();
        opState = HOME; 
        currentMenuItem = 0;
        return;
      }
    }
  }

//PRIME NUTRIENT A PUMP
  void NutrientA(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      lcd.setCursor(0, 0);
      lcd.print("Hold RIGHT bttn");
      lcd.setCursor(0, 1);
      lcd.print("to prime pump");
      
      //BUTTON ASSIGNMENT
      buttons = ReadButtons();
      if (buttons & BUTTON_LEFT){
        opState = MOTORS;
        return;
      }    
      if (buttons & BUTTON_RIGHT){
        digitalWrite(Motor.getFertA_pin(), LOW);
        //while (buttons & BUTTON_RIGHT != 0){} 
        delay(5000); 
        digitalWrite(Motor.getFertA_pin(), HIGH);
        return;
      }
      if (buttons & BUTTON_SELECT) {
        opState = HOME;
        currentMenuItem = 0;
        return;  
      }
             
      //RETURN TO HOME SCREEN IF IDLE 
      if ((millis() - lastButtonPress) > lcdIdleTime){
        lcd.clear();
        opState = HOME; 
        currentMenuItem = 0;
        return;
      }
    }
  }

//PRIME NURTIENT B PUMP
  void NutrientB(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      lcd.setCursor(0, 0);
      lcd.print("Hold RIGHT bttn");
      lcd.setCursor(0, 1);
      lcd.print("to prime pump"); 
      
      //BUTTON ASSIGNMENT
      buttons = ReadButtons();
      if (buttons & BUTTON_LEFT){
        opState = MOTORS;
        return;
      }    
      if (buttons & BUTTON_RIGHT){
        digitalWrite(Motor.getFertB_pin(), LOW);
        //while (buttons & BUTTON_RIGHT != 0){  
        //} 
        delay(5000); 
        digitalWrite(Motor.getFertB_pin(), HIGH);
        return;
      }
      if (buttons & BUTTON_SELECT) {
        opState = HOME;
        currentMenuItem = 0;
        return;  
      }
    
      //RETURN TO HOME SCREEN IF IDLE 
      if ((millis() - lastButtonPress) > lcdIdleTime){
        lcd.clear();
        opState = HOME; 
        currentMenuItem = 0;
        return;
      }
    }
  }

//PRIME PH UP PUMP
  void phUP(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      lcd.setCursor(0, 0);
      lcd.print("Hold RIGHT bttn");
      lcd.setCursor(0, 1);
      lcd.print("to prime pump");
      
      //BUTTON ASSIGNMENT
      buttons = ReadButtons();
      if (buttons & BUTTON_LEFT){
        opState = MOTORS;
        return;
      }    
      if (buttons & BUTTON_RIGHT){
        digitalWrite(Motor.getPHup_pin(), LOW); 
        //while (buttons & BUTTON_RIGHT != 0){}  
        delay(5000);
        digitalWrite(Motor.getPHup_pin(), HIGH);
        return;
      }
      if (buttons & BUTTON_SELECT) {
        opState = HOME;
        currentMenuItem = 0;
        return;  
      }

      //RETURN TO HOME SCREEN IF IDLE 
      if ((millis() - lastButtonPress) > lcdIdleTime){
        lcd.clear();
        opState = HOME; 
        currentMenuItem = 0;
        return;
      }
    }
  }

//PRIME PH DOWN PUMP
  void phDOWN(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      lcd.setCursor(0, 0);
      lcd.print("Hold RIGHT bttn");
      lcd.setCursor(0, 1);
      lcd.print("to prime pump");
      
      //BUTTON ASSIGNMENT
      buttons = ReadButtons();
      if (buttons & BUTTON_LEFT){
        opState = MOTORS;
        return;
      }    
      if (buttons & BUTTON_RIGHT){
        digitalWrite(Motor.getPHdown_pin(), LOW);
        //while (buttons & BUTTON_RIGHT != 0){} 
        delay(5000);
        digitalWrite(Motor.getPHdown_pin(), HIGH);
        return;
      }
      if (buttons & BUTTON_SELECT) {
        opState = HOME;
        currentMenuItem = 0;
        return;  
      }
  
      //RETURN TO HOME SCREEN IF IDLE 
      if ((millis() - lastButtonPress) > lcdIdleTime){
        lcd.clear();
        opState = HOME; 
        currentMenuItem = 0;
        return;
      }
    }
  }

//TOGGLE LIGHTS

//TOGGLE AIR PUMP

//TOGGLE WATER PUMP
