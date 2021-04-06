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

#include "InterruptRoutines.h"
#include "stdio.h"
#include "RGBLedDriver.h"




extern volatile uint8_t flag_UART;
extern volatile uint8_t value;
extern volatile uint16_t count;


            
CY_ISR(Custom_UART_RX_ISR)
{
    if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY){  
        value = UART_ReadRxData();
        flag_UART = 1;    
    }
}

CY_ISR(Custom_TIMER_SET_ISR)
{
    TIMER_SET_ReadStatusRegister();
    count++; 
}


/* [] END OF FILE */
