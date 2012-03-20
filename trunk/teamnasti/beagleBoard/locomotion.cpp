/*****************************************************************************************
* Author: Terry Max Christy
* Modified: Jeremy Borgman 2/21/2012
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Date written: 10/20/11 Last Modified: 1/26/12
*****************************************************************************************/
#ifdef unix
#include <iostream>
#endif

#include "stdafx.h"
using namespace std;
using namespace cv;

void navigateChannel(vector<path> &path, vector<float> &motors)
{
	float closingOnGate = path[0].height/4 ;
	float throttlePWM = 0;
	float closingPWM  = 60;
	float PWMoffset = 60;
	char direction = 'N';
	char severity = 'N';

	if(path[0].length < closingOnGate)
	{
		cout<<"WE'RE GOING IN!!!!!"<<endl;

		//set throttle value to an 'enter gate' speed
		throttlePWM = closingPWM;
	}
	else
	{
		//don't need to turn
		if(abs(path[0].slope) > 5)
		{
			cout<<"Dead ahead Cap'n"<<endl;
			severity = 'N';
			direction = 'F';
		}
		//need to turn
		else
		{
			//need to turn left
			if(path[0].slope > 0)
			{
				direction = 'L';
			}
			//need to turn right
			else
			{
				direction = 'R';
			}
			//need a hard turn
			if(abs(path[0].slope) < 1.0)
			{
				cout<<"Turning Hard:  "<<direction<<endl;
				severity = 'H';
			}
			//need a slight turn
			else
			{
				cout<<"Turning Slightly:  "<<direction<<endl;
				severity = 'S';
			}
		}
		//set throttle value proportional to the length of the path line
		throttlePWM = (path[0].length / path[0].height)*100 + PWMoffset;
	}

	turn(severity, direction, motors);
	throttle(throttlePWM, motors);
}

void throttle(float PWM, vector<float> &motors)
{
	mainThrust(PWM, PWM, motors);
}

/* 
* Currently performs a pivot motion using only the side thrusters
*/
void turn( char severity, char direction, vector<float> &motors)
{
	float PWM = 0;
	if(severity == 'H')      {PWM = 100;}
	else if(severity == 'S') {PWM =  50;}
	else                    {PWM =   0;}

	if( direction == 'L' ) 
	{sideThrust(-PWM, PWM, -PWM, PWM, motors);}
	else                  
	{sideThrust(PWM, -PWM, PWM, -PWM, motors);}
}

void mainThrust(float motor1, float motor2, vector<float> &motors)
{
	if     (motor1 < -100) motor1 = -100;
	else if(motor1 >  100) motor1 =  100;
	motors[drive1] = motor1;
	if     (motor2 < -100) motor2 = -100;
	else if(motor2 >  100) motor2 =  100;
	motors[drive2] = motor2;

}

/*
* I made this a seperate function from turn so that turn can be used in main buoy nav easily
* but if you want individual control of the side thrusters, use sideThrust
*/
void sideThrust(float frontLeft, float backLeft, float frontRight, float backRight, vector<float> &motors)
{
	if     (frontLeft < -100) frontLeft = -100;
	else if(frontLeft >  100) frontLeft =  100;
	motors[aftPort] = frontLeft;

	if     (backLeft < -100) backLeft = -100;
	else if(backLeft >  100) backLeft =  100;
	motors[forPort] = backLeft;

	if     (frontRight < -100) frontRight = -100;
	else if(frontRight >  100) frontRight =  100;
	motors[aftStarboard] = frontRight;

	if     (backRight < -100) backRight = -100;
	else if(backRight >  100) backRight =  100;
	motors[forStarboard] = backRight;
}

void pwm2uchar(vector<float> &motors, unsigned char *motorschar)
{
	motorschar[drive1] = (char)(abs(motors[drive1]) * (255/100));
	if( motors[drive1] > 0 )       {motorschar[drive1] |= 0x01;}
	else                           {motorschar[drive1] &= 0xFE;}

	motorschar[drive2] = (char)(abs(motors[drive2]) * (255/100));
	if( motors[drive2] > 0 )       {motorschar[drive2] |= 0x01;}
	else                           {motorschar[drive2] &= 0xFE;}

	motorschar[aftPort] = (char)(abs(motors[aftPort]) * (255/100));
	if( motors[aftPort] < 0 )      {motorschar[aftPort] |= 0x01;}
	else                           {motorschar[aftPort] &= 0xFE;}
	
	motorschar[forPort] = (char)(abs(motors[forPort]) * (255/100));
	if( motors[forPort] < 0 )      {motorschar[forPort] |= 0x01;}
	else                           {motorschar[forPort] &= 0xFE;}

	motorschar[aftStarboard] = (char)(abs(motors[aftStarboard]) * (255/100));
	if( motors[aftStarboard] < 0 ) {motorschar[aftStarboard] |= 0x01;}
	else                           {motorschar[aftStarboard] &= 0xFE;}
	
	motorschar[forStarboard] = (char)(abs(motors[forStarboard]) * (255/100));
	if(motors[forStarboard] < 0 ) {motorschar[forStarboard] |= 0x01;}
	else                          {motorschar[forStarboard] &= 0xFE;}
}