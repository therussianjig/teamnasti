/*****************************************************************************************
// Author: Terry Max Christy
// Bradley University 
// Nautical Autonomous System with Task Integration (code name NASTI)
// Date written: 1/24/12 Last Modified: 1/24/12
*****************************************************************************************/
// #pragma once ensures that any file that is inlcuded after it is only included in the 
// overall project once.
#pragma once


void throttle(float height, float length, vector<char> &motors);
void turn( char severity, char direction, vector<char> &motors);
void mainThrust(int motor1, int motor2, vector<char> &motors);
void sideThrust(int frontLeft, int backLeft, int frontRight, int backRight, vector<char> &motors);
void navigateChannel(vector<path> &path, vector<char> &motors);