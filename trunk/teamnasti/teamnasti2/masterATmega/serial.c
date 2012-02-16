#include "serial.h"

void sendByte(unsigned char sendInfo) {
    while ((UCSR0A & (1 << UDRE0)) == 0) {}; //Wait for sending to be available
    UDR0 = sendInfo;
}

unsigned char getByte(void) {
    unsigned char recieved;
  
    while ((UCSR0A & (1 << RXC0)) == 0) {}; //wait for incoming data
      recieved = UDR0;
      return recieved;
}

void configUART0 (void) {

   UCSR0B |= ((1 << RXEN0) | (1 << TXEN0)); // Turn on the transmit and revieve
   UCSR0C |= ((1 << UCSZ01) | (1<< UCSZ00)); //8bit buffer

   UBRR0H = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
   UBRR0L = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register

}

void sendString(unsigned char str[], char len){
	int i=0;
	for (i=0;i<len;i++){
		sendByte(str[i]);
	}
}

void sendIntHex (unsigned int intToSend) {			
						
	unsigned int tempInt;
	unsigned char tempChar;


	tempInt = intToSend >> 12;
	tempInt = tempInt & 0x0F;
	tempChar = (char)tempInt;
	sendByte(hexToAscii(tempChar));

	tempInt = intToSend >> 8;
	tempInt = tempInt & 0x0F;
	tempChar = (char)tempInt;
	sendByte(hexToAscii(tempChar));

	tempInt = intToSend >> 4;
	tempInt = tempInt & 0x0F;
	tempChar = (char)tempInt;
	sendByte(hexToAscii(tempChar));

	tempInt = intToSend;
	tempInt = tempInt & 0x0F;
	tempChar = (char)tempInt;
	sendByte(hexToAscii(tempChar));

}

unsigned char hexToAscii(unsigned char num) {
	unsigned char retNum;

	if (num < 10) { 
		retNum = num + '0';

	}

	else {
		retNum = num + ('A' - 0x0A);
	}
	return retNum;
}
