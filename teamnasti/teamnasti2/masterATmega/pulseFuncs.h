/*****************************************************************************************
* Author: Jeremy Borman Edited: Terry Max Christy
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Main Microcontroller Code
* Date written: 2/2/12 Last Modified: 2/2/12
*****************************************************************************************/

/* fine the data direction of the I/O pins */
void dataDirection(void)
{
	DDRB = 0xFF; //Debugging Port
	PORTB=0x00;	

	DDRF = 0x00;
	PORTF = 0x00; //no pullup


	DDRE = 0x00;
	PORTE = 0x00; //no pullup

	DDRA = (1<<DDA0);
}


/* Grab the voltage from the temperature sensor from the ADC register */
void getTemp(void) {
	ADCSRA |= (1 << ADSC);
}

unsigned char goingRight(unsigned char motor){ 
	return !(motor&0x01);
}

unsigned char goingLeft(unsigned char motor){
	return (motor&0x01);
}

unsigned char readMemLoc(unsigned char addr, unsigned char loc) {
	unsigned char checkCom;

	checkCom = i2c_start(addr+I2C_WRITE);
	if (checkCom) {
		i2c_stop();	                            
	}
	
	else { 
		i2c_start_wait(addr+I2C_WRITE);     
		i2c_write(loc);               
		i2c_rep_start(addr+I2C_READ);       
		globBuff = i2c_readNak(); 
		i2c_stop();
	}
	return globBuff;
}
