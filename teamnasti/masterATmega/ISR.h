/*****************************************************************************************
* Author: Jeremy Borman & Terry Max Christy
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Main Microcontroller Code
* Date written: 2/2/12 Last Modified: 2/2/12

* This file contains all the ISR functions, the ISRs use the interrupt vector
* locations specified in iom128.h file and they make use of global variables
* stored in GVAR.h
*****************************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

ISR(INT4_vect) {
	unsigned char sreg;

	if(risingExt == 1 ) {
		
		EICRB &= 0xFE; //Falling Edge
		risingExt = 0; 
		TCNT3=0;
	}

	else { 
		
		sreg = SREG;
		cli();
		dutyCount4=TCNT3; //TCNT1; //or should it be ICR1??
		
		SREG = sreg;
		sei();

		risingExt = 1; 
		TCNT3 = 0; //Since small duty cycle, clear for falling edge
		
		EICRB |=0x01;

		if(cycleCount1 > 3)
		{
			EIMSK = 0x00;
			
			ETIMSK |= (1<<TICIE3);
			cycleCount1 = 0;
		}
		else {cycleCount1++;}
	}
}

ISR(TIMER3_CAPT_vect) {

	unsigned char sreg;

	if(rising3 == 1 ) {

		TCCR3B = 0x82; //falling edge, noise cancel, prescaler 1024
		rising3 = 0; 
		TCNT3=0;
	}

	else { 
		
		sreg = SREG;
		cli();
		dutyCount3=ICR3; //TCNT1; //or should it be ICR1??

		SREG = sreg;
		sei();
		TCCR3B = 0xC2; //rising edge, noise cancel, prescaler 1024
		rising3 = 1; 
		TCNT3 = 0; //Since small duty cycle, clear for falling edge


		EICRB |= (1 << ISC41);
		EICRB |= (1 << ISC40);

		if(cycleCount2 > 3)
		{

	
			ETIMSK &= 0xDF;
			EIMSK |= (1 << INT4);
			cycleCount2 = 0;
		}
		else {cycleCount2++;}
	}
}




ISR(TIMER1_CAPT_vect){

	unsigned char sreg;
	unsigned int dutyTemp=0;
	
	//sendByte('J');
	if(rising1 == 1 ) {
		
		TCCR1B = 0x82; //falling edge, noise cancel, prescaler 1024
		rising1 = 0; 
		TCNT1=0;
	}
	

	else { 
		sreg = SREG;
		cli();
		dutyTemp=ICR1; //TCNT1; //or should it be ICR1??

		SREG = sreg;
		sei();
		TCCR1B = 0xC2; //rising edge, noise cancel, prescaler 1024
		rising1 = 1; 
		TCNT1 = 0; //Since small duty cycle, clear for falling edge

		if (ICR1conf == ADC_ICR_DISABLE_ON_1) {
			dutyCount1 = dutyTemp;

			ICR1conf = ADC_ICR_ACTIVE_ON_1;
			
			ACSR &= 0x3F; //set ADC enable
			TIFR &= 0xDF;

			ACSR |= 0x04; //set ADC capture
			SFIOR &= 0xF7;
			TIFR &=0xDF;
		}

		else {
			dutyCount2 = dutyTemp;
			ICR1conf = ADC_ICR_DISABLE_ON_1;
			
			ACSR &= 0xFB; //clear ADC capture
			TIFR &=0xDF;
		}
	}


}


SIGNAL(SIG_OUTPUT_COMPARE0) {
	
	if (wait250mils < 250) {
		wait250mils++;
	}

	else {
		//begin transmission
		okToTrans = 1;
		wait250mils=0;


		//getTemp();	
	}
	TCNT0=0;
}


ISR(ADC_vect) {
	unsigned int temperature;
	temperature = (ADCH * 200);
	temperature = temperature / 1024;
	
	if (temperature > 35){ //aka 3.5 volts or ~75c
		overheat = 1;
	}

	else {
		overheat = 0;
	}
}
