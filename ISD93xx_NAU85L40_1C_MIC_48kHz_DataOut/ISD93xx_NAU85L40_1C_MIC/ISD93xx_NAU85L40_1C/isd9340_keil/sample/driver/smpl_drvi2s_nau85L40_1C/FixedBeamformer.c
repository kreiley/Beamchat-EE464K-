#include <stdio.h>
#include <math.h>
#include "delay.h"
#include "delaytest.h"
#include "FixedBeamformer.h"

#define num_mics 8
static mic_array mics[num_mics];

void calculate_delay(float angle){
	printf("\nAngle is : %f\n",angle);
	if(fmod(angle,45) != 0){
		float m = tan((angle*M_PI)/180);
		printf("\nSlope is : %f\n",m);
		float perp_m;
		perp_m = -1/m;
		printf("\nPerp Slope is : %f\n",perp_m);
		for(int i = 0; i < num_mics; i++){
			float b = mics[i].y - perp_m * mics[i].x ;
			printf("\nb =  %f , b/2 =%f\n m + perp_m = %f",b, b/2, m+perp_m);
			float x = b/(perp_m + m);
			float y = m * x;
			float c = sqrt(pow(x,2) + pow(y,2));
			printf("\nb = %f, x = %f, y=%f,c=%f",b,x,y,c);
			if(y < 0){c = c * -1;}
			mics[i].current_delay = c;		
		}
	}
	else if (fmod(angle, 360) == 0){
		for(int i = 0; i < num_mics; i++){
			mics[i].current_delay = mics[i].x;
		}
	}
	else if (fmod(angle, 180) == 0){
		for(int i = 0; i < num_mics; i++){
			mics[i].current_delay = -mics[i].x;
		}
	}
	else if (fmod(angle, 270) == 0){
		for(int i = 0; i < num_mics; i++){
			mics[i].current_delay = -mics[i].y;
		}
	}
	else if (fmod(angle, 90) == 0){
		for(int i = 0; i < num_mics; i++){
			mics[i].current_delay = mics[i].y;
		}
	}	
	else if (fmod(angle, 135) == 0){
		for(int i = 0; i < num_mics; i++){
			float b = mics[i].y - mics[i].x ;
			float x;
			if(mics[i].y - mics[i].x == 0) {x = 0; b =0;}
			else{x = -b/2;}
			float y = -1 * x;
			float c = sqrt(2)* x;
			printf("\nb = %f, x = %f, y=%f,c=%f",b,x,y,c);
			if(y < 0){c = c * -1;}
			mics[i].current_delay = c;	
		}
	}
	else if (fmod(angle, 45) == 0){
		for(int i = 0; i < num_mics; i++){
			float b = mics[i].y + mics[i].x ;
			float x;
			if(mics[i].y + mics[i].x == 0) {x = 0; b =0;}
			else{x = b/2;}
			float y = x;
			float c = sqrt(2)* x;
			printf("\nb = %f, x = %f, y=%f,c=%f",b,x,y,c);
			if(y < 0){c = c * -1;}
			mics[i].current_delay = c;	
		}
	}
	float temp = mics[0].current_delay;
	for(int i =1; i< num_mics; i++){
		if(mics[i].current_delay < temp){
			temp = mics[i].current_delay;
		}
	}
	for(int i =0; i < num_mics; i++){
		mics[i].current_delay = mics[i].current_delay - temp;
	}
}

void mic_array_init(){
	printf("\n Microphone Positions:\n");
	for(int i = 4; i < num_mics; i++){
		float theta = i * M_PI/4;
		float a =.01;
		float b =.7;
		float shift = -.5;
		mics[i].x = a*cos(theta - shift) * exp(b*(theta-shift)); 
		mics[i].y = a*sin(theta - shift) * exp(b*(theta - shift));
	}
	//mics[num_mics-1].x = 0.489641;
	//mics[num_mics-1].y = 0.08055;
	printf("\nx =\n");
	for(int i = 0; i < num_mics; i++){printf("%f\n",mics[i].x);}
	printf("\ny =\n");
	for(int i = 0; i < num_mics; i++){printf("%f\n",mics[i].y);}
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
