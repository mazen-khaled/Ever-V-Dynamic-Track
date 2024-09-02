/*****************************************************************************************
 * 	File Name		: app.c
 *	Author			: Mazen Khaled
 *  Created on		: Feb 16, 2024
 *	Description		: Application to communicate with 2 MCU's using UART Protocol
 *****************************************************************************************/
#include <avr/io.h>
#include "uart.h"

int main(void){
	UART_init(9600);
	DDRA = 0XFF;

	while (1){
		uint8 x = UART_reciveByte();

		LCD_displayCharacter(x);
	}
	return 0;
}
