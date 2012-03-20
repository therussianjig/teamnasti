/*****************************************************************************************
* Author: Terry Max Christy
* Modified: Jeremy Borgman 3/20/2012
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Date written: 1/24/12 Last Modified: 3/20/2012
*****************************************************************************************/
// #pragma once ensures that any file that is inlcuded after it is only included in the 
// overall project once.
#ifndef unix
#pragma once
#endif


void throttle(float PWM, char severity, char direction, vector<float> &motors);
void turn( char severity, char direction, vector<float> &motors);
void mainThrust(float motor1, float motor2, vector<float> &motors);
void sideThrust(float frontLeft, float backLeft, float frontRight, float backRight, vector<float> &motors);
void navigateChannel(vector<path> &path, vector<float> &motors);
void pwm2uchar(vector<float> &motors, unsigned char *motorschar);