/***********LOCALIZATION.h*************************************/
#ifndef __LOCALIZATION_H__
#define __LOCALIZATION_H__

typedef struct microphone {
	float x;
	float y;
	float buffer[100];
} microphone;

extern void build_mic(float x, float y,int mic);
extern float find_source();
extern void set_buffer_size(int bf_size);
float distance_calc(int m1, int m2);
float degree_calc(int m1, int m2);
extern void init_triangle(int m1, int m2, int m3); 
extern void add_to_buffer(int m1, float add, int placement);
void find_mic_3(int m1, int m2);
float calc_right(float delay, float distance, float angle);
float calc_left(float delay, float distance, float angle);
float to_degrees(float angle);
#endif
