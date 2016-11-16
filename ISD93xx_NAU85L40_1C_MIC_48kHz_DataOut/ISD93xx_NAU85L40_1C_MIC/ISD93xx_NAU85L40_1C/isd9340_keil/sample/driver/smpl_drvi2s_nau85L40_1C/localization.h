/***********LOCALIZATION.h*************************************/
#ifndef __LOCALIZATION_H__
#define __LOCALIZATION_H__

typedef struct microphone {
	float x;
	float y;
	float * buffer;
} microphone;

extern microphone * build_mic(float x, float y);
extern float find_source(microphone * m1, microphone * m2, microphone * m3);
extern void set_buffer_size(int bf_size);
float distance_calc(microphone * m1, microphone * m2);
float degree_calc(microphone * m1, microphone * m2);
extern void init_triangle(microphone * m1, microphone * m2, microphone * m3); 
extern void set_buffer(microphone * m1, float * buffer);
void find_mic_3(microphone * m1, microphone * m2);
float calc_right(float delay, float distance, float angle);
float calc_left(float delay, float distance, float angle);
float to_degrees(float angle);
#endif
