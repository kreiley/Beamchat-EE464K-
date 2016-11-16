#ifndef __BEAMFORMING_H__
#define __BEAMFORMING_H__

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

extern void Beamforming_Initialization(void);
extern uint32_t * run(uint32_t * in0, uint32_t * in1, uint32_t * in2, uint32_t * in3);
void delays_init();
void localization_init();
void change_delay();
float locate(float * buffer1, float * buffer2, float * buffer3, int buf_size);
void sin_test();
void print_delay_info();
float * wav_file_tests();


#endif
