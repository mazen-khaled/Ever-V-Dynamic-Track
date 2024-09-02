# Ever-V-Dynamic-Track
As the Embedded Team Leader at Shoubra Racing Team, I had the privilege of working alongside my Team on Our SRT Vehicle in EVER V Competition. 

![1723933682316](https://github.com/user-attachments/assets/22d36326-e240-43b5-a55c-9fa3b617e009)

# Embedded System Stack:
![Embedded Stack](https://github.com/user-attachments/assets/c4bca202-f7de-4d7b-8e59-2f2b9b5a7722)

# Our Vehicle Contain two essential ECUs: 
# 🔋 Battery ECU: 
- Measures battery capacity, voltage drop, and discharging current.
- Transmits data to the Gate ECU via UART.
- Drivers Used: ADC, TIMER, GPIO, UART.
- Hardware Used: ATmega32, shunt, voltage divider.

# PCB Designed Using Altium design:
## Schematic
![image](https://github.com/user-attachments/assets/1e729f6c-5594-4678-b3a0-460495e25dab)

## PCB
![image](https://github.com/user-attachments/assets/5107a0f8-a0b7-42de-88c2-b2ca25308ea4)


# 🚦 Gate ECU:
- Measures vehicle speed, receives data from the Battery ECU, and Take Reading From all car switches.
- Displays data on the Nextion display using UART.
- Using Time trigger RTOS To handel data sending to the display.
- Drivers Used: UART 1 for Battery ECU and UART 5 For the screen, SYSTICK TIMER, GPIO
- Hardware Used: Tiva C, proximity sensor, Nextion display

![1723933681656](https://github.com/user-attachments/assets/8206a351-e89c-4ec1-95af-8d8d540353e2)


