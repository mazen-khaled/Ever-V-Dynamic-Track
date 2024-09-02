#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "tm4c123gh6pm_registers.h"
#include "uart0.h"
#include "uart5.h"

#define DISK_HOLES 6
#define DISK_RADIUS 0.000297  // radius in KM

#define PROXIMITY_SENSOR_PIN 0
#define LED_PIN 1

uint8 previous_state = 0;

uint16 distance = 0;
uint8 remaing_capacity = 0;
float energy_consumed = 0;
float Available_Range = 0;
float total_distance = 0;
uint8 max_capacity = 150;
uint16 watt = 0;
uint8 current = 0;
uint8 voltage = 0;
uint8 capacity = 0;
uint8 disk_count = 0;
uint8 speed = 0;

uint8 button_1 = 0;
uint8 button_2 = 0;
uint8 button_3 = 0;
uint8 button_4 = 0;


void buttons_init(void) {
    // Enable clock for Port D
    SYSCTL_RCGCGPIO_R |= (1 << 3);
    while((SYSCTL_PRGPIO_R & (1 << 3)) == 0);

    // Disable analog function on PD0-3
    GPIO_PORTD_AMSEL_REG &= ~((0xF) << 0);

    // Clear PCTL register bits for PD0-3 to use as GPIO
    GPIO_PORTD_PCTL_REG &= 0xFFFF0000;

    // Set PD0-3 as input
    GPIO_PORTD_DIR_REG &= ~((0xF) << 0);

    // Disable alternate functions on PD0-3
    GPIO_PORTD_AFSEL_REG &= ~((0xF) << 0);

    // Enable pull-up resistors on PD0-3
    GPIO_PORTD_PUR_REG |= (0xF << 0);

    // Enable digital function on PD0-3
    GPIO_PORTD_DEN_REG |= (0xF << 0);
}

void buttons_check(void) {
    char *left_pic = (!(GPIO_PORTD_DATA_REG & (1 << 0))) ? "L.pic=3" : "L.pic=1";
    char *right_pic = (!(GPIO_PORTD_DATA_REG & (1 << 1))) ? "R.pic=4" : "R.pic=2";
    char *warning_pic = ((!(GPIO_PORTD_DATA_REG & (1 << 0))) && (!(GPIO_PORTD_DATA_REG & (1 << 1)))) ? "W.pic=6" : "W.pic=5";
    char *forwared_txt = (!(GPIO_PORTD_DATA_REG & (1 << 3))) ? "F.txt=\"R\"" : "F.txt=\"F\"";

    UART5_SendString(left_pic);
    UART5_SendByte(0xFF);
    UART5_SendByte(0xFF);
    UART5_SendByte(0xFF);

    UART5_SendString(right_pic);
    UART5_SendByte(0xFF);
    UART5_SendByte(0xFF);
    UART5_SendByte(0xFF);

    UART5_SendString(warning_pic);
    UART5_SendByte(0xFF);
    UART5_SendByte(0xFF);
    UART5_SendByte(0xFF);

    UART5_SendString(forwared_txt);
    UART5_SendByte(0xFF);
    UART5_SendByte(0xFF);
    UART5_SendByte(0xFF);
}

void Leds_Init(void){
    GPIO_PORTF_AMSEL_REG &= ~0x0E;
    GPIO_PORTF_PCTL_REG  &= ~0x0000FFF0;
    GPIO_PORTF_DIR_REG   |= 0x0E;
    GPIO_PORTF_AFSEL_REG &= ~0x0E;
    GPIO_PORTF_DEN_REG   |= 0x0E;
    GPIO_PORTF_DATA_REG  &= ~0x0E;
}

void int_to_str(int num, char *str) {
    int i = 0;
    int is_negative = 0;

    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % 10;
        str[i++] = rem + '0';
        num = num / 10;
    }

    // If number is negative, add the negative sign
    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char str[10];

/* Enable the SysTick Timer to run using the System Clock with Frequency 16Mhz and Count one second */
void SysTick_Init(void)
{
    SYSTICK_CTRL_REG    = 0;              /* Disable the SysTick Timer by Clear the ENABLE Bit */
    SYSTICK_RELOAD_REG  = 15999999/4;       /* Set the Reload value with 15999999 to count 1/4 Second */
    SYSTICK_CURRENT_REG = 0;              /* Clear the Current Register value */
    /* Configure the SysTick Control Register
     * Enable the SysTick Timer (ENABLE = 1)
     * Disable SysTick Interrupt (INTEN = 0)
     * Choose the clock source to be System Clock (CLK_SRC = 1) */
    SYSTICK_CTRL_REG   |= 0x05;
}

void proxomity_Init(void){
    GPIO_PORTF_LOCK_REG   = 0x4C4F434B;   /* Unlock the GPIO_PORTF_CR_REG */
    GPIO_PORTF_CR_REG    |= (1<<0);       /* Enable changes on PF0 */
    GPIO_PORTF_AMSEL_REG &= ~(1<<0);      /* Disable Analog on PF0 */
    GPIO_PORTF_PCTL_REG  &= 0xFFFFFFF0;   /* Clear PMCx bits for PF0 to use it as GPIO pin */
    GPIO_PORTF_DIR_REG   &= ~(1<<0);      /* Configure PF0 as input pin */
    GPIO_PORTF_AFSEL_REG &= ~(1<<0);      /* Disable alternative function on PF0 */
    GPIO_PORTF_PUR_REG   |= (1<<0);       /* Enable pull-up on PF0 */
    GPIO_PORTF_DEN_REG   |= (1<<0);       /* Enable Digital I/O on PF0 */
}

int main(void){
    volatile unsigned long delay = 0;
    SYSCTL_REGCGC2_REG |= 0x00000020;
    delay = SYSCTL_REGCGC2_REG;

    SysTick_Init();
    buttons_init();
    proxomity_Init();
    Leds_Init();
    UART0_Init();
    UART5_Init();
    uint8_t count = 0;

    while(1){
        buttons_check();
        if (SYSTICK_CTRL_REG & (1 << 16)){ /* Check if the COUNT flag is set */
            count++;

            if (count > 5){
                count = 1;
            }

            switch(count) {
                case 1:
                    str[0] = '\0';
                    UART0_SendByte(4);
                    capacity = UART0_ReceiveByte();
                    int_to_str(capacity, str);
                    UART5_SendString("capacity.val=");
                    UART5_SendString(str);
                    UART5_SendByte(0xFF);
                    UART5_SendByte(0xFF);
                    UART5_SendByte(0xFF);
                    break;
                case 2:
                    str[0] = '\0';
                    UART0_SendByte(5);
                    voltage = UART0_ReceiveByte();
                    int_to_str(voltage, str);
                    UART5_SendString("voltage.val=");
                    UART5_SendString(str);
                    UART5_SendByte(0xFF);
                    UART5_SendByte(0xFF);
                    UART5_SendByte(0xFF);
                    break;
                case 3:
                    str[0] = '\0';
                    watt = voltage * current * 0.25;
                    int_to_str(watt, str);
                    UART5_SendString("watt.val=");
                    UART5_SendString(str);
                    UART5_SendByte(0xFF);
                    UART5_SendByte(0xFF);
                    UART5_SendByte(0xFF);
                    break;
                case 4:
                    str[0] = '\0';
                    UART0_SendByte(3);
                    current = UART0_ReceiveByte();
                    if (current > 200){
                        current = 50;
                    }
                    int_to_str(current, str);
                    UART5_SendString("current.val=");
                    UART5_SendString(str);
                    UART5_SendByte(0xFF);
                    UART5_SendByte(0xFF);
                    UART5_SendByte(0xFF);
                    break;
                case 5:
                    str[0] = '\0';
                    distance = ((disk_count / (float)DISK_HOLES) * (2 * DISK_RADIUS * 3.14));
                    speed = distance * 60.0 * 60.0; // speed in km/h
                    int_to_str(disk_count, str);
                    UART5_SendString("speed.val=");
                    UART5_SendString(str);
                    UART5_SendByte(0xFF);
                    UART5_SendByte(0xFF);
                    UART5_SendByte(0xFF);
                    disk_count = 0;

                    total_distance += distance;
                    int_to_str(total_distance, str);
                    UART5_SendString("DAvailable.val=");
                    UART5_SendString(str);
                    UART5_SendByte(0xFF);
                    UART5_SendByte(0xFF);
                    UART5_SendByte(0xFF);

                    if(total_distance > 0){
                        remaing_capacity = ((max_capacity * (capacity / 100.0)));
                        energy_consumed = (max_capacity - remaing_capacity) / total_distance;
                        Available_Range = remaing_capacity / energy_consumed;

                        int_to_str(Available_Range, str);
                        UART5_SendString("RAvailable.val=");
                        UART5_SendString(str);
                        UART5_SendByte(0xFF);
                        UART5_SendByte(0xFF);
                        UART5_SendByte(0xFF);
                    }

                default:
                    break;
            }
        }
        else{
            uint8_t current_state = (GPIO_PORTF_DATA_REG  & (1 << PROXIMITY_SENSOR_PIN)) ? 1 : 0;

            if (current_state != previous_state) {
                if (current_state == 1) {
                    disk_count++;
                }
                previous_state = current_state;
            }
        }
    }
    return 0;
}
