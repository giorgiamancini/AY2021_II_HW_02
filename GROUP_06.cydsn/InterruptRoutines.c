/* ========================================
 *
Electronic Technologies and Biosensors Laboratory
Academic Year 2020/2021 - II Semester
Assignment 02
GROUP_06 

ISR source file

-   Custom_UART_RX_ISR: the UART triggers an interrupt as soon as a single byte is received
-   Custom_TIMER_SET_ISR: timeout for user input during inbetween states

 *
 * ========================================
*/

#include "InterruptRoutines.h"
#include "stdio.h"
#include "RGBLedDriver.h"

extern volatile uint8_t flag_UART;
extern volatile uint8_t value;
extern volatile uint16_t count;

/***************************************
*         Custom_UART_RX_ISR
***************************************/

/* The interrupt is triggered by a single byte received */

CY_ISR(Custom_UART_RX_ISR)
{
    if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY)
    { 
        /* Store the current byte in the variable "value" */
        value = UART_ReadRxData();
        
        /* the variable "flag_UART" is set to 1 to indicate that the user has sent a byte */
        flag_UART = 1;    
    }
}

/***************************************
*         Custom_TIMER_SET_ISR
***************************************/

/* The interrupt starts counting */

CY_ISR(Custom_TIMER_SET_ISR)
{
    TIMER_SET_ReadStatusRegister();
    count++; 
}


/* [] END OF FILE */
