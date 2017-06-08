
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the pH circuit.
//This code was written in the Arduino 1.6.5 IDE
//An Arduino UNO was used to test this code.


#include <SoftwareSerial.h>                           // Include the SoftwareSerial library for pH sensor
#include "DHT.h"                                      // Include the DHT library for DHT22 sensor
            
//#define DHTTYPE DHT11   // DHT 11                   // Uncomment the DHT type you're using
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22   // DHT 22  (AM2302), (AM2321)
#define RX 2                                          // Define RX pin for pH sensor
#define TX 3                                          // Define TX pin for pH sensor
#define DHTPIN 4                                      // Define D/O pin for DHT sensor

DHT dht(DHTPIN, DHTTYPE);                             // Define how the DHT sensor is going to work
SoftwareSerial myserial(RX, TX);                      // Define how the soft serial port is going to work

String inputstring = "";                              // Creates a string to hold incoming data from the PC
String sensorstring = "";                             // Creates a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                // have we received all the data from the PC
boolean sensor_string_complete = false;               // have we received all the data from the Atlas Scientific product
float pH;                                             // Creates a floating point number that is the pH



void setup() {                                        // Set up the hardware
  Serial.begin(9600);                                 // Set baud rate for the hardware serial port_0 to 9600
  myserial.begin(9600);                               // Set baud rate for the software serial port to 9600
  inputstring.reserve(10);                            // Set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           // Set aside some bytes for receiving data from Atlas Scientific product
  Serial.println("Collecting Data...");               // Test
  dht.begin();                                        // Start DHT sensor
}


void serialEvent() {                                  // FOR TYPING COMMANDS IN SERIAL MONITOR
  inputstring = Serial.readStringUntil(13);           // If the hardware serial port_0 receives a char, read the string until we see a <CR>
  input_string_complete = true;                       // Set the flag used to tell if we have received a completed string from the PC
}


void loop() {                                         // LOOP FUNCTION                    
  if (input_string_complete) {                        // FOR SENDING COMMANDS TO ATLAS SENSOR
    myserial.print(inputstring);                      // If a string from the PC has been received in its entirety, send that string to the Atlas Scientific product
    myserial.print('\r');                             // Add a <CR> to the end of the string
    inputstring = "";                                 // Clear the string
    input_string_complete = false;                    // Reset the flag used to tell if we have received a completed string from the PC
  }

  if (myserial.available() > 0) {                     // FOR RECEIVING RESPONSES FROM ATLAS SENSOR
    char inchar = (char)myserial.read();              // If we see that the Atlas Scientific product has sent a character, get the char we just received
    sensorstring += inchar;                           // Add the char to the var called sensorstring
    if (inchar == '\r') {                             // If the incoming character is a <CR>
      sensor_string_complete = true;                  // Set the flag
    }
  }

  if (sensor_string_complete == true) {               // FOR DISPLAYING RESPONSES ON SERIAL MONITOR
    Serial.print("pH: ");                             // If a string from the Atlas Scientific product has been received in its entirety
    Serial.print(sensorstring);                       // Send that string to the PC's serial monitor
    Serial.print("\t");
//    if (isdigit(sensorstring[0])) {                   // If the first character in the string is a digit
//      pH = sensorstring.toFloat();                    // Convert the string to a floating point number so it can be evaluated by the Arduino
//      if (pH >= 7.0) {                                //if the pH is greater than or equal to 7.0
//        Serial.println("high");                       //print "high" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
//      }
//      if (pH <= 6.999) {                              //if the pH is less than or equal to 6.999
//        Serial.println("low");                        //print "low" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
//      }
//    }
    sensorstring = "";                                // Clear the string
    sensor_string_complete = false;                   // Reset the flag used to tell if we have received a completed string from the Atlas Scientific product
    
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
  }  
}
