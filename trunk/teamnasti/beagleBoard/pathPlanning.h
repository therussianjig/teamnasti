/*****************************************************************************************
* Author: Terry Max Christy
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Date written: 10/20/11 Last Modified: 1/26/12
*****************************************************************************************/
#pragma once
using namespace std;
using namespace cv; 

void findBuoy(IplImage* in, int horizon, char color, vector<buoy> &buoys);
int constructGates( vector<buoy> &greenBuoys, vector<buoy> &redBuoys, vector<buoy> &yellowBuoys, vector<gate> &gates);
bool redRightReturn(vector<gate> &gates);
int findPath(IplImage* in, vector<gate> &gates, vector<path> &path);
int constructWall(vector<buoy> &Buoys, vector<wall> &walls);