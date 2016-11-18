#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "delay_sound.h"
#include "FixedBeamformer.h"
#include "localization.h"
#include "Beamforming.h"

#define ANGLE_RESOLUTION 500    // Number of angle points to calculate
#define number_of_mics 8
#define feedback_volume .4
#define mix_volume .35
#define blend_parameter .4
#define buffer_size 100
#define BUFFER_SIZE 64000

delay * delays[8];
float freq = 16000; //frequency of data input
float speed_of_sound = 340.3; //speed of sound at sea level
int filterLength = 11; //Numer of FIR filter taps for fractional delay(shouled be odd)
float angle = 0;
float volume = .75;
bool turn_off = false;
char mode = 'M';
microphone * m1;
microphone * m2;
microphone * m3;
float angles[360] = {0};

void Beamforming_Initialization(void){
	angle = 0;
	mic_array_init();
	delays_init();
	localization_init();
}


int main(int argc, char* argv[]){
	if(argc == 2){angle = atof(argv[1]);}
	if(argc == 3){angle = atof(argv[1]); volume = atof(argv[2]);}
	if(argc == 4){angle = atof(argv[1]); volume = atof(argv[2]); mode = argv[3][0];}
/*	if(argc == 12){
		angle = atof(argv[1]); 
		volume = atof(argv[2]); 
		mode = argv[3][0];
		FILE * in0 = fopen(argv[4], "rb");
		FILE * in1 = fopen(argv[5], "rb");
		FILE * in2 = fopen(argv[6], "rb");
		FILE * in3 = fopen(argv[7], "rb");
		FILE * in4 = fopen(argv[7], "rb");
		FILE * in5 = fopen(argv[7], "rb");
		FILE * in6 = fopen(argv[7], "rb");
		FILE * in7 = fopen(argv[7], "rb");
	}
*/
	mic_array_init();
	delays_init();
	print_delay_info();
	localization_init();
	sin_test();
	localization_test();
	/*wav_file_tests();*/
	//run(in);
}

uint16_t run(float in0, float in1, float in2, float in3){
	float output = 0;
	float sum;
	sum = delay_out(0, in0);	
	sum+= delay_out(1, in1);	
	sum+= delay_out(2, in2);	
	sum+= delay_out(3, in3);
	return sum/4;
}
/*
float * wav_file_tests(){
	
	FILE * outfile = fopen("output.wav", "wb");
	int BUFSIZE = 512;
	int count = 0;
	short int buff16[BUFSIZE];
	header_p meta = (header_p)malloc(sizeof(header));
	int nb
	if(in0 && in1 && in2 && in3 && in4 && in5 && in6 && in7){
		fread(meta, 1, sizeof(header), in1);
		fwrite(meta, 1, sizeof(*meta), outfile);
		
	}
}
*/
void delays_init(){
	calculate_delay(angle);
	for(int i =0; i < number_of_mics; i++){
		float num_of_samples_to_delay = (freq * get_current_delay(i))/speed_of_sound;
		float fractional = fmod(num_of_samples_to_delay,1);	
		delay_init(num_of_samples_to_delay,fractional, i);
	}
}

void localization_init(){
	m1 = build_mic(get_x(4), get_y(4));
	m2 = build_mic(get_x(6), get_y(6));
	m3 = build_mic(get_x(7), get_y(7));
	init_triangle(m1,m2,m3);
	set_buffer_size(buffer_size);

}

void change_delay(){
	calculate_delay(angle);
	for(int i = 0; i < number_of_mics; i++){
		float num_of_samples_to_delay = (freq * get_current_delay(i))/speed_of_sound;
		float fractional = fmod(num_of_samples_to_delay, 1);
		delay_init(num_of_samples_to_delay, fractional, i);
	}
}

float locate(float * buffer1, float * buffer2, float * buffer3, int buf_size){
	set_buffer(m1, buffer1);
	set_buffer(m2, buffer2);
	set_buffer(m3, buffer3);
	set_buffer_size(buf_size);
	return find_source(m1,m2,m3);
}

void print_delay_info(){
	for(int i =0; i<number_of_mics; i++){
		float time_of_delay = get_current_delay(i)/speed_of_sound;
		float num_of_samples_to_delay = (freq * get_current_delay(i))/speed_of_sound;
		printf("\n\nMicrophone %d:\n",i);
		printf("Distance of mic in meters from furthest mic from Audio Source:%f\n",get_current_delay(i));
		printf("Time of delay in seconds: %f\n", time_of_delay);
		printf("Number of Samples to delay: %f \n\n",num_of_samples_to_delay);
	}
}

void sin_test(){
	float * y_mic[number_of_mics];
	float * y = malloc(8*180*sizeof(float));
	for (int g = 0; g < number_of_mics; g++){
		float *y_delayed = malloc(8*180*sizeof(float));
		int x = 0;
		for(float i = 0; i*M_PI/180 < 4*M_PI; i = i + 4){
			y[x] = sin(i*M_PI/180);
			y_delayed[x] = delay_out(g, y[x]);
			x+=1;

		}
		y_mic[g] = y_delayed;

	}
	angles[(int) angle] = locate(y_mic[4],y_mic[6], y_mic[7],buffer_size);
	/*
	printf("\n---------INPUT-------\n\n");
	for(int i = 0; i < 180; i++){
		printf("%f\n",y[i]);
	}
	for(int mic = 0; mic < number_of_mics; mic++){
		printf("\n---------MICROPHONE %d----------\n\n",mic);	
		for(int i = 0; i < 180; i++){
			printf("%f\n",y_mic[mic][i]);
		}
	}
	*/
}

void localization_test(){
	for (int i = 0; i < 360; i++){
		angle = i;
		change_delay();
		sin_test();
	}
	printf("\n------------ANGLE RESOLUTIONS------\n");
	for (int i = 0; i < 360; i++){
		printf("%f\n",angles[i]);	
	}
}
