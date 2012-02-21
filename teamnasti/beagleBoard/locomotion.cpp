/*****************************************************************************************
* Author: Terry Max Christy
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

void throttle(float length, vector<char> &motors)
{
	float maxLength = 300;
	float minLength = 100;
	int PWM = 0;
	if(length > maxLength)
	{
		PWM = maxPWM;
	}
	else if (length < minLength)
	{
		PWM = minPWM;
	}
	else 
	{
		PWM = (int)((length - 100) * 0.5);
	}
	mainThrust(PWM, PWM, motors);
}

void turn( char severity, char direction, vector<char> &motors)
{
	int PWM = 0;
	if(severity = 'H')      {PWM = 100;}
	else if(severity = 'S') {PWM =  50;}
	else                    {PWM =   0;}

	if( direction = 'L' ) 
	{sideThrust(-PWM, PWM, -PWM, PWM, motors);}
	else                  
	{sideThrust(PWM, -PWM, PWM, -PWM, motors);}
}

void mainThrust(int motor1, int motor2, vector<char> &motors)
{
	motors[drive1] = (char)(abs(motor1) * (255/100));
	if(motor1 < 0) {motors[drive1] ^= 0x01;}
	else           {motors[drive1] &= 0xFE;}
	motors[drive2] = (char)(abs(motor2) * (255/100));
	if(motor1 < 0) {motors[drive2] ^= 0x01;}
	else           {motors[drive2] &= 0xFE;}
}

void sideThrust(int frontLeft, int backLeft, int frontRight, int backRight, vector<char> &motors)
{
	motors[aftPort] = (char)(abs(frontLeft) * (255/100));
	if(frontLeft < 0) {motors[aftPort] ^= 0x01;}
	else              {motors[aftPort] &= 0xFE;}
	
	motors[forPort] = (char)(abs(backLeft) * (255/100));
	if(backLeft < 0) {motors[forPort] ^= 0x01;}
	else             {motors[forPort] &= 0xFE;}

	motors[aftStarboard] = (char)(abs(frontRight) * (255/100));
	if(frontRight < 0) {motors[aftStarboard] ^= 0x01;}
	else               {motors[aftStarboard] &= 0xFE;}
	
	motors[forStarboard] = (char)(abs(backRight) * (255/100));
	if(backRight < 0) {motors[forStarboard] ^= 0x01;}
	else              {motors[forStarboard] &= 0xFE;}
}
void navigateChannel(vector<path> &path, vector<char> &motors)
{
	unsigned int closingOnGate = 100;
	char direction = 'N';
	char severity = 'N';
	if(path[0].length < closingOnGate)
	{
		cout<<"WE'RE GOING IN!!!!!"<<endl;
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
		turn(severity, direction, motors);
		throttle(path[0].length, motors);
	}
}