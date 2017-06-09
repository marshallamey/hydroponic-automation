
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the pH circuit.
//This code was written in the Arduino 1.6.5 IDE
//An Arduino UNO was used to test this code.


#include <SoftwareSerial.h>                           // Include the SoftwareSerial library for pH sensor
#include "DHT.h"                                      // Include the DHT library for DHT22 sensor
            
//#define DHTTYPE DHT11   // DHT 11                   // Uncomment the DHT type you're using
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22   // DHT 22  (AM2302), (AM2321)
#define RX 11                                         // Define RX pin for DO sensor
#define TX 10                                         // Define TX pin for DO sensor
#define DHTPIN 4                                      // Define D/O pin for DHT sensor

DHT dht(DHTPIN, DHTTYPE);                             // Define how the DHT sensor is going to work
SoftwareSerial Serial4 (RX, TX);                      // Define how the soft serial port is going to work


String inputstringRTD = "";                           // Creates a string to hold incoming data from the PC
String sensorstringRTD = "";                          // Creates a string to hold the data from the Atlas Scientific product
boolean input_string_RTD_complete = false;            // have we received all the data from the PC
boolean sensor_string_RTD_complete = false;           // have we received all the data from the Atlas Scientific product
float waterTemp;                                      // Creates a floating point number that is the pH

//String inputstringEC = "";                            // Creates a string to hold incoming data from the PC
//String sensorstringEC = "";                           // Creates a string to hold the data from the Atlas Scientific product
//boolean input_string_EC_complete = false;             // have we received all the data from the PC
//boolean sensor_string_EC_complete = false;            // have we received all the data from the Atlas Scientific product
//float conductivity;                                   // Creates a floating point number that is the pH

String inputstringPH = "";                            // Creates a string to hold incoming data from the PC
String sensorstringPH = "";                           // Creates a string to hold the data from the Atlas Scientific product
boolean input_string_PH_complete = false;             // have we received all the data from the PC
boolean sensor_string_PH_complete = false;            // have we received all the data from the Atlas Scientific product
float pH;                                             // Creates a floating point number that is the pH

String inputstringDO = "";                            // Creates a string to hold incoming data from the PC
String sensorstringDO = "";                           // Creates a string to hold the data from the Atlas Scientific product
boolean input_string_DO_complete = false;             // have we received all the data from the PC
boolean sensor_string_DO_complete = false;            // have we received all the data from the Atlas Scientific product
float oxygen;                                         // Creates a floating point number that is the pH


void setup() {                                        // Set up the hardware
  Serial.begin(9600);                                 // Set baud rate for the hardware serial port_0 to 9600
  Serial1.begin(9600);                                // Set baud rate for the hardware serial port_0 to 9600
  //Serial2.begin(9600);                                // set baud rate for software serial port_3 to 9600
  Serial3.begin(9600);                                // set baud rate for software serial port_3 to 9600
  Serial4.begin(9600);                                // set baud rate for software serial port_3 to 9600
  inputstringRTD.reserve(10);                         // Set aside some bytes for receiving data from the PC
  sensorstringRTD.reserve(30);                        // Set aside some bytes for receiving data from Atlas Scientific product
  //inputstringEC.reserve(10);                          // Set aside some bytes for receiving data from the PC
  //sensorstringEC.reserve(30);                         // Set aside some bytes for receiving data from Atlas Scientific product
  inputstringPH.reserve(10);                          // Set aside some bytes for receiving data from the PC
  sensorstringPH.reserve(30);                         // Set aside some bytes for receiving data from Atlas Scientific product
  inputstringDO.reserve(10);                          // Set aside some bytes for receiving data from the PC
  sensorstringDO.reserve(30);                         // Set aside some bytes for receiving data from Atlas Scientific product
  
  Serial.println("Collecting Data...");               // Display start of data collection
  dht.begin();                                        // Start DHT sensor
}

void serialEvent() {                                  // FOR TYPING COMMANDS IN SERIAL MONITOR
  inputstringRTD = Serial.readStringUntil(13);           // If the hardware serial port_0 receives a char, read the string until we see a <CR>
  input_string_RTD_complete = true;                       // Set the flag used to tell if we have received a completed string from the PC
}
void serialEvent1() {                                 //if the hardware serial port_3 receives a char
  sensorstringRTD = Serial1.readStringUntil(13);         //read the string until we see a <CR>
  sensor_string_RTD_complete = true;                      //set the flag used to tell if we have received a completed string from the PC
}
//void serialEvent2() {                                 //if the hardware serial port_2 receives a char
//  sensorstringEC = Serial2.readStringUntil(13);         //read the string until we see a <CR>
//  sensor_string_EC_complete = true; 

void serialEvent3() {                                 //if the hardware serial port_3 receives a char
  sensorstringPH = Serial3.readStringUntil(13);         //read the string until we see a <CR>
  sensor_string_PH_complete = true;                      //set the flag used to tell if we have received a completed string from the PC
}
void serialEvent4() {                                 //if the hardware serial port_4 receives a char
  sensorstringDO = Serial4.readStringUntil(13);         //read the string until we see a <CR>
  sensor_string_DO_complete = true; 
}
  
void loop() {                                         // LOOP FUNCTION 

  if (input_string_PH_complete == true) {                        // FOR SENDING COMMANDS TO ATLAS SENSOR
    Serial3.print(inputstringPH);                      // If a string from the PC has been received in its entirety, send that string to the Atlas Scientific product
    Serial3.print('\r');                             // Add a <CR> to the end of the string
    inputstringPH = "";                                 // Clear the string
    input_string_PH_complete = false;                    // Reset the flag used to tell if we have received a completed string from the PC
  }

                                                      // WATER TEMPERATURE
  if (sensor_string_RTD_complete == true) {           // If a string from the Atlas Scientific product has been received in its entirety
    if (isdigit(sensorstringRTD[0])) {                // If the first character in the string is a digit
      waterTemp = sensorstringRTD.toFloat();          // Convert the string to a floating point number so it can be evaluated by the Arduino        if (waterTemp >= 25.0) {                      //if the RTD temperature is greater than or equal to 25 C
      waterTemp = waterTemp*1.8+32;                   // Convert from Celsius to Fahrenheit
        Serial.print("Water Temperature: ");
        Serial.print(waterTemp);
        Serial.print("\t");
    }
      sensorstringRTD = "";                           // Clear the string
      sensor_string_RTD_complete = false;             // Reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }   

 if (Serial4.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
    char inchar = (char)Serial4.read();              //get the char we just received
    sensorstringDO += inchar;                           //add the char to the var called sensorstring
    if (inchar == '\r') {                             //if the incoming character is a <CR>
      sensor_string_DO_complete = true;                  //set the flag
    }
  }
                                                       // DISSOLVED OXYGEN
  if (sensor_string_DO_complete == true) {           // If a string from the Atlas Scientific product has been received in its entirety
    if (isdigit(sensorstringDO[0])) {                // If the first character in the string is a digit
      oxygen = sensorstringDO.toFloat();          // Convert the string to a floating point number so it can be evaluated by the Arduino        if (waterTemp >= 25.0) {                      //if the RTD temperature is greater than or equal to 25 C
      
        Serial.print("Dissolved Oxygen: ");
        Serial.print(oxygen);
        Serial.print("\t\t");
    }
      sensorstringDO = "";                           // Clear the string
      sensor_string_DO_complete = false;             // Reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }   

  if (sensor_string_PH_complete == true) {            // PH
    if (isdigit(sensorstringPH[0])) {                 // If the first character in the string is a digit
      pH = sensorstringPH.toFloat();                  // Convert the string to a floating point number so it can be evaluated by the Arduino
      Serial.print("pH: ");                           // If a string from the Atlas Scientific product has been received in its entirety
      Serial.print(pH);                               // Send that string to the PC's serial monitor
      if (pH >= 7.0) {                                //if the pH is greater than or equal to 7.0
        Serial.print(" (high)");                       //print "high" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
        Serial.print("\t\t");     
      }
      if (pH <= 6.999 && pH >= 5.001) {               //if the pH is 5.0 and 7.0
        Serial.print(" (normal)");                     //print "low" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
        Serial.print("\t\t");
      }
      if (pH <= 5.000) {                              //if the pH is less than or equal to 5.0
        Serial.print(" (low)");                        //print "low" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
        Serial.print("\t\t");
      }
    }
    sensorstringPH = "";                              // Clear the string
    sensor_string_PH_complete = false;                // Reset the flag used to tell if we have received a completed string from the Atlas Scientific product

                                                      // AIR TEMPERATURE & HUMIDITY (DHT)
    float h = dht.readHumidity();                     // Read humidity as percentage
    float t = dht.readTemperature();                  // Read temperature as Celsius (the default)
    float f = dht.readTemperature(true);              // Read temperature as Fahrenheit (isFahrenheit = true) 
    if (isnan(h) || isnan(t) || isnan(f)) {           // Check if any reads failed and exit early (to try again).
      Serial.println("Failed to read from DHT sensor!");
      return;
    } 
//  float hif = dht.computeHeatIndex(f, h);             // Compute heat index in Fahrenheit (the default) 
//  float hic = dht.computeHeatIndex(t, h, false);      // Compute heat index in Celsius (isFahreheit = false)

  Serial.print("Humidity: ");                         //Display Humidity and Temperature
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(f);
  Serial.println(" *F");
  Serial.println(sensorstringRTD); 
  }
  

  
    
}
  
  



