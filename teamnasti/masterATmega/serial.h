#include <avr/io.h>
#include <avr/interrupt.h>

#include <inttypes.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((16000000 / (USART_BAUDRATE * 16UL))) - 1)

void sendByte(unsigned char sendInfo);
unsigned char getByte (void);

void sendString(unsigned char str[], char len);
void sendIntHex(unsigned int intToSend);
void configUART0(void);
unsigned char hexToAscii(unsigned char num);
