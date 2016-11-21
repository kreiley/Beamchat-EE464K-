#include <stdio.h>
#include <math.h>
#include "delay.h"
#include "delaytest.h"
#include "FixedBeamformer.h"

#define num_mics 8
#define pi  3.14159
static mic_array mics[num_mics];

void calculate_delay(float angle){
	while(angle > 360){angle -= 360;}
	while(angle < 0){angle += 360;}
	if(angle == 0 || angle == 360){
		for(int i = 0; i < num_mics; i++){
			mics[i].current_delay = mics[i].x;
		}
	}
	else if (angle > 0 && angle < 45){
		for(int i = 0; i < num_mics; i++){
			float x, y, c;
			float b = mics[i].y - tan((angle + 90)*pi/180)*mics[i].x;
			float m = (tan(angle*pi/180) - tan(((angle + 90)*pi)/180));
			if(b == 0){x = 0; y = 0;}
			else{
				x = b / m;	
				y = tan(angle * pi /180)*x;
			}
			c = sqrt(x*x + y*y);
			if(x < 0){c = -c;}
			mics[i].current_delay = c;
		}
	}
	else if (angle == 45){
		for(int i = 0; i < num_mics; i++){
			float x, y,c;
			float b = mics[i].y + mics[i].x;
			if(b == 0){x = 0; y = 0;}
			else{
				x = b/2;
				y = x;
			}
			c = sqrt(x*x + y*y);
			if(x < 0){c = -c;}
			mics[i].current_delay = c;
		}
	}
	else if (angle > 45 && angle < 90){
		for(int i = 0; i < num_mics; i++){
			float x, y,c;
			float b = mics[i].y - tan((angle + 90)*pi/180)*mics[i].x;
			float m = (tan(angle*pi/180) - tan(((angle + 90)*pi)/180));
			if(b == 0){x = 0; y = 0;}
			else{
				x = b / m;	
				y = tan(angle * pi /180)*x;
			}
			c = sqrt(x*x + y*y);
			if(x < 0){c = -c;}
			mics[i].current_delay = c;
		}
	}
	else if (angle == 90){
		for(int i = 0; i < num_mics; i++){
			mics[i].current_delay = mics[i].y;
		}
	}
	else if (angle > 90 && angle < 135){
		for(int i = 0; i < num_mics; i++){
			float x, y,c;
			float b = mics[i].y - tan((angle + 90)*pi/180)*mics[i].x;
			float m = (tan(angle*pi/180) - tan(((angle + 90)*pi)/180));
			if(b == 0){x = 0; y = 0;}
			else{
				x = b / m;	
				y = tan(angle * pi /180)*x;
			}
			c = sqrt(x*x + y*y);
			if(x > 0){c = -c;}
			mics[i].current_delay = c;
		}
	}
	else if (angle == 135){
		for(int i = 0; i < num_mics; i++){
			float x, y,c;
			float b = mics[i].y - mics[i].x;
			if(b == 0){x = 0; y = 0;}
			else{
				x = -b/2;
				y = -x;
			}
			c = sqrt(x*x + y*y);
			if(x > 0){c = -c;}
			mics[i].current_delay = c;
		}
	}
	else if (angle > 135 && angle < 180){
		for(int i = 0; i < num_mics; i++){
			float x, y,c;
			float b = mics[i].y - tan((angle + 90)*pi/180)*mics[i].x;
			float m = (tan(angle*pi/180) - tan(((angle + 90)*pi)/180));
			if(b == 0){x = 0; y = 0;}
			else{
				x = b / m;	
				y = tan(angle * pi /180)*x;
			}
			c = sqrt(x*x + y*y);
			if(x > 0){c = -c;}
			mics[i].current_delay = c;
		}
	}
	else if (angle == 180){
		for(int i = 0; i < num_mics; i++){
			mics[i].current_delay = -mics[i].x;
		}
	}
	else if (angle > 180 && angle < 225){
		for(int i = 0; i < num_mics; i++){
			float x, y,c;
			float b = mics[i].y - tan((angle + 90)*pi/180)*mics[i].x;
			float m = (tan(angle*pi/180) - tan(((angle + 90)*pi)/180));
			if(b == 0){x = 0; y = 0;}
			else{
				x = b / m;	
				y = tan(angle * pi /180)*x;
			}
			c = sqrt(x*x + y*y);
			if(x > 0){c = -c;}
			mics[i].current_delay = c;
		}
	}
	else if (angle == 225){
		for(int i = 0; i < num_mics; i++){
			float x, y,c;
			float b = mics[i].y + mics[i].x;
			if(b == 0){x = 0; y = 0;}
			else{
				x = b/2;
				y = x;
			}
			c = sqrt(x*x + y*y);
			if(x > 0){c = -c;}
			mics[i].current_delay = c;
		}
	}
	else if (angle > 225 && angle < 270){
		for(int i = 0; i < num_mics; i++){
			float x, y,c;
			float b = mics[i].y - tan((angle + 90)*pi/180)*mics[i].x;
			float m = (tan(angle*pi/180) - tan(((angle + 90)*pi)/180));
			if(b == 0){x = 0; y = 0;}
			else{
				x = b / m;	
				y = tan(angle * pi /180)*x;
			}
			c = sqrt(x*x + y*y);
			if(x > 0){c = -c;}
			mics[i].current_delay = c;
		}
	}
	else if (angle == 270){
		for(int i = 0; i < num_mics; i++){
			mics[i].current_delay = -mics[i].y;
		}
	}
	else if (angle > 270 && angle < 315){
		for(int i = 0; i < num_mics; i++){
			float x, y,c;
			float b = mics[i].y - tan((angle - 90)*pi/180)*mics[i].x;
			float m = (tan(angle*pi/180) - tan(((angle + 90)*pi)/180));
			if(b == 0){x = 0; y = 0;}
			else{
				x = b / m;	
				y = tan(angle * pi /180)*x;
			}
			c = sqrt(x*x + y*y);
			if(x < 0){c = -c;}
			mics[i].current_delay = c;
		}
	}
	else if (angle == 315){
		for(int i = 0; i < num_mics; i++){
			float x, y, c;
			float b = mics[i].y - mics[i].x;
			if(b == 0){x = 0; y = 0;}
			else{
				x = -b/2;
				y = -x;
			}
			c = sqrt(x*x + y*y);
			if(x < 0){c = -c;}
			mics[i].current_delay = c;
		}
	}
	else if (angle > 315 && angle < 360){
		for(int i = 0; i < num_mics; i++){
			float x, y,c;
			float b = mics[i].y - tan((angle - 90)*pi/180)*mics[i].x;
			float m = (tan(angle*pi/180) - tan(((angle + 90)*pi)/180));
			if(b == 0){x = 0; y = 0;}
			else{
				x = b / m;	
				y = tan(angle * pi /180)*x;
			}
			c = sqrt(x*x + y*y);
			if(x < 0){c = -c;}
			mics[i].current_delay = c;
		}
	}
	float min = mics[0].current_delay;
	for(int i = 1; i < num_mics; i++){
		if(mics[i].current_delay < min){
			min = mics[i].current_delay;
		}
	}
	for(int i = 0; i < num_mics; i++){
		mics[i].current_delay = mics[i].current_delay - min;
	}
}	

void mic_array_init(){
//	printf("\n Microphone Positions:\n");
/*	for(int i = 4; i < num_mics; i++){
		float theta = i * M_PI/4;
		float a =.01;
		float b =.7;
		float shift = -.5;
		mics[i].x = a*cos(theta - shift) * exp(b*(theta-shift)); 
		mics[i].y = a*sin(theta - shift) * exp(b*(theta - shift));
	}
	//mics[num_mics-1].x = 0.489641;
	//mics[num_mics-1].y = 0.08055;
*/
	mics[0].x =  0.013092;
	mics[1].x =  0.007873;
	mics[2].x = -0.025130;
	mics[3].x = -0.094278;
	mics[4].x = -0.112293;
	mics[5].x = -0.062427;
	mics[6].x =  0.184213;
	mics[7].x =  0.274856;

	mics[0].y =  0.007152;
	mics[1].y =  0.026832;
	mics[2].y =  0.046000;
	mics[3].y =  0.027662;
	mics[4].y = -0.061346;
	mics[5].y = -0.212765;
	mics[6].y = -0.337199;
	mics[7].y =  0.057511;

//	printf("\nx =\n");
//	for(int i = 0; i < num_mics; i++){printf("%f\n",mics[i].x);}
//	printf("\ny =\n");
//	for(int i = 0; i < num_mics; i++){printf("%f\n",mics[i].y);}
}

float get_current_delay(int mic_number){
	return mics[mic_number].current_delay;
}

float get_x(int mic_number){
	return mics[mic_number].x;
}

float get_y(int mic_number){
	return mics[mic_number].y;
}
