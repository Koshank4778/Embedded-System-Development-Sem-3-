                                                 Task 1.1P – Switching ON Lights

                                                      Project Description
This project uses an Arduino Nano 33 IoT, a push button and two LEDs to create a simple lighting system for Linda's home.
When the button is pressed, both lights turn ON. The porch light stays ON for 30 seconds, while the hallway light stays ON for 60 seconds.

------------------------------------------------------------------------------------------------------------------------------------------

                                                           Components
Arduino Nano 33 IoT
Push button
Red LED – Porch light
Green LED – Hallway light
Resistors
Jumper wires
Pin Configuration
D8 – Porch LED
D7 – Hallway LED
D6 – Push button

------------------------------------------------------------------------------------------------------------------------------------------

                                                         Code Overview

The setup() function configures the LEDs as outputs and the button as an input using INPUT_PULLUP. Both LEDs are initially turned OFF.
The loop() function checks for a button press. When the button is pressed, both LEDs turn ON. After 30 seconds, the porch LED turns OFF, and after another 30 seconds, the hallway LED turns OFF.
The program uses separate sections for setup, button detection and light control to make the code easier to understand and modify.