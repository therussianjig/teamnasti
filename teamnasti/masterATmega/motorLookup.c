/*****************************************************************************************
* Author: Jeremy Borman Edited: 
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Main Microcontroller Code
* Date written: 2/2/12 Last Modified: 2/2/12
*****************************************************************************************/

#include "motorLookup.h"

unsigned char isSwitchOn(unsigned int switchState){
	unsigned char retVal;
	
	if (switchState < 0x800) { 
		retVal = 1;
	}

	else {
		retVal = 0;
	}

	return retVal;
}

unsigned char adjToMotorPacket(unsigned int intMotor){
	unsigned char charVal;

	if (intMotor >=3013 + 30) {
		if (intMotor >=3307) {
			if (intMotor > 3699) {
				charVal = 255;
			}

			else if (intMotor > 3650) {
				charVal = 241;
			}

			else if (intMotor > 3601) {
				charVal = 227;
			}

			else if (intMotor > 3552) {
				charVal = 213;
			}

			else if (intMotor > 3503) {
				charVal = 199;
			}

			else if (intMotor > 3459) {
				charVal = 185;
			}

			else if (intMotor > 3405) {
				charVal = 171;
			}

			else if (intMotor > 3356) {
				charVal = 157;
			}

			else {
				charVal = 143;
			}
		}
	
		else {
			if (intMotor > 3258) {
				charVal = 129;
			}

			else if (intMotor > 3209) {
				charVal = 115;
			}

			else if (intMotor > 3160) {
				charVal = 101;
			}

			else if (intMotor > 3111) {
				charVal = 87;
			}

			else if (intMotor > 3062) {
				charVal = 73;
			}

			else {
				charVal = 59;
			}
	
		}
		charVal = charVal & 0xFE;
	}
	
	else if (intMotor <= 2973){ //going the other way 
		if (intMotor <= 2679) {
			if (intMotor < 2287) { 
				charVal = 255;
			}

			else if (intMotor < 2336){
				charVal = 241;
			}

			else if (intMotor < 2385){
				charVal = 227;
			}

			else if (intMotor < 2434) {
				charVal = 213;
			}

			else if (intMotor < 2483) {
				charVal = 199;
			}

			else if (intMotor < 2532) {
				charVal = 185;
			}

			else if (intMotor < 2581) {
				charVal = 171;
			}

			else if (intMotor < 2630) {
				charVal = 157;
			}

			else  {
				charVal = 143;
			}

		}

		else {
			if (intMotor < 2728) {
				charVal = 129;
			}

			else if(intMotor < 2777) {
				charVal = 115;
			}

			else if(intMotor < 2826) {
				charVal = 101;
			}

			else if(intMotor < 2875) {
				charVal = 87;
			}

			else if (intMotor < 2924){
				charVal = 73;
			}

			else {
				charVal = 59;
			}

		}

		charVal = charVal | 0x01;
	}

	else {
		charVal = 0;
	}

	return charVal;
}
