/******************************************************************************
 *
 * Module: UART0
 *
 * File Name: uart0.h
 *
 * Description: Header file for the UART0 driver
 *
 *******************************************************************************/

#ifndef UART5_H_
#define UART5_H_

#include "std_types.h"

#define UART_DATA_5BITS          0x0
#define UART_DATA_6BITS          0x1
#define UART_DATA_7BITS          0x2
#define UART_DATA_8BITS          0x3
#define UART_LCRH_WLEN_BITS_POS  5
#define UART_CTL_UARTEN_MASK     0x00000001
#define UART_CTL_TXE_MASK        0x00000100
#define UART_CTL_RXE_MASK        0x00000200
#define UART_FR_TXFE_MASK        0x00000080
#define UART_FR_RXFE_MASK        0x00000010

void UART5_Init(void);

void UART5_SendByte(uint8 data);

void UART5_SendData(const uint8 *pData, uint32 uSize);

void UART5_SendString(const uint8 *pData);




#endif
