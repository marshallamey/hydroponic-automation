#ifndef Menu_h
#define Menu_h

        
#include <Adafruit_RGBLCDShield.h>
#include "Sensor.h"
#include "Motor.h"


class MENU {
  
  private:

//TIMING VARIABLES
    long pumpRunTime = 2000;
    unsigned long currentMillis;
    unsigned long previousMillis;

    //LCD MENU VARIABLES
int currentMenuItem = 0;
unsigned long lastButtonPress = 0;
int lcdPageNumber = 0;
int lcdIdleTime = 10 * 1000;
long menuScrollTime = 5 * 1000;
enum operatingState {HOME, MAINMENU, RIGHT, SENSORS, MOTORS, NUTRIENT_A, NUTRIENT_B, PH_UP, PH_DOWN}; 


  public:
  SENSOR Sensor;
  MOTOR Motor;
  operatingState opState = HOME;
  Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

  //CONSTRUCTOR
    MENU(){}


/************************************************************************************************
MEMBER FUNCTIONS    
************************************************************************************************/

//LCD DISPLAY DATA
  void lcdMenu(){
    // wait for button release before changing state
    while(ReadButtons() != 0) {}
    
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

//WATCH FOR BUTTON PUSH 
  uint8_t ReadButtons(){
    uint8_t buttons = lcd.readButtons();
    if (buttons != 0){
      lastButtonPress = millis();
    }
    return buttons;
  }  
  
//HOME SCREEN
  void HomeScreen(){
    uint8_t buttons = 0;
    //while(true){
      buttons = ReadButtons();
      if (buttons !=0 ){
        opState = MAINMENU;
        return;   
      }
      else{
        currentMillis = millis();       
        if (currentMillis - previousMillis > menuScrollTime) {
        printToLCD();
        previousMillis = currentMillis;
        }  
   
      //}
    }
  }

//PRINT DATA TO LCD DISPLAY
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
    lcd.clear();
    lcd.setCursor(0, 0);
    int wtemp = Sensor.getWaterTemp();
    int atemp = Sensor.getAirTemp();
    lcd.print("H20 ");
    lcd.print(wtemp);
    lcd.print("F ");
    lcd.print("AIR ");
    lcd.print(atemp);
    lcd.print("F");
    lcd.setCursor(0, 1);
    lcd.print("PH ");
    lcd.print(Sensor.getPH());
    lcd.print(" ");
    lcd.print("EC ");
    lcd.print(Sensor.getConductivity());
    lcd.print("");
  }

//PRINT SECOND PAGE TO LCD
  void printPageNumber1() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HUMIDITY ");
    lcd.print(Sensor.getHumidity());
    lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print("DO ");
    lcd.print(Sensor.getOxygen());
    lcd.print(" ");
    lcd.print("CO2 ");
    lcd.print(Sensor.getCarbon());
    lcd.print("");
  }
  
//MAIN MENU SCREEN
  void MainMenu(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      
      //BUTTON ASSIGNMENT
      buttons = ReadButtons();
        upButton();
        downButton();
        
        if (buttons & BUTTON_LEFT){
           opState = HOME;
           return;
        }    
        if (buttons & BUTTON_RIGHT){
          if(currentMenuItem == 0){
            opState = SENSORS;
            currentMenuItem = 0;
            return;
          }
          if(currentMenuItem == 1){
            opState = MOTORS;
            currentMenuItem = 0;
            return;
          }
        }
        selectButton();
      
      //SCROLL THROUGH MENU ITEMS
      if(currentMenuItem == 0){
        lcd.setCursor(0, 0);
        lcd.print(">");
        lcd.setCursor(1, 0);
        lcd.print("Sensors");
        lcd.setCursor(1, 1);
        lcd.print("Motors");
        }
      if(currentMenuItem == 1){
        lcd.setCursor(1, 0);
        lcd.print("Sensors");
        lcd.setCursor(0, 1);
        lcd.print(">");
        lcd.print("Motors");
        }

      cycleThruMenu(0,1);
      ifIdleReturnHome();
    }   
  }

//VIEW INDIVIDUAL SENSOR READINGS
  void Sensors(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
  
    //BUTTON ASSIGNMENT
    buttons = ReadButtons();
      upButton();
      downButton();
      
      if (buttons & BUTTON_RIGHT){
         opState = HOME;
         currentMenuItem = 0;
         return;
      }
      if (buttons & BUTTON_LEFT){
         opState = MAINMENU;
         return;
      }
      selectButton();
  
    //SCROLL THROUGH MENU ITEMS
      if(currentMenuItem == 0){
        lcd.setCursor(0, 0);
        lcd.print("H2O Temperature");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getWaterTemp());
        }
      if(currentMenuItem == 1){
        lcd.setCursor(0, 0);
        lcd.print("Conductivity");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getConductivity());
        }
      if(currentMenuItem == 2){
        lcd.setCursor(0, 0);
        lcd.print("pH");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getPH());
        }
      if(currentMenuItem == 3){
        lcd.setCursor(0, 0);
        lcd.print("Dissolved Oxygen");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getOxygen());
        }
      if(currentMenuItem == 4){
        lcd.setCursor(0, 0);
        lcd.print("Air Temperature ");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getAirTemp());
        }               
      if(currentMenuItem == 5){
        lcd.setCursor(0, 0);
        lcd.print("Humidity ");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getHumidity());
        }      
      if(currentMenuItem == 6){
        lcd.setCursor(0, 0);
        lcd.print("Carbon Dioxide ");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getCarbon());
        }
      if(currentMenuItem == 7){
        lcd.setCursor(0, 0);
        lcd.print("Flow Rate");
        lcd.setCursor(0, 1);
        lcd.print(Sensor.getFlowRate());
        }
  
      cycleThruMenu(0, 7);
      ifIdleReturnHome();
    }
  }


//OPERATE MOTORS MANUALLY 
  void Motors(){
    lcd.clear();
    uint8_t buttons = 0;

    while(true){
  
      //BUTTON ASSIGNMENT
      buttons = ReadButtons();
      upButton();
      downButton();
      
      if (buttons & BUTTON_RIGHT){
        if(currentMenuItem == 0){
          opState = NUTRIENT_A;
          currentMenuItem = 0;
          return;
        }
        if(currentMenuItem == 1){
          opState = NUTRIENT_B;
          currentMenuItem = 0;
          return;
        }
        if(currentMenuItem == 2){
            opState = PH_UP;
            currentMenuItem = 0;
            return;
          }
        if(currentMenuItem == 3){
          opState = PH_DOWN;
          currentMenuItem = 0;
          return;
        }
      }
      if (buttons & BUTTON_LEFT){
         opState = MAINMENU;
         return;
      }
      selectButton();
  
    //SCROLL THROUGH MENU ITEMS
      if(currentMenuItem == 0){
        lcd.setCursor(0, 0);
        lcd.print("> Nutrient A");
        lcd.setCursor(0, 1);
        lcd.print("Nutrient B");
        }
      if(currentMenuItem == 1){
        lcd.setCursor(0, 0);
        lcd.print("> Nutrient B");
        lcd.setCursor(0, 1);
        lcd.print("pH UP");
        }
      if(currentMenuItem == 2){
        lcd.setCursor(0, 0);
        lcd.print("> pH UP");
        lcd.setCursor(0, 1);
        lcd.print("pH DOWN");
        }
      if(currentMenuItem == 3){
        lcd.setCursor(0, 0);
        lcd.print("> pH DOWN");
        lcd.setCursor(0, 1);
        lcd.print("Nutrient A");
        }
      
      cycleThruMenu(0, 3);      
      ifIdleReturnHome();
    }
  }

//PRIME NUTRIENT A PUMP
  void NutrientA(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      lcd.setCursor(0, 0);
      lcd.print("Hold RIGHT bttn");
      lcd.setCursor(0, 1);
      lcd.print("to prime pump");
      
      //BUTTON ASSIGNMENT
      buttons = ReadButtons();
      if (buttons & BUTTON_LEFT){
        opState = MOTORS;
        return;
      }    
      if (buttons & BUTTON_RIGHT){
        digitalWrite(Motor.getFertA_pin(), LOW);
        //while (buttons & BUTTON_RIGHT != 0){} 
        delay(5000); 
        digitalWrite(Motor.getFertA_pin(), HIGH);
        return;
      }
      selectButton();    
      ifIdleReturnHome();
    }
  }

//PRIME NURTIENT B PUMP
  void NutrientB(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      lcd.setCursor(0, 0);
      lcd.print("Hold RIGHT bttn");
      lcd.setCursor(0, 1);
      lcd.print("to prime pump"); 
      
      //BUTTON ASSIGNMENT
      buttons = ReadButtons();
      if (buttons & BUTTON_LEFT){
        opState = MOTORS;
        return;
      }    
      if (buttons & BUTTON_RIGHT){
        digitalWrite(Motor.getFertB_pin(), LOW);
        //while (buttons & BUTTON_RIGHT != 0){  
        //} 
        delay(5000); 
        digitalWrite(Motor.getFertB_pin(), HIGH);
        return;
      }
      selectButton();
      ifIdleReturnHome();
    }
  }

//PRIME PH UP PUMP
  void phUP(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      lcd.setCursor(0, 0);
      lcd.print("Hold RIGHT bttn");
      lcd.setCursor(0, 1);
      lcd.print("to prime pump");
      
      //BUTTON ASSIGNMENT
      buttons = ReadButtons();
      if (buttons & BUTTON_LEFT){
        opState = MOTORS;
        return;
      }    
      if (buttons & BUTTON_RIGHT){
        digitalWrite(Motor.getPHup_pin(), LOW); 
        //while (buttons & BUTTON_RIGHT != 0){}  
        delay(5000);
        digitalWrite(Motor.getPHup_pin(), HIGH);
        return;
      }
      
      selectButton();
      ifIdleReturnHome();
      
    }
  }
//PRIME PH DOWN PUMP
  void phDOWN(){
    lcd.clear();
    uint8_t buttons = 0;
    while(true){
      lcd.setCursor(0, 0);
      lcd.print("Hold RIGHT bttn");
      lcd.setCursor(0, 1);
      lcd.print("to prime pump");
      
      //BUTTON ASSIGNMENT
      buttons = ReadButtons();
      if (buttons & BUTTON_LEFT){
        opState = MOTORS;
        return;
      }    
      if (buttons & BUTTON_RIGHT){
        digitalWrite(Motor.getPHdown_pin(), LOW);
        //while (buttons & BUTTON_RIGHT != 0){} 
        delay(5000);
        digitalWrite(Motor.getPHdown_pin(), HIGH);
        return;
      }
      selectButton();    
      ifIdleReturnHome();
    }
  }

//RETURN TO HOME SCREEN IF IDLE 
void ifIdleReturnHome(){
  if ((millis() - lastButtonPress) > lcdIdleTime){
    lcd.clear();
    opState = HOME; 
    currentMenuItem = 0;
    return;
  }
}

void cycleThruMenu(int firstItem, int lastItem){
  if (currentMenuItem < firstItem) {
    currentMenuItem = lastItem;
  }
  else if (currentMenuItem > lastItem) {
    currentMenuItem = firstItem;
  }
}

void upButton (){
  if (ReadButtons() & BUTTON_UP){
  currentMenuItem = currentMenuItem-1;
  return;
  }
}

void downButton(){
  if (ReadButtons() & BUTTON_DOWN){
  currentMenuItem = currentMenuItem+1;
  return;
  }
}

void selectButton(){
  if (ReadButtons() & BUTTON_SELECT) {
    opState = HOME;
    currentMenuItem = 0;
    return;  
  }
}

};
#endif
