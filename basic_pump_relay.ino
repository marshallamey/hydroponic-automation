//Pump control pin variables
int pHup_pin = 30;
int pHdown_pin = 31;
int fertA_pin = 32;
int fertB_pin = 33;

int pump_duration = 200;

void setup() {
  Serial.begin(9600); 
  // put your setup code here, to run once:
  pinMode(pHup_pin, OUTPUT);
  pinMode(pHdown_pin, OUTPUT);
  pinMode(fertA_pin, OUTPUT);
  pinMode(fertB_pin, OUTPUT);
}

void loop() {
  //Set pins to low
  digitalWrite(pHup_pin, LOW);
  digitalWrite(pHdown_pin, LOW);
  Serial.println("LOW");
  digitalWrite(fertA_pin, HIGH);
  digitalWrite(fertB_pin, HIGH);
  delay(3000);
  //Set pins to high
  digitalWrite(pHup_pin, HIGH);
  digitalWrite(pHdown_pin, HIGH);
  Serial.println("HIGH");
  delay(10000);
}
