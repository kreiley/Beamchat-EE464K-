#ifndef __DELAY_SOUND_H__
#define __DELAY_SOUND_H__

typedef struct delay {
	int mic;
	int read_pointer;
	int write_pointer;
	float num_samples_to_delay;
	float fractional_delay;
	float delay_tap_mix_volume;
	float thiran_coeff_1;
	float thiran_coeff_2;
	float thiran_coeff_3;
	float prev_in[3];
	float prev_out[3];
	float buffer[100];
} delay;

void delay_init(float num_samples_to_delay, float fractional_delay, int mic);
extern float delay_out(int mic, float in);

#endif
