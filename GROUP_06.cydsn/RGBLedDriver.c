/* ========================================
 *
Electronic Technologies and Biosensors Laboratory
Academic Year 2020/2021 - II Semester
Assignment 02
GROUP_06 

Source file for the LED driver

 *
*/


#include "RGBLedDriver.h"
#include "InterruptRoutines.h"

static void RGBLed_WriteRed(uint8_t red);
static void RGBLed_WriteGreen(uint8_t green);
static void RGBLed_WriteBlu(uint8_t blu);

/* Start the RGB LED */
void RGBLed_Start()
{
    PWM_RG_Start();
    PWM_B_Start();
}

/* Stop the RGB LED */
void RGBLed_Stop()
{
    PWM_RG_Stop();
    PWM_B_Stop();
}

/* Change the color of the RGB LED */
void RGBLed_WriteColor(Color c)
{
    RGBLed_WriteRed(c.red);
    RGBLed_WriteGreen(c.green);
    RGBLed_WriteBlu(c.blu);
}

/* Assign the RED channel of the RGB Led */
static void RGBLed_WriteRed(uint8_t red)
{
    PWM_RG_WriteCompare1(red);
}

/* Assign the GREEN channel of the RGB Led */
static void RGBLed_WriteGreen(uint8_t green)
{
    PWM_RG_WriteCompare2(green);
}

/* Assign the BLUE channel of the RGB Led */
static void RGBLed_WriteBlu(uint8_t blu)
{
    PWM_B_WriteCompare(blu);
}



/* [] END OF FILE */
