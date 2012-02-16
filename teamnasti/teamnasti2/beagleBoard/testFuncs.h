/*****************************************************************************************
// Author: Terry Max Christy
// Bradley University 
// Nautical Autonomous System with Task Integration (code name NASTI)
// Date written: 10/20/11 Last Modified: 10/20/11
*****************************************************************************************/
#pragma once
using namespace std;
using namespace cv; 

IplImage* doPyrDown(IplImage* in,int filter);
void addROI(IplImage* in, int x, int y, int width, int height);
IplImage* addROI(IplImage* in, int horizon);