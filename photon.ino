//Prints whatever comes in over serial
void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);
}

void loop() {
	if(Serial1.available())
		Serial.write(Serial1.read());

}
