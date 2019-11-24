/* 
* Location.cpp
*
* Created: 28/05/2018 15:20:42
* Author: liori
*/


#include "Location.h"


//  Constructors
Location::Location(Adafruit_AMG88xx &amg )
{
	this->amg = amg;
}
Location::Location()
{
	this->amg = amg;
}

void Location::load(){
		
		#define N AMG88xx_PIXEL_ARRAY_SIZE //8*8 =64 pixels
		#define LINES  8
		#define COLUMNS  8
		#define OFFSET 0.95
		#define DEVIATION_OFFSET 0.4
		#define POS 0.5
		#define TRMP_DELTA 1.5
		float pixels[N];
		amg.readPixels(pixels);
		time = millis();
		
		//Find median value for reference
		float tempPixels[N];
		for(int i = 0 ; i<N;i++){
			tempPixels[i]=pixels[i];
		}
		
		int k =  N*(POS); //middle position
		double median = selectKth(tempPixels,N, k);
		//calculate the location of the person (x,y) from pixels array reference to the median value
		boolean isFound = false;
		int fixCounter =0 ;
		for(int i = 0 ; i<N ; i++)
		{
			if(pixels[i] > median+TRMP_DELTA){
				isFound = true;
				y_Pos = y_Pos + (i / COLUMNS)*OFFSET;
				x_Pos = x_Pos + (i % LINES );
				fixCounter++;
			}
		}
		if(isFound){
			y_Pos/=fixCounter;
			x_Pos/=fixCounter;
		}
	}
	
void Location::calculateRelativeInfo(Location previousLocation){
		if( abs(y_Pos-previousLocation.y_Pos)<DEVIATION_OFFSET)
		y_Pos=previousLocation.y_Pos;
		if( abs(x_Pos-previousLocation.x_Pos)<DEVIATION_OFFSET)
		x_Pos=previousLocation.x_Pos;
		
		durationTime = time- previousLocation.time;//calculate movement time  (dT)
		distance =sqrt(pow(x_Pos-previousLocation.x_Pos,2) + pow(y_Pos-previousLocation.y_Pos,2)); // (dL) = sqrt((dX)^2 + (dY)^2)
		velocity =((float)distance)/((float)durationTime/1000);//(V) = (dL)/(dT)
	}

void Location::Send(char binary_ASCI='b')
	{
		byte buf[] = {time/100,x_Pos,y_Pos,distance,velocity};
		switch(binary_ASCI)
		{
			case 'b' :
			//export data as Binary
			Serial.write(buf,7); // send as binary - will return an error (not bytes)
			break;
			case 'a':
			//char out[50];
			//sprintf(out, "[ %7d , %4d , %4d  , %4d , %4d ]", buf[0], buf[1], buf[2],buf[3],buf[4]);
			//Serial.println(out); // send as ASCII - to display on the serial monitor
			
			//export data as ASCII
			Serial.print("[");
			Serial.print(time/100);
			Serial.print(",");
			Serial.print(x_Pos);
			Serial.print(",");
			Serial.print(y_Pos);
			Serial.print(",");
			Serial.print(distance);
			Serial.print(",");
			Serial.print(velocity);
			Serial.println("]");
			break;
		}
	}

float Location::selectKth(float* arr,int size, int k) {
	int from = 0, to = size - 1;
	// if from == to we reached the kth element
	while (from < to) {
		int r = from, w = to;
		float mid = arr[(r + w) / 2];
		
		// stop if the reader and writer meets
		while (r < w) {
			if (arr[r] >= mid) { // put the large values at the end
				float tmp = arr[w];
				arr[w] = arr[r];
				arr[r] = tmp;
				w--;
				} else { // the value is smaller than the pivot, skip
				r++;
			}
		}
		// if we stepped up (r++) we need to step one down
		if (arr[r] > mid)
		r--;
		
		// the r pointer is on the end of the first k elements
		if (k <= r) {
			to = r;
			} else {
			from = r + 1;
		}
	}
	return arr[k];
}