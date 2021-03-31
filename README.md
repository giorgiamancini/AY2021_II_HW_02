# Electronic Technologies and Biosensors Laboratory
## Academic Year 2020/2021 - II Semester
---
## Assignment 02
This repository contains the PSoC Creator workspace to be used as a starting point for the successfull submission of the Assignment 02 of the course "Electronic Technologies and Biosensors Laboratory" of the II Semester of the academic year 2020/2021.

---
### Objective
In this assignment you are required to create a project using [PSoC Creator](https://www.cypress.com/products/psoc-creator-integrated-design-environment-ide), the [CY8CKIT-059 KIT](https://www.cypress.com/documentation/development-kitsboards/cy8ckit-059-psoc-5lp-prototyping-kit-onboard-programmer-and), a [RGB LED](https://www.adafruit.com/product/2739) and three 330 Ohm resistors. 

### Context
The LTEBS srl asks you to design a system able to control the color of the RGB LED through a serial port. The first time the system is powered after programming, the RGB LED should be on with a "black" color (RGB: 0,0,0). Then, with the protocol described here below, the color emitted with the RGB LED could be changed by sending commands through the serial port. The RGB LED has a common anode configuration, and must be connected as follows:

<a href="https://ibb.co/tsBGGY2"><img src="https://i.ibb.co/Fw6rr8Y/Schermata-2020-10-13-alle-17-08-08.png" alt="Schermata-2020-10-13-alle-17-08-08" border="0"></a>

The pins 2.5, 2.6, and 2.7 are the pins on the [CY8CKIT-059 KIT](https://www.cypress.com/documentation/development-kitsboards/cy8ckit-059-psoc-5lp-prototyping-kit-onboard-programmer-and) that you must use to control the channels of the RGB LED.

### Connections
The RED channel of the RGB Led must be connected to pin **2.7**. The GREEN channel of the RGB Led must be connected to pin **2.6**. The BLUE channel of the RGB Led must be connected to pin **2.5**. The UART must use the pins **12.6** and **12.7** in order to communicate through the KitProg.

### Serial Port Communication
#### 1. RGB Color Packet
The commands to be sent through the serial port must be structured as follows:
Byte (HEX)   | Description |
:-----------:|-------------|
0xA0         | Packet header (decimal value: 160)
R            | Red light (0x00-0xFF in Hex, 0-255 in decimal)
G            | Green light (0x00-0xFF in Hex, 0-255 in decimal)
B            | Blue light (0x00-0xFF in Hex, 0-255 in decimal)
0xC0         | Packet tail (decimal value: 192)

The UART peripheral must be configured so as to trigger an interrupt when receiving a single byte (you can configure this kind of behaviour in the Advanced Tab of the UART component in PSoC Creator). A simple state machine will then allow you to perform a parsing of the received data packet:

<a href="https://ibb.co/4mbRfmp"><img src="https://i.ibb.co/bW453WX/Schermata-2020-10-13-alle-09-51-44.png" alt="Schermata-2020-10-13-alle-09-51-44" border="0"></a>

Depending on the received bytes, it is necessary to control the duty cycle of the PWM components that are connected to the RGB LED channels. Here are some working examples:
Received Packet | Color |
:--------------:|-------|
[0xA0, 0xFF, 0x00, 0x00, 0xC0] | RED
[0xA0, 0x00, 0xFF, 0x00, 0xC0] | GREEN
[0xA0, 0x7F, 0x00, 0x7F, 0xC0] | PURPLE

If the system finds itself in one of the intermediate states of the state machine (i.e., exlcuding the IDLE and TAIL states), a 5 seconds timeout must be started in order to allow the system to go back to the idle state if no command is received in 5 seconds.

#### 2. Timeout Configuration
As the 5 seconds of timeout may not be sufficient when manually testing the program from a serial monitor (e.g., CoolTerm), a packet can be sent to configure the number of seconds before triggering a timeout condition:

Byte (HEX)   | Description |
:-----------:|-------------|
0xA1         | Packet header (decimal value: 161)
T            | Number of seconds for timeout (1-20)
0xC0         | Packet tail (decimal value: 192)

#### 3. Connection Command
Upon receiving the character `v`, the system must send this string on the serial port:

`RGB LED Program $$$`

This is required for the subsequent testing of the program through a simple GUI.

### Test
You can test your program using a GUI designed in Kivy that we provide to you. You are required to download the zip file of the GUI, named *RGB_Led_Control.zip* from the course webpage. Unzip the file, and inside the folder *rgbledcontrol*, double click on `rgbcontrol.exe`. The GUI will automatically scan all the available serial ports, connect to the right one and will allow you to send a new color with the described packet structure.

<a href="https://ibb.co/wdj0GVG"><img src="https://i.ibb.co/L0Hkjmj/Schermata-2020-10-13-alle-17-01-23.png" alt="Schermata-2020-10-13-alle-17-01-23" border="0"></a>

The GUI only work on Windows. For those of you who are using VirtualBox, please configure the display settings as follows:

<a href="https://ibb.co/zZtznx9"><img src="https://i.ibb.co/Hq9jF4w/2020-10-13-17-02-35.jpg" alt="2020-10-13-17-02-35" border="0"></a>

When using the GUI, please make sure to close all the active serial connections with the CY8CKIT-059 (e.g., CoolTerm, Bridge Control Panel, ...)
### Setup and Assignment Delivery
- One student from the team forks this repository 
- Following the forking of the repository, the other team member(s) are added as collaborators via GitHub (as you did with assignment #01)
- Each team member clones the forked repository by entering the following command in the terminal:

    `git clone https://github.com/[your_username]/AY2021_II_HW_02.git`
- Open up the workspace in PSoC Creator
- Activate the project with your group number
- Work on your project
- The last commit must be named '*GROUP_XX* Final Delivery', where XX is the group number
- One member of the team proposes a pull request before the deadline for the assignment, which is on **April 9 at 6PM**


### Evaluation
The evaluation of the assignment will take into consideration the following aspects of the project:
- successful build process without any warning/error generated
- successful programming without any error generated
- correct functioning of the device as per the requirements
- code organization (header and source files, useful comments)
- schematic organization and documentation
- successful use of git for version control (use of branches, ...)
- successful collaboration with git across group members
- successful pull request on GitHub


### Additional Resources:
- [PSoC PWM Component Datasheet](https://www.cypress.com/file/376411/download)
- [PSoC 5LP Interrupts](https://www.cypress.com/file/44256/download)
- [PSoC 101 Video Tutorial](https://www.youtube.com/watch?v=LrXXpQr1itY&list=PLX6sqqUB8iOjsMfGEDcsPSuYLEFCh50Hr)