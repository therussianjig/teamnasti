/*****************************************************************************************
* Author: Jeremy Borman & Terry Max Christy
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Main Microcontroller Code
* Date written: 2/2/12 Last Modified: 2/2/12

* This file contains the global variables that need to be shared across the main 
* microcontroller unit code. 
*****************************************************************************************/

//#define Pic28x2  0xA0 // memory location for the PicAxe

#define ADC_ICR_ACTIVE_ON_1 0x02
#define ADC_ICR_DISABLE_ON_1 0x01

//flags to determine rising/falling edge of reciever signal pulses
unsigned char rising1 = 0;
unsigned char rising3 = 0;
unsigned char risingExt = 0;

// cycle counters to ensure there is at least one good pulse reading from signals that 
// share timers. 
unsigned char cycleCount1 = 0;
unsigned char cycleCount2 = 0;

//Define Flags
volatile unsigned char okToTrans = 1;
unsigned int wait250mils=0;
volatile unsigned char overheat=0;

// Containers for cycles counted in timers
volatile unsigned int dutyCount1;
volatile unsigned int dutyCount2;
volatile unsigned int dutyCount3;
volatile unsigned int dutyCount4;

//Define Global Variables
unsigned char RCSig[6];
unsigned char motorInfo[9];
volatile unsigned char globBuff=0;

volatile unsigned char ICR1conf = 0; //1 means not connected to ADC
									 //2 means connected to ADC
									 