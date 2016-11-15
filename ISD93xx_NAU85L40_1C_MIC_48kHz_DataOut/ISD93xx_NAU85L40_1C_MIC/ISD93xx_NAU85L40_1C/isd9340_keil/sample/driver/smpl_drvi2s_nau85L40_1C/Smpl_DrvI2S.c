/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2013 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "DrvUART.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
#include "DrvI2S.h"
#include "DrvI2C.h"
#include "DrvDPWM.h"
#include "DrvPDMA.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define NAU85L40_DEMO
#define BUFF_LEN    16

uint32_t PcmBuff[BUFF_LEN] = {0};

uint32_t u32BuffPos = 0;

uint32_t Fs = 48000;

static BOOL DataReady;

static uint16_t DataOut[4] = {0},DataOut_sel;

static uint8_t chsel = 1;

static BOOL DPWM_OVF = FALSE;





void  NAU85L40_EnterADCandDACMode(uint8_t deviceID);
/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define AUDIOBUFFERSIZE	8  //Play buffer length
__align(4) int16_t AudioBuffer[2][AUDIOBUFFERSIZE];	//Square wave buffer
uint16_t DPWMModFreqDiv[8] = {228, 156, 76, 52, 780, 524, 396, 268}; //Modulation Division
uint32_t DPWMSampleRateFreq[4] = {8000, 16000, 32000, 48000}; //Sample rate

/*---------------------------------------------------------------------------------------------------------*/
/* InitialDPWM                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void InitialDPWM(uint32_t u32SampleRate)
{  
	DrvDPWM_Open();	 // Enable DPWM clock
	DrvDPWM_SetDPWMClk(E_DRVDPWM_DPWMCLK_HCLK); //Speaker driver PWM clock source select
	DrvDPWM_SetSampleRate(u32SampleRate); //Set sample rate
	DrvDPWM_SetFrequency(eDRVDPWM_FREQ_0);//Set FREQ_0( 431158Hz  )
	DrvDPWM_Enable(); //Enable DPWM IP
}

/*---------------------------------------------------------------------------------------------------------*/
/* Set PDMA0 to move ADC FIFO to MIC buffer with wrapped-around mode                                       */
/*---------------------------------------------------------------------------------------------------------*/
void PDMA1forDPWM(void)
{
	STR_PDMA_T sPDMA;  

//	sPDMA.sSrcAddr.u32Addr 			= (uint32_t)&AudioBuffer[0][0]; 
	sPDMA.sSrcAddr.u32Addr 			= (uint32_t)&PcmBuff[0]; 
	sPDMA.sDestAddr.u32Addr 		= (uint32_t)&DPWM->FIFO;
	sPDMA.u8Mode 					= eDRVPDMA_MODE_MEM2APB;;
	sPDMA.u8TransWidth 				= eDRVPDMA_WIDTH_32BITS;
	sPDMA.sSrcAddr.eAddrDirection 	= eDRVPDMA_DIRECTION_WRAPAROUND;
	sPDMA.sDestAddr.eAddrDirection 	= eDRVPDMA_DIRECTION_FIXED;  
	sPDMA.u8WrapBcr				 	= eDRVPDMA_WRA_WRAP_HALF_INT; //Interrupt condition set fro Half buffer & buffer end
  sPDMA.i32ByteCnt = BUFF_LEN*4;	   	//Full buffer length (byte)
  DrvPDMA_Open(eDRVPDMA_CHANNEL_1, &sPDMA);

	DrvPDMA_SetCHForAPBDevice(eDRVPDMA_CHANNEL_1, eDRVPDMA_DPWM, eDRVPDMA_WRITE_APB); // PDMA Setting 
	DrvDPWM_EnablePDMA(); // Enable DPWM DMA
}


/*---------------------------------------------------------------------------------------------------------*/
/*  I2S Rx Threshold Level Callback Function when Rx FIFO is more than Rx FIFO Threshold Level             */
/*---------------------------------------------------------------------------------------------------------*/
void Rx_thresholdCallbackfn(uint32_t status)
{
	uint32_t u32Len, i;
	uint32_t *pBuff;
	uint16_t *pBuff16;

	
/*---------------------------------------*/		
	if (DataReady) DPWM_OVF = TRUE;
/*---------------------------------------*/			
	
	if (u32BuffPos < (BUFF_LEN))
	{
		//set the pointer point to the current position of PcmBuff
		pBuff = &PcmBuff[u32BuffPos];
		pBuff16 = (uint16_t*) pBuff;

		/* Read Rx FIFO Level */
		u32Len = _DRVI2S_READ_RX_FIFO_LEVEL();

		
		//copy data in RX_FIFO from the beginning to the current RX_FIFO address
		for ( i = 0; i < u32Len; i++ )
		{
			pBuff[i] = _DRVI2S_READ_RX_FIFO();
		}

/*---------------------------------------*/		
//	pBuff16 = (uint16_t*) &PcmBuff[u32BuffPos-u32Len];
	DataOut_sel = pBuff16[chsel];
	DataOut[0] = pBuff16[0];
	DataOut[1] = pBuff16[1];
	DataOut[2] = pBuff16[2];	
	DataOut[3] = pBuff16[3];	
	DataReady = TRUE;
/*---------------------------------------*/						
		
		
		u32BuffPos += u32Len;

		if (u32BuffPos >= BUFF_LEN)
		{
			u32BuffPos =	0;
		}
		
	}
}
void LdoOn(void)
{
	SYSCLK->APBCLK.ANA_EN=1;
	ANA->LDOPD.PD=0;
	ANA->LDOSET=3;
}

void UART_Callback(uint32_t status)
{
	int n;
	DrvUART_ClearInt(UART_PORT0,DRVUART_RDAINT);
	DrvUART_DisableInt(UART_PORT0,DRVUART_RDAINT);
	DrvDPWM_Close();
	DrvI2S_EnableRx(FALSE);
	printf("Program Finishes\n");
	for (n=0; n<BUFF_LEN; n++)
	{
		printf("PcmBuff[%d]\t:\t0x%08x \tDataOut =0x%04x \tDataOut0 =0x%04x \tDataOut1 =0x%04x \tDataOut2 =0x%04x \tDataOut3 =0x%04x \n",n,PcmBuff[n],DataOut_sel,DataOut[0],DataOut[1],DataOut[2],DataOut[3]);
	}
	
	while(!UART0->FSR.TE);
}


/*---------------------------------------------------------------------------------------------------------*/
/*  MAIN function																										  */
/*---------------------------------------------------------------------------------------------------------*/
int main (void)
{
  S_DRVI2S_DATA_T st;
	STR_UART_T param;

	/* Step 1. Enable and select clock source*/
	/* Unlock the protected registers */	
	UNLOCKREG();
	DrvSYS_SetOscCtrl(E_SYS_OSC49M, 1);
	DrvSYS_SetHCLK(E_DRVSYS_48M, 1);
	DrvSYS_SetIPClockSource(E_SYS_I2S_CLKSRC, 2);
	LOCKREG();

	LdoOn();

	/* Step 2. GPIO initial and select operation mode for UART*/
	/* Init UART for printf */
	DrvGPIO_UART_TXRX_PA8_PA9();
	
    param.u32BaudRate 	     = 115200;
    param.u8cDataBits        = DRVUART_DATABITS_8;
    param.u8cStopBits        = DRVUART_STOPBITS_1;
    param.u8cParity          = DRVUART_PARITY_NONE;
    param.u8cRxTriggerLevel  = DRVUART_FIFO_1BYTES;
    param.u8TimeOut        	 = 0;
    DrvUART_Open(UART_PORT0, &param);
		DrvUART_EnableInt(UART_PORT0,DRVUART_RDAINT,UART_Callback);

	/* Step 3. GPIO initial and select operation mode for I2C*/
	DrvGPIO_I2C_PA10_PA11();//PA11: SCL, PA10: SDA
	DrvI2C_Open(I2C_PORT0, (DrvSYS_GetHCLK() * 1000), 100000);  //clock = 48Kbps
	
	/* Step 6. Configure CODEC registers*/
	NAU85L40_EnterADCandDACMode(0x1C);


	/* Step 4. GPIO initial and select operation mode for I2S*/  
	DrvGPIO_I2S_PA4_PA5_PA6_PA7();
	DrvGPIO_I2S_MCLK_PB1();		//MCLK
	
	
/***************************DPWM Setup*******************************/
	DrvPDMA_Init();	//PDMA initialization
	InitialDPWM(Fs);	//Enable DPWM clock, Set DPWM clock source and sample rate 
	PDMA1forDPWM(); //Set PDMA parameter for DPWM
	//start PDMA transfer after done reading I2S_RX_FIFO
	DrvPDMA_CHEnablelTransfer(eDRVPDMA_CHANNEL_1); //start PDMA transfer
/********************************************************************/	
	

    st.u32SampleRate 	 = Fs;
		//st.u32SampleRate 	 = 48000;
    st.u8WordWidth 	 	 = DRVI2S_DATABIT_32;
		//st.u8WordWidth 	 	 = DRVI2S_DATABIT_32;
    st.u8AudioFormat 	 = DRVI2S_STEREO;
		//st.u8DataFormat  	 = DRVI2S_FORMAT_MSB;
		st.u8DataFormat  	 = DRVI2S_FORMAT_I2S;
    st.u8Mode 		 	 = DRVI2S_MODE_MASTER;
    st.u8RxFIFOThreshold = DRVI2S_FIFO_LEVEL_WORD_2;
    //st.u8TxFIFOThreshold = DRVI2S_FIFO_LEVEL_WORD_8-1;
		DrvI2S_Open(&st);

	/* Step 5. Set and enable MCLK*/ 
	//DrvI2S_SetMCLK(4096000);  //MCLK = 4096000 MHz
	DrvI2S_SetMCLK(Fs*256);  //MCLK = 4096000 MHz
	DrvI2S_EnableMCLK(1);	   //enable MCLK	  
	DrvI2S_SetBCLK(Fs*16*4); //16-bits * 4 channels * 48kHz




	printf("\n\n");
    printf("+----------------------------------------------------------------------+\n");
    printf("|                       I2S Driver Sample Code                         |\n");
    printf("+----------------------------------------------------------------------+\n");
    printf("\n");
	printf("Pin configuration: PA4=FS, PA5=BCLK, PA6=SDI, PA7=SDO, PB1=MCLK, PA10=SDA, PA11=SCL\n");
	printf("ISD93xx as I2S Master, NAU85L40 as I2S Slave                   	\n");
	printf("BCLK = %d Hz, MCLK = %d Hz \n", DrvI2S_GetBCLK(), DrvI2S_GetMCLK());
	printf("NAU85L40 LMIC/RMIC input and LHP/RHP output\n");
	printf("\n");

	/* Step 7. Enable Rx and interrupt*/
	//Enable Rx threshold level interrupt and install its callback function
	DrvI2S_EnableInt(I2S_RX_FIFO_THRESHOLD, Rx_thresholdCallbackfn);



	// Enable I2S Rx function to receive data
	DrvI2S_EnableRx(TRUE);
	
	while(1)
	{
		if (DataReady)
		{
			//DPWM->FIFO = DataOut_sel;
			/*Beamforming algorithm can be put here*/
			DataReady = FALSE;
		}
	}
}

