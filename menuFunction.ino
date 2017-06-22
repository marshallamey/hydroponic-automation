/*********************

Example code for the Adafruit RGB Character LCD Shield and Library

This code displays text on the shield, and also reads the buttons on the keypad.
When a button is pressed, the backlight changes color.

**********************/

// include the library code:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <DHT.h>
#include <DHT_U.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

unsigned long lastInput = 0; // last button press
int level = 1;

enum operatingState {TOP, RIGHT, SENSORS, UTILITIES};
operatingState opState = TOP;

void setup() {
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  // Print a message to the LCD. We track how long it takes since
  // this library has been optimized a bit and we're proud of it :)
  int time = millis();
  lcd.print("Hello, world!");
  time = millis() - time;
  Serial.print("Took "); Serial.print(time); Serial.println(" ms");
  lcd.setBacklight(WHITE);
}

//uint8_t i=0;


void loop()
{
   // wait for button release before changing state
   while(lcd.readButtons() != 0) {}

   lcd.clear();

   switch (opState)
   {
   case TOP:
      Top();
      break;
   case SENSORS:
      Sensors();
      break;
   case UTILITIES:
      Utilities();
      break;
   }
}

void Top(){
  {
   // lcd.clear();
   uint8_t buttons = 0;
   while(true)
   {
      buttons = ReadButtons();

      if (buttons & BUTTON_RIGHT)
      {
        if(level == 1){
          opState = SENSORS;
          return;
        }

        if(level == 2){
          //level = 1;
          opState = UTILITIES;
          return;
        }
      }

      if (buttons & BUTTON_UP)
      {
         level = level-1;
         return;
      }

      if (buttons & BUTTON_DOWN)
      {
         level = level+1;
         return;
      }

      if(level == 1){
        lcd.setCursor(0, 0);
        lcd.print(">");
        lcd.setCursor(1, 0);
        lcd.print("Sensors");
        lcd.setCursor(1, 1);
        lcd.print("Utilities");
        }

      if(level == 2){
        lcd.setCursor(1, 0);
        lcd.print("Sensors");
        lcd.setCursor(0, 1);
        lcd.print(">");
        lcd.print("Utilities");
        }
        

      
      if ((millis() - lastInput) > 10000 && level > 1)  // return to RUN after 10 seconds idle
      {
        lcd.clear();
        level = 1;
      }
      //DoControl();
   }
}}

void Sensors(){
  {
   //lcd.clear();
   lcd.print(">");
   lcd.print("PH");
   uint8_t buttons = 0;
   while(true)
   {
      buttons = ReadButtons();

//      if (buttons & BUTTON_RIGHT)
//      {
//         opState = RIGHT;
//         return;
//      }

      if (buttons & BUTTON_LEFT)
      {
         opState = TOP;
         return;
      }
    
      if ((millis() - lastInput) > 10000)  // return to RUN after 10 seconds idle
      {
         opState = TOP;
         return;
      }
   }
}
}

void Utilities(){
  {
   //lcd.clear();
   lcd.print(">");
   lcd.print("Pumps");
   uint8_t buttons = 0;
   while(true)
   {
      buttons = ReadButtons();

//      if (buttons & BUTTON_RIGHT)
//      {
//         opState = RIGHT;
//         return;
//      }

      if (buttons & BUTTON_LEFT)
      {
         opState = TOP;
         return;
      }
    
      if ((millis() - lastInput) > 10000)  // return to RUN after 10 seconds idle
      {
         opState = TOP;
         return;
      }
   }
}
}

uint8_t ReadButtons()
{
  uint8_t buttons = lcd.readButtons();
  if (buttons != 0)
  {
    lastInput = millis();
  }
  return buttons;
}
