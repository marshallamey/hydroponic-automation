#include "ThingSpeak.h"

TCPClient client;

/*
  *****************************************************************************************
  **** Visit https://www.thingspeak.com to sign up for a free account and create
  **** a channel.  The video tutorial http://community.thingspeak.com/tutorials/thingspeak-channels/
  **** has more information. You need to change this to your channel, and your write API key
  **** IF YOU SHARE YOUR CODE WITH OTHERS, MAKE SURE YOU REMOVE YOUR WRITE API KEY!!
  *****************************************************************************************/
// Thinkspeak channel information
char thingSpeakAddress[] = "api.thingspeak.com";
unsigned long myChannelNumber = 298095;
String myWriteAPIKey = "F8Q6F3XFDI3QDLUY";

//Prints whatever comes in over serial
void setup() {
    ThingSpeak.begin(client);
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial.println("===Starting===");
}

void loop()
{
    String data="I am null";
	if(Serial1.available())
	{
	    data = Serial1.readStringUntil(13);

	    Serial.print(data);
        ThingSpeakUpdate(data);
    }
}


/*------------------------------------------------
Sends sensor data to Thingspeak
Inputs: String, data to be entered for each field
Returns:
------------------------------------------------*/
void ThingSpeakUpdate(String readings)

{
    Serial.println("Date string: " + readings);

    Serial.println("...Connecting to Thingspeak");

    // Connecting and sending data to Thingspeak
    if(client.connect(thingSpeakAddress, 80))
    {
        Serial.println("...Connection succesful, updating datastreams");

        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+ myWriteAPIKey +"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(readings.length());
        client.print("\n\n");

        client.println(readings); //the ""ln" is important here.

        // This delay is pivitol without it the TCP client will often close before the data is fully sent
        delay(200);

        Serial.println("Thingspeak update sent.");
    }
    else{
        // Failed to connect to Thingspeak
        Serial.println("Unable to connect to Thingspeak.");
    }

    if(!client.connected()){
        client.stop();
    }
    client.flush();
    client.stop();
}
