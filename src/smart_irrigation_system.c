#include<LPC21XX.H>

#include<stdio.h>

#include"lcd.h"

#include"UART.h"

#include"Motor.h"

void LCD_ROTATE(char*s,int i);


int main()

{	int i;

	LCD_INIT();

	uart_init();

	motor_init();




	for(i=0x80;i<0x90;i++)

	{

   		LCD_COMMAND(i);

		LCD_ROTATE("SMART IRRIGATION SYSTEM ",i);

		delay_ms(400);

		LCD_COMMAND(0x01);


	}

	while(1)

	mois_status();

}

void LCD_ROTATE(char*s,int i)

{

	while(*s)

	{

	
							LCD_DATA(*s++);

		i++;

		if(i==0x90)

			LCD_COMMAND(0x80);

	}

}
