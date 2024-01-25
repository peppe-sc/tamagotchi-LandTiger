/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include "../adc/adc.h"
/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down=0;
extern char led_value;
extern int not_dead;
extern int happiness;
extern int satiety;
extern int secondi;
extern int minuti;

extern int ore;
extern int volume_bar;
volatile int prolunga=0;
volatile int selected=0;
extern void drawFood(int type);
extern void drawVolumeIcon(int startx,int starty,int size);
volatile const int freqs[8]={2120,1890,1684,1592,1417,1263,1125,1062};
extern void drawT(int type,int startx,int starty);
volatile enum DogType{alive1,alive2,dead};
volatile int timerset=0;
void RIT_IRQHandler (void)
{					
	static int select=0;
	static int J_down = 0,J_left=0,J_right=0;
	static int position=0;	
	volatile int i;
	
	ADC_start_conversion();
	
	
	
	if(timerset>0){
		timerset++;
	}
	
	/*for(i=20;i<10*3+20;i+=3){
		drawRect(i,60,3,3,White);
	}
	for(i=20;i<volume_bar*3+20;i+=3){
		drawRect(i,60,3,3,Black);
	}*/
	
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		/* Joytick Select pressed p1.25*/
		/* Joytick Down pressed p1.26 --> using J_DOWN due to emulator issues*/
		
		J_down++;
		switch(J_down){
			case 1:
				reset_RIT();
				break;
			default:
				break;
		}
	}
	else{
			J_down=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0 && not_dead){	
		/* Joytick Select pressed p1.25*/
		/*Joytick Left pressed p1.27*/
		/* Joytick Down pressed p1.26 --> using J_DOWN due to emulator issues*/
		
		init_timer(3,0,0,3,2120);
		reset_timer(3);
		enable_timer(3);
		disable_timer(2);
		timerset=1;
		J_left++;
		switch(J_left){
			case 1:
				GUI_Text(40,290,"Meal",Red,White);				//PREMENDO A SINISTRA ATTIVO MEAL
				GUI_Text(160,290,"Snack",Black,White);
				selected=0;
				reset_RIT();
				break;
			default:
				break;
		}
		
	}
	else{
			J_left=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0 && not_dead){	
		/* Joytick Select pressed p1.25*/
		/*Joytick Left pressed p1.27*/
		/*Joytick Right pressed p1.28*/
		/* Joytick Down pressed p1.26 --> using J_DOWN due to emulator issues*/
		
		init_timer(3,0,0,3,2120);
		reset_timer(3);
		enable_timer(3);
		disable_timer(2);
		timerset=1;
		J_right++;
		switch(J_right){
			case 1:
				GUI_Text(40,290,"Meal",Black,White);
				GUI_Text(160,290,"Snack",Red,White);			//PREMENDO A DESTRA ATTIVO SNACK
				selected=1;
				reset_RIT();
				break;
			default:
				break;
		}
		
	}
	else{
			J_right=0;
	}
	
	
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
		/* Joytick Select pressed p1.25*/
		/* Joytick Down pressed p1.26 --> using J_DOWN due to emulator issues*/
		
		volatile int pos,i;
		
		//init_timer(3,0,0,3,1263);
		//reset_timer(3);
		//enable_timer(3);
		
		
		
		if(!not_dead){											//SE IL GIOCO È FINITO PREMENDO SELECT RESETTO
			LCD_Clear(White);
			satiety=5;
			happiness=5;
			selected=0;
			not_dead=1;
			drawT(alive1,80,200);
			pos=0;
			drawVolumeIcon(5,50,2);
			GUI_Text(0,10,"Happiness:",Black,White);
			for (i=0;i<happiness;i++){
				GUI_Text(pos,30,"O",Red,White);
				pos+=10;
			}
			GUI_Text(150,10,"Satiety:",Black,White);
			pos=150;
			for (i=0;i<satiety;i++){
				GUI_Text(pos,30,"O",Blue2,White);
				pos+=10;
			}
			minuti=0;
			secondi=0;
			ore=0;
			LCD_DrawLine(0,270,240,270,Black);
			LCD_DrawLine(120,270,120,320,Black);
			GUI_Text(40,290,"Meal",Red,White);
			GUI_Text(160,290,"Snack",Black,White);
			GUI_Text(90,10,"Age",Black,White);
			reset_timer(0);
			enable_timer(0);
			enable_RIT();
			reset_RIT();
			LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
			return;
		}
		
		select++;
		switch(select){							//SE IL GIOCO NON È FINITO FACCIO PARTIRE L'ANIMAZIONE PER MANGIARE
			case 1:
				
				
				disable_RIT();
				drawFood(selected);
				
				reset_timer(1);
				enable_timer(1);								//UTILIZZO IL TIMER PER CREARE L'ANIMAZIONE
				
				
				
				reset_RIT();
				break;
			default:
				break;
		}
	}
	else{
			select=0;
	}
	
	/* button management */
	
	//NVIC_SetPriority(TIMER0_IRQn, 0);
	if(timerset>=2){
		disable_timer(3);
		enable_timer(2);
		timerset=0;
	}
	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
