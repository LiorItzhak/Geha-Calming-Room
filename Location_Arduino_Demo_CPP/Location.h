/* 
* Location.h
*
* Created: 28/05/2018 15:20:42
* Author: liori
*/

#include "Adafruit_AMG88xx.h"

#ifndef __LOCATION_H__
#define __LOCATION_H__


class Location
{
//variables
	private: Adafruit_AMG88xx amg;
	public: double x_Pos=0, y_Pos=0;
	private: double distance=-1;
	private: unsigned long durationTime=-1;
	private: double velocity=-1;
	private: unsigned long time;
//functions
public:
	Location(Adafruit_AMG88xx &amg );
	Location();

	void load();
	void calculateRelativeInfo(Location previousLocation);
	void Send(char binary_ASCI='b');
protected:
private:
	float selectKth(float* arr,int size, int k);

}; //Location

#endif //__LOCATION_H__
