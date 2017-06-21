//Pump control pin variables
int pHup_pin = 30;
int pHdown_pin = 31;
int fertA_pin = 32;
int fertB_pin = 33;
int mixPump_pin = 34;
int stonePump_pin = 35;
int water_pin = 36;

// Set environment parameters

int pump_duration = 10000;
float pH_high = 6.5;
float pH_low = 5.5;
float conductivity_high = 5000;
float conductivity_low = 2000;
float oxygen_high = 5000;
float oxygen_low = 2000;

void setup() {
	Serial.begin(9600);
	// put your setup code here, to run once:
	pinMode(pHup_pin, OUTPUT);
	pinMode(pHdown_pin, OUTPUT);
	pinMode(fertA_pin, OUTPUT);
	pinMode(fertB_pin, OUTPUT);
	pinMode(mixPump_pin, OUTPUT);
	pinMode(stonePump_pin, OUTPUT);
}

void loop() {

	// USE PH DOWN SOLUTION IF PH IS TOO HIGH
	if (pH > pH_high) {
		digitalWrite(pHdown_pin, HIGH);
		digitalWrite(mixPump_pin, HIGH);
		delay(pump_duration);
		digitalWrite(pHdown_pin, LOW);
		digitalWrite(mixPump_pin, LOW);
	}

	// USE PH UP SOLUTION IT PH IS TOO LOW
	if (pH < pH_low) {
		digitalWrite(pHup_pin, HIGH);
		digitalWrite(mixPump_pin, HIGH);
		delay(pump_duration);
		digitalWrite(pHup_pin, LOW);
		digitalWrite(mixPump_pin, LOW);
	}

	// USE NUTRIENTS A & B IF CONDUCTIVITY IS TOO LOW
	if (conductivity < conductivity_low) {
		digitalWrite(fertA_pin, HIGH);
		digitalWrite(fertB_pin, HIGH);
		digitalWrite(mixPump_pin, HIGH);
		delay(pump_duration);
		digitalWrite(fertA_pin, LOW);
		digitalWrite(fertB_pin, LOW);
		digitalWrite(mixPump_pin, LOW);
	}

	// ADD WATER IF CONDUCTIVITY IS TOO HIGH
	//if (conductivity > conductivity_high) {
	//	digitalWrite(water_pin, HIGH);
	//	digitalWrite(mixPump_pin, HIGH);
	//	delay(pump_duration);
	//	digitalWrite(water_pin, LOW);
	//	digitalWrite(mixPump_pin, LOW);
	//}

	// USE AIRSTONE IF DISSOLVED OXYGEN IS TOO LOW
	if (oxygen < oxygen_low) {
		digitalWrite(stonePump_pin, HIGH);
		digitalWrite(mixPump_pin, HIGH);
		delay(pump_duration);
		digitalWrite(stonePump_pin, LOW);
		digitalWrite(mixPump_pin, LOW);
	}

	/*Set pins to low
	digitalWrite(pHup_pin, LOW);
	digitalWrite(pHdown_pin, LOW);
	digitalWrite(fertA_pin, LOW);
	digitalWrite(fertB_pin, LOW);
	digitalWrite(mixPump_pin, LOW);
	digitalWrite(stonePump_pin, LOW);
	Serial.println("LOW");
	delay(3000);
	//Set pins to high
	digitalWrite(pHup_pin, HIGH);
	digitalWrite(pHdown_pin, HIGH);
	digitalWrite(fertA_pin, HIGH);
	digitalWrite(fertB_pin, HIGH);
	digitalWrite(mixPump_pin, HIGH);
	digitalWrite(stonePump_pin, HIGH);
	Serial.println("HIGH");
	delay(10000);
	*/
}
