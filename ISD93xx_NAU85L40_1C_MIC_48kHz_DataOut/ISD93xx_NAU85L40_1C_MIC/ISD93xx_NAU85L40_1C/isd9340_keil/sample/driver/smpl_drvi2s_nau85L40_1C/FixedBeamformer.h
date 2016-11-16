/***********FixedBeamformer.h*************************************/
#ifndef __FIXEDBEAMFORMER_H__
#define __FIXEDBEAMFORMER_H__

typedef struct mic_array{
    	float x;
	float y;
	float current_delay;
} mic_array;

extern void mic_array_init();
extern void calculate_delay(float angle);
extern float get_current_delay(int mic_number);
extern float get_x(int mic_number);
extern float get_y(int mic_number);

#endif
