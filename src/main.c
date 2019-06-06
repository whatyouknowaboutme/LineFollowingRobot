//********************************************************************
//*                    EEE2046F C template                           *
//*==================================================================*
//* WRITTEN BY:    	                 		             *
//* DATE CREATED:                                                    *
//* MODIFIED:                                                        *
//*==================================================================*
//* PROGRAMMED IN: Eclipse Luna Service Release 1 (4.4.1)            *
//* DEV. BOARD:    UCT STM32 Development Board                       *
//* TARGET:	   STMicroelectronics STM32F051C6                    *
//*==================================================================*
//* DESCRIPTION:                                                     *
//*                                                                  *
//********************************************************************
// INCLUDE FILES
//====================================================================
#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
//====================================================================
// SYMBOLIC CONSTANTS
//====================================================================
#define SW0 GPIO_IDR_0
//#define SW1 GPIO_IDR_7

#define LN1 GPIO_IDR_1
#define LN2 GPIO_IDR_2
#define LN3 GPIO_IDR_3
#define LN4 GPIO_IDR_4

#define MIN_WIDTH 544
#define MAX_WIDTH 2400
#define NEUTRAL_PULSE_WIDTH 1500
#define REFRESH_INTERVAL 20000
#define CLOCKFREQ 48
#define MICRO 0.000001
#define detect1 ((GPIOA->IDR & LN1)!=0)
#define detect2 ((GPIOA->IDR & LN2)!=0)
#define detect3 ((GPIOA->IDR & LN3)!=0)
#define detect4 ((GPIOA->IDR & LN4)!=0)
#define SW0_pressed ((GPIOA->IDR & SW0)==0)
//====================================================================
// GLOBAL VARIABLES
//====================================================================
 uint16_t duty_value=3000;

 unsigned char detected1 =0b0;
 unsigned char detected2 = 0b0;
 unsigned char detected3 = 0b0;
 unsigned char detected4 = 0b0;
 int degrees = 90;
 int degrees_prev=90;
 uint16_t calibrated_distance;
 uint8_t start_stop=0;
 uint8_t L_Flag=0;
 uint8_t R_Flag=0;
//====================================================================
// FUNCTION DECLARATIONS
//====================================================================
 void init_Port(void);
 void change_pos (int degree);


 void init_Servo(void);
 void init_switch(void);
 void init_ADC(void);

 /*shit to remember:
  * Specifify the ODR for motor output
  *
  */

//====================================================================
// MAIN FUNCTION
//====================================================================
void main (void)
{


	//init_switch();
	init_Port();

	/*
	while((GPIOA->IDR & SW1)!=0);
	calibrated_distance = ADC1->DR;
	*/

	while ((GPIOA->IDR & SW0)!=0);
	start_stop=1;

	//must set the PWM of the motor at this point aswell (as in assign it a value) after initialization of it of course
	init_Servo();
	change_pos(degrees);

	//init_Timer();
	//GPIOB->BSRR |= (GPIO_BSRR_BS_0);




	//init_ADC();
//	uint ADC_value = ADC1->DR;



	for(;;)// Loop forever
	{
		 if(((GPIOA->IDR & LN1)!=0))
		 {
			 //int degree =100;
			 TIM2->ARR=51779;
			 TIM2->CCR3 =1575;
			 //change_pos(degree);
		 }
			/*if ((detect1&&detect2)||(detect3&&detect4)||(detect2&&detect3)||(detect1&&detect2&&detect3&&detect4))
			{
				degrees=90;
				detected1 = 0, detected2=0, detected3 = 0, detected4=0;
			}
			else
			{//after checking line crossing
				if (detect2)
				{//begin detected 2
					if (detected2==0)
					{//if it hasnt triggered
						if (detected3==1||detected1==1)
						{
							degrees=90;
							detected3=0;
							detected4=0;
							detected1=0;

						}
						else
						{
						detected2=1;
						degrees=100;
						}
					}
					else
					{//if it was triggered and the circle radius isnt big enough
						detected2=1;
						degrees =110;
					}
				}//end detected 2
				else
				{//after checking line sensor 2
					if(detect3)
					{//begin detected 3
						if(detected3==0)
						{//if it hasnt triggered
							if (detected2==1||detected4==1)
							{
								degrees=90;
								detected1=0, detected2=0, detected4=0;
							}
							else
							{
								detected3 =1;
								degrees=80;
							}

						}
						else
						{
							detected3= 1;
							degrees= 70;
						}
					}//end detected 3
					else
					{
						if (detect1)
						{//begin detected1

							if (detected1==0)
							{
								detected1 = 1;
								degrees=120;
							}
							else
							{
								//don't know if i need this
						}
						}//end detected1

						else
						{
							if (detect4)
							{//begin detected 4
								if (detected4==0)
								{
									detected4 = 1;
									degrees=60;
								}
								else
								{
									//don't know if i will need this
								}
							}//end detected4
						}
					}
				}
			}
			}
			if(degrees_prev!=degrees)
			{
				degrees_prev=degrees;
				change_pos(degrees);
			}
*/
		//}
		//else
		//{
		//set enable motors to be off
		//}
	}
}							// End of main

//====================================================================
// FUNCTION DEFINITIONS
//====================================================================
void init_switch(void)
{
	RCC-> AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA-> MODER &= ~(GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER2|GPIO_MODER_MODER3);
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR0_0|GPIO_PUPDR_PUPDR1_0|GPIO_PUPDR_PUPDR2_0|GPIO_PUPDR_PUPDR3_0);

	//Testing
	RCC-> AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB-> MODER |= (GPIO_MODER_MODER0_0|GPIO_MODER_MODER0_0);
	//end testing
}

void init_Port(void)
{
	/*
	 * PA0 - Start Switch
	 * PA1 - Line Sensor 1
	 * PA2 - Line Sensor 2
	 * PA3 - Line Sensor 3
	 * PA4 - Line Sensor 4
	 * PA5 - Proximity Sensor
	 * PA7 - Calibration sensor for Proximity sensor
	 * PB0 - IN pin IN3
	 * PB1 - IN pin IN4
	 */

	RCC-> AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA-> MODER &= ~(GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER2|GPIO_MODER_MODER3|GPIO_MODER_MODER4);
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR0_0|GPIO_PUPDR_PUPDR1_1|GPIO_PUPDR_PUPDR2_1|GPIO_PUPDR_PUPDR3_1|GPIO_PUPDR_PUPDR4_1);

	RCC-> AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB-> MODER |= (GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0);
	GPIOB->BSRR |= (GPIO_BSRR_BR_0|GPIO_BSRR_BR_1);
	//analog configuration
	GPIOA->MODER |= GPIO_MODER_MODER5;

}

void init_ADC(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_ADCEN; //enable clock for ADC
	ADC1->CR &= ~ADC_CR_ADSTART;
	ADC1->CFGR1 |= ADC_CFGR1_RES_0;
	ADC1->CFGR1 &= ~(ADC_CFGR1_ALIGN);
	ADC1->CFGR1 |= ADC_CFGR1_CONT; //may want to go back to single shot??
	//ADC1->CFGR1 &= ~(ADC_CFGR1_SCANDIR)
	ADC1->CHSELR |= ADC_CHSELR_CHSEL5;

	//ADC1->CR |= ADC_CR_ADCAL;  // set ADCAL high and wait for it to go low
	//while( (ADC1->CR & ADC_CR_ADCAL) != 0);
	ADC1->CR |= ADC_CR_ADEN;  // set ADEN=1 in the ADC_CR register
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0); //wait until ADRDY==1 in ADC_ISR
	ADC1 -> CR |= ADC_CR_ADSTART;
	while((ADC1->ISR & ADC_ISR_EOC) == 0);
}




void init_Servo(void)
{
	RCC-> AHBENR |= RCC_AHBENR_GPIOBEN;
	 RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	 GPIOB->MODER |= GPIO_MODER_MODER10_1; // PB10 = AF
	 GPIOB->AFR[1] |= (2 << (4*(10 - 8))); // PB10_AF = AF2 (ie: map to TIM2_CH3)
	 TIM2->ARR =51599;
	 TIM2->PSC = 20;
	 TIM2->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1); // PWM Mode 1
	 TIM2->CCR3 = 3680; // Red = 20%
	 TIM2->CCER |= TIM_CCER_CC3E;
	 TIM2->CR1 |= TIM_CR1_CEN;


}



void change_pos (int degree)
{
	double t_length =(double) ((MAX_WIDTH-MIN_WIDTH)/180.0)*degree+MIN_WIDTH;
	int ARR_val = (int) (t_length+REFRESH_INTERVAL)*CLOCKFREQ/20-1;
	int CRR_val = ((double) (t_length)/(t_length+REFRESH_INTERVAL))*ARR_val;

	TIM2->ARR=ARR_val;
	TIM2->CCR3=CRR_val;
}

