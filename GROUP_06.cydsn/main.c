/* ========================================

Electronic Technologies and Biosensors Laboratory
Academic Year 2020/2021 - II Semester
Assignment 02
GROUP_06 

main source file

 * ========================================
*/


/*
Include the necessary headers and libraries
*/
#include "project.h"
#include "stdio.h"
#include "InterruptRoutines.h"
#include "RGBLedDriver.h"
#include "colors.h"

/*
Define the system states
*/
#define IDLE 0
#define HEAD 1
#define SET_TIMEOUT 2
#define END_SET_TIMEOUT 3
#define RED 4
#define GREEN 5
#define BLUE 6
#define TAIL 7

/*
Define the decimal values for the packet headers and tails
*/
#define HEAD_VALUE 160      // RGB Color Packet header
#define TAIL_VALUE 192      // RGB Color Packet tail
#define TIMER_VALUE 161     // Timeout configuration header

#define F_ISR 1000          // Frequency of the timer
            
volatile uint8_t flag_UART = 0;     // 0 if no new byte is available, 1 if a new byte is available
volatile uint8_t value;             // Value of the currently available byte
volatile uint16_t count;            // Timer
const Color BLACK = {0,0,0};        // RGB value of the color BLACK
uint8_t status = 0;                 // Current state of the system, initialised to IDLE
uint8_t timeout = 5;                // Seconds of timeout timer
Color color;                        // Struct storing RGB LED values

static char message [20] = {'\0'};

/***************************************
*         state_change
***************************************/
/*

This function brings the system from one state to another. 
arguments:
-   new_state: state to bring the system to (IDLE, HEAD, SET_TIMEOUT, END_SET_TIMEOUT, RED, GREEN, BLUE, TAIL)
-   flag_UART: indicate wether there is a new readable byte available
-   count: timer
-   status: state of the system
-   count_reset: if the value 1 is passed, the counter is reset to 0.

*/

void state_change(int new_state, volatile uint8_t* flag_UART, volatile uint16_t* count, volatile uint8_t* status, int count_reset)
{
    if (count_reset==1)
    {
        *count=0;           // Restart the timer
    }
   
    *flag_UART=0;           // Indicate that there are no new readable bytes
    *status=new_state;      // Assign the new state
}


/***************************************
*          revert_to_idle
***************************************/
/*

Reverts the system to IDLE state and notifies the user of the change.

*/

void revert_to_idle()
{
    sprintf(message, "State: IDLE\n");                  // Notify the user the system is back to IDLE state
    UART_PutString(message);
    state_change(IDLE, &flag_UART, &count, &status,1);  // Revert to IDLE state
}

/***************************************
*                main
***************************************/

int main(void)
{
    // Start the various components and interrupts
    RGBLed_Start();    
    UART_Start();
    TIMER_SET_Start();
    ISR_UART_StartEx(Custom_UART_RX_ISR);
    ISR_TIMER_StartEx(Custom_TIMER_SET_ISR);
    
    RGBLed_WriteColor(BLACK);
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    for(;;)
     {   
        switch(status)
        {
            case IDLE:
                if (flag_UART == 1)     // check if a new byte is present
                {
                    state_change(HEAD, &flag_UART, &count, &status,1);      // Switch to HEAD state
                }
                break;
            
            case HEAD:                
                if (count == timeout*F_ISR && flag_UART == 0)       // check if the timeout is reached and no new byte is available
                {
                    revert_to_idle();
                }
                
                else if (value == TIMER_VALUE)      // check if the received value corresponds to the header for the timeout configuration packet
                {
                    state_change(SET_TIMEOUT, &flag_UART, &count, &status,1);       // Switch to timeout configuration state
                }
                    
                else if (value == HEAD_VALUE)       // check if the received value corresponds to the header for the RGB color packet
                {
                    state_change(RED, &flag_UART, &count, &status,1);   // Switch to RED state
                }
                
                else if (value == 'v'){             // check if the new value is the connection command 'v'
                    sprintf(message, "RGB LED Program $$$\n");
                    UART_PutString(message);
                    state_change(IDLE, &flag_UART, &count, &status,1);  // Go back to IDLE state
                }
                
                break;
                
            case SET_TIMEOUT:

                if (flag_UART == 1 && value>0 && value<=20)             // Check if the received timeout value is between 0 and 20 seconds
                {
                    timeout = value;                                                // Set the new timeout
                    state_change(END_SET_TIMEOUT, &flag_UART, &count, &status,1);   // Exit the timeout configuration
                }
                else if (flag_UART==1 && value <=0)                      // If the timeout value is negative or null, default to timeout = 5 seconds
                {
                    timeout = 5;
                    state_change(END_SET_TIMEOUT, &flag_UART, &count, &status,1);
                }
                else if (flag_UART==1 && value >20)                     // If the timeout value is greater than 20 seconds, default to timeout = 20 seconds
                {
                    timeout = 20;
                    state_change(END_SET_TIMEOUT, &flag_UART, &count, &status,1);
                }
                break;
                
            case END_SET_TIMEOUT:

                if (flag_UART == 1 && value == TAIL_VALUE)              // Check if a new byte is available and it corresponds to the tail of the timer configuration packet
                {
                    state_change(IDLE, &flag_UART, &count, &status,1);  // Go back to IDLE state
                }
                break;
                
            case RED:
                
                if (count == timeout*F_ISR && flag_UART == 0)           // Check if timeout is reached before a new byte is received
                {
                    revert_to_idle();                                   // Revert to IDLE state
                }
                
                else if (flag_UART == 1)                                        // Check if a new byte is received before timeout is reached
                {
                    color.red = value;                                          // Store the received byte as the RED value
                    sprintf(message, "The RED value is: %d\r\n", color.red);    // Display the received byte
                    UART_PutString(message);
                    state_change(GREEN, &flag_UART, &count, &status,1);         // Switch to GREEN state
                }
                break;
            
            case GREEN:
                
                if (count == timeout*F_ISR && flag_UART == 0)           // Check if timeout is reached before a new byte is received
                {
                    revert_to_idle();                                   // Revert to IDLE state
                }
                
                else if (flag_UART == 1)
                {
                    color.green = value;                                            // Store the received byte as the GREEN value
                    sprintf(message, "The GREEN value is: %d\r\n", color.green);    // Display the received byte
                    UART_PutString(message);
                    state_change(BLUE, &flag_UART, &count, &status,1);              // Switch to BLUE state
                }
                break;
                
            case BLUE:
                
                if (count == timeout*F_ISR && flag_UART == 0)           // Check if timeout is reached before a new byte is received
                {
                    revert_to_idle();                                   // Revert to IDLE state
                }
                
                else if (flag_UART == 1)
                {
                    color.blu = value;                                          // Store the received byte as the BLUE value
                    sprintf(message, "The BLUE value is: %d\r\n", color.blu);   // Display the received byte
                    UART_PutString(message);
                    state_change(TAIL, &flag_UART, &count, &status,1);          // Switch to TAIL state
                }
                break;
                
            case TAIL:
                
                if (count == timeout*F_ISR && flag_UART == 0)           // Check if timeout is reached before a new byte is received
                {
                    revert_to_idle();                                   // Revert to IDLE state
                }
                
                else if (flag_UART == 1 && value == TAIL_VALUE){
                    sprintf(message, "The TAIL value is: %d\r\n", value);       // Display the received byte
                    UART_PutString(message);        
                    RGBLed_WriteColor(color);                                   // Change the LED color
                    state_change(IDLE, &flag_UART, &count, &status,1);          // Switch to IDLE state
                }
                break;   
            
        } 
     }            
}
            
    
           


/* [] END OF FILE */