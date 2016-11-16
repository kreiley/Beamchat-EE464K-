#include "delay_sound.h"
#include "math.h"
#include "FixedBeamformer.h"
#include <stdlib.h>
#include "Beamforming.h"

#define BUFFER_SIZE 100
#define ORDER 3

static delay d[8] = {0};

void delay_init(float num_samples_to_delay, float fractional_delay, int mic){
	if(fractional_delay > 0.5){
		num_samples_to_delay +=1;
		fractional_delay-=1;
	}
	fractional_delay += ORDER;
	float tc = fractional_delay;
	
	d[mic].thiran_coeff_1 = -3 * (tc -3)/(tc+1);
	d[mic].thiran_coeff_2 = 3 * (tc-2) * (tc - 3)/((tc + 1) * (tc+2));
	d[mic].thiran_coeff_3 = -1 * (tc -1) * (tc -2) * (tc - 3)/((tc + 1) * (tc + 2) * (tc + 3));
	
	for(int i = 0; i < ORDER; i++){
		d[mic].prev_in[i] = 0;
		d[mic].prev_out[i] = 0;
	}	

	d[mic].num_samples_to_delay = num_samples_to_delay;
	d[mic].fractional_delay = fractional_delay;
	d[mic].write_pointer = BUFFER_SIZE -1;
}


float delay_out(int mic, float in) {
	float out;
	float x;

	d[mic].buffer[d[mic].write_pointer] = in;
		
	/*apply integer part of delay*/
	d[mic].read_pointer = d[mic].write_pointer - (int)d[mic].num_samples_to_delay;

	/*loop buffer*/
	if(d[mic].read_pointer < 0){
		d[mic].read_pointer = d[mic].read_pointer + BUFFER_SIZE - 1;
	}

	x = d[mic].buffer[d[mic].read_pointer];
	out =	  (d[mic].prev_in[0]                   )
		+ (d[mic].prev_in[1]  * d[mic].thiran_coeff_1)
		+ (d[mic].prev_in[2]  * d[mic].thiran_coeff_2)
		+ (x              * d[mic].thiran_coeff_3)
		- (d[mic].prev_out[2] * d[mic].thiran_coeff_1)
		- (d[mic].prev_out[1] * d[mic].thiran_coeff_2)
		- (d[mic].prev_out[0] * d[mic].thiran_coeff_3);
	
	d[mic].prev_in[0]  = d[mic].prev_in[1];
	d[mic].prev_in[1]  = d[mic].prev_in[2];
	d[mic].prev_in[2]  = x;
	d[mic].prev_out[0] = d[mic].prev_out[1];
	d[mic].prev_out[1] = d[mic].prev_out[2];
	d[mic].prev_out[2] = out;

		
	d[mic].write_pointer = d[mic].write_pointer + 1;

	if(d[mic].write_pointer > BUFFER_SIZE - 1) {
		d[mic].write_pointer = 0;
	}
	return out;
}
