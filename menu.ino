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

enum operatingState {TOP, RIGHT, TUNE_P, TUNE_I, TUNE_D, AUTO};
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
//
//enum operatingState { TOP, RIGHT};
//operatingState opState = TOP;

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
   case RIGHT:
      Right();
      break;
   }
}

void Top(){
  {
   //lcd.setBacklight(TEAL);
   lcd.print("TOP");
   uint8_t buttons = 0;
   while(true)
   {
      buttons = ReadButtons();

      if (buttons & BUTTON_RIGHT)
      {
         opState = RIGHT;
         return;
    
      if ((millis() - lastInput) > 3000)  // return to RUN after 3 seconds idle
      {
         opState = TOP;
         return;
      }
      //DoControl();
   }
}}}

void Right(){
  {
   //lcd.setBacklight(TEAL);
   lcd.print("RIGHT-->");
   uint8_t buttons = 0;
   while(true)
   {
      buttons = ReadButtons();

      if (buttons & BUTTON_RIGHT)
      {
         opState = RIGHT;
         return;
      }
    
      if ((millis() - lastInput) > 3000)  // return to RUN after 3 seconds idle
      {
         opState = TOP;
         return;
      }
//      DoControl();
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

