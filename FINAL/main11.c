/*
 * main11.c
 *
 *  Created on: Jan 2, 2022
 *      Author: Yumna-HP
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include"lcd.h"
#include"micro_config.h"
#include"std_types.h"
#include "common_macros.h"
#include "keypad.h"
#include "i2c.h"
#include "external_eeprom.h"
#include "uart.h"
#define M2_READY 0x10
#define M1_READY 0x20
#define RESET 0x30

void ResetPassword(void){
	unsigned char keyy;
	uint8 k[5];
	uint8 oldPassword[6];
	char flag_unused;
	LCD_displayString("Reset  password");
	_delay_ms(500);
	LCD_goToRowColumn(1,0);
	for(int i=0;i<5; i++)
			{
				keyy = KeyPad_getPressedKey();
				k[i]=keyy;
					LCD_displayString("*");
							_delay_ms(500);
			}
	UART_sendByte(M1_READY);
	while(UART_recieveByte() != M2_READY){}
	UART_sendByte(0x0f); //to skip the reset condition in main 2

	UART_sendByte(M1_READY);
	while(UART_recieveByte() != M2_READY){}
	flag_unused = UART_recieveByte(); //receive the unused flag
	_delay_ms(500);
	LCD_displayString("k");
	for(int i=0;i<5; i++)
	{

		UART_sendByte(M1_READY);
		while(UART_recieveByte() != M2_READY){}

		oldPassword[i]	=UART_recieveByte();
		LCD_intgerToString(i);

	}
	LCD_displayString("m");

	/*for (int i=1;i<6;i++)
	{
		_delay_ms(10);
		LCD_intgerToString(oldPassword[i]);

	}*/
	unsigned char is_true = 1;
	LCD_clearScreen();
	for(int i=0;i<5; i++){
		LCD_intgerToString(oldPassword[i]);
		LCD_intgerToString(k[i]);
		_delay_ms(2000);
		if (oldPassword[i]!=k[i]){
			is_true=0;
			break;
		}
	}
	if (is_true==0){
		LCD_clearScreen();
		LCD_displayString("Wrong Password");
	}else if(is_true==1){
		LCD_clearScreen();
		LCD_displayString("Enter New Password");
		LCD_goToRowColumn(1,0);
		for(int i=0;i<5; i++)
		{
			keyy = KeyPad_getPressedKey();
			k[i]=keyy;
			LCD_intgerToString(k[i]);
			_delay_ms(500);
		}
		UART_sendByte(M1_READY);
		while(UART_recieveByte() != M2_READY){}
		LCD_displayString("m");

		UART_sendByte(RESET);

		for(int j=0;j<5;j++){
			UART_sendByte(M1_READY);
			while(UART_recieveByte() != M2_READY){} // wait until MC2 is ready

			UART_sendByte(k[j]);
			LCD_intgerToString(j);

}
		LCD_clearScreen();
		LCD_displayString("Password Reset Succ");
	}
}
int main(void)
{
	unsigned char key;
	unsigned char keyy;
	UART_init();
	LCD_init();
	//LCD_displayString("Enter new password");
	/*LCD_displayStringRowColumn(0,0,"enter");
	LCD_displayStringRowColumn(1,0,"new");
	LCD_displayStringRowColumn(2,0,"password");*/
	//LCD_goToRowColumn(1,0);
	uint8 s[5];
	uint8 check[5];
	uint8 k[5];
	uint8 rec[5];
	uint8 old[5];
	uint8 oldPassword[8];


	while(1)
	{
		unsigned char press=KeyPad_getPressedKey();
		LCD_clearScreen();
		LCD_intgerToString(press);
		_delay_ms(20000);
		LCD_clearScreen();
		if(press==0){
			LCD_clearScreen();
			ResetPassword();
		}
		else{
		UART_sendByte(M1_READY);
		while(UART_recieveByte() != M2_READY){}
		UART_sendByte(0x0f); //to skip the reset condition in main 2
		_delay_ms(500);

		UART_sendByte(M1_READY);
		while(UART_recieveByte() != M2_READY){}
		char what=UART_recieveByte();
		if(what==1)
		{
			LCD_displayString("Enter  password");
			_delay_ms(500);
			LCD_goToRowColumn(1,0);
			for(int i=0;i<5; i++)
					{keyy = KeyPad_getPressedKey();
					k[i]=keyy;
							LCD_displayString("*");
									_delay_ms(500);
					}
			for(int i=0;i<5; i++)
			{UART_sendByte(M1_READY);
			while(UART_recieveByte() != M2_READY){}

			rec[i]	=UART_recieveByte();
			_delay_ms(10);
			//LCD_intgerToString(rec[i]);

						}
			for(int i=0;i<5; i++)
			{
				if(k[i]==rec[i])
				{
					LCD_displayStringRowColumn(0,0,"correct  password");
					_delay_ms(500);


				}
				else
				{
					i=5;
					LCD_displayStringRowColumn(0,0,"incorrect  password");
					_delay_ms(500);

				}
			}
		}
		else{LCD_displayString("Enter new passw");
		_delay_ms(500);
			LCD_goToRowColumn(1,0);
				for(int i=0;i<5; i++)
				{key = KeyPad_getPressedKey();
				 /* get the pressed key number */
				/*
				if((key <= 9) && (key >= 0))
						{
							LCD_displayString("*");
							UART_sendByte(key);
						}
						else
						{
							LCD_displayString("*");
							UART_sendByte(key);
						}
						*/

				s[i]=key;

				LCD_intgerToString(s[i]);
						_delay_ms(500);
						/* Press time */
						//printf("String is %s \n" , key);
			}
				LCD_displayStringRowColumn(0,0,"check your password");
				LCD_goToRowColumn(1,0);
				LCD_displayStringRowColumn(1,0,"     ");
				for(int i=0;i<5; i++)
						{key = KeyPad_getPressedKey();
						check[i]=key;
						LCD_intgerToString(check[i]);
						_delay_ms(500);
						}
				bool ch = TRUE;
				for(int i=0;i<5; i++)
				{
					if(s[i]!=check[i])
					{
						ch = FALSE;


					}
				}
				if (ch)
				{
					LCD_displayStringRowColumn(0,0,"great");
					for(int j=0;j<5;j++){
									UART_sendByte(M1_READY);
									while(UART_recieveByte() != M2_READY){} // wait until MC2 is ready
													//UART_sendString(check);
													UART_sendByte(s[j]);
													_delay_ms(500);}
						}
					else
					{

						LCD_displayStringRowColumn(0,0,"nope");
					}
				}


			}
	}
}





