/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../RIT/RIT.h"
#include "../GLCD/GLCD.h"
//#include "../led/led.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */

volatile int volume_bar=10;

void ADC_IRQHandler(void) {
  int i;
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
  if(AD_current != AD_last){//n:fff=x:5
		volume_bar=	AD_current*10/0xFFF;
		for(i=20;i<10*3+20;i+=3){
		drawRect(i,60,3,3,White);
	}
	for(i=20;i<volume_bar*3+20;i+=3){
		drawRect(i,60,3,3,Black);
	}
		AD_last = AD_current;
  }	
	reset_RIT();
}
