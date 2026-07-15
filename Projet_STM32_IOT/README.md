@par Application description
This application show how to send data from sensor (potentiometer: using ADC) to a server using the STM32F4 MCU and a WiFi module ESP8266-E01.

@par Hardware and Software environment 
Below the different steps of the process:

  -1- connect the module to the STM32F4-Discovery board:
      
              _____________________              _____________________
             |                     |            |                     |
             |                  Tx - ---------- - Rx-PA3              |
             |      ESP8266     RST- ---------- - 3V3     STM32F4     |
             |                  Rx - ---------- - Tx-PA2              |
             |        E01       EN - ---------- - 3V3      Disco      |
             |                  GND- ---------- - GND                 |
             |                  3V3- ---------- - 3V3                 |
             |_____________________|            |_____________________|

  -2- connect the potentiometer to the STM32F4-Discovery board:

  Potentiometer with outside connections connected to 3V and GND on the STM32F4-Disc. The center tap of the potentiometer goes to pin A1.

  -3- Set the STM32F4 ADC as following:
       - Input : IN1 (PA1)
       - Resolution = 8 bits ( It means that the maximum value available is 2^8 = 256.
          So we can handle 256 different values, from 0 to 256.)

  -4- Set the STM32F4 USART as following:
       - Baude Rate = 115200
       - Parity     = none
       - Stop bit   = 1
       - wordLength = 8 Bit 

  -5- All the ESP8266 commands are set in the main.h file
