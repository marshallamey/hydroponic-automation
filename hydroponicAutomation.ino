
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the pH circuit.
//This code was written in the Arduino 1.6.5 IDE
//An Arduino UNO was used to test this code.


#include <SoftwareSerial.h>                           // Include the SoftwareSerial library for pH sensor
#include "DHT.h"                                      // Include the DHT library for DHT22 sensor
            
#define DHTTYPE DHT22                                 // DHT22 (AM2302) (AM2321)
#define RX 11                                         // Define RX pin for DO sensor
#define TX 10                                         // Define TX pin for DO sensor
#define DHTPIN 4                                      // Define D/O pin for DHT sensor

                                                      // *****SETTING UP COMMUNICATION WITH SENSORS***** //
DHT dht(DHTPIN, DHTTYPE);                             // Define how the DHT sensor is going to work
SoftwareSerial Serial4 (RX, TX);                      // Define how the soft serial port is going to work

String inputstringRTD = "";                           // Creates a string to hold incoming data from the PC
String sensorstringRTD = "";                          // Creates a string to hold the data from the Atlas Scientific RTD
boolean input_string_RTD_complete = false;            // Have we received all the data from the PC?
boolean sensor_string_RTD_complete = false;           // Have we received all the data from the Atlas Scientific product RTD?
float waterTemp;                                      // Creates a floating point number that is the solution temperature

String inputstringEC = "";                            // Creates a string to hold incoming data from the PC
String sensorstringEC = "";                           // Creates a string to hold the data from the Atlas Scientific EC
boolean input_string_EC_complete = false;             // Have we received all the data from the PC?
boolean sensor_string_EC_complete = false;            // Have we received all the data from the Atlas Scientific EC?
float conductivity;                                   // Creates a floating point number that is the conductivity of solution

String inputstringPH = "";                            // Creates a string to hold incoming data from the PC
String sensorstringPH = "";                           // Creates a string to hold the data from the Atlas Scientific PH
boolean input_string_PH_complete = false;             // Have we received all the data from the PC?
boolean sensor_string_PH_complete = false;            // Have we received all the data from the Atlas Scientific PH?
float pH;                                             // Creates a floating point number that is the pH of solution

String inputstringDO = "";                            // Creates a string to hold incoming data from the PC
String sensorstringDO = "";                           // Creates a string to hold the data from the Atlas Scientific DO
boolean input_string_DO_complete = false;             // Have we received all the data from the PC?
boolean sensor_string_DO_complete = false;            // Have we received all the data from the Atlas Scientific DO?
float oxygen;                                         // Creates a floating point number that is the dissolved oxygen in solution


void setup() {                                        // *****SETUP FUNCTION***** //
  Serial.begin(9600);                                 // Set baud rate for the hardware serial port_0 to 9600
  Serial1.begin(9600);                                // Set baud rate for the hardware serial port_1 to 9600
  Serial2.begin(9600);                                // Set baud rate for the hardware serial port_2 to 9600
  Serial3.begin(9600);                                // Set baud rate for the hardware serial port_3 to 9600
  Serial4.begin(9600);                                // Set baud rate for the software serial port_4 to 9600
  dht.begin();                                        // Start DHT sensor  
            
  inputstringRTD.reserve(10);                         // Set aside some bytes for receiving data from the PC
  sensorstringRTD.reserve(30);                        // Set aside some bytes for receiving data from Atlas Scientific RTD
  inputstringEC.reserve(10);                          // Set aside some bytes for receiving data from the PC
  sensorstringEC.reserve(30);                         // Set aside some bytes for receiving data from Atlas Scientific EC
  inputstringPH.reserve(10);                          // Set aside some bytes for receiving data from the PC
  sensorstringPH.reserve(30);                         // Set aside some bytes for receiving data from Atlas Scientific PH
  inputstringDO.reserve(10);                          // Set aside some bytes for receiving data from the PC
  sensorstringDO.reserve(30);                         // Set aside some bytes for receiving data from Atlas Scientific DO
  
  Serial.println("Collecting Data...");               // Display start of data collection
}

void serialEvent() {                                  // LOOKING FOR COMMANDS FROM PC TO RTD //
  inputstringRTD = Serial.readStringUntil(13);        // If the hardware serial port_0 receives a char, read the string until we see a <CR>
  input_string_RTD_complete = true;                   // Set the flag used to tell if RTD received a completed string from the PC
}
/*
void serialEvent22() {                                // LOOKING FOR COMMANDS FROM PC TO EC //
  inputstringEC = Serial.readStringUntil(13);        // If the hardware serial port_0 receives a char, read the string until we see a <CR>
  input_string_EC_complete = true;                   // Set the flag used to tell if RTD received a completed string from the PC
}
void serialEvent33() {                                // LOOKING FOR COMMANDS FROM PC TO PH //
  inputstringPH = Serial.readStringUntil(13);        // If the hardware serial port_0 receives a char, read the string until we see a <CR>
  input_string_PH_complete = true;                   // Set the flag used to tell if RTD received a completed string from the PC
}
void serialEvent44() {                                // LOOKING FOR COMMANDS FROM PC TO DO //
  inputstringRTD = Serial.readStringUntil(13);        // If the hardware serial port_0 receives a char, read the string until we see a <CR>
  input_string_DO_complete = true;                   // Set the flag used to tell if RTD received a completed string from the PC
}
*/
void serialEvent1() {                                 // LOOK FOR SENSOR READINGS FROM RTD TO PC //
  sensorstringRTD = Serial1.readStringUntil(13);      // If the hardware serial port_1 receives a char, read the string until we see a <CR>
  sensor_string_RTD_complete = true;                  // Set the flag used to tell if PC has received data from RTD 
}            
void serialEvent2() {                                 // LOOK FOR SENSOR READINGS FROM EC TO PC //
  sensorstringEC = Serial2.readStringUntil(13);       // If the hardware serial port_2 receives a char, read the string until we see a <CR>
  sensor_string_EC_complete = true;                   // Set the flag used to tell if PC has received data from EC 
}
void serialEvent3() {                                 // LOOK FOR SENSOR READINGS FROM PH TO PC //
  sensorstringPH = Serial3.readStringUntil(13);       // If the hardware serial port_3 receives a char, read the string until we see a <CR>
  sensor_string_PH_complete = true;                   // Set the flag used to tell if PC has received data from PH
}


void loop() {                                         // *****LOOP FUNCTION***** // 
  if (input_string_RTD_complete == true) {            // FOR SENDING COMMANDS TO ATLAS RTD SENSOR //
    Serial1.print(inputstringRTD);                    // If a string from the PC has been received in its entirety, send that string to the Atlas Scientific RTD
    Serial1.print('\r');                              // Add a <CR> to the end of the string
    inputstringRTD = "";                              // Clear the string
    input_string_RTD_complete = false;                // Reset the flag used to tell if we have received a completed string from the PC
  }
            
  if (input_string_EC_complete == true) {             // FOR SENDING COMMANDS TO ATLAS EC SENSOR //
    Serial2.print(inputstringEC);                     // If a string from the PC has been received in its entirety, send that string to the Atlas Scientific EC
    Serial2.print('\r');                              // Add a <CR> to the end of the string
    inputstringEC = "";                               // Clear the string
    input_string_EC_complete = false;                 // Reset the flag used to tell if we have received a completed string from the PC
  }
  
  if (input_string_PH_complete == true) {             // FOR SENDING COMMANDS TO ATLAS PH SENSOR //
    Serial3.print(inputstringPH);                     // If a string from the PC has been received in its entirety, send that string to the Atlas Scientific PH
    Serial3.print('\r');                              // Add a <CR> to the end of the string
    inputstringPH = "";                               // Clear the string
    input_string_PH_complete = false;                 // Reset the flag used to tell if we have received a completed string from the PC
  }
            
  if (input_string_DO_complete == true) {             // FOR SENDING COMMANDS TO ATLAS DO SENSOR //
    Serial4.print(inputstringDO);                     // If a string from the PC has been received in its entirety, send that string to the Atlas Scientific DO
    Serial4.print('\r');                              // Add a <CR> to the end of the string
    inputstringDO = "";                               // Clear the string
    input_string_DO_complete = false;                 // Reset the flag used to tell if we have received a completed string from the PC
  }

                                                      // LOOK FOR SENSOR READINGS FROM DO TO PC //
 if (Serial4.available() > 0) {                       // If we see that the Atlas Scientific DO has sent a character
    char inchar = (char)Serial4.read();               // Get the char we just received
    sensorstringDO += inchar;                         // Add the char to the var called sensorstringdo
    if (inchar == '\r') {                             // If the incoming character is a <CR>
      sensor_string_DO_complete = true;               // Set the flag
    }
  }        
                                                      // GET WATER //
  if (sensor_string_RTD_complete == true) {           // If a string from the Atlas Scientific RTD has been received in its entirety
    if (isdigit(sensorstringRTD[0])) {                // If the first character in the string is a digit
      waterTemp = sensorstringRTD.toFloat();          // Convert the string to a floating point number so it can be evaluated by the Arduino        if (waterTemp >= 25.0) {                      //if the RTD temperature is greater than or equal to 25 C
      waterTemp = waterTemp*1.8+32;                   // Convert from Celsius to Fahrenheit
        Serial.print("Water Temperature: ");          // Display water temperature
        Serial.print(waterTemp);
        Serial.print("\t");
    }
      sensorstringRTD = "";                           // Clear the string
      sensor_string_RTD_complete = false;             // Reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }   

                                                      // GET DISSOLVED OXYGEN //
  if (sensor_string_DO_complete == true) {            // If a string from the Atlas Scientific product has been received in its entirety
    if (isdigit(sensorstringDO[0])) {                 // If the first character in the string is a digit
      oxygen = sensorstringDO.toFloat();              // Convert the string to a floating point number so it can be evaluated by the Arduino        if (waterTemp >= 25.0) {                      //if the RTD temperature is greater than or equal to 25 C
      
        Serial.print("Dissolved Oxygen: ");
        Serial.print(oxygen);
        Serial.print("\t\t");
    }
      sensorstringDO = "";                            // Clear the string
      sensor_string_DO_complete = false;              // Reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }   
                                                      
                                                      // GET PH //
  if (sensor_string_PH_complete == true) {            // If a string from the Atlas Scientific product has been received in its entirety
    if (isdigit(sensorstringPH[0])) {                 // If the first character in the string is a digit
      pH = sensorstringPH.toFloat();                  // Convert the string to a floating point number so it can be evaluated by the Arduino
      Serial.print("pH: ");                           // If a string from the Atlas Scientific product has been received in its entirety
      Serial.print(pH);                               // Send that string to the PC's serial monitor
      if (pH >= 7.0) {                                // If the pH is greater than or equal to 7.0
        Serial.print(" (high)");                      // Print "high" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
        Serial.print("\t\t");     
      }
      if (pH <= 6.999 && pH >= 5.001) {               // If the pH is 5.0 and 7.0
        Serial.print(" (normal)");                    // Print "normal" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
        Serial.print("\t\t");
      }
      if (pH <= 5.000) {                              // If the pH is less than or equal to 5.0
        Serial.print(" (low)");                       // Print "low" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
        Serial.print("\t\t");
      }
    }
    sensorstringPH = "";                              // Clear the string
    sensor_string_PH_complete = false;                // Reset the flag used to tell if we have received a completed string from the Atlas Scientific product

              
                                                      // GET AIR TEMPERATURE & HUMIDITY (DHT) //
    float h = dht.readHumidity();                     // Read humidity as percentage
    float t = dht.readTemperature();                  // Read temperature as Celsius (the default)
    float f = dht.readTemperature(true);              // Read temperature as Fahrenheit (isFahrenheit = true) 
    if (isnan(h) || isnan(t) || isnan(f)) {           // Check if any reads failed and exit early (to try again).
      Serial.println("Failed to read from DHT sensor!");
      return;
    } 
//  float hif = dht.computeHeatIndex(f, h);             // Compute heat index in Fahrenheit (the default) 
//  float hic = dht.computeHeatIndex(t, h, false);      // Compute heat index in Celsius (isFahreheit = false)

  Serial.print("Humidity: ");                         // Display Humidity and Temperature
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(f);
  Serial.println(" *F");
  Serial.println(sensorstringRTD); 
  }
    
}
  
  



