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

#define IDLE 0
#define HEAD 1
#define SET_TIMEOUT 2
#define END_SET_TIMEOUT 3
#define RED 4
#define GREEN 5
#define BLUE 6
#define TAIL 7


#define HEAD_VALUE 160
#define TAIL_VALUE 192
#define TIMER_VALUE 161
#define F_ISR 1000
            
volatile uint8_t flag_UART = 0;
volatile uint8_t value;
volatile uint16_t count;
const Color BLACK = {0,0,0}; // posso definirlo qui senza fare il file colors.h?
uint8_t status = 0;
uint8_t timeout = 5;
Color color;

static char message [20] = {'\0'};

int main(void)
{
    
    RGBLed_Start();    
    UART_Start();
    TIMER_SET_Start();
    ISR_UART_StartEx(Custom_UART_RX_ISR);
    ISR_TIMER_StartEx(Custom_TIMER_SET_ISR);
    
    RGBLed_WriteColor(BLACK);
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    for(;;)
     {   
        switch(status){
            case IDLE:
                if (flag_UART == 1){
                    //sprintf(message, "Write the HEAD value\r\n");
                    //UART_PutString(message);
                    flag_UART = 0;
                    count = 0;
                    status = HEAD;
                }
                break;
            
            case HEAD:
                
                if (count == timeout*F_ISR && flag_UART == 0){
                    count = 0;
                    status = IDLE;
                }
                
                else if (value == TIMER_VALUE){
                    //sprintf(message, "The HEAD value is: %d\r\n", value);
                    //UART_PutString(message);
                    flag_UART = 0;
                    count = 0;
                    status = SET_TIMEOUT;
                }
                    
                else if (value == HEAD_VALUE){
                    //sprintf(message, "The HEAD value is: %d\r\n", value);
                    //UART_PutString(message);
                    flag_UART = 0;
                    count = 0;
                    status = RED;
                }
                else if (value == 'v'){
                    sprintf(message, "RGB LED Program $$$\n");
                    UART_PutString(message);
                    //count = 0 ???
                    flag_UART = 0;
                    status = IDLE;
                }
                break;
                
            case SET_TIMEOUT:

                if (flag_UART == 1 && value>0 && value<=20){ // controllo se il valore del timer è tra 0 e 20 secondi
                    timeout = value;
                    count = 0;
                    flag_UART = 0;
                    status = END_SET_TIMEOUT;
                }
                break;
                
            case END_SET_TIMEOUT:

                if (flag_UART == 1 && value == TAIL_VALUE){
                    //sprintf(message, "The new timeout is %d\r\n", timeout);
                    //UART_PutString(message); NON LO STAMPA NON SO PERCHè???
                    count = 0;
                    flag_UART = 0;
                    status = IDLE;
                }
                break;
                

            case RED:
                
                if (count == timeout*F_ISR && flag_UART == 0){
                    sprintf(message, "Come back to initial state\n"); //stampo solo per vedere se funziona
                    UART_PutString(message);
                    count = 0;
                    status = IDLE;
                }
                
                else if (flag_UART == 1){
                    color.red = value;
                    //sprintf(message, "The RED value is: %d\r\n", color.red);
                    //UART_PutString(message);
                    flag_UART = 0;
                    count = 0;
                    status = GREEN;
                }
                break;
            
            case GREEN:
                
                if (count == timeout*F_ISR && flag_UART == 0){
                    sprintf(message, "Come back to initial state\n"); //stampo solo per vedere se funziona
                    UART_PutString(message);
                    count = 0;
                    status = IDLE;
                }
                
                else if (flag_UART == 1){
                    color.green = value;
                    //sprintf(message, "The GREEN value is: %d\r\n", color.green);
                    //UART_PutString(message);
                    count = 0;
                    flag_UART = 0;
                    status = BLUE;
                }
                break;
                
            case BLUE:
                
                if (count == timeout*F_ISR && flag_UART == 0){
                    sprintf(message, "Come back to initial state\n"); //stampo solo per vedere se funziona
                    UART_PutString(message);
                    count = 0;
                    status = IDLE;
                }
                
                else if (flag_UART == 1){
                    color.blu = value;
                    //sprintf(message, "The BLUE value is: %d\r\n", color.blu);
                    //UART_PutString(message);
                    count = 0;
                    flag_UART = 0;
                    status = TAIL;
                }
                break;
                
            case TAIL:
                
                if (count == timeout*F_ISR && flag_UART == 0){
                    sprintf(message, "Come back to initial state\n"); //stampo solo per vedere se funziona
                    UART_PutString(message);
                    count = 0;
                    status = IDLE;
                }
                
                else if (flag_UART == 1 && value == TAIL_VALUE){
                    //sprintf(message, "The TAIL value is: %d\r\n", value);
                    //UART_PutString(message);        
                    RGBLed_WriteColor(color);
                    count = 0;
                    flag_UART = 0;
                    status = IDLE;
                }
                break;   
            
         
                
           
            } 
    }            
}
            
    
           


/* [] END OF FILE */