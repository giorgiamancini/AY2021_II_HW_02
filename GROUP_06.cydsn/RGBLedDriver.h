/* ========================================
 *
 * Header file for the RGB Led interface
 * /Group 06
*/



#ifndef __RGB_LED_DRIVER_H__
    #define __RGB_LED_DRIVER_H__
    
    #include "project.h"
   
    //I want black as first color so I define Struct holding color data to create the three channels
    
    typedef struct {
        uint8_t red; //red value 0-255
        uint8_t green; //green value 0-255
        uint8_t blu; //blu value 0-255
    } Color;
    
    
    
    // per iniziare e stoppare il driver
    void RGBLed_Start(void);
    
    void RGBLed_Stop(void);
    
    //to change color manually on the led 
    void RGBLed_WriteColor(Color c);
    
    
#endif


/* [] END OF FILE */
