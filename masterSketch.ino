/*
This is a program to automate and control the parameters of a hydroponic garden.
Code was designed to be run with an Arduino Mega 2560
The project was completed by research students at San Antonio College

*****ATLAS SCIENTIFIC SENSORS WILL REQUIRE A POINTER AND A COMMAND FOR SERIAL COMMUNICATION*****
The pointers designate to which sensor you wish to send a command.
For commands, please refer to Atlas Scientific Datasheet in the GitHub Repository.

4 Atlas Scientific Sensors:					POINTER (Case sensitive)
Water Temperature Sensor					RTD
Conductivity Sensor						EC
pH Sensor							PH
Dissolved Oxygen Sensor						DO

+ Flow Rate Sensor
+ DHT Air Temperature and Humidity Sensor
+ Carbon Dioxide Sensor

DIRECTIONS: Type the pointer using the serial monitor and press enter.  Type your command and press enter.
Make sure to use Carriage Return only
Baud rate is 9600 bps
*/


#include <SoftwareSerial.h>			// Include the SoftwareSerial library for DO sensor
#include "DHT.h"				// Include the DHT library for DHT22 sensor
#include <Adafruit_RGBLCDShield.h>

#define RX 11
#define TX 10
#define DHTPIN 4				// Define D/O pin for DHT sensor
#define DHTTYPE DHT22				// DHT 22  (AM2302), (AM2321)

DHT dht(DHTPIN, DHTTYPE);			// Initialize DHT sensor
SoftwareSerial Serial4(RX, TX);			// Initialize software serial port 4
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();	// Initialize LCD display

float waterTemp;				// Creates a floating point number that is the water temperature (Fahrenheit)
float conductivity;				// Creates a floating point number that is the nutrient conductivity
float pH;					// Creates a floating point number that is the pH
float oxygen;				// Creates a floating point number that is the dissolved oxygen
float humidity;				// Creates a floating point number that is the humidity
float airTemp;				// Creates a floating point number that is the air temperature (Fahrenheit)
float carbon;				// Creates a floating point number that is the carbon dioxide
float flowRate;				// Creates a floating point number that is the rate of pump flow

int seconds = 5000;				// LCD scroll delay in milliseconds (x1000)
unsigned long currentMillis = millis();                     // Timer
long interval = 15000;                                      //Interval between sensor readings
long scrollInterval = 5000;                                 //Interval between screen change on LCD display

void setup() {
	Serial.begin(9600);                                
	Serial1.begin(9600);                               
	Serial2.begin(9600);                              
	Serial3.begin(9600);                                
	Serial4.begin(9600);                                

	Serial.println("Initializing data collection...");  
	dht.begin();                                        
	lcd.begin(16, 2);                                  
	lcd.print("Collecting data...");
	uint8_t i = 0;
}
 
void loop() {

	sendCommand();
	readData();
	printToLCD();
}

void sendCommand() {
	/*
	SENDING COMMANDS TO ATLAS SENSORS FROM SERIAL MONITOR

	*****ATLAS SCIENTIFIC SENSORS WILL REQUIRE A POINTER AND A COMMAND FOR SERIAL COMMUNICATION*****

	POINTERS
	RTD - Water Temperature
	EC - Electroconductivity Sensor
	PH - pH Sensor
	DO - Dissolved Oxygen Sensor

	DIRECTIONS: Type the pointer using the serial monitor and press enter.  Type your command and press enter.
	Make sure to use Carriage Return only!!\
	(FOR COMMANDS PLEASE REFER TO ATLAS SCIENTIFIC DATASHEETS IN THE GITHUB REPOSITORY)
	*/

	// Flags to signal that a string has been assigned to a variable
	boolean pointerStringComplete = false;
	boolean commandStringComplete = false;

	// Variables to hold the strings
	String pointerString = "";
	String commandString = "";
	String sendStringRTD = "";
	String sendStringEC = "";
	String sendStringPH = "";
	String sendStringDO = "";

	// How many bytes of data should be reserved for each string?
	pointerString.reserve(10);
	commandString.reserve(10);
	sendStringRTD.reserve(30);
	sendStringEC.reserve(30);
	sendStringPH.reserve(30);
	sendStringDO.reserve(30);

	//Read pointer and command
	if (Serial.available() > 0) {										
		pointerString = Serial.readStringUntil(13);						

		if (pointerString.length() > 0) {								
			pointerStringComplete = true;								
			Serial.print("Enter your command for ");							
			Serial.println(pointerString);

		}

		while (commandStringComplete == false) {						
			delay(50);													
			commandString = Serial.readStringUntil(13);					
			if (commandString.length() > 0) {							
				commandStringComplete = true;							
			}
		}

		Serial.print("Sending command [");
		Serial.print(commandString);
		Serial.print("] to ");
		Serial.println(pointerString);
	}

	if (pointerStringComplete == true && commandStringComplete == true) {

		// SEND TO WATER TEMPERATURE SENSOR
		if (pointerString == "RTD") {
			Serial1.print(commandString);
			Serial1.print('\r');
			Serial.print("Response from RTD: ");

			sendStringRTD = Serial1.readStringUntil(13);
			Serial.println(sendStringRTD);
			sendStringRTD = "";

			sendStringRTD = Serial1.readStringUntil(13);
			Serial.println(sendStringRTD);
			Serial.println();
			sendStringRTD = "";

			sendStringRTD = Serial1.readStringUntil(13);
			sendStringRTD = "";
		}

		// SEND TO CONDUCTIVITY SENSOR
		else if (pointerString == "EC") {
			Serial2.print(commandString);
			Serial2.print('\r');
			Serial.print("Response from EC: ");

			sendStringEC = Serial2.readStringUntil(13);
			Serial.println(sendStringEC);
			sendStringEC = "";

			sendStringEC = Serial2.readStringUntil(13);
			Serial.println(sendStringEC);
			Serial.println();
			sendStringEC = "";

			sendStringEC = Serial2.readStringUntil(13);
			sendStringEC = "";
		}

		// SEND TO PH SENSOR
		else if (pointerString == "PH") {
			Serial3.print(commandString);
			Serial3.println('\r');
			Serial.print("Response from PH: ");

			sendStringPH = Serial3.readStringUntil(13);
			Serial.println(sendStringPH);
			sendStringPH = "";

			sendStringPH = Serial3.readStringUntil(13);
			Serial.println(sendStringPH);
			Serial.println();
			sendStringPH = "";

			sendStringPH = Serial3.readStringUntil(13);
			sendStringPH = "";
		}

		//SEND TO DISSOLVED OXYGEN SENSOR
		else if (pointerString == "DO") {
			Serial4.print(commandString);
			Serial4.print('\r');
			Serial.print("Response from DO: ");

			sendStringDO = Serial4.readStringUntil(13);
			Serial.println(sendStringDO);
			sendStringDO = "";

			sendStringDO = Serial4.readStringUntil(13);
			Serial.println(sendStringDO);
			Serial.println();
			sendStringDO = "";

			sendStringDO = Serial4.readStringUntil(13);
			sendStringDO = "";
		}

		// Clear the strings
		pointerString = "";
		commandString = "";

		// Reset the flags
		pointerStringComplete = false;
		commandStringComplete = false;
	}
}

void readData() {
	/*
	READING DATA FROM THE SENSORS
	The command to get a reading (R) is sent to the Atlas sensors
	(see datasheet for list of all commands).
	Change the interval between readings in the global variable 'interval' above

	The Atlas sensors are capable of submitting continuous readings
	but they have been filling up the buffer with too much information.  
	There may be a more efficient way in the future.  But this works great.
	*/

	// Variables to hold the strings

	String readStringRTD = "";
	String readStringEC = "";
	String readStringPH = "";
	String readStringDO = "";

	// How many bytes of data should be reserved for each string?
	readStringRTD.reserve(30);
	readStringEC.reserve(30);
	readStringPH.reserve(30);
	readStringDO.reserve(30);

	//Set the timer
	unsigned long readMillis = 0;
	if (currentMillis - readMillis > interval) {
		readMillis = currentMillis;
		Serial.println("Reading sensors... ");

		
		// Read flow rate meter (FR)

		// Read CO2 sensor (CO)

		// Read air temperature and humidity (DHT)           
		humidity = dht.readHumidity();									    // Read humidity as percentage
		airTemp = dht.readTemperature(true);								// Read temperature as Fahrenheit (isFahrenheit = true) 

		if (isnan(humidity) || isnan(airTemp)) {							// Check if any DHT reads failed and exit early (to try again).
			Serial.println("Failed to read from DHT sensor!");
			return;
		}

		// Read water temperature sensor (RTD)
		Serial1.print('R');
		Serial1.print('\r');

		readStringRTD = Serial1.readStringUntil(13);
		waterTemp = readStringRTD.toFloat();
		waterTemp = waterTemp*1.8 + 32;
		readStringRTD = "";

		readStringRTD = Serial1.readStringUntil(13);
		Serial.print("Reading RTD: ");
		Serial.print(readStringRTD);
		Serial.print("\t");
		readStringRTD = "";

		Serial.print("Water Temperature: ");
		Serial.print("\t");
		Serial.print(waterTemp);
		Serial.println(" *F");

		// Read conductivity sensor (EC)
		Serial2.print('R');
		Serial2.print('\r');

		readStringEC = Serial2.readStringUntil(13);
		conductivity = readStringEC.toFloat();
		readStringEC = "";

		readStringEC = Serial2.readStringUntil(13);
		Serial.print("Reading EC:  ");
		Serial.print(readStringEC);
		Serial.print("\t");
		readStringEC = "";

		Serial.print("Conductivity: ");
		Serial.print("\t\t");
		Serial.print(conductivity);
		Serial.println(" µS");


		// Read pH sensor (PH)
		Serial3.print('R');
		Serial3.print('\r');

		readStringPH = Serial3.readStringUntil(13);
		pH = readStringPH.toFloat();
		readStringPH = "";

		readStringPH = Serial3.readStringUntil(13);
		Serial.print("Reading PH:  ");
		Serial.print(readStringPH);
		Serial.print("\t");
		readStringPH = "";

		Serial.print("pH: ");
		Serial.print("\t\t\t");
		Serial.print(pH);
		if (pH >= 7.0) {
			Serial.println(" (high)");
		}
		else if (pH <= 5.000) {
			Serial.println(" (low)");
		}

		// Read disolved oxygen sensor (DO)
		Serial4.print('R');
		Serial4.print('\r');

		readStringDO = Serial4.readStringUntil(13);
		oxygen = readStringDO.toFloat();
		readStringDO = "";

		readStringDO = Serial4.readStringUntil(13);
		Serial.print("Reading DO:  ");
		Serial.print(readStringDO);
		Serial.print("\t");
		readStringDO = "";

		Serial.print("Dissolved Oxygen: ");
		Serial.print("\t");
		Serial.print(oxygen);
		Serial.println(" mg/L");

		Serial.print("\t\t\t");
		Serial.print("Humidity: ");
		Serial.print("\t\t");
		Serial.print(humidity);
		Serial.println(" %");

		Serial.print("\t\t\t");
		Serial.print("Air Temperature: ");
		Serial.print("\t");
		Serial.print(airTemp);
		Serial.println(" *F");
		Serial.println();
		Serial.println();
	}
}

void printToLCD() {
	//This function prints all sensor readings to LCD display

	long printMillis = 0;
	int scroll = 0;

	if (currentMillis - printMillis > scrollInterval) {
		// save the last time you blinked the LED 
		printMillis = currentMillis;

		if (scroll == 0) {
			//Print to LCD display
			lcd.clear();
			lcd.setCursor(0, 0);
			int wtemp = waterTemp;
			int atemp = airTemp;
			lcd.print("H20 ");
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
			scroll = 1;
		}
		else if (scroll == 1) {
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
			scroll = 0;
		}
	}
}
