#ifndef Menu_h
#define Menu_h
       
#include <Adafruit_RGBLCDShield.h>
#include "Sensor.h"
#include "Motor.h"

enum operatingState {HOME, MAINMENU, RIGHT, SENSORS, MOTORS, NUTRIENT_A, NUTRIENT_B, PH_UP, PH_DOWN};
SENSOR Sensor;
MOTOR Motor;
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

class MENU {
  
  private:

//TIMING VARIABLES
    long pumpRunTime = 2000;
    unsigned long currentMillis;
    unsigned long previousMillis = 0;

//LCD MENU VARIABLES
    int currentMenuItem = 0;
    unsigned long lastButtonPress = 0;
    int lcdPageNumber = 0;
    int lcdIdleTime = 10 * 1000;
    long menuScrollTime = 5 * 1000;
     
  public:

    operatingState opState = HOME;
    

  //CONSTRUCTOR
    MENU(){}

/************************************************************************************************
MAIN FUNCTIONS    
************************************************************************************************/

//DISPLAYS INFORMATION ON LCD SCREEN
  void lcdMenu(){
    
    //WAIT FOR BUTTON RELEASE TO CHANGE STATE
    while(ReadButtons() != 0) {}

    //CHANGE STATE
    switch (opState){
    case HOME:
      HomeScreen();
      break;
    case MAINMENU:
      MainMenu();
      break;
    case SENSORS:
      Sensors();
      break;
    case MOTORS:
      Motors();
      break;   
    case NUTRIENT_A:
      NutrientA();
      break;
    case NUTRIENT_B:
      NutrientB();
      break;
    case PH_UP:
      phUP();
      break;
    case PH_DOWN:
      phDOWN();
      break;   
    }
  }


//PRINTS DATA TO LCD DISPLAY IN TWO SEPARATE PAGES
  void printToLCD() {
    if (lcdPageNumber == 0) {
      printPageNumber0();
      lcdPageNumber = 1;
    }
    else {
      printPageNumber1();
      lcdPageNumber = 0;
    }
  }


//PRINT FIRST PAGE TO LCD
  void printPageNumber0() {
    int wtemp = Sensor.getWaterTemp();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WT ");
    lcd.print(wtemp);
    lcd.print("F  ");
    lcd.setCursor(9, 0);
    lcd.print("DO ");
    lcd.print(Sensor.getOxygen());
    lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print("PH ");
    lcd.print(Sensor.getPH());
    lcd.print(" ");
    lcd.setCursor(9, 1);
    lcd.print("EC ");
    lcd.print(Sensor.getConductivity());
    lcd.print("");
  }

//PRINT SECOND PAGE TO LCD
  void printPageNumber1() {
    int atemp = Sensor.getAirTemp();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AT ");
    lcd.print(atemp);
    lcd.print("F  ");
    lcd.setCursor(9, 0);
    lcd.print("HM ");
    lcd.print(Sensor.getHumidity());
    lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print("CB ");
    lcd.print(Sensor.getCarbon());
    lcd.print(" ");
    lcd.setCursor(9, 1);
    lcd.print("PR ");
    lcd.print(Sensor.getFlowRate());
    lcd.print("");
  }

/***************************************************************************************
* MENU SCREENS  (OPSTATES)
***************************************************************************************/

//**HOME SCREEN**//
  void HomeScreen(){
    uint8_t buttons = 0;
    while(true){
      buttons = ReadButtons();

      //GOES TO MAIN MENU IF ANY BUTTON IS PUSHED
      if (buttons !=0 ){
        opState = MAINMENU;
        return;   
      } 

      //OR ELSE DISPLAYS DATA EVERY (menuScrollTime) SECONDS
      else{
        currentMillis = millis();       
        if (currentMillis - previousMillis > menuScrollTime) {
          printToLCD();
          previousMillis = currentMillis;
        }  
      }
    }
  }

  
//**MAIN MENU SCREEN**//
  void MainMenu(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      buttons = ReadButtons();
      menuCycleReset(0,1);
      ifIdleReturnHome();
      
    //BUTTON ASSIGNMENTS
        if (buttons & BUTTON_SELECT){selectButton();}
        if (buttons & BUTTON_UP){upButton();}
        if (buttons & BUTTON_DOWN){downButton();}      
        if (buttons & BUTTON_LEFT){leftButton(HOME);return;}          
        if (buttons & BUTTON_RIGHT){
          if(currentMenuItem == 0){rightButton(SENSORS);return;}
          if(currentMenuItem == 1){rightButton(MOTORS);return;}
        }
      
    //MENU ITEMS
      if(currentMenuItem == 0){
        //lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(">");
        lcd.setCursor(1, 0);
        lcd.print("Sensors");
        lcd.setCursor(1, 1);
        lcd.print("Motors");
      }      
      if(currentMenuItem == 1){
        //lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Sensors");
        lcd.setCursor(0, 1);
        lcd.print(">");
        lcd.print("Motors");
      }
    }   
  }


//**SENSOR SCREEN**//
  void Sensors(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      buttons = ReadButtons();
      menuCycleReset(0, 7);
      ifIdleReturnHome();
      
    //BUTTON ASSIGNMENTS      
      if (buttons & BUTTON_SELECT){selectButton();}      
      if (buttons & BUTTON_UP){upButton();}
      if (buttons & BUTTON_DOWN){downButton();}      
      if (buttons & BUTTON_LEFT){leftButton(MAINMENU);return;} 
      if (buttons & BUTTON_RIGHT){rightButton(HOME);return;}

    //MENU ITEMS
      if(currentMenuItem == 0){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("H2O Temperature");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getWaterTemp());
      }
      if(currentMenuItem == 1){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Conductivity");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getConductivity());
      }
      if(currentMenuItem == 2){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("pH");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getPH());
      }
      if(currentMenuItem == 3){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Dissolved Oxygen");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getOxygen());
      }
      if(currentMenuItem == 4){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Air Temperature ");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getAirTemp());
      }               
      if(currentMenuItem == 5){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Humidity ");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getHumidity());
      }      
      if(currentMenuItem == 6){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Carbon Dioxide ");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getCarbon());
      }
      if(currentMenuItem == 7){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Flow Rate");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getFlowRate());
      } 
    }
  }


//**MOTOR SCREEN**// 
  void Motors(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      buttons = ReadButtons();
      menuCycleReset(0, 3);      
      ifIdleReturnHome(); 
       
    //BUTTON ASSIGNMENTS
      if (buttons & BUTTON_SELECT){selectButton();}
      if (buttons & BUTTON_UP)    {upButton();}
      if (buttons & BUTTON_DOWN)  {downButton();}      
      if (buttons & BUTTON_LEFT)  {leftButton(MAINMENU);} 
      if (buttons & BUTTON_RIGHT)
      {
        if(currentMenuItem == 0)  {rightButton(NUTRIENT_A);}
        if(currentMenuItem == 1)  {rightButton(NUTRIENT_B);}
        if(currentMenuItem == 2)  {rightButton(PH_UP);}
        if(currentMenuItem == 3)  {rightButton(PH_DOWN);}
      }

    //MENU ITEMS
      if(currentMenuItem == 0){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("> Nutrient A");
        lcd.setCursor(0, 1);
        lcd.print("Nutrient B");
      }       
      if(currentMenuItem == 1){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("> Nutrient B");
        lcd.setCursor(0, 1);
        lcd.print("pH UP");
      }        
      if(currentMenuItem == 2){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("> pH UP");
        lcd.setCursor(0, 1);
        lcd.print("pH DOWN");
      }        
      if(currentMenuItem == 3){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("> pH DOWN");
        lcd.setCursor(0, 1);
        lcd.print("Nutrient A");
      }      
    }
  }


//**NUTRIENT A SCREEN**//
  void NutrientA(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      buttons = ReadButtons();
      ifIdleReturnHome();
            
      //BUTTON ASSIGNMENTS 
      if (buttons & BUTTON_SELECT){selectButton();}    
      if (buttons & BUTTON_LEFT){leftButton(MOTORS);} 
      if (buttons & BUTTON_RIGHT){
        digitalWrite(Motor.getFertA_pin(), LOW);
        //while (buttons & BUTTON_RIGHT != 0){} 
        delay(5000); 
        digitalWrite(Motor.getFertA_pin(), HIGH);
        return;
      }

      //MENU ITEMS
      lcd.setCursor(0, 0);
      lcd.print("Hold RIGHT bttn");
      lcd.setCursor(0, 1);
      lcd.print("to prime pump");
    }
  }
  

//**NURTIENT B SCREEN**//
  void NutrientB(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      buttons = ReadButtons();
      ifIdleReturnHome();
            
      //BUTTON ASSIGNMENTS 
      if (buttons & BUTTON_SELECT){selectButton();}    
      if (buttons & BUTTON_LEFT){leftButton(MOTORS);} 
      if (buttons & BUTTON_RIGHT){
        digitalWrite(Motor.getFertB_pin(), LOW);
        //while (buttons & BUTTON_RIGHT != 0){  
        //} 
        delay(5000); 
        digitalWrite(Motor.getFertB_pin(), HIGH);
        return;
      }
      
      //MENU ITEMS
      lcd.setCursor(0, 0);
      lcd.print("Hold RIGHT bttn");
      lcd.setCursor(0, 1);
      lcd.print("to prime pump");      
    }
  }


//**PH UP SCREEN**//
  void phUP(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      buttons = ReadButtons();
      ifIdleReturnHome();
            
      //BUTTON ASSIGNMENTS 
      if (buttons & BUTTON_SELECT){selectButton();}    
      if (buttons & BUTTON_LEFT){leftButton(MOTORS);} 
      if (buttons & BUTTON_RIGHT){
        digitalWrite(Motor.getPHup_pin(), LOW); 
        //while (buttons & BUTTON_RIGHT != 0){}  
        delay(5000);
        digitalWrite(Motor.getPHup_pin(), HIGH);
        return;
      }
      
      //MENU ITEMS
      lcd.setCursor(0, 0);
      lcd.print("Hold RIGHT bttn");
      lcd.setCursor(0, 1);
      lcd.print("to prime pump");       
    }
  }

  
//**PH DOWN PUMP**//
  void phDOWN(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      buttons = ReadButtons();
      ifIdleReturnHome();
            
      //BUTTON ASSIGNMENTS 
      if (buttons & BUTTON_SELECT){selectButton();}    
      if (buttons & BUTTON_LEFT){leftButton(MOTORS);} 
      if (buttons & BUTTON_RIGHT){
        digitalWrite(Motor.getPHdown_pin(), LOW); 
        //while (buttons & BUTTON_RIGHT != 0){}  
        delay(5000);
        digitalWrite(Motor.getPHdown_pin(), HIGH);
        return;
      }
      
      //MENU ITEMS
      lcd.setCursor(0, 0);
      lcd.print("Hold RIGHT bttn");
      lcd.setCursor(0, 1);
      lcd.print("to prime pump");       
    }
  }

  
/********************************************************************************
 * MEMBER FUNCTIONS
 ********************************************************************************/

//READS BUTTON PRESSES 
  uint8_t ReadButtons(){
    uint8_t buttons = lcd.readButtons();
    if (buttons != 0){
      lastButtonPress = millis();
    }
    return buttons;
  }  

  
//RETURNS USER TO HOME SCREEN IF IDLE 
  void ifIdleReturnHome(){
    if ((millis() - lastButtonPress) > lcdIdleTime){
      lcd.clear();
      opState = HOME; 
      currentMenuItem = 0;
      return;
    }
  }


//RESETS MENU AFTER SCROLLING PAST FIRST AND LAST ITEMS
  void menuCycleReset(int firstItem, int lastItem){
    if (currentMenuItem < firstItem) {
      currentMenuItem = lastItem;
    }
    else if (currentMenuItem > lastItem) {
      currentMenuItem = firstItem;
    }
  }


//SETS UP BUTTON FUNCTION
  void upButton (){
    currentMenuItem = currentMenuItem-1;
    return;
  }


//SETS DOWN BUTTON FUNCTION
  void downButton(){
    currentMenuItem = currentMenuItem+1;
    return;
  }


//SETS LEFT BUTTON FUNCTION
  void leftButton(operatingState newOpState){
      opState = newOpState;
      //currentMenuItem = 0;
      //return;
  }


//SETS RIGHT BUTTON FUNCTION
  void rightButton(operatingState newOpState){
      opState = newOpState;
      //currentMenuItem = 0;
      //return;
  }


//SETS SELECT BUTTON FUNCTION  
  void selectButton(){
      opState = HOME;
      //currentMenuItem = 0;
      return;  
  }


//SETS OPERATING STATE
  void setOpState(operatingState newOpState){
    opState = newOpState;
  }


//GETS OPERATING STATE
  operatingState getOpState(operatingState newOpState){
    return opState;
  }

};
#endif
