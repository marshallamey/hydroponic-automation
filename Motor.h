#ifndef Motor_h
#define Motor_h

/*NOTE: The relay used in this project uses a LOW state for ON and HIGH for OFF
  Location of pins for motors must be updated in this file*/

class MOTOR {
  
  private:

    // MOTOR PIN LOCATIONS
    int lights_pin = 30;
    int stonePump_pin = 31;
    int waterPump_pin = 32;
    int fertB_pin = 33;
    int fertA_pin = 34;
    int pHdown_pin = 35;
    int pHup_pin = 36; 
    int solenoid_pin = 37; 

    //TIMING VARIABLES
    long runTimePH = 3000;
    long runTimeEC = 6000;
    long solenoidRunTime = 5000;
    unsigned long currentMillis;
    unsigned long previousMillis;

  public:

    //Constructor
    MOTOR(){}

/************************************************************************************************
                                        MEMBER FUNCTIONS    
************************************************************************************************/

//INITIALIZE MOTOR PINS
    void begin() { 
      pinMode(pHup_pin, OUTPUT);
      pinMode(pHdown_pin, OUTPUT);
      pinMode(fertA_pin, OUTPUT);
      pinMode(fertB_pin, OUTPUT);
      pinMode(stonePump_pin, OUTPUT);
      pinMode(waterPump_pin, OUTPUT);
      pinMode(lights_pin, OUTPUT);
      pinMode(solenoid_pin, OUTPUT);
    
      digitalWrite(pHup_pin, HIGH);
      digitalWrite(pHdown_pin, HIGH);
      digitalWrite(fertA_pin, HIGH);
      digitalWrite(fertB_pin, HIGH);
      digitalWrite(stonePump_pin, HIGH);
      digitalWrite(waterPump_pin, HIGH);
      digitalWrite(lights_pin, HIGH);
      digitalWrite(solenoid_pin, HIGH);     
    }

//RAISE WATER TEMPERATURE OF THE NUTRIENT SOLUTION
    //void raiseWaterTemp() {}

//LOWER WATER TEMPERATURE OF THE NUTRIENT SOLUTION
    //void lowerWaterTemp() {}

//RAISE CONDUCTIVITY OF THE NUTRIENT SOLUTION
    void raiseConductivity() {
      Serial.println("Adding Nutrient A...");
      digitalWrite(fertA_pin, LOW);     
      currentMillis = millis();
      previousMillis = millis();      
      while (currentMillis - previousMillis < runTimeEC) { currentMillis = millis(); }
      digitalWrite(fertA_pin, HIGH);
      delay(1000);
      
      Serial.println("Adding Nutrient B...");
      digitalWrite(fertB_pin, LOW);
      currentMillis = millis();
      previousMillis = millis();     
      while (currentMillis - previousMillis < runTimeEC) { currentMillis = millis(); }   
      digitalWrite(fertB_pin, HIGH);
      delay(1000);
      
      Serial.println("Completed");         
    }

//LOWER CONDUCTIVITY OF THE NUTRIENT SOLUTION
    void lowerConductivity() {   
      Serial.print("Adding water..");
      digitalWrite(solenoid_pin, LOW);     
        currentMillis = millis();
        previousMillis = millis();     
        while (currentMillis - previousMillis < solenoidRunTime) { currentMillis = millis(); }
      digitalWrite(solenoid_pin, HIGH);
      delay(1000);
      Serial.println("Completed");
    }

//RAISE PH OF THE NUTRIENT SOLUTION
    void raisePH() {      
      Serial.println("Adding pH UP");
      digitalWrite(pHup_pin, LOW);    
      currentMillis = millis();
      previousMillis = millis();     
      while (currentMillis - previousMillis < runTimePH) { currentMillis = millis(); }  
      digitalWrite(pHup_pin, HIGH);
      delay(1000);
      Serial.println("Completed");
    }

//LOWER PH OF THE NUTRIENT SOLUTION
    void lowerPH() {    
      Serial.println("Adding pH DOWN...");
      digitalWrite(pHdown_pin, LOW);      
      currentMillis = millis();
      previousMillis = millis();     
      while (currentMillis - previousMillis < runTimePH) { currentMillis = millis(); }     
      digitalWrite(pHdown_pin, HIGH);
      delay(1000);
      Serial.println("Completed");
    }

//RAISE OXYGEN LEVEL OF THE NUTRIENT SOLUTION
// void raiseOxygenLevel() {}

// LOWER OXYGEN LEVEL OF THE NUTRIENT SOLUTION
// void lowerOxygenLevel() {}

// RAISE HUMIDITY OF THE ENVIRONMENT
// void raiseHumidity() {}

// LOWER HUMIDITY OF THE ENVIRONMENT
// void lowerHumidity() {}

// RAISE AIR TEMPERATURE OF THE ENVIRONMENT
// void raiseAirTemp() {}

// LOWER AIR TEMPERATURE OF THE ENIVRONMENT
// void lowerAirTemp() {}

// RAISE CARBON DIOXIDE LEVEL OF THE ENVIRONMENT
// void raiseCarbonLevel() {}

// LOWER CARBON DIOXIDE LEVEL OF THE ENVIRONMENT
// void lowerCarbonLevel() {}

// RAISE PAR LEVEL OF THE ENVIRONMENT
// void raiseParLevel() {}

// LOWER PAR LEVEL OF THE ENVIRONMENT
// void lowerParLevel() {}

/*************************************************************************************************
                                      ACCESSOR FUNCTIONS
*************************************************************************************************/

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
  
  int getLights_pin(){
    return lights_pin;
  }
      
  int getStonePump_pin(){
    return stonePump_pin;
  }
      
  int getWaterPump_pin(){
    return waterPump_pin;
  }
  
  int getSolenoid_pin(){
    return solenoid_pin;
  }

};
#endif
