/*
 * main_2.c
 *
 *  Created on: Jan 27, 2021
 *      Author: Yumna-HP
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "i2c.h"
#include "external_eeprom.h"
#include "uart.h"
#include"micro_config.h"
#include"std_types.h"
#include "common_macros.h"
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#define M2_READY 0x10
#define M1_READY 0x20
#define RESET 0x30

//#define NULL ((char *)0)
int main(void)
{
	//int *ptr;
uint8 x;
uint8 y;
	UART_init();
	EEPROM_init();
	while(1){
		int key_num[5];
		char flag;
		char reset=0;
		uint8 check_num[5];
		EEPROM_readByte(0x0111, &x);
		EEPROM_readByte(0x0315, &y);
		if(y==255)
		{
			//EEPROM_writeByte(0x0315,5);
			flag=0;
			while(UART_recieveByte() != M1_READY){}
			UART_sendByte(M2_READY);
			UART_sendByte(flag);
			_delay_ms(500);


			for(int i=0;i<5; i++)
			{
				while(UART_recieveByte() != M1_READY){}
							UART_sendByte(M2_READY);
							key_num[i]	=UART_recieveByte();
							_delay_ms(500);
			}
			/*EEPROM_writeByte(0x0311,key_num[0]);
			EEPROM_writeByte(0x0312,key_num[1]);
			EEPROM_writeByte(0x0313,key_num[2]);
			EEPROM_writeByte(0x0314,key_num[3]);
			EEPROM_writeByte(0x0315,key_num[4]);*/
			//UART_receiveString(key_num);
			for(int i=0;i<5; i++)
			{
			EEPROM_writeByte(0x0311+i, key_num[i]);
			_delay_ms(500);

			}
		   }

  else
	{
			while (UART_recieveByte() != M1_READY) {
			}
			UART_sendByte(M2_READY);
			reset = UART_recieveByte();

			_delay_ms(500);


			if (reset == RESET) {
				for(int i=0;i<5; i++)
							{
								while(UART_recieveByte() != M1_READY){}
											UART_sendByte(M2_READY);
											key_num[i]	=UART_recieveByte();
											_delay_ms(500);
							}

				for(int i=0;i<5; i++)
				{
				EEPROM_writeByte(0x0311+i, key_num[i]);
				_delay_ms(500);

				}

			} else {

				flag = 1;
				while (UART_recieveByte() != M1_READY) {
				}
				UART_sendByte(M2_READY);
				UART_sendByte(flag);

				_delay_ms(500);

				for (int i = 0; i < 5; i++) {
					EEPROM_readByte(0x0311 + i, &check_num[i]);
					_delay_ms(500);

				}
				for (int i = 0; i < 5; i++) {
					while (UART_recieveByte() != M1_READY) {
					}
					UART_sendByte(M2_READY);
					UART_sendByte(check_num[i]);
					_delay_ms(500);
				}

			}
		}
	}

}

