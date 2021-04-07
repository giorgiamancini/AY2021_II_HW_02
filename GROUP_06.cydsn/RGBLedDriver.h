/* ========================================
Electronic Technologies and Biosensors Laboratory
Academic Year 2020/2021 - II Semester
Assignment 02
GROUP_06 

Header file for the RGB Led interface

*/



#ifndef __RGB_LED_DRIVER_H__
    #define __RGB_LED_DRIVER_H__
    
    #include "project.h"
   
    // Define a struct containing RGB color data
    
    typedef struct {
        uint8_t red;     // red value 0-255
        uint8_t green;   // green value 0-255
        uint8_t blu;     // blu value 0-255
    } Color;
    
    // Starting and stopping the driver
    void RGBLed_Start(void);
    
    void RGBLed_Stop(void);
    
    // Manually chosing the LED color
    void RGBLed_WriteColor(Color color);
    
    
#endif


/* [] END OF FILE */
