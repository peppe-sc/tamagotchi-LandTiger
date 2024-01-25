/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "string.h"
#include "stdio.h"
#include "../GLCD/GLCD.h"
#include "../RIT/RIT.h"
#include "../TouchPanel/TouchPanel.h"
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
volatile int type=0;
extern int selected;
volatile enum DogType{alive1,alive2,dead};
extern void clearGuscio(int x,int y);
extern void drawGuscio(int x,int y);
extern void clearT(int x,int y);
extern void drawbigT(int startx,int starty,int size);
extern void clearbigT(int startx,int starty,int size);
extern void drawC(int startx,int starty,int size);
extern void clearC(int startx,int starty,int size);
extern int volume_bar;
extern int ore;
extern int minuti;
extern int secondi;
extern int happiness;
extern int satiety;
extern int prolunga;
extern void drawT(int type,int startx,int starty);
volatile char tempo[10];
volatile int seq=0,not_dead=1,cuddle=0;
volatile int animation=0;


volatile uint16_t SinTable[45] =                                       /*                      */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

extern unsigned char led_value;					/* defined in funct_led								*/
void TIMER0_IRQHandler (void)
{
	/* Match register 0 interrupt service routine */
	int i=0,pos;
	//NVIC_SetPriority(TIMER0_IRQn, 4);
	if (LPC_TIM0->IR & 01)
	{	
		//disable_timer(0);
		
		secondi++;										//AUMENTA IL TEMPO DI VITA
		if(secondi>60){
			secondi=0;
			minuti++;
			GUI_Text(90,30,"            ",Black,White);
			if (minuti>60){
				minuti=0;
				ore++;
				GUI_Text(90,30,"            ",Black,White);
			}
		}
		
		
		sprintf((char *)tempo,"%d:%d:%d",ore,minuti,secondi); 	//STAMPA IL TEMPO
		//GUI_Text(90,10,"Age",Black,White);
		GUI_Text(90,30,tempo,Black,White);
		
		//finished=0;
		if(type==alive1 && !cuddle && !animation){							//GESTISCE LE ANIMAZIONI
			drawT(alive2,80,200);
			type=alive2;
		}else if(type==alive2 && !cuddle && !animation){
			drawT(alive1,80,200);
			type=alive1;
		}
		//finished=1;
		
		
		if(secondi%5==0){							//OGNI 5 SECONDI PERDE FELICITA' E SAZIETA'
																	
			satiety--;
		happiness--;
		
		
		GUI_Text(0,30,"        ",Black,White);		//AGGIORNA GLI INDICATORI
		pos=0;
		for (i=0;i<happiness;i++){
			GUI_Text(pos,30,"O",Red,White);
			pos+=10;
		}
		pos=150;
		GUI_Text(150,30,"        ",Black,White);
		for (i=0;i<satiety;i++){
			GUI_Text(pos,30,"O",Blue2,White);
			pos+=10;
		}
		
		if(happiness==0 || satiety==0){				//FINE DEL GIOCO
			reset_timer(0);
			disable_timer(0);
			drawRect(0,271,240,49,White);
			GUI_Text(100,290,"RESET",Red,White);
			
			not_dead=0;
			disable_RIT();
			reset_RIT();
			reset_timer(1);											//UTILIZZO IL TIMER1 PER CREARE L'ANIMAZIONE DI 1 SECONDO
			enable_timer(1);
			//drawDog(dead);
		}
		
		}
		//enable_timer(0);
		LPC_TIM0->IR = 1;			/* clear interrupt flag */
	}
		/* Match register 1 interrupt service routine */
	else if(LPC_TIM0->IR & 02)
  {
		
	  

		LPC_TIM0->IR =  2 ;			/* clear interrupt flag */	
	}
	/* Match register 2 interrupt service routine */
	else if(LPC_TIM0->IR & 4)
  {
		//led_value ^= 1;
		
		
		
		
		LPC_TIM0->IR =  4 ;			/* clear interrupt flag */	
	}
		/* Match register 3 interrupt service routine */
	else if(LPC_TIM0->IR & 8)
  {
	 
		LPC_TIM0->IR =  8 ;			/* clear interrupt flag */	
	}
	
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	volatile int pos,i;
	/* Match register 0 interrupt service routine */
	if (LPC_TIM1->IR & 01)
	{	
		seq++;
		
		if(cuddle && seq==1){
			init_timer(3,0,0,3,2120);
			reset_timer(3);
			enable_timer(3);
			drawbigT(70,180,5);
			drawC(150,180,3);
			LPC_TIM1->IR = 1;
			return;
		}else if(seq==2 && cuddle){
			disable_timer(3);
			init_timer(3,0,0,3,1890);
			reset_timer(3);
			enable_timer(3);
			drawbigT(60,165,6);
			drawC(180,160,3);
			LPC_TIM1->IR = 1;
			return;
		}else if(seq==3 && cuddle){
			disable_timer(3);
			init_timer(3,0,0,3,1062);
			reset_timer(3);
			enable_timer(3);
			drawbigT(50,150,7);
			drawC(200,130,3);
			LPC_TIM1->IR = 1;
			return;
		}else if(seq==4 && cuddle){
			disable_timer(3);
			reset_timer(3);
			clearbigT(50,150,7);
			clearC(150,180,3);
			clearC(180,160,3);
			clearC(200,130,3);
			pos=0;
			for (i=0;i<happiness;i++){
			GUI_Text(pos,30,"O",Red,White);
			pos+=10;
			}
			drawT(alive1,80,200);
			cuddle=0;
			
			disable_timer(3);
			reset_timer(3);
			
			reset_timer(2);
			enable_timer(2);
			
			disable_timer(1);
			reset_timer(1);
			enable_RIT();
			reset_RIT();
			seq=0;
			
			
			
			LPC_TIM1->IR = 1;
			return;
		}
		
		
		
		if(!not_dead && seq==1){				//SE IL GIOCO È FINITO UTILIZZO LA VARIABILE SEQ PER CREARE UN ANIMAZIONE DI DUE FRAME
			disable_RIT();
			disable_timer(2);
			init_timer(3,0,0,3,2160);
			reset_timer(3);
			enable_timer(3);
			clearT(80,200);
			drawT(alive1,80,150);
			
			LPC_TIM1->IR = 1;	
			return;
		}else if(!not_dead && seq==2){
			clearT(80,150);
			drawGuscio(80,170);
			disable_timer(3);
			init_timer(3,0,0,3,1062);
			reset_timer(3);
			enable_timer(3);
			LPC_TIM1->IR = 1;	
			return;
			
		}else if(!not_dead && seq==3){
			
			clearGuscio(80,170);
			drawGuscio(80,215);
			GUI_Text(30,150,"Sei un.. hai fallito",Magenta,White);
			LCD_DrawLine(30,160,80,160,Black);
			enable_RIT();
			
			disable_timer(1);
			reset_timer(1);
			seq=0;
			disable_timer(3);
			reset_RIT();
			enable_timer(2);
			LPC_TIM1->IR = 1;
			return;
		}
		
		if(seq==1){													//ANIMAZIONE PER MANGIARE IL CIBO O SNACK
				animation=1;
				disable_RIT();
				disable_timer(2);
				init_timer(3,0,0,3,1417);
				reset_timer(3);
				enable_timer(3);
				clearT(80,200);
				drawT(alive1,80,150);
		}else if(seq==2){
				disable_timer(3);
				init_timer(3,0,0,3,1684);
				reset_timer(3);
				enable_timer(3);
				
				clearT(80,150);
				drawT(alive1,80,200);
				
				
				if(selected==0){				//INCREMENTO SAZIETA' O FELICITA'
					satiety++;
					if(satiety>5){
						satiety=5;
					}
					pos=150;
					GUI_Text(150,30,"        ",Black,White);
					for (i=0;i<satiety;i++){
						GUI_Text(pos,30,"O",Blue2,White);
						pos+=10;
					}
				}else if(selected==1){
					happiness++;
					if(happiness>5){
						happiness=5;
					}
					GUI_Text(0,30,"        ",Black,White);
					pos=0;
					for (i=0;i<happiness;i++){
						GUI_Text(pos,30,"O",Red,White);
						pos+=10;
					}
				}
				
		}else if(seq==3){
				disable_timer(1);
				reset_timer(1);
				seq=0;
				drawT(alive2,80,200);
				reset_RIT();
				enable_RIT();
				reset_timer(2);
				disable_timer(3);
				enable_timer(2);
			animation=0;
		}
		LPC_TIM1->IR = 1;			/* clear interrupt flag */
	}
		/* Match register 1 interrupt service routine */
	else if(LPC_TIM1->IR & 02)
  {
		
	  /*if(LPC_TIM0->MR1 ==0x0bebc20)
		{  
			 LPC_TIM0->MR1 = 0x017D7840;
		}
		else
		{ 
			 LPC_TIM0->MR1 = 0x0bebc20;
		}*/

		LPC_TIM1->IR =  2 ;			/* clear interrupt flag */	
	}
	/* Match register 2 interrupt service routine */
	else if(LPC_TIM1->IR & 4)
  {
		//led_value ^= 1;
		
		
		
		
		LPC_TIM1->IR =  4 ;			/* clear interrupt flag */	
	}
		/* Match register 3 interrupt service routine */
	else if(LPC_TIM1->IR & 8)
  {
	 
		LPC_TIM1->IR =  8 ;			/* clear interrupt flag */	
	}
	
  
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void)
{
  if (LPC_TIM2->IR & 01){
		if(getDisplayPoint(&display, Read_Ads7846(), &matrix )){
			if(display.x>80 && display.y>200 && display.x<(80+56) && display.y<200+68 && not_dead){
				cuddle=1;
				if(happiness<5){
					happiness++;
				}
				
				disable_RIT();
				disable_timer(2);
				enable_timer(1);
				
			}
		}
	}
	
	
	LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}


void TIMER3_IRQHandler (void)
{
  if (LPC_TIM3->IR & 01){
		static int ticks=0;
		
		/* DAC management */	
		LPC_DAC->DACR = (SinTable[ticks]<<6)*volume_bar;
		ticks++;
		if(ticks==45){ 
			ticks=0;
			//disable_timer(3);
		}
	}
	
	
	LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
