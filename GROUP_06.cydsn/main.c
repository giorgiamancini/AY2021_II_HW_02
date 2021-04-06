/* ========================================
 File main.c with cases
 Group_06
 * ========================================
*/



#include "project.h"
#include "stdio.h"
#include "InterruptRoutines.h"
#include "RGBLedDriver.h"
#include "colors.h"
#include <unistd.h>
#include "sys/time.h"



#define IDLE 0
#define HEAD 1
#define RED 2
#define GREEN 3
#define BLUE 4
#define TAIL 5
#define LED_ON 7

#define HEAD_VALUE 160
#define TAIL_VALUE 192
            
volatile uint8_t flag_UART = 0;
volatile uint8_t value;
uint8_t status = 0;
Color color;
int timeout=5;

static char message [20] = {'\0'};

int timeout_idle(int st){
    
    fd_set          input_set;
    struct timeval  timeout;
    int             user_input = 0;

    /* Empty the FD Set */
    FD_ZERO(&input_set );
    /* Listen to the input descriptor */
    FD_SET(0, &input_set);

    
    user_input = select(1, &input_set, NULL, NULL, &timeout);


    if (user_input == -1) {
        status=IDLE;
    } 
    else
    {
        status=st;
    }
 
        return st;
    
}
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
            case IDLE:
            
                if (flag_UART == 1){
                    //sprintf(message, "Write the HEAD value\r\n");
                    //UART_PutString(message);
                    flag_UART = 0;
                    status = HEAD;
                }
                break;
            
            case HEAD:
                
                if (value == HEAD_VALUE){
                    //sprintf(message, "The HEAD value is: %d\r\n", value);
                    //UART_PutString(message);
                    flag_UART = 0;
                    status = RED;
                    
                }
                else if (value == 'v'){
                    sprintf(message, "RGB LED Program $$$\n");
                    UART_PutString(message);
                    flag_UART = 0;
                    status = IDLE;
                }
                break;
                
            case RED:
                if (flag_UART == 1){
                    color.red = value;
                    //sprintf(message, "The RED value is: %d\r\n", color.red);
                    //UART_PutString(message);
                    flag_UART = 0;
                    status =timeout_idle(GREEN);
                }
                break;
            
            case GREEN:
                if (flag_UART == 1){
                    color.green = value;
                    //sprintf(message, "The GREEN value is: %d\r\n", color.green);
                    //UART_PutString(message);
                    flag_UART = 0;
                    status =timeout_idle(BLUE);
                }
                break;
                
            case BLUE:
                if (flag_UART == 1){
                    color.blu = value;
                    //sprintf(message, "The BLUE value is: %d\r\n", color.blu);
                    //UART_PutString(message);
                    flag_UART = 0;
                    status =timeout_idle(TAIL);
                }
                break;
                
            case TAIL:
                if (flag_UART == 1 && value == TAIL_VALUE){
                    //sprintf(message, "The TAIL value is: %d\r\n", value);
                    //UART_PutString(message);        
                    RGBLed_WriteColor(color);
                    flag_UART = 0;
                    status = IDLE;
                }
                break;   
            
         
                
           
            } 
    }            
}
            
    
           


/* [] END OF FILE */