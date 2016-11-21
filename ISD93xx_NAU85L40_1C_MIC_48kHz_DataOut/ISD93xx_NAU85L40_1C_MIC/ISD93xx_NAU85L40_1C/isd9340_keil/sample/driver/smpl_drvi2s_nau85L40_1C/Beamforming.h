#ifndef __BEAMFORMING_H__
#define __BEAMFORMING_H__
#include <stdint.h>
#include <stdbool.h>
#include "localization.h"
extern void Beamforming_Initialization(void);
extern uint16_t run(float in0, float in1, float in2, float in3);
extern uint16_t run_and_locate(float in0, float in1, float in2, float in3);
void delays_init();
void localization_init();
void localization_test();
void change_delay();
float locate(int buf_size);
void sin_test();
void print_delay_info();
float * wav_file_tests();

int location_buffer;


#endif
