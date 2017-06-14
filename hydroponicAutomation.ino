
/* 
 *  This is a program to automate and control the parameters of a hydroponic garden.
 *  Code was designed to be run with an Arduino Mega 2560
 *  The project was completed by research students at San Antonio College
*/

#include <SoftwareSerial.h>                           // Include the SoftwareSerial library for DO sensor
#include "DHT.h"                                      // Include the DHT library for DHT22 sensor
#include <Adafruit_RGBLCDShield.h>
            
#define DHTTYPE DHT22   // DHT 22  (AM2302), (AM2321)
#define RX 11
#define TX 10
#define DHTPIN 4                                      // Define D/O pin for DHT sensor

DHT dht(DHTPIN, DHTTYPE);                             // Define how the DHT sensor is going to work
SoftwareSerial Serial4 (RX, TX);                      // Define how the soft serial port is going to work
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

boolean input_string_1_complete = false;              // have we received all the data from the PC
boolean input_string_2_complete = false;              // have we received all the data from the PC

String inputString1 = "";                             // Creates a string to hold incoming data from the PC
String inputString2 = "";                             // Creates a string to hold incoming data from the PC
String sensorstringRTD = "";                          // Creates a string to hold the data from the Atlas Scientific temperature sensor
String sensorstringEC = "";                           // Creates a string to hold the data from the Atlas Scientific conductivity sensor
String sensorstringPH = "";                           // Creates a string to hold the data from the Atlas Scientific pH sensor
String sensorstringDO = "";                           // Creates a string to hold the data from the Atlas Scientific dissolved oxygen sensor

float waterTemp;                                      // Creates a floating point number that is the water temperature
float conductivity;                                   // Creates a floating point number that is the nutrient conductivity
float pH;                                             // Creates a floating point number that is the pH
float oxygen;                                         // Creates a floating point number that is the dissolved oxygen
float humidity;                                       // Creates a floating point number that is the humidity
float airTempC;                                       // Creates a floating point number that is the air temperature (Celsius)
float airTempF;                                       // Creates a floating point number that is the air temperature (Fahrenheit)

int seconds = 5000;                                   // LCD scroll delay in milliseconds (x1000)


void setup() {                                        // *****SETUP FUNCTION***** //
  Serial.begin(9600);                                 // Set baud rate for the hardware serial port_0 to 9600
  Serial1.begin(9600);                                // Set baud rate for the hardware serial port_1 to 9600
  Serial2.begin(9600);                                // set baud rate for software serial port_2 to 9600
  Serial3.begin(9600);                                // set baud rate for software serial port_3 to 9600
  Serial4.begin(9600);                                // set baud rate for software serial port_4 to 9600
  inputString1.reserve(10);                           // Set aside some bytes for receiving data from the PC
  inputString2.reserve(10);                           // Set aside some bytes for receiving data from the PC
  sensorstringRTD.reserve(30);                        // Set aside some bytes for receiving data from Atlas Scientific product
  sensorstringEC.reserve(30);                         // Set aside some bytes for receiving data from Atlas Scientific product
  sensorstringPH.reserve(30);                         // Set aside some bytes for receiving data from Atlas Scientific product
  sensorstringDO.reserve(30);                         // Set aside some bytes for receiving data from Atlas Scientific product

  Serial.println("Collecting Data...");               // Display start of data collection
  dht.begin();                                        // Start DHT sensor
  lcd.begin(16, 2);                                   // Start LCD screen
  lcd.print("Collecting data...");
  uint8_t i=0; 
}

void loop() {                                         // *****LOOP FUNCTION***** //  
  sendData ();      
  readData ();
  printData ();
}

void serialEvent() {                                  // *****READING USER COMMANDS IN SERIAL MONITOR***** //  
  inputString1 = Serial.readStringUntil(13);          // If the hardware serial port_0 receives a char, read the string until we see a <CR>
  input_string_1_complete = true; 
  inputString2 = Serial.readStringUntil(13);          // If the hardware serial port_0 receives a char, read the string until we see a <CR>
  input_string_2_complete = true;
 
  Serial.println(inputString1);
  Serial.println(inputString2);                       
}

void sendData () {                                    // *****SENDING COMMANDS TO ATLAS SENSORS***** //
  
  if (input_string_1_complete == true && input_string_2_complete == true) { 
      
    if (inputString1 == "RTD") {
      Serial1.print(inputString2);                    // If a string from the PC has been received in its entirety, send that string to the Atlas Scientific product
      Serial1.print('\r'); 
    }
    else if (inputString1 == "EC") {
      Serial2.print(inputString2);                    // If a string from the PC has been received in its entirety, send that string to the Atlas Scientific product
      Serial2.print('\r'); 
    }
    else if (inputString1 == "PH") {
      Serial3.print(inputString2);                    // If a string from the PC has been received in its entirety, send that string to the Atlas Scientific product
      Serial3.print('\r');                            // Add a <CR> to the end of the string
    }
    else if (inputString1 == "DO") {
      Serial4.print(inputString2);                    // If a string from the PC has been received in its entirety, send that string to the Atlas Scientific product
      Serial4.print('\r'); 
    }
  inputString1 = "";                                  // Clear the string1
  inputString2 = "";                                  // Clear the string2
  input_string_1_complete = false;                    // Reset the flag used to tell if we have received a completed string from the PC
  input_string_2_complete = false;                    // Reset the flag used to tell if we have received a completed string from the PC
  }
}

void readData() {                                     // *****FUNCTION TO READ DATA FROM SENSORS***** //  
  sensorstringRTD = Serial1.readStringUntil(13);      // Read the string until we see a <CR>
  waterTemp = sensorstringRTD.toFloat();
  waterTemp = waterTemp*1.8+32;                       // Convert from Celsius to Fahrenheit
        
  sensorstringEC = Serial2.readStringUntil(13);       // Read the string until we see a <CR>
  conductivity = sensorstringEC.toFloat();            // Convert the string to a floating point number so it can be evaluated by the Arduino                            
        
  sensorstringPH = Serial3.readStringUntil(13);       // Read the string until we see a <CR>
  pH = sensorstringPH.toFloat();                      // Convert the string to a floating point number so it can be evaluated by the Arduino
      
  if (Serial4.available() > 0) {                      // If we see that the Atlas Scientific DO has sent a character
    char inchar = (char)Serial4.read();               // Get the char we just received
    sensorstringDO += inchar;                         // Add the char to the var called sensorstring
    oxygen = sensorstringDO.toFloat();                // Convert the string to a floating point number so it can be evaluated by the Arduino                            
  } 
                                                      // AIR TEMPERATURE & HUMIDITY (DHT)           
  humidity = dht.readHumidity();                      // Read humidity as percentage
  airTempC = dht.readTemperature();                   // Read temperature as Celsius (the default)
  airTempF = dht.readTemperature(true);               // Read temperature as Fahrenheit (isFahrenheit = true) 

  if (isnan(humidity) || isnan(airTempC) || isnan(airTempF)) {       // Check if any DHT reads failed and exit early (to try again).
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}

void printData () {                                   // *****FUNCTION TO PRINT DATA TO SERIAL MONITOR AND LCD***** //
  Serial.print("Water Temperature: ");                // Display Water Temperature
  Serial.print(waterTemp);
  Serial.print(" *F");
  Serial.print("\t");
  
  Serial.print("Conductivity: ");                     // Display Conductivity
  Serial.print(conductivity);
  Serial.print("\t\t");
  
  Serial.print("pH: ");                               // Display pH
  Serial.print(pH);                               
    if (pH >= 7.0) {                                  // If the pH is greater than or equal to 7.0
      Serial.print(" (high)");                        // Print "high" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
      Serial.print("\t\t");     
    }
    if (pH <= 6.999 && pH >= 5.001) {                 // If the pH is 5.0 and 7.0
      Serial.print(" (normal)");                      // Print "low" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
      Serial.print("\t\t");
    }
    if (pH <= 5.000) {                                // If the pH is less than or equal to 5.0
      Serial.print(" (low)");                         // Print "low" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
      Serial.print("\t\t"); 
    } 

  Serial.print("Dissolved Oxygen: ");                 //Display Dissolved Oxygen
  Serial.print(oxygen);
  Serial.print("\t\t"); 
  
  Serial.print("Humidity: ");                         //Display Humidity
  Serial.print(humidity);
  Serial.print(" %\t");
  
  Serial.print("Temperature: ");                      // Display Air Temperature
  Serial.print(airTempF);
  Serial.println(" *F");
  
  uint8_t buttons = lcd.readButtons();                //  ????????????????? Has a button been pushed?
 
  lcd.clear();                                        // Clear contents of LCD
  lcd.setCursor(0, 0);                                // Set the cursor to column 0, line 0 
  int wtemp = waterTemp;                              // Change floating point numbers to integers
  int atemp = airTempF;
  lcd.print("H20 ");                                  //Print data to LCD screen
  lcd.print(wtemp);
  lcd.print("F ");
  lcd.print("AIR ");
  lcd.print(atemp);
  lcd.print("F");
  lcd.setCursor(0, 1);
  lcd.print("PH ");
  lcd.print(pH);
  lcd.print(" ");
  lcd.print("EC ");
  lcd.print(conductivity);
  lcd.print("");
  
  delay (seconds);                                     // Pause before displaying more information
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HUMIDITY ");
  lcd.print(humidity);
  lcd.print(" ");
  lcd.setCursor(0, 1);
  lcd.print("DO ");
  lcd.print(oxygen);
  lcd.print(" ");
  lcd.print("CO2 ");
  lcd.print("N/A");
  lcd.print("");

  delay (seconds);                                    // Pause before displaying more information

  if (buttons) {                                      // If a button is pushed...
    lcd.clear();                                      // Clear the screen and reset the cursor
    lcd.setCursor(0,0);
    
    if (buttons & BUTTON_UP) {                        // Top button instructions
      lcd.print("WATER TEMP ");
      lcd.print(waterTemp);
      lcd.print(" F");
      lcd.setCursor(0, 1);
      lcd.print("AIR TEMP");
      lcd.print(airTempF);
      lcd.print(" F");
      delay(seconds);
    }
    
    if (buttons & BUTTON_DOWN) {                      // Down button instructions
      lcd.print("DOWN ");
    }
    if (buttons & BUTTON_LEFT) {                      // Left button instructions
      lcd.print("LEFT ");
    }
    if (buttons & BUTTON_RIGHT) {                     // Right button instructions
      lcd.print("RIGHT ");
    }
    if (buttons & BUTTON_SELECT) {                    // Select button instructions
      lcd.print("SELECT ");
    }
  }  
}
