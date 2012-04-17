/*****************************************************************************************
* Author: Terry Max Christy
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Date written: 10/20/11 Last Modified: 1/26/12
*****************************************************************************************/
// #pragma once ensures that any file that is inlcuded after it is only included in the 
// overall project once.
#pragma once

void inputParams(float &closingOnGateDen, float &closingPWM, float &PWMoffset, float &maxThrottle, float &diffCoef, float &leftOff, float &rightOff);
void drawStuff(IplImage* out, vector<buoy> &greenBuoys, vector<buoy> &redBuoys, vector<buoy> &yellowBuoys, vector<buoy> &blueBuoys, vector<gate> &gates, vector<path> &path,	vector<wall> &greenWall, vector<wall> &redWall, vector<wall> &blueWall, CvPoint target);