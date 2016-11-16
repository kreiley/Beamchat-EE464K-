#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "localization.h"

#define threshold 0.5
#define speed_of_sound 343.0
#define freq 16000

/*must be equalateral triangle microphone array because its easier*/

int buffer_size = 0;
float distance12 = 0;
float distance13 = 0;
float distance23 = 0;
float angle12 = 0;
float angle13 = 0;
float angle21 = 0;
float angle23 = 0;
float angle31 = 0;
float angle32 = 0;

microphone * build_mic(float x, float y){
	microphone * mic = malloc(sizeof(microphone));
	mic->x = x;
	mic->y = y;
	mic->buffer = NULL;
	return mic;
}

/*tan((speed of sound * number of samples / freq)/ distance) = angle */ 

float find_source(microphone * m1, microphone * m2, microphone * m3){
	bool done1  = false;
	bool done2  = false;
	bool done3  = false;
	int first = 0;
	int second = 0;
	int third = 0;
	int num_samples1 = -1;
	int num_samples2 = -1;
	for(int i = 0; i < buffer_size; i++){
		printf("m1 = %f  m2 =%f  m3 =%f\n", m1->buffer[i], m2->buffer[i], m3->buffer[i]);
		if(!done1 && m1->buffer[i] >= threshold){
			done1 = true;
			if(!first){first = 1;}
			else if(!second){second = 1;}
			else {third = 1;}
		}
		if(!done2 && m2->buffer[i] >= threshold){
			done2 = true;
			if(!first){first = 2;}
			else if(!second){second = 2;}
			else {third = 2;}
		
		}
		if(!done3 && m3->buffer[i] >= threshold){
			done3 = true;
			if(!first){first = 3;}
			else if(!second){second = 3;}
			else {third = 3;}
		
		}
		if(first > 0 && second == 0){
			num_samples1++;
			num_samples2++;
			printf("num_samples1 = %d, num_samples2 = %d\n", num_samples1, num_samples2);
		}
		if(second > 0 && third == 0){
			if(num_samples1 < 0){num_samples1 = 0;}
			num_samples2++;
			printf("num_samples2 = %d\n",num_samples2);
		}
		if(third > 0){
			printf("\nfirst = %d, second = %d, third = %d\n", first, second, third);
			break;
		}
	}
	printf("m1 x = %f y = %f, m2 x = %f y = %f, m3 x = %f y = %f\n",m1->x,m1->y,m2->x,m2->y,m3->x,m3->y);
	if(num_samples1 < 0){return -1;}
	float delay = (speed_of_sound * num_samples1)/freq;
	if     (first == 1 && second == 2){return calc_left (delay, distance12, angle12);}
	else if(first == 1 && second == 3){return calc_right(delay, distance13, angle13);}
	else if(first == 2 && second == 1){return calc_right(delay, distance12, angle21);}
	else if(first == 2 && second == 3){return calc_left (delay, distance23, angle23);}
	else if(first == 3 && second == 1){return calc_left (delay, distance13, angle31);}
	else if(first == 3 && second == 2){return calc_right(delay, distance23, angle32);}
	return -1;
}

float calc_right(float delay, float distance, float angle){
	return to_degrees(asin(delay/distance) + angle + M_PI/2);
}

float calc_left(float delay, float distance, float angle){
	return to_degrees(angle - (asin(delay/distance) + M_PI/2));
}

float to_degrees (float angle){
	while(angle > 2*M_PI){
		angle-=(2*M_PI);
	}
	while(angle < 0){
		angle+=(2*M_PI);
	}
	return angle * 180/M_PI;
} 

void set_buffer_size(int bf_size){
	buffer_size = bf_size;
}

void set_buffer(microphone * m1, float * buffer){
	m1->buffer = buffer;	
}

float distance_calc(microphone * m1, microphone * m2){
	float a, b;
	a = m1->x - m2->x;
	b = m1->y - m2->y;	
	return sqrt(a*a + b*b);
}

float degree_calc(microphone * m1, microphone *m2){
	float x , y;
	x = m2->x - m1->x;
	y = m2->y - m1->y;
	if(x == 0 && y == 0){printf("error");return -7;}
	return atan2(y,x);
}

void init_triangle(microphone * m1, microphone * m2, microphone * m3){
	angle12 = degree_calc(m1, m2);
	angle13 = degree_calc(m1, m3);
	angle21 = degree_calc(m2, m1);
	angle23 = degree_calc(m2, m3);
	angle31 = degree_calc(m3, m1);
	angle32 = degree_calc(m3, m2);
	distance12 = distance_calc(m1, m2);
	distance13 = distance_calc(m1, m3);
	distance23 = distance_calc(m2 ,m3); 
	find_mic_3(m1,m2);
}

void find_mic_3(microphone * m1, microphone * m2){
	float angle1 = degree_calc(m1,m2) + 1.0472;
	float angle2 = degree_calc(m2,m1) - 1.0472;
	float b1 = m1->y - tan(angle1) * m1->x;
	float b2 = m2->y - tan(angle2) * m2->x;
	printf("angle1 = %f, angle2 = %f, b1 = %f, b2 = %f \n", angle1, angle2,b1,b2);
	float x = (b2 - b1)/(tan(angle1) - tan(angle2));
	float y = (tan(angle1))*x + b1;
	printf("x = %f, y = %f", x, y);
}
