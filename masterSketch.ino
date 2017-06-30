
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
#include "waterTemperature.h"
#include "DHT.h"  

// Establish Serial Ports
SoftwareSerial Serial4(11, 10);


// DHT Temperature and Humidity Sensor
#define DHTPIN 4        
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);

// SENSOR VARIABLES				// POINTERS TO SENSOR (only first 4 are used in this version)
float waterTemp;				// RTD
float conductivity;				// EC
float pH;						// PH
float oxygen;					// DO
float humidity;					// HM**
float airTemp;					// AT**
float carbon;					// CO**
float flowRate;					// FR**

// MOTOR PIN LOCATIONS
int pHup_pin = 30;
int pHdown_pin = 31;
int fertA_pin = 32;
int fertB_pin = 33;
int mixPump_pin = 34;
int stonePump_pin = 35;
int water_pin = 36;

int pump_duration = 10000;
int lcdPageNumber = 0;
unsigned long currentMillis;
unsigned long readMillis = 0;
unsigned long printMillis = 0;
unsigned long pumpMillis = 0;
unsigned long phInterval = 0;
unsigned long ecInterval = 0;
unsigned long doInterval = 0;


long readInterval = 10000;
long printInterval = 5000;
long pumpInterval = 5000;

String commandString = "";
String pointerString = "";

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

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
	dht.begin();
	lcd.begin(16, 2);
	lcd.print("Collecting data...");
	uint8_t i = 0;
}

void loop() {

	currentMillis = millis();
	WaterTemperature RTD;


	if (Serial.available() > 0) {
		getPointer();
		getCommand();
		Serial.println("Sending command");
		if (pointerString == "RTD") { RTD.sendCommand(); }
		else if (pointerString == "EC") { sendTOEC(); }
		else if (pointerString == "PH") { sendToPH(); }
		else if (pointerString == "DO") { sendToDO(); }
		pointerString = "";
		commandString = "";
	}

	if (currentMillis - readMillis > readInterval) {
		readMillis = currentMillis;
		readData();
		RTD.readData();
		RTD.printData();
		//thingSpeak();
		monitorSolution();
	}

	if (currentMillis - printMillis > printInterval) {
		printMillis = currentMillis;
		lcdMenu();
	}
}

/*************************************************************************************
*			SENDING COMMANDS TO SENSORS
**************************************************************************************/

void getPointer() {

	boolean pointerStringComplete = false;
	pointerString = Serial.readStringUntil(13);
	if (pointerString.length() > 0) {
		pointerStringComplete = true;
	}
	Serial.print("Sending to: ");
	Serial.println(pointerString);
}

void getCommand() {

	boolean commandStringComplete = false;
	Serial.print("Enter a command: ");

	while (commandStringComplete == false) {
		commandString = Serial.readStringUntil(13);
		if (commandString.length() > 0) {
			commandStringComplete = true;
		}
	}
	Serial.println(commandString);
}
/*
void sendToRTD() {
	String sendStringRTD = "";
	sendStringRTD.reserve(30);

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
*/

void sendTOEC() {
	String sendStringEC = "";
	sendStringEC.reserve(30);

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

void sendToPH() {
	String sendStringPH = "";
	sendStringPH.reserve(30);

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

void sendToDO() {
	String sendStringDO = "";
	sendStringDO.reserve(30);

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

/*************************************************************************************
*				READING DATA FROM SENSORS
**************************************************************************************/
void readData() {

	Serial.println("Reading sensors... ");
	
	getConductivity();
	getPH();
	getOxygen();
	//getCarbon();
	getAirTemp();
	getHumidity();
	//getFlowRate();
}

/*void getWaterTemp() {
	String readStringRTD = "";
	readStringRTD.reserve(30);

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

}*/

void getConductivity() {
	String readStringEC = "";
	readStringEC.reserve(30);

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
	Serial.println(" (micro)S");

}

void getPH() {
	String readStringPH = "";
	readStringPH.reserve(30);

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
}

void getOxygen() {
	String readStringDO = "";
	readStringDO.reserve(30);

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
}

//void getCarbon() {}

void getAirTemp() {
	airTemp = dht.readTemperature(true);
	if (isnan(airTemp)) {
		Serial.println("Failed to read air temperature!");
	}
	else {
		Serial.print("\t\t\t");
		Serial.print("Air Temperature: ");
		Serial.print("\t");
		Serial.print(airTemp);
		Serial.println(" *F");
		Serial.println();
		Serial.println();
	}
}

void getHumidity() {
	humidity = dht.readHumidity();
	if (isnan(humidity)) {
		Serial.println("Failed to read humidity!");
	}
	else {
		Serial.print("\t\t\t");
		Serial.print("Humidity: ");
		Serial.print("\t\t");
		Serial.print(humidity);
		Serial.println(" %");
	}
}

//void getFlowRate() {}

/*************************************************************************************
*		MONITORING AND FIXING THE NUTRIENT SOLUTION
*		You can change the parameters of your variables here
**************************************************************************************/
void monitorSolution() {

	float pH_high = 6.5;
	float pH_low = 5.5;
	float conductivity_high = 20000;
	float conductivity_low = 2000;
	float oxygen_high = 5000;
	float oxygen_low = 2000;
	float wt_high = 80.0;
	float wt_low = 70.0;

	if (pH < pH_low) { raisePH(); }
	if (pH > pH_high) { lowerPH(); }
	if (conductivity < conductivity_low) { raiseConductivity(); }
	//if (conductivity > conductivity_high) { lowerConductivity(); }		
	if (oxygen < oxygen_low) { raiseOxygenLevel(); }
	//if (oxygen > oxygen_high) { lowerOxygenLevel(); }
	//if (waterTemp < wt_low) { raiseWaterTemp(); }
	//if (waterTemp > wt_high) { lowerWaterTemp(); }
}

void raisePH() {
	digitalWrite(pHup_pin, LOW);
	digitalWrite(mixPump_pin, LOW);
	phInterval = millis();
	currentMillis = millis();

	while (currentMillis - phInterval < pumpInterval) {	
		currentMillis = millis();
	}

	digitalWrite(pHup_pin, HIGH);
	digitalWrite(mixPump_pin, HIGH);
}

void lowerPH() {
	digitalWrite(pHdown_pin, LOW);
	digitalWrite(mixPump_pin, LOW);

	while (phInterval < pumpInterval) {
		++phInterval;
	}
	digitalWrite(pHdown_pin, HIGH);
	digitalWrite(mixPump_pin, HIGH);
	phInterval = 0;
}

void raiseConductivity() {
	digitalWrite(fertA_pin, LOW);
	digitalWrite(fertB_pin, LOW);
	digitalWrite(mixPump_pin, LOW);

	while (ecInterval < pumpInterval) {
		++ecInterval;
	}
	digitalWrite(fertA_pin, HIGH);
	digitalWrite(fertB_pin, HIGH);		
	digitalWrite(mixPump_pin, HIGH);
	ecInterval = 0;
	
}

//void lowerConductivity() {}

void raiseOxygenLevel() {
	digitalWrite(stonePump_pin, LOW);
	digitalWrite(mixPump_pin, LOW);

	while (doInterval < pumpInterval) {
		++doInterval;
	}

	digitalWrite(stonePump_pin, HIGH);
	digitalWrite(mixPump_pin, HIGH);
	doInterval = 0;
}

//void lowerOxygenLevel() {}

//void raiseWaterTemp() {}

//void lowerWaterTemp() {}

/*************************************************************************************
*			SENDING DATA TO THE INTERNET
**************************************************************************************/
void thingSpeak() {}
/*************************************************************************************
*			MENU FOR THE LCD DISPLAY
**************************************************************************************/
void lcdMenu() {
	//This function prints all sensor readings to LCD display

	if (lcdPageNumber == 0) {
		printPageNumber0();
		lcdPageNumber = 1;
	}
	else {
		printPageNumber1();
		lcdPageNumber = 0;
	}
}

void printPageNumber0() {
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
}

void printPageNumber1() {
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
}
