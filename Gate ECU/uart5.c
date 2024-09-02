/******************************************************************************
 *
 * Module: UART5
 *
 * File Name: 1.c
 *
 * Description: Source file for the UART5 driver
 *
 *******************************************************************************/

#include <uart5.h>
#include "tm4c123gh6pm_registers.h"

static void GPIO_SetupUART5Pins(void)
{
    SYSCTL_RCGCGPIO_REG |= 0x10; /* Enable clock for GPIO PORTE */
    while (!(SYSCTL_PRGPIO_REG & 0x10))
        ; /* Wait until GPIO PORTA clock is activated and it is ready for access*/

    GPIO_PORTE_AMSEL_REG &= 0xCF; /* Disable Analog on PE4 & PE5 */
    GPIO_PORTE_DIR_REG &= 0xEF; /* Configure PE4 as input pin */
    GPIO_PORTE_DIR_REG |= 0x20; /* Configure PE5 as output pin */
    GPIO_PORTE_AFSEL_REG |= 0x30; /* Enable alternative function on PE4 & PE5 */
    /* Set PMCx bits for PE4 & PE5 with value 1 to use PE4 as UART5 RX pin and PE5 as UART5 Tx pin */
    GPIO_PORTE_PCTL_REG = (GPIO_PORTE_PCTL_REG & 0xFF00FFFF) | 0x00110000;
    GPIO_PORTE_DEN_REG |= 0x30; /* Enable Digital I/O on PE4 & PE5 */

}

void UART5_Init(void)
{
    /* Setup UART5 pins PE4 --> U1RX & PE5 --> U1TX */
    GPIO_SetupUART5Pins();

    SYSCTL_RCGCUART_REG |= 0X20;  // enable clock for UART5
    while (!(SYSCTL_PRUART_REG & 0x20))
        ; /* Wait until UART5 clock is activated and it is ready for access*/

    UART5_CTL_REG = 0; /* Disable UART5 at the beginning */
    /* To Configure UART5 with Baud Rate 9600 */
    UART5_IBRD_REG = 104;
    UART5_FBRD_REG = 11;
    UART5_CC_REG = 0; /* Use System Clock*/

    /* UART Line Control Register Settings
     * BRK = 0 Normal Use
     * PEN = 0 Disable Parity
     * EPS = 0 No affect as the parity is disabled
     * STP2 = 0 1-stop bit at end of the frame
     * FEN = 0 FIFOs are disabled
     * WLEN = 0x3 8-bits data frame
     * SPS = 0 no stick parity
     */
    UART5_LCRH_REG = (UART_DATA_8BITS << UART_LCRH_WLEN_BITS_POS);

    /* UART Control Register Settings
     * RXE = 1 Enable UART Receive
     * TXE = 1 Enable UART Transmit
     * HSE = 0 The UART is clocked using the system clock divided by 16
     * UARTEN = 1 Enable UART
     */
    UART5_CTL_REG =
    UART_CTL_UARTEN_MASK | UART_CTL_TXE_MASK | UART_CTL_RXE_MASK;
}

void UART5_SendByte(uint8 data)
{
    while (!(UART5_FR_REG & UART_FR_TXFE_MASK))
        ; /* Wait until the transmit FIFO is empty */
    UART5_DR_REG = data; /* Send the byte */
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART5_SendString(const uint8 *pData)
{
    uint32 uCounter = 0;
    /* Transmit the whole string */
    while (pData[uCounter] != '\0')
    {
        UART5_SendByte(pData[uCounter]); /* Send the byte */
        uCounter++; /* increment the counter to the next byte */
    }
}

/*
 * Description :
 * Send the required number of bytes through UART to the other UART device.
 */
void UART5_SendData(const uint8 *pData, uint32 uSize)
{
    /* Transmit the number of bytes requested on the UART port */
    while (uSize--)
    {

        UART5_SendByte(*pData); /* Send the byte */
        pData++; /* increment pointer to the next byte */
    }
}

