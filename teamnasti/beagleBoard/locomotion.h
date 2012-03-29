/*****************************************************************************************
// Author: Terry Max Christy
// Bradley University 
// Nautical Autonomous System with Task Integration (code name NASTI)
// Date written: 1/24/12 Last Modified: 1/24/12
*****************************************************************************************/
// #pragma once ensures that any file that is inlcuded after it is only included in the 
// overall project once.
#pragma once


void throttle(float PWM, vector<float> &motors);
void turn( char severity, char direction, vector<float> &motors);
void mainThrust(float motor1, float motor2, vector<float> &motors);
void sideThrust(float frontLeft, float backLeft, float frontRight, float backRight, vector<float> &motors);
void navigateChannel(vector<path> &path, vector<float> &motors, float closingOnGateDen, float closingPWM, float PWMoffset, float maxThrottle, float diffCoef, float leftOff, float rightOff);
void pwm2uchar(vector<float> &motors, unsigned char *motorschar);