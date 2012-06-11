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

void aboutFace(unsigned char direction, vector<float> &motors){

	if (direction == 'L') {
		turn('H', 'L', motors);
		mainThrust(-100, 100, motors);
	}
	
	else {
		turn('H', 'R', motors);
		mainThrust(100, -100, motors);
	}

}

void speedGateRun(path *control, vector<float> &motors, float height, float PWMoffset, float maxThrottle, float diffCoef, float leftOff, float rightOff)
{
	float throttlePWM = 0;
	float LturnOffset = 0;
	float RturnOffset = 0;
	float aheadSlope = 5.0;

	//set throttle value proportional to the length of the path line
	throttlePWM = maxThrottle;
	if(throttlePWM > maxThrottle) throttlePWM = maxThrottle;
	else throttlePWM = throttlePWM;

	if(control->slope > 0)
	{
		LturnOffset = diffCoef*(aheadSlope - abs(control->slope));
		if(LturnOffset >= throttlePWM){ LturnOffset = throttlePWM; }
		else if(LturnOffset < 1.0)    { LturnOffset = 0; }
		RturnOffset = 0.0;
	}
	else if(control->slope < 0)
	{
		RturnOffset = diffCoef*(aheadSlope - abs(control->slope));
		if(RturnOffset >= throttlePWM){ RturnOffset = throttlePWM; }
		else if(RturnOffset < 1.0)    { RturnOffset = 0; }
		RturnOffset = 0.0;
	}
	else 
	{
		LturnOffset = 0;
		RturnOffset = 0;
	}
	//turn(severity, direction, motors);
	//throttle(throttlePWM, motors);
	mainThrust(leftOff*(throttlePWM - LturnOffset), rightOff*(throttlePWM - RturnOffset), motors);
}

void navigateChannel(path *control, vector<float> &motors, float height, float closingOnGateDen, float closingPWM, float PWMoffset, float maxThrottle, float diffCoef, float leftOff, float rightOff) 
{
	float closingOnGate = height/closingOnGateDen;
	float throttlePWM = 0;
	float LturnOffset = 0;
	float RturnOffset = 0;
	float aheadSlope = 5.0;
	char direction = 'N';
	char severity = 'N';

	if(control->length < closingOnGate)
	{
		cout<<"WE'RE GOING IN!!!!!"<<endl;

		//set throttle value to an 'enter gate' speed
		throttlePWM = closingPWM;
	}
	else
	{
		//don't need to turn
		if(abs(control->slope) > aheadSlope)
		{
			cout<<"Dead ahead Cap'n"<<endl;
			severity = 'N';
			direction = 'F';
		}
		//need to turn **REMOVING THE SEVERITY, SIDE THRUST DOESNT MAKE ENOUGH DIFERENCE**
		else
		{
			//need to turn left
			if(control->slope > 0)
			{
				direction = 'L';
			}
			//need to turn right
			else
			{
				direction = 'R';
			}
			//need a hard turn
			if(abs(control->slope) < 1.0)
			{
				cout<<"Turning Hard:  "<<direction<<endl;
				severity = 'H';
			}
			//need a slight turn
			else
			{
				cout<<"Turning Slightly:  "<<direction<<endl;
				severity = 'H';
			}
		}
		//set throttle value proportional to the length of the path line
		throttlePWM = (control->length / height)*(float)100.0 + PWMoffset;
		if(throttlePWM > maxThrottle) throttlePWM = maxThrottle;
		else throttlePWM = throttlePWM;
	}
	if(direction == 'L')
	{
		LturnOffset = diffCoef*(aheadSlope - abs(control->slope));
		if(LturnOffset >= throttlePWM){ LturnOffset = throttlePWM; }
		else if(LturnOffset < 1.0)    { LturnOffset = 0; }
		RturnOffset = 0.0;
	}
	else if(direction == 'R')
	{
		RturnOffset = diffCoef*(aheadSlope - abs(control->slope));
		if(RturnOffset >= throttlePWM){ RturnOffset = throttlePWM; }
		else if(RturnOffset < 1.0)    { RturnOffset = 0; }
		RturnOffset = 0.0;
	}
	else 
	{
		LturnOffset = 0;
		RturnOffset = 0;
	}
	turn(severity, direction, motors);
	//throttle(throttlePWM, motors);
	mainThrust(leftOff*(throttlePWM - LturnOffset), rightOff*(throttlePWM - RturnOffset), motors);
}
void avoidObsticle(path *control, vector<path> &path, vector<float> &motors, float height, float PWMoffset, float maxThrottle, float diffCoef, float yellowCoef, float leftOff, float rightOff, float optimalSlope, float deadBand)
{
	float throttlePWM = 0;
	float LturnOffset = 0;
	float RturnOffset = 0;
	float aheadSlope = 5.0;
	char direction = 'N';
	char severity = 'H';
	float controlSlope = 0.0;

	cout<<"Avoid Yellow Buoy"<<endl;
	{
		//Need to move away from Obstacle
		if(abs(control->slope) > (optimalSlope + deadBand))
		{
			if(control->slope < 0) direction = 'L';
			else                  direction = 'R';
			cout<<"Away from Obstacle: "<<direction<<endl;
			controlSlope = abs(control->slope) - optimalSlope;
		}
		//Moving too far away from the Obstacle
		if(abs(control->slope) < (optimalSlope - deadBand))
		{
			if(control->slope < 0) direction = 'R';
			else                  direction = 'L';
			cout<<"Toward Obstacle: "<<direction<<endl;
			controlSlope = (optimalSlope - abs(control->slope))*yellowCoef;
		}
		else
		{
			direction = 'N';
			cout<<"Dead Ahead"<<endl;
		}
		//set throttle value proportional to the length of the path line
		throttlePWM = (control->length / height)*(float)100.0 + PWMoffset;
		if(throttlePWM > maxThrottle) throttlePWM = maxThrottle;
		else throttlePWM = throttlePWM;
	}
	if(direction == 'L')
	{
		LturnOffset = diffCoef*(aheadSlope - controlSlope);
		if(LturnOffset >= throttlePWM){ LturnOffset = throttlePWM; }
		else if(LturnOffset < 1.0)    { LturnOffset = 0; }
		RturnOffset = 0.0;
	}
	else if(direction == 'R')
	{
		RturnOffset = diffCoef*(aheadSlope - controlSlope);
		if(RturnOffset >= throttlePWM){ RturnOffset = throttlePWM; }
		else if(RturnOffset < 1.0)    { RturnOffset = 0; }
		RturnOffset = 0.0;
	}
	else 
	{
		LturnOffset = 0;
		RturnOffset = 0;
	}	
	
	turn(severity, direction, motors);
	mainThrust(leftOff*(throttlePWM - LturnOffset), rightOff*(throttlePWM - RturnOffset), motors);
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
	{sideThrust(-PWM, PWM, PWM, -PWM, motors);}
	else                  
	{sideThrust(PWM, -PWM, -PWM, PWM, motors);}
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
	motorschar[drive1] = (char)(abs(motors[drive1]) * (255.0/100.0));
	if( motors[drive1] > 0 )       {motorschar[drive1] |= 0x01;}
	else                           {motorschar[drive1] &= 0xFE;}

	motorschar[drive2] = (char)(abs(motors[drive2]) * (255.0/100.0));
	if( motors[drive2] > 0 )       {motorschar[drive2] |= 0x01;}
	else                           {motorschar[drive2] &= 0xFE;}

	motorschar[aftPort] = (char)(abs(motors[aftPort]) * (255.0/100.0));
	if( motors[aftPort] < 0 )      {motorschar[aftPort] |= 0x01;}
	else                           {motorschar[aftPort] &= 0xFE;}
	
	motorschar[forPort] = (char)(abs(motors[forPort]) * (255.0/100.0));
	if( motors[forPort] < 0 )      {motorschar[forPort] |= 0x01;}
	else                           {motorschar[forPort] &= 0xFE;}

	motorschar[aftStarboard] = (char)(abs(motors[aftStarboard]) * (255.0/100.0));
	if( motors[aftStarboard] < 0 ) {motorschar[aftStarboard] |= 0x01;}
	else                           {motorschar[aftStarboard] &= 0xFE;}
	
	motorschar[forStarboard] = (char)(abs(motors[forStarboard]) * (255.0/100.0));
	if(motors[forStarboard] < 0 ) {motorschar[forStarboard] |= 0x01;}
	else                          {motorschar[forStarboard] &= 0xFE;}
}