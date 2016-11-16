/***********FixedBeamformer.h*************************************/
#ifndef __FIXEDBEAMFORMER_H__
#define __FIXEDBEAMFORMER_H__

extern void mic_array_init();
extern void calculate_delay(float angle);
extern float get_current_delay(int mic_number);
extern float get_x(int mic_number);
extern float get_y(int mic_number);

#endif
