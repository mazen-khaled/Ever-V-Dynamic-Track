/******************************************************************************************
 * 	File Name		: uart.c
 *	Author			: kmaze
 *  Created on		: Feb 16, 2024
 *	Description		: UART Module Source File
 *****************************************************************************************/
/************************************** Includes *****************************************/
#include "uart.h"

/*********************************** Global Variables ************************************/

/********************************* Function Definitions **********************************/
/******************************************************************************************
* Function Name			: UART_init
* Function Description	: Initialize UART setting
* 						  (Baud Rate / Data Interrupts / Frame bits)
* Function Arguments	: BoaudRate
* Function Returns		: void
******************************************************************************************/
void UART_init(uint32 BaudRate){
	uint16 ubrr_bits = 0;

	// To use UCSRA register need to clear those bits
	CLEAR_BIT(UCSRA,FE);
	CLEAR_BIT(UCSRA,PE);

	SET_BIT(UCSRA,U2X); 	//Double Speed UART

	/******************** UCSRB Register ***************************
	Disable all interrupt (RX/TX/UDR)
	Set Enable UART To receive and transmit bit (RXEN/TXEN)
	Disable 9th bit as data frame (UCSZ2 = 0) / (RXB8/TXB8 = 0)
	****************************************************************/
	UCSRB = UCSRB | (3 << 3);	// 0001 1000

	/******************** UCSRC Register ***************************
	Determine Asynchronous Mode
	Disable Parity Bit
	1-bit as Stop Bit
	8-bits as data bits
	****************************************************************/
	SET_BIT(UCSRC,URSEL);
	UCSRC = UCSRC | (3<<1); // 0000 0110

	/********************* UBRR Register ***************************
	Setting Baud Rate
	****************************************************************/
	CLEAR_BIT(UCSRC,URSEL);

	//Calculate the value of UBRR
	ubrr_bits = (uint16)(((F_CPU)/(8*BaudRate)) - 1);

	UBRRH = ubrr_bits >> 8;
	UBRRL = ubrr_bits;
}
/******************************************************************************************
* Function Name			: UART_sendByte
* Function Description	: Transmit data
* Function Arguments	: uint8 DataByte
* Function Returns		: void
******************************************************************************************/
void UART_sendByte(uint8 DataByte){
	UDR = DataByte; //The transmission begin

	// Wait until the transmission is complete TXC = 1
	while(BIT_IS_CLEAR(UCSRA,TXC));

	SET_BIT(UCSRA,TXC); // Clear the TXC flag
}
/******************************************************************************************
* Function Name			:
* Function Description	:
* Function Arguments	:
* Function Returns		:
******************************************************************************************/
void UART_sendString(const uint8 *DataString){
	uint8 i = 0;
	while (DataString[i] != '\0'){
		UART_sendByte(DataString[i]);
		i++;
	}
	UART_sendByte('\0');
}
/******************************************************************************************
* Function Name			:
* Function Description	:
* Function Arguments	:
* Function Returns		:
******************************************************************************************/
uint8 UART_reciveByte(){
	// Wait until the receive is complete RXC = 1
	while(BIT_IS_CLEAR(UCSRA,RXC));

	return UDR;
}
/******************************************************************************************
* Function Name			:
* Function Description	:
* Function Arguments	:
* Function Returns		:
******************************************************************************************/
void UART_reciveString(uint8 *DataString){
	uint8 i = 0;

	while (UDR != '\0'){
		DataString[i] = UART_reciveByte();
		i++;
	}

	DataString[i] = '\0';
}
/***************************************** ISR *******************************************/
