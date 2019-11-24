#include "Adafruit_AMG88xx.h"
#include "Location.h"

#include <Wire.h>
#define Adafruit_AMG88xx_ADDR 0x68
Adafruit_AMG88xx amg;
Location prevLoc(amg);
boolean isSent=false;
void setup()
{
	/*setup code here */
	Serial.begin(9600);
	Serial.println(F("AMG88xx test"));
	// default settings
	boolean status = amg.begin(Adafruit_AMG88xx_ADDR);
	while (!status){
		status = amg.begin(Adafruit_AMG88xx_ADDR);
		Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
		delay(5000);
	}
	delay(100); // let sensor boot up
	prevLoc.load();
}



void loop()
{
	// reply only when you receive data:
	if (Serial.available() > 0) {
		// read the incoming byte:
		String strRec = Serial.readStringUntil('|');
		if(strRec.equalsIgnoreCase("identify")){
			Serial.flush();
			Serial.println("Location Arduino");
			delay(1000);
		}
	}
	Location loc(amg);
	loc.load();
	loc.calculateRelativeInfo(prevLoc);
	if(loc.x_Pos != prevLoc.x_Pos || loc.y_Pos != prevLoc.y_Pos)
	{
		if(!isSent)
			prevLoc.Send('a');
		loc.Send('a');
		isSent=true;
	}
	else
		isSent=false;

	prevLoc = loc;
	delay(500);

}

