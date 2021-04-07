/* ========================================
Electronic Technologies and Biosensors Laboratory
Academic Year 2020/2021 - II Semester
Assignment 02
GROUP_06 

Header file for the interrupt service routines

-   Custom_UART_RX_ISR: the UART triggers an interrupt as soon as a single byte is received
-   Custom_TIMER_SET_ISR: timeout for user input during inbetween states

 * ========================================
*/


#ifndef __INTERRUPT_ROUTINES_H__
    #define __INTERRUPT_ROUTINES_H__
    
    #include "project.h"
    
    CY_ISR_PROTO(Custom_UART_RX_ISR);
    CY_ISR_PROTO(Custom_TIMER_SET_ISR);
    
#endif



/* [] END OF FILE */



