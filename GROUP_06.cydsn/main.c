/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/



#include "project.h"
#include "stdio.h"
#include "InterruptRoutines.h"
#include "RGBLedDriver.h"
#include "colors.h"

#define START 0
#define HEAD 1
#define RED 2
#define GREEN 3
#define BLUE 4
#define TAIL 5

#define HEAD_VALUE 160
#define TAIL_VALUE 192
            
volatile uint8_t flag_UART = 0;
uint8_t value;

uint8_t status = 0;
uint8_t red;
uint8_t green;
uint8_t blu;

static char message [20] = {'\0'};

int main(void)
{
    
    RGBLed_Start();
    RGBLed_WriteColor(BLACK);
    
    UART_Start();
    ISR_UART_StartEx(Custom_UART_RX_ISR);
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    for(;;)
     {   
        switch(status){
            case START:
                if (flag_UART == 1){
                    sprintf(message, "Write the HEAD value\r\n");
                    UART_PutString(message);
                    flag_UART = 0;
                    status = HEAD;
                }
                break;
            
            case HEAD:
                if (value == HEAD_VALUE){
                    sprintf(message, "The HEAD value is: %d\r\nWrite the RED value\r\n", value);
                    UART_PutString(message);
                    flag_UART = 0;
                    status = RED;
                }
                else if (value == 'v'){
                    sprintf(message, "RGB LED Program $$$\r\n");
                    UART_PutString(message);
                    flag_UART = 0;
                    status = START;
                }
                break;
                
            case RED:
                if (flag_UART == 1){
                    red = value;
                    sprintf(message, "The RED value is: %d\r\nWrite the GREEN value\r\n", red);
                    UART_PutString(message);
                    flag_UART = 0;
                    status = GREEN;
                }
                break;
            
            case GREEN:
                if (flag_UART == 1){
                    green = value;
                    sprintf(message, "The GREEN value is: %d\r\nWrite the BLUE value\r\n", green);
                    UART_PutString(message);
                    flag_UART = 0;
                    status = BLUE;
                }
                break;
                
            case BLUE:
                if (flag_UART == 1){
                    blu = value;
                    sprintf(message, "The BLUE value is: %d\r\nWrite the TAIL value\r\n", blu);
                    UART_PutString(message);
                    flag_UART = 0;
                    status = TAIL;
                }
                break;
                
            case TAIL:
                if (flag_UART == 1 && value == TAIL_VALUE){
                    sprintf(message, "The TAIL value is: %d\r\n", value);
                    UART_PutString(message);
                    Color COLOR = {red, green, blu};
                    RGBLed_WriteColor(COLOR);
                    flag_UART = 0;
                    status = START;
                }
                break;        
            } 
    }            
}
            
    
           


/* [] END OF FILE */