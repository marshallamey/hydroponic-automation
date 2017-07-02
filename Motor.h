#ifndef Motor_h
#define Motor_h

/*NOTE: The relay used in this project uses a LOW state for ON and HIGH for OFF
  Location of pins for motors must be updated in this file*/

class MOTOR {
  
  private:

// MOTOR PIN LOCATIONS
    int pHup_pin = 30;
    int pHdown_pin = 31;
    int fertA_pin = 32;
    int fertB_pin = 33;
    int mixPump_pin = 34;
    int stonePump_pin = 35;
    int water_pin = 36;  

//TIMING VARIABLES
    long pumpRunTime = 5000;
    unsigned long currentMillis;
    unsigned long previousMillis;

  public:

//CONSTRUCTOR
    MOTOR(){}

/************************************************************************************************
MEMBER FUNCTIONS    
************************************************************************************************/

//INITIALIZE MOTOR PINS
    void initializePins() { 
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
    }

//RAISE WATER TEMPERATURE OF THE NUTRIENT SOLUTION
    //void raiseWaterTemp() {}

//LOWER WATER TEMPERATURE OF THE NUTRIENT SOLUTION
    //void lowerWaterTemp() {}

//RAISE CONDUCTIVITY OF THE NUTRIENT SOLUTION
    void raiseConductivity() {
      digitalWrite(fertA_pin, LOW);
      digitalWrite(fertB_pin, LOW);
      digitalWrite(mixPump_pin, LOW);

      currentMillis = millis();
      previousMillis = millis();
      
      while (currentMillis - previousMillis < pumpRunTime) {
        currentMillis = millis();
      }
      digitalWrite(fertA_pin, HIGH);
      digitalWrite(fertB_pin, HIGH);    
      digitalWrite(mixPump_pin, HIGH);
    }

//LOWER CONDUCTIVITY OF THE NUTRIENT SOLUTION
    //void lowerConductivity() {}

//RAISE PH OF THE NUTRIENT SOLUTION
    void raisePH() {
      digitalWrite(pHup_pin, LOW);
      digitalWrite(mixPump_pin, LOW);
      
      currentMillis = millis();
      previousMillis = millis();
      while (currentMillis - previousMillis < pumpRunTime) { 
        currentMillis = millis();
      }
    
      digitalWrite(pHup_pin, HIGH);
      digitalWrite(mixPump_pin, HIGH);
    }

//LOWER PH OF THE NUTRIENT SOLUTION
    void lowerPH() {
      digitalWrite(pHdown_pin, LOW);
      digitalWrite(mixPump_pin, LOW);
      
      currentMillis = millis();
      previousMillis = millis();
      while (currentMillis - previousMillis < pumpRunTime) { 
        currentMillis = millis();
      }
      
      digitalWrite(pHdown_pin, HIGH);
      digitalWrite(mixPump_pin, HIGH);
    }

//RAISE OXYGEN LEVEL OF THE NUTRIENT SOLUTION
    void raiseOxygenLevel() {
      digitalWrite(stonePump_pin, LOW);
      digitalWrite(mixPump_pin, LOW);
      
      currentMillis = millis();
      previousMillis = millis();
      while (currentMillis - previousMillis < pumpRunTime) { 
        currentMillis = millis();
      }
    
      digitalWrite(stonePump_pin, HIGH);
      digitalWrite(mixPump_pin, HIGH);
    }

// LOWER OXYGEN LEVEL OF THE NUTRIENT SOLUTION
// void lowerOxygenLevel() {}

// RAISE HUMIDITY OF THE ENVIRONMENT
// LOWER HUMIDITY OF THE ENVIRONMENT

// RAISE AIR TEMPERATURE OF THE ENVIRONMENT
// LOWER AIR TEMPERATURE OF THE ENIVRONMENT

// RAISE CARBON DIOXIDE LEVEL OF THE ENVIRONMENT
// LOWER CARBON DIOXIDE LEVEL OF THE ENVIRONMENT

// RAISE FLOW RATE OF THE NUTRIENT SOLUTION
// LOWER FLOW RATE OF THE NUTRIENT SOLUTION

/***************************************************************************************************
ACCESSOR FUNCTIONS
******************/
  
int getFertA_pin(){
  return fertA_pin; 
}

int getFertB_pin(){
  return fertB_pin; 
}

int getPHup_pin(){
  return pHup_pin; 
}

int getPHdown_pin(){
  return pHdown_pin; 
}
  
int getAirPump_pin(){
  return airPump_pin; 
}
  
int getStonePump_pin(){
  return stonePump_pin; 
}
  
int getWaterPump_pin(){
  return waterPump_pin; 
}

};
#endif
