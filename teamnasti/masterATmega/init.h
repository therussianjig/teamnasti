/*****************************************************************************************
* Author: Jeremy Borman & Terry Max Christy
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Main Microcontroller Code
* Date written: 2/2/12 Last Modified: 2/2/12
*****************************************************************************************/

void initTimer0(void) {
	TCCR0 = ((1<<WGM01) | (1<<CS02) | (1<<CS00)); //ctc prescale=128
	TCNT0 = 0;

	TIMSK |= (1<<OCIE0);
	OCR0 = 124;
	PORTB= 0xFF;
}

void initCapture1(void) {
	TCCR1A = 0x00; //Normal Mode disconnect OCnA/B/C

	TCCR1B |= ((1<<ICNC1) | (1<<ICES1) | (1<<CS10)); //Turn on noise cancel and look for rising edge
	rising1 = 1; //Just to be sure

	TCNT1 = 0;
	TIMSK |= (1<<TICIE1);

	ACSR &= 0xFB; //clear ADC capture
	
	ICR1conf = ADC_ICR_DISABLE_ON_1;
}

void initCapture2(void) {
	TCCR1A = 0x00; //Normal Mode disconnect OCnA/B/C

	TCCR1B |= ((1<<ICNC1) | (1<<ICES1)); //Turn on noise cancel and look for rising edge
	
	rising1 = 1; //Just to be sure

	TCNT1 = 0;
	TIMSK |= (1<<TICIE1);

	
	ICR1conf = ADC_ICR_ACTIVE_ON_1;

	ACSR &= 0x3F; //set ADC enable
	TIFR &= 0xDF;

	ACSR |= 0x04; //set ADC capture
	SFIOR &= 0xF7;
	TIFR &=0xDF;
}


void initCapture3(void) {
	TCCR3A = 0x00; //Normal Mode disconnect OCnA/B/C

	TCCR3B |= ((1<<ICNC3) | (1<<ICES3) | (1<<CS30)); //Turn on noise cancel and look for rising edge
	rising3 = 1; //Just to be sure
	
	//risingExt = 1;
	
	TCNT3 = 0;
	ETIMSK |= (1<<TICIE3);
}

void configADC(void) {
	ADMUX = 0;
	ADMUX |= ((1 << REFS0) | (1 << ADLAR));
	ADCSRA = ( (1 << ADEN) | (1 << ADIE) | (1<<ADPS2) | (1<<ADPS1) |(1<<ADPS0) );
}
