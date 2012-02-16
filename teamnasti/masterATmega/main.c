/*****************************************************************************************
* Author: Jeremy Borman & Terry Max Christy
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Main Microcontroller Code
* Date written: 2/2/12 Last Modified: 2/2/12

* The code provided in this file will read in various signals from an R/C reciever and 
* using timers and interupt logic determine the pulse width of each signal. Using this 
* information, the program can determine a signal to be output onto the RS232 serial 
* communication line to the motor control unit that will provide a pwm and direction signal
* to the motor signal based on the information provided. 
*****************************************************************************************/

#include "main.h"

int main(void) {
	//Enable interrupts, configure timers, uarts, i2c, and ports		
	sei();
    //i2c_init();
    configUART0();
	initTimer0();
	initCapture1();
	initCapture3();	
	//configADC();	
	
	// set up I/O pins
	dataDirection();
	
	while (1==1) {
		// If the emergency R/C override is set, use the signals from the 
		// transmitter to determine motor control
		if (isSwitchOn(dutyCount4)){
			if(okToTrans==1){
				okToTrans=0;
										
				motorInfo[0] ='*';
				motorInfo[1] ='*';
				motorInfo[2] ='*';
			
				//Convert Main Thrusters
				motorInfo[5] = adjToMotorPacket(dutyCount1);
				motorInfo[6] = adjToMotorPacket(dutyCount1);
			
												
				//Convert RC Signals to Thruster Signals
				if (!adjToMotorPacket(dutyCount3)) {
					motorInfo[3] = adjToMotorPacket(dutyCount2);
					motorInfo[4] = adjToMotorPacket(dutyCount2);
					motorInfo[7] = adjToMotorPacket(dutyCount2);
					motorInfo[8] = adjToMotorPacket(dutyCount2);
				}

				else {
				//Convert RC Signals to Differential Motors
				motorInfo[3] = adjToMotorPacket(dutyCount2);
				motorInfo[8] = adjToMotorPacket(dutyCount2);

				motorInfo[4] = adjToMotorPacket(dutyCount2)^0x01;
				motorInfo[7] = adjToMotorPacket(dutyCount2)^0x01;
			
				}
		
				//Send the converted info to the ATMega328
				//sendString(motorInfo,9);
			
				sendByte(' ');
				sendByte('D');
				sendByte('1');
				sendByte(':');			
				sendIntHex((int)adjToMotorPacket(dutyCount1));

				sendByte(' ');
				sendByte('D');
				sendByte('2');
				sendByte(':');			
				sendIntHex((int)adjToMotorPacket(dutyCount2));

				sendByte(' ');
				sendByte('D');
				sendByte('3');
				sendByte(':');			
				sendIntHex((int)adjToMotorPacket(dutyCount3));
				
				unsigned char space =0;
				for (space = 0; space < 16; space ++){

				sendByte(' ');
				}
/*
				sendByte(' ');
				sendByte('S');
				sendByte('1');
				sendByte(':');			
				sendIntHex(dutyCount4);
*/
			}
	    }
	}
}
