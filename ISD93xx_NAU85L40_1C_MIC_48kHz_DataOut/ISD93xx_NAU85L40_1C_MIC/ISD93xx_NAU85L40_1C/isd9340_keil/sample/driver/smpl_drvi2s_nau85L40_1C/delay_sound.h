#ifndef __DELAY_SOUND_H__
#define __DELAY_SOUND_H__

typedef struct delay {
	int mic;
	float *read_pointer;
	float *write_pointer;
	float num_samples_to_delay;
	float fractional_delay;
	float delay_blend_param;
	float delay_tap_mix_volume;
	float feedback_volume;
	float thiran_coeff_1;
	float thiran_coeff_2;
	float thiran_coeff_3;
	float prev_in[3];
	float prev_out[3];
	float *buffer;
} delay;


extern delay * delay_init(float num_samples_to_delay, float fractional_delay, float fb_v, float dtm_v,float blend_param, int mic);
extern void delay_set_delay(delay *d, float num_samples_to_delay, float fractional_delay);
void delay_set_feedback_volume(delay *d, float fb_v);
void delay_set_delay_tap_mix_volume(delay *d, float dtm_v);
void delay_set_mic(delay *d, int mic);
void delay_set_blend_param(delay *d, float delay_blend_param);
extern float delay_out(delay *d, float in);
//extern void change_delay(float angle);


#endif
