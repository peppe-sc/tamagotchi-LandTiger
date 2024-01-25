/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: 
 *		to control led11 and led 10 through EINT buttons (similarly to project 03_)
 *		to control leds9 to led4 by the timer handler (1 second - circular cycling)
 * Note(s): this version supports the LANDTIGER Emulator
 * Author: 	Paolo BERNARDI - PoliTO - last modified 15/12/2020
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/

                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "GLCD/GLCD.h"
#include "GLCD/AsciiLib.h"
#include "GLCD/HzLib.h"
#include "TouchPanel/TouchPanel.h"
#include "joystick/joystick.h"
#include "adc/adc.h"

volatile uint16_t tartaruga1[17][14]={{White,White,White,Black,Black,Black,Black,Black,Black,Black,Black,White,White,White},
{White,White,Black,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,Black,White,White},
{White,Black,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,Black,White},
{White,Black,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,Black,White},
{White,Black,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,Black,White},
{White,Black,LightGreen,Black,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,Black,LightGreen,Black,White},
{White,Black,LightGreen,LightGreen,LightGreen,Black,Black,Black,Black,LightGreen,LightGreen,LightGreen,Black,White},
{White,White,Black,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,Black,White,White},
{White,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,White,White},
{Black,LightGreen,LightGreen,Black,Cachi,Cachi,Cachi,Black,LightGreen,LightGreen,Black,Green,Black,White},
{Black,LightGreen,LightGreen,Black,Cachi,Cachi,Cachi,Black,LightGreen,LightGreen,Black,LightGreen,Green,Black},
{White,Black,Black,Black,Cachi,Cachi,Cachi,Cachi,Black,Black,Black,Green,LightGreen,Black},
{White,White,White,Black,Cachi,Cachi,Cachi,Cachi,Cachi,Cachi,Black,LightGreen,Green,Black},
{White,White,White,Black,Black,Cachi,Cachi,Cachi,Cachi,Black,Black,Green,Black,White},
{White,White,White,Black,LightGreen,Black,Black,Black,Black,LightGreen,Black,Black,White,White},
{White,White,White,Black,LightGreen,LightGreen,Black,Black,LightGreen,LightGreen,Black,White,White,White},
{White,White,White,White,Black,Black,White,White,Black,Black,White,White,White,White}};

volatile uint16_t tartaruga2[17][14]={{White,White,White,Black,Black,Black,Black,Black,Black,Black,Black,White,White,White},
{White,White,Black,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,Black,White,White},
{White,Black,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,Black,White},
{White,Black,LightGreen,Black,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,Black,LightGreen,Black,White},
{White,Black,LightGreen,Black,Black,Black,Black,Black,Black,Black,Black,LightGreen,Black,White},
{White,Black,LightGreen,LightGreen,Black,White,White,White,White,Black,LightGreen,LightGreen,Black,White},
{White,Black,LightGreen,LightGreen,LightGreen,Black,Black,Black,Black,LightGreen,LightGreen,LightGreen,Black,White},
{White,White,Black,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,LightGreen,Black,White,White},
{White,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,White,White},
{Black,LightGreen,LightGreen,Black,Cachi,Cachi,Cachi,Black,LightGreen,LightGreen,Black,Green,Black,White},
{Black,LightGreen,LightGreen,Black,Cachi,Cachi,Cachi,Black,LightGreen,LightGreen,Black,LightGreen,Green,Black},
{White,Black,Black,Black,Cachi,Cachi,Cachi,Cachi,Black,Black,Black,Green,LightGreen,Black},
{White,White,White,Black,Cachi,Cachi,Cachi,Cachi,Cachi,Cachi,Black,LightGreen,Green,Black},
{White,White,White,Black,Black,Cachi,Cachi,Cachi,Cachi,Black,Black,Green,Black,White},
{White,White,White,Black,LightGreen,Black,Black,Black,Black,LightGreen,Black,Black,White,White},
{White,White,White,Black,LightGreen,LightGreen,Black,Black,LightGreen,LightGreen,Black,White,White,White},
{White,White,White,White,Black,Black,White,White,Black,Black,White,White,White,White}};


volatile uint16_t meal[15][13]={{White,White,White,White,White,Black,Black,Black,White,White,White,White,White},
{White,White,White,Black,Black,Orange,Orange,Orange,Black,Black,White,White,White},
{White,White,Black,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Black,White,White},
{White,Black,Orange,Orange,White,White,Orange,Orange,Orange,Orange,Orange,Black,White},
{White,Black,Orange,White,White,Orange,Orange,Orange,Orange,Orange,Orange,Black,White},
{Black,DeepOrange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,DeepOrange,Black},
{Black,DeepOrange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,DeepOrange,Black},
{Black,Orange,DeepOrange,DeepOrange,Orange,Orange,Orange,Orange,Orange,DeepOrange,DeepOrange,Orange,Black},
{White,Black,Orange,Orange,DeepOrange,DeepOrange,DeepOrange,DeepOrange,Orange,Orange,Black,White},
{Black,Red,Red,Red,Orange,Orange,Orange,Orange,Orange,Red,Red,Red,Black},
{Black,Green,Red,Red,Red,Red,Red,Red,Red,Red,Red,Green,Black},
{Black,DeepOrange,Green,Green,Red,Red,Red,Red,Red,Green,Green,DeepOrange,Black},
{White,Black,DeepOrange,DeepOrange,Green,Green,Green,Green,Green,DeepOrange,DeepOrange,Black,White},
{White,White,Black,Black,DeepOrange,DeepOrange,DeepOrange,DeepOrange,DeepOrange,Black,Black,White,White},
{White,White,White,White,Black,Black,Black,Black,Black,White,White,White,White}};

volatile uint16_t snack[18][10]={{White,Black,Black,Black,Black,Black,Black,Black,Black,White},
{Black,Blue,Blue,Grey,Grey,Grey,Blue,Blue,Blue,Black},
{Black,Red,Red,White,Red,Red,Red,DarkRed,DarkRed,Black},
{Black,Red,Red,White,Red,Red,Red,DarkRed,DarkRed,Black},
{Black,Red,Red,White,Red,Red,Red,DarkRed,DarkRed,Black},
{Black,Red,Red,White,Red,Red,Red,DarkRed,DarkRed,Black},
{Black,Red,Red,White,White,Red,Red,DarkRed,DarkRed,Black},
{Black,Red,Red,Red,White,Red,Red,DarkRed,DarkRed,Black},
{Black,Red,Red,Red,White,Red,Red,DarkRed,DarkRed,Black},
{Black,Red,Red,Red,White,White,Red,DarkRed,DarkRed,Black},
{Black,Red,Red,Red,Red,White,White,DarkRed,DarkRed,Black},
{Black,Red,Red,Red,Red,White,White,DarkRed,DarkRed,Black},
{Black,Red,Red,Red,Red,White,White,DarkRed,DarkRed,Black},
{Black,Red,Red,Red,Red,White,White,DarkRed,DarkRed,Black},
{Black,Red,Red,Red,Red,White,White,DarkRed,DarkRed,Black},
{Black,Red,Red,Red,Red,White,White,DarkRed,DarkRed,Black},
{Black,Blue,Blue,Grey,Grey,Grey,Blue,Blue,Blue,Black},
{White,Black,Black,Black,Black,Black,Black,Black,Black,White}};

volatile uint16_t guscio[17][23]={{White,White,White,White,White,White,White,White,White,Black,Black,Black,Black,Black,White,White,White,White,White,White,White,White,White},
{White,White,White,White,White,White,Black,Black,Black,Cachi,Cachi,Cachi,Cachi,Cachi,Black,Black,Black,White,White,White,White,White,White},
{White,White,White,White,White,Black,Black,Cachi,Cachi,Cachi,Cachi,Cachi,Cachi,Cachi,Black,Green,Black,Black,White,White,White,White,White},
{White,White,White,White,Black,Black,Black,Black,Cachi,Cachi,Cachi,Cachi,Cachi,Cachi,Black,Green,Green,Black,Black,White,White,White,White},
{White,White,White,Black,Black,Green,Green,Black,Cachi,Cachi,Cachi,Cachi,Black,Black,Green,Green,Green,Black,Black,White,White,White,White},
{White,White,White,Black,Green,Green,Green,Green,Black,Black,Black,Black,Green,Green,Green,Black,Black,Cachi,Black,Black,White,White,White},
{White,White,Black,Black,Black,Black,Green,Green,Green,Green,Green,Green,Green,Green,Green,Black,Cachi,Cachi,Cachi,Black,White,White,White},
{White,White,Black,Cachi,Cachi,Cachi,Black,Green,Green,Green,Black,Black,Green,Green,Green,Black,Cachi,Cachi,Cachi,Black,Black,White,White},
{White,White,Black,Cachi,Cachi,Cachi,Black,Green,Green,Black,Cachi,Cachi,Black,Green,Green,Black,Black,Cachi,Cachi,Cachi,Black,Black,Black},
{White,White,Black,Cachi,Cachi,Cachi,Black,Green,Black,Cachi,Cachi,Cachi,Cachi,Black,Green,Green,Black,Cachi,Black,Black,Black,Green,Black},
{White,Black,Black,Cachi,Cachi,Black,Black,Green,Black,Cachi,Cachi,Cachi,Cachi,Black,Green,Green,Green,Black,Black,Green,Green,Black,Black},
{Black,Black,Black,Black,Black,Green,Green,Green,Black,Cachi,Cachi,Cachi,Cachi,Black,Green,Green,Green,Green,Green,Green,Black,Black,White},
{Black,Green,Green,Green,Green,Green,Green,Green,Black,Black,Cachi,Cachi,Black,Black,Green,Green,Green,Green,Green,Black,Black,White,White},
{Black,Black,Green,Green,Green,Green,Green,Green,Green,Black,Black,Black,Green,Green,Green,Green,Green,Green,Green,Black,White,White,White},
{Black,Black,Black,Black,Green,Green,Green,Green,Green,Green,Green,Green,Green,Green,Green,Green,Green,Black,Black,Black,White,White,White},
{White,White,White,Black,Black,Black,Black,Green,Green,Green,Green,Green,Green,Green,Green,Black,Black,Black,White,White,White,White,White},
{White,White,White,White,White,White,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,White,White,White,White,White,White}};

volatile uint16_t cuore[6][7]={{White,Red,Red,White,Red,Red,White},
{Red,Red,Red,Red,Red,Red,Red},
{Red,Red,Red,Red,Red,Red,Red},
{White,Red,Red,Red,Red,Red,White},
{White,White,Red,Red,Red,White,White},
{White,White,White,Red,White,White,White}};

volatile uint16_t volume[11][5]={{White,White,White,White,Black},
{White,White,White,Black,Black},
{White,White,Black,Black,Black},
{White,White,Black,Black,Black},
{Black,Black,Black,Black,Black},
{Black,Black,Black,Black,Black},
{Black,Black,Black,Black,Black},
{White,White,Black,Black,Black},
{White,White,Black,Black,Black},
{White,White,White,Black,Black},
{White,White,White,White,Black}};

enum DogType{alive1,alive2,dead};

volatile int happiness=5;
volatile int satiety=5;
void drawT(int type,int startx,int starty);
void drawVolumeIcon(int startx,int starty,int size);
volatile int ore,minuti,secondi;
/* Led external variables from funct_led */
extern unsigned char led_value;					/* defined in lib_led								*/
#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {	//GIUSEPPE SCARSO S308807
  volatile int pos=0,i,j;
	volatile int posx=0,posy=0;
	SystemInit();  												/* System Initialization (i.e., PLL)  */
  //LED_init();                           /* LED Initialization                 */
  //BUTTON_init();												/* BUTTON Initialization              */
	LCD_Initialization();									//240x320
	TP_Init();
	joystick_init();											/* Joystick Initialization            */
	ADC_init();
	
	TouchPanel_Calibrate();
	LCD_Clear(White);
	
	
	drawT(alive1,80,200);												//PRIMO DISEGNO
	
	GUI_Text(0,10,"Happiness:",Black,White);		//BARRE FAME E FELICITA'
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
	
	drawVolumeIcon(5,50,2);
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
	
	
	init_timer(0,0,0,3,25000000);						//imposto timer 0 per interrompere e fare l'animazione ogni secondo
	
																					
	init_RIT(5000000);											/* RIT Initialization 50 msec per il joystick      	*/
	
	LCD_DrawLine(0,270,240,270,Black);			//BARRA DI SELEZIONE
	LCD_DrawLine(120,270,120,320,Black);
	GUI_Text(40,290,"Meal",Red,White);
	GUI_Text(160,290,"Snack",Black,White);
	
	
	
	
	/* K = T*Fr = [s]*[Hz] = [s]*[1/s]	  */
																				/* T = K / Fr = 0x017D7840 / 25MHz    */
																				/* T = K / Fr = 25000000 / 25MHz      */
																				/* T = 1s	(one second)   							*/
	
	
	enable_timer(0);
	enable_RIT();
	//NVIC_SetPriority(TIMER0_IRQn, 4);			//IMPOSTO PRIORITA' 4 IN MODO DA LASCIARE SPAZIO AL RIT solo emulatore
	GUI_Text(90,10,"Age",Black,White);
	init_timer(1,0,0,3,25000000/2);				//IMPOSTO QUESTO TIMER A MEZZO SECONDO(SERVIRA' PER LE ANIMAZIONI)
	//LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	//LPC_SC->PCON &= 0xFFFFFFFFD;						
	init_timer(2,0,0,3,0xC8);							//timer per il touch panel
  enable_timer(2);
	while (1) {                           /* Loop forever                       */	
		//__ASM("wfi");
  }

}

//FUNZIONE PER DISEGNARE LA TARTARUGA
void drawT(int type,int startx,int starty){
	volatile int i,j,posx=0,posy=0;
	
	if(type==alive1){
		for (i=starty;i<starty+68;i+=4){
			for(j=startx;j<startx+56;j+=4){
				drawRect(j,i,4,4,tartaruga1[posx][posy]);
				posy++;
				if(posy>=14){
					posy=0;
					posx++;
				}
			}
		}
	}else if (type==alive2){
		for (i=starty;i<starty+24;i+=4){
			for(j=startx;j<startx+56;j+=4){
				drawRect(j,i,4,4,tartaruga2[posx][posy]);
				posy++;
				if(posy>=14){
					posy=0;
					posx++;
				}
			}
	}
}
	}

//FUNZIONE PER DISEGNARE IL CIBO
void drawFood(int type){
	volatile int i,j,posx=0,posy=0,x=80,y=150;
	if(type==0){
		
	for (i=y;i<y+30;i+=2){
			for(j=x;j<x+26;j+=2){
				drawRect(j,i,2,2,meal[posx][posy]);
				posy++;
				if(posy>=13){
					posy=0;
					posx++;
				}
			}
		}
	}else if(type==1){
		for (i=y;i<y+36;i+=2){
			for(j=x;j<x+20;j+=2){
				drawRect(j,i,2,2,snack[posx][posy]);
				posy++;
				if(posy>=10){
					posy=0;
					posx++;
				}
			}
		}
	}
}

//FUNZIONE PER PULIRE LO SCHERMO IN DETERMINATI PUNTI PRIMA DI FARE L'ANIMAZIONE
void clearT(int x,int y){
	volatile int i,j,posx=0,posy=0;
	for (i=y;i<y+68;i+=4){
			for(j=x;j<x+56;j+=4){
				drawRect(j,i,4,4,White);
				posy++;
				if(posy>=14){
					posy=0;
					posx++;
				}
			}
		}
}

void drawGuscio(int x,int y){
		volatile int i,j,posx=0,posy=0;
		for (i=y;i<y+51;i+=3){
			for(j=x;j<x+69;j+=3){
				drawRect(j,i,3,3,guscio[posx][posy]);
				posy++;
				if(posy>=23){
					posy=0;
					posx++;
				}
			}
		}
	
}

void clearGuscio(int x,int y){
		volatile int i,j,posx=0,posy=0;
		for (i=y;i<y+51;i+=3){
			for(j=x;j<x+69;j+=3){
				drawRect(j,i,3,3,White);
				posy++;
				if(posy>=23){
					posy=0;
					posx++;
				}
			}
		}
}

void drawbigT(int startx,int starty,int size){
	volatile int i,j,posx=0,posy=0;
	for (i=starty;i<starty+17*size;i+=size){
			for(j=startx;j<startx+14*size;j+=size){
				drawRect(j,i,size,size,tartaruga2[posx][posy]);
				posy++;
				if(posy>=14){
					posy=0;
					posx++;
				}
			}
	}
}

void clearbigT(int startx,int starty,int size){
	volatile int i,j,posx=0,posy=0;
	for (i=starty;i<starty+17*size;i+=size){
			for(j=startx;j<startx+14*size;j+=size){
				drawRect(j,i,size,size,White);
				posy++;
				if(posy>=14){
					posy=0;
					posx++;
				}
			}
	}
}

void drawC(int startx,int starty,int size){
	volatile int i,j,posx=0,posy=0;
	for (i=starty;i<starty+6*size;i+=size){
			for(j=startx;j<startx+7*size;j+=size){
				drawRect(j,i,size,size,cuore[posx][posy]);
				posy++;
				if(posy>=14){
					posy=0;
					posx++;
				}
			}
	}
}

void clearC(int startx,int starty,int size){
	volatile int i,j,posx=0,posy=0;
	for (i=starty;i<starty+6*size;i+=size){
			for(j=startx;j<startx+7*size;j+=size){
				drawRect(j,i,size,size,White);
				posy++;
				if(posy>=14){
					posy=0;
					posx++;
				}
			}
	}
}

void drawVolumeIcon(int startx,int starty,int size){
	volatile int i,j,posx=0,posy=0;
	for (i=starty;i<starty+11*size;i+=size){
			for(j=startx;j<startx+5*size;j+=size){
				drawRect(j,i,size,size,volume[posx][posy]);
				posy++;
				if(posy>=5){
					posy=0;
					posx++;
				}
			}
	}
}