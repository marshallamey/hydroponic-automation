// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// Depends on the following Arduino libraries:
// - Adafruit Unified Sensor Library: https://github.com/adafruit/Adafruit_Sensor
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define DHTPIN            2         // Pin which is connected to the DHT sensor.

#define RED 0x1                     //color for display
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

// Uncomment the type of sensor in use:
//#define DHTTYPE           DHT11     // DHT 11 
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;
float t = 12;
float h = 12;
void setup() {


//LCD SETUP

  
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  // Print a message to the LCD. We track how long it takes since
  // this library has been optimized a bit and we're proud of it :)
  int time = millis();
//  lcd.print(t);
  time = millis() - time;
  Serial.print("Took "); Serial.print(time); Serial.println(" ms");
  lcd.setBacklight(WHITE);


//LCD END 




//SENSOR SETUP 


  
//  Serial.begin(9600); 
  // Initialize device.
  dht.begin();
  Serial.println("DHTxx Unified Sensor Example");
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.println("------------------------------------");
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.println("------------------------------------");
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
  
  //SENSOR END
}


void loop() {

  
//SENSOR CODE
  
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");

    t = event.temperature;   // read temperature
//    lcd.setCursor(0, 0); 
//    lcd.print("Temp:");  
//    lcd.setCursor(5,0);
//    lcd.print((t*1.8)+32);
//    lcd.setCursor(10,0);
//    lcd.print  ( "F");
     
    
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");



    h = event.relative_humidity;  // read temperature
//     lcd.setCursor(0,1); 
//     lcd.print("Humidity: ");            // write to LCD
//     lcd.setCursor(10,1); 
//     lcd.print(h);
//     lcd.setCursor(12, 1); 
//     lcd.print(" % ");
  }


//LCD CODE!!!!


 // set the cursor to column 0, line 1                        
  // (note: line 1 is the second row, since counting begins with 0):
//  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
//  lcd.print(millis()/1000);

  uint8_t buttons = lcd.readButtons();

  if (buttons) {
    lcd.clear();
    lcd.setCursor(0,0);
    if (buttons & BUTTON_UP) {
  
      lcd.setCursor(0,0 ); 
      lcd.print("Temp:");  
      lcd.setCursor(5,0);
      lcd.print((t*1.8)+32);
      lcd.setCursor(10,0);
      lcd.print  ( "F");
//      lcd.setBacklight(RED);

    }
    if (buttons & BUTTON_DOWN) {
     lcd.setCursor(0,0); 
     lcd.print("Humidity: ");            // write to LCD
     lcd.setCursor(10,0); 
     lcd.print(h);
     lcd.setCursor(12, 0); 
     lcd.print(" % ");
    }
  
  }else{
    
    for(int i = 0; i<20; i++){
      
    lcd.scrollDisplayLeft();
  
    
      lcd.setCursor(1, 0); 
      lcd.print("Temp:");  
      lcd.setCursor(6,0);
      lcd.print((t*1.8)+32);
      lcd.setCursor(11,0);
      lcd.print  ( "F");


      lcd.setCursor(1,1); 
      lcd.print("Humidity: ");            // write to LCD
      lcd.setCursor(10,1); 
      lcd.print(h);
      lcd.setCursor(12, 1); 
      lcd.print(" % ");
      
      delay(500);   
    }


  }
  }


