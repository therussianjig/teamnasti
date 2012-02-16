/*****************************************************************************************
* Author: Jeremy Borman Edited:
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Main Microcontroller Code
* Date written: 2/2/12 Last Modified: 2/2/12
*****************************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

#include <inttypes.h>
unsigned char adjToMotorPacket(unsigned int intMotor);
unsigned char isSwitchOn(unsigned int switchState);
