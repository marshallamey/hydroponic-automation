/*********************************************************************************************************
This is a program to automate and control 8 variables of a hydroponic garden.
It was designed to be run on an Arduino Mega 2560

*****ATLAS SCIENTIFIC SENSORS WILL REQUIRE AN ADDRESS AND A COMMAND FOR SERIAL COMMUNICATION*****
The address designates which sensor receives a command.
For a list of addresses, please refer to the README file
For commands, please refer to Atlas Scientific Datasheet in the GitHub Repository.
DIRECTIONS: Type the address into the serial monitor and press enter.  Type your command and press enter.
Make sure to use Carriage Return only
Baud rate is 9600 bps
*********************************************************************************************************/

//LIBRARIES
  #include <SoftwareSerial.h>  
  #include <SPI.h>       
  #include <Adafruit_RGBLCDShield.h>
  #include <Wire.h>
  #include "RTClib.h"
  #include "DHT.h"  
  #include "Sensor.h"
  #include "Motor.h"

//TIMING VARIABLES
  unsigned long theCurrentTime;         // In millis();
  unsigned long theLastReadTime = 0;    // For timer in loop();
  unsigned long theLastPrintTime = 0;   // For timer in loop();
  long readDataInterval = 30000;        //Read data every 30 seconds
  long lcdPrintInterval = 5000;         //Change LCD display every 5 seconds
  int lcdPageNumber = 0;

//INITIALIZERS
  SENSOR Sensor;
  MOTOR Motor;
  Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
  RTC_PCF8523 rtc;


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
  Serial6.begin(9600);
  dht.begin();
  Motor.begin();

  //CHECK TO MAKE SURE CLOCK IS INITIALIZED
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println("Initializing data collection...");
  Serial.println();
  lcd.clear();
  lcd.print("Collecting data...");
  readData();
}


void loop() {
  theCurrentTime = millis();

  if (Serial.available() > 0) { 
    sendSensorCommand(); 
  } 
  
  if (theCurrentTime - theLastReadTime > readDataInterval) {
    theLastReadTime = theCurrentTime; 
    readData();
    printToSerial();
    printToInternet();
    monitorSystem();
  }  

  if (theCurrentTime - theLastPrintTime > lcdPrintInterval) {  
    theLastPrintTime = theCurrentTime; 
    printToLCD();
  }  
}


/*****************************************************************************************
                                   MAIN FUNCTIONS
*****************************************************************************************/


//SEND A COMMAND TO AN ATLAS SCIENTIFIC SENSOR
  void sendSensorCommand() {
    String sensorAddress = Serial.readStringUntil(13);
    Serial.print("Connecting to: ");
    Serial.println(sensorAddress);
    Sensor.getCommand();
    Serial.println("Sending command");
    Serial.println();
    
     if      (sensorAddress == "WT" || sensorAddress == "wt") { Sensor.WTsendCommand(); }
     else if (sensorAddress == "EC" || sensorAddress == "ec") { Sensor.ECsendCommand(); }
     else if (sensorAddress == "PH" || sensorAddress == "ph") { Sensor.PHsendCommand(); }
     else if (sensorAddress == "DO" || sensorAddress == "do") { Sensor.DOsendCommand(); }    
     else    { Serial.println("Nothing was sent!  Try again."); }      
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
    Serial.println("Reading Complete.");
    Serial.println();
  }


//PRINT DATA TO SERIAL MONITOR
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
    Serial.println();
  }


//PRINT DATA TO INTERNET
  void printToInternet() {
    Serial6.listen();
    Serial.print("Sending data to Photon at ");
    printTime();
    Serial.println();
    
    String Readings = "field1=" + String(Sensor.getWaterTemp()) + " &field2=" + String(Sensor.getConductivity()) 
    + " &field3=" + String(Sensor.getPH()) + " &field4=" + String(Sensor.getOxygen())
    + " &field5=" + String(Sensor.getAirTemp()) + " &field6=" + String(Sensor.getHumidity())
    + " &field7=" + String(Sensor.getCarbon()) + " &field8=" + String(Sensor.getPar());
    
    Serial6.print(Readings);
  }

  
//MONITOR PARAMETERS OF SOLUTION
  void monitorSystem() {
    
    //**VARIABLE PARAMETERS CAN BE CHANGED HERE**//
    float WT_high = 80.0;
    float WT_low = 70.0;
    float EC_high = 2200;
    float EC_ideal = 2000;
    float EC_low = 1800;
    float PH_high = 6.3;
    float PH_ideal = 6.0;
    float PH_low = 5.7;
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
    int lightsON = 6;                     //2300 hours (11:00pm)
    int lightsOFF = 21;                   //1500 hours (3:00pm) 
    int waterPumpON = 0;                  //Start water pump on the hour
    int waterPumpOFF = 30;                //Stop water pump on the half hour
     
    Serial.println("Monitoring system...");
    DateTime now = rtc.now();
    printTime();
    
    //LIGHT CYCLE MANAGEMENT
    if (now.hour() >= lightsON && now.hour() < lightsOFF) {  
      digitalWrite(Motor.getLights_pin(), LOW);
      Serial.println("Lights are ON");
      }   
    else { 
      digitalWrite(Motor.getLights_pin(), HIGH);
      Serial.println("Lights are OFF");
    } 

    //EBB AND FLOW CYCLE MANAGEMENT  
    // Four times per day...
    if (now.hour() == 0 || now.hour() == 6 || now.hour() == 12 || now.hour() == 18) {
      
      //For 30 minutes each...  Turn water pump on
      if (now.minute() < waterPumpOFF) { 
        Serial.println("Water pump is ON");
        digitalWrite(Motor.getWaterPump_pin(), LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("   FLOW CYCLE   ");
        lcd.setCursor(0, 1);
        lcd.print(" Water Pump  ON ");
      } 
    }  
    
    //Otherwise, it stays off
    else { 
      Serial.println("Water pump is OFF");
      digitalWrite(Motor.getWaterPump_pin(), HIGH); 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("   EBB  CYCLE   ");
      lcd.setCursor(0, 1);
      lcd.print(" Water Pump OFF ");
    }
    
//PARAMETER CONTROL STATEMENTS
    
    //RAISE PH
    if (Sensor.getPH() < PH_low) { 
      Serial.print("LOW pH level: ");
      Serial.println(Sensor.getPH());
      digitalWrite(Motor.getWaterPump_pin(), HIGH);
      while (Sensor.getPH() < PH_ideal)
      { 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("   FIXING  PH   ");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getPH());
        Motor.raisePH(); 
        digitalWrite(Motor.getStonePump_pin(), LOW); 
        Serial.println("Mixing solution...");
        delay(60000); //Wait one minute for solution to mix
        Serial.println("Mixing complete.");
        readData();
        Serial.print("New pH level: ");
        Serial.println(Sensor.getPH());
        printToInternet();       
      }
      digitalWrite(Motor.getStonePump_pin(), HIGH);
      Serial.println("pH levels fixed.");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("   PH FIXED!   ");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getPH());
    }

//    //LOWER PH
    if (Sensor.getPH() > PH_high) { 
      Serial.print("HIGH pH level: ");
      Serial.println(Sensor.getPH());
      digitalWrite(Motor.getWaterPump_pin(), HIGH);
      while (Sensor.getPH() > PH_ideal)
      { 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("   FIXING  PH   ");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getPH());
        Motor.lowerPH(); 
        digitalWrite(Motor.getStonePump_pin(), LOW);
        Serial.println("Mixing solution..."); 
        delay(60000); //Wait one minute for solution to mix
        Serial.println("Mixing complete.");
        readData();
        Serial.print("New pH level: ");
        Serial.println(Sensor.getPH());
        printToInternet();
      }
      digitalWrite(Motor.getStonePump_pin(), HIGH);
      Serial.println("pH levels fixed.");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("   PH FIXED!   ");
      lcd.setCursor(0, 1);
      lcd.print(Sensor.getPH());
    }  

//    //RAISE CONDUCTIVITY
//    if (Sensor.getConductivity() < EC_low) 
//    { 
//      Serial.print("LOW conductivity level: ");
//      Serial.println(Sensor.getConductivity());
//      digitalWrite(Motor.getWaterPump_pin(), HIGH);
//      while (Sensor.getConductivity() < EC_ideal)
//      { 
//        lcd.clear();
//        lcd.setCursor(0, 0);
//        lcd.print("FIXING  SOLUTION");
//        lcd.setCursor(0, 1);
//        lcd.print(Sensor.getConductivity());
//        Motor.raiseConductivity();
//        digitalWrite(Motor.getStonePump_pin(), LOW); 
//        Serial.println("Mixing solution..."); 
//        delay(60000); //Wait one minute for solution to mix
//        Serial.println("Mixing complete.");
//        readData();
//        Serial.print("New conductity level: ");
//        Serial.println(Sensor.getConductivity());
//        printToInternet();
//      }
//      digitalWrite(Motor.getStonePump_pin(), HIGH);
//      Serial.println("Conductivity levels fixed.");
//      lcd.clear();
//      lcd.setCursor(0, 0);
//      lcd.print("SOLUTION  FIXED!");
//      lcd.setCursor(0, 1);
//      lcd.print(Sensor.getConductivity());
//    }

    //LOWER CONDUCTIVITY
//    if (Sensor.getConductivity() > EC_high) 
//    { 
//      Serial.print("HIGH conductivity level: ");
//      Serial.println(Sensor.getConductivity());
//      digitalWrite(Motor.getWaterPump_pin(), HIGH);
//      while (Sensor.getConductivity() > EC_ideal)
//      { 
//        Motor.lowerConductivity(); 
//        digitalWrite(Motor.getStonePump_pin(), LOW); 
//        Serial.println("Mixing solution..."); 
//        delay(60000); //Wait one minute for solution to mix
//        Serial.println("Mixing complete. Getting conductivity... ");
//        readData();
//        Serial.print("New conductity level: ");
//        Serial.println(Sensor.getConductivity());
//        printToInternet();
//      }
//      digitalWrite(Motor.getStonePump_pin(), HIGH);
//      Serial.println("Conductivity levels fixed.");
//    }
//      

    //if (Sensor.getWaterTemp() < WT_low) { Motor.raiseWaterTemp(); }
    //if (Sensor.getWaterTemp() > WT_high) { Motor.lowerWaterTemp(); }
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

    Serial.println("System is stable");
    Serial.println();
  }


//PRINT TIME TO SERIAL MONITOR
  void printTime(){
    DateTime now = rtc.now(); 
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(" ");
    
    if (now.hour() < 10) {
      Serial.print("0");
      Serial.print(now.hour(), DEC); }
    else { Serial.print(now.hour(), DEC); }
    
    Serial.print(':');
    
    if (now.minute() < 10) {
      Serial.print("0");
      Serial.print(now.minute(), DEC); }
    else { Serial.print(now.minute(), DEC); }
    
    Serial.print(':');
    
    if (now.second() < 10) {
      Serial.print("0");
      Serial.print(now.second(), DEC); }
    else { Serial.print(now.second(), DEC); }
    Serial.println();
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
