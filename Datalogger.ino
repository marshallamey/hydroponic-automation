#include <SPI.h>
#include <SD.h>
#include "DHT.h"
#include "RTClib.h"


// DHT Temperature and Humidity Sensor
#define DHTPIN 4        
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);
RTC_PCF8523 rtc;

float humidity;
float airTemp;

//needs to be 10 for our SD card shield
const int chipSelect = 10;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop() {
  // make a string for assembling the data to log:
  DateTime now = rtc.now();
  String dataString = "";
  dataString = String(readHM()) + ", " + String(readAT()) + ", " + (int)now.month();

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("test.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  delay(5000);
}

//READ DATA FROM HUMIDITY SENSOR (HM)
    float readHM() {
      humidity = dht.readHumidity();
      if (isnan(humidity)) {
        Serial.println("Failed to read humidity!");
      }
      return humidity;
    }

//READ DATA FROM AIR TEMPERATURE SENSOR (AT)
    float readAT() {
      airTemp = dht.readTemperature(true);
      if (isnan(airTemp)) {
        Serial.println("Failed to read air temperature!");
      }
      return airTemp;
    }

  void printTime(){
    DateTime now = rtc.now(); 
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(" ");
    
    if (now.hour() < 10){
      Serial.print("0");
      Serial.print(now.hour(), DEC);
    }
    else{
      Serial.print(now.hour(), DEC);
    }
    Serial.print(':');
    if (now.minute() < 10){
      Serial.print("0");
      Serial.print(now.minute(), DEC);
    }
    else{
      Serial.print(now.minute(), DEC);
    }
    Serial.print(':');
    if (now.second() < 10){
      Serial.print("0");
      Serial.print(now.second(), DEC);
    }
    else{
      Serial.print(now.second(), DEC);
    }
    Serial.println();
  }





