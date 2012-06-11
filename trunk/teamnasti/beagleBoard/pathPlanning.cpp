/*****************************************************************************************
* Author: Terry Max Christy
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
*
* Description:
* The path planning file contains all of the functions that are used in determining the 
* desired path for the boat to travel. It contains all of the constructors as well as the
* control functions.
*****************************************************************************************/

#include "stdafx.h"
using namespace std;
using namespace cv;
/*
	This function will eiter fill a vector of 'gates' to represent the information of each pair of red and 
	green buoys that are gates, or it will use the obstacle(yellow buoy) as a gate if it is in obstacle 
	avoidance mode. 
*/
int constructGates( vector<buoy> &greenBuoys, vector<buoy> &redBuoys, vector<buoy> &yellowBuoys, vector<gate> &gates, bool avoidYellow, float gateSeperation)
{
	cout<<gateSeperation<<endl;
	//Check if the boat needs to avoid a yellow buoy, if so set the first target to the yellow buoy
	if(avoidYellow == true)
	{
		gates.resize(1);
		//If we wanted to avoid some computation, we could just set the goal to the location
		//of the yellow buoy, however this is currently untested
		gates[0].green = cvPoint(cvRound(yellowBuoys[0].x), cvRound(yellowBuoys[0].y));
		gates[0].red = cvPoint(cvRound(yellowBuoys[0].x), cvRound(yellowBuoys[0].y));
		gates[0].goal = cvPoint(cvRound((gates[0].green.x + gates[0].red.x)/2.0), 
			cvRound((gates[0].green.y + gates[0].red.y)/2.0));
	}
	else 
	{
		//make sure there is a pair of red/green buoys
		if ((greenBuoys.size() > 0) && (redBuoys.size() > 0))
		{
			//determine which color buoys there are more of, use the smaller 
			if (greenBuoys.size() > redBuoys.size())
			{
				gates.resize(redBuoys.size());
				for(unsigned int i = 0; i < redBuoys.size(); i++)
				{
					if(abs(redBuoys[i].y - greenBuoys[i].y) < 100)
					{
						gates[i].green = cvPoint(cvRound(greenBuoys[i].x), cvRound(greenBuoys[i].y));
						gates[i].red = cvPoint(cvRound(redBuoys[i].x), cvRound(redBuoys[i].y));
						gates[i].goal = cvPoint(cvRound((gates[i].green.x + gates[i].red.x)/2.0), 
							cvRound((gates[i].green.y + gates[i].red.y)/2.0));
					}

				}
			}
			else
			{
				gates.resize(greenBuoys.size());
				for(unsigned int i = 0; i < greenBuoys.size(); i++)
				{
					if(abs(redBuoys[i].y - greenBuoys[i].y) < 100)
					{
						gates[i].red = cvPoint(cvRound(redBuoys[i].x), cvRound(redBuoys[i].y));
						gates[i].green = cvPoint(cvRound(greenBuoys[i].x), cvRound(greenBuoys[i].y));
						gates[i].goal = cvPoint(cvRound((gates[i].green.x + gates[i].red.x)/2.0), 
							cvRound((gates[i].green.y + gates[i].red.y)/2.0));
					}
				}
			}
		}
		else
		{
			//return different values for different cases, not used currently
			if(greenBuoys.size() <= 0 && redBuoys.size() <= 0) return(1);
			else if (greenBuoys.size() <= 0)                   return(2);
			else if (redBuoys.size() <= 0)                     return(3);
			else                                               return(4);
		}
	}
	return(0);
}

//determine if the red buoy is to the left or right of the boat
bool redRightReturn(vector<gate> &gates)
{
	char redRightCount = 0;
	for(unsigned int i = 0; i < gates.size(); i++)
	{
		if(gates[i].red.x > gates[i].green.y) redRightCount++;
		else redRightCount = redRightCount;
	}
	if(gates.size() > 0) redRightCount = redRightCount/gates.size();
	else redRightCount = 0;
	if(redRightCount >= 1.0) return(TRUE);
	else return(FALSE);
}

/*
	Determine the desired path based on the series of gates. Origionally the control sheme used 
	the information gathered from the bottom middle of the image to the first gate, since then 
	a new control signal is used. This function doesn't serve much of a purpuse other than providing
	visual information to be drawn on the debugging image. 
*/
int findPath(IplImage *in, vector<gate> &gates, vector<path> &path)
{
	int x, y;
	if(gates.size() > 0){path.resize(gates.size());}
	else {path.resize(1);}
	//cout<<path.size()<<endl;
	path[0].nearEnd = cvPoint( in->width/2, in->height);
	if( gates.size() > 0 )
	{
		path[0].height = (float)in->height;
		path[0].farEnd = gates[0].goal;
		path[0].slope = (float)(path[0].farEnd.y - path[0].nearEnd.y)/(float)(path[0].farEnd.x - path[0].nearEnd.x);
		x = (path[0].farEnd.x - path[0].nearEnd.x);
		y = (path[0].farEnd.y - path[0].nearEnd.y);
		path[0].length = sqrt((float)((x*x) + (y*y)));
		for(unsigned int i = 1; i < gates.size(); i++)
		{
			path[i].nearEnd = gates[i-1].goal;
			path[i].farEnd = gates[i].goal;
			path[i].slope = (float)(path[i].farEnd.y - path[i].nearEnd.y)/(float)(path[i].farEnd.x - path[i].nearEnd.x);
			x = (path[i].farEnd.x - path[i].nearEnd.x);
			y = (path[i].farEnd.y - path[i].nearEnd.y);
			path[i].length = (float)y; //sqrt((float)((x*x) + (y*y)));
		}

	}
	else
	{
		path[0].farEnd = cvPoint(cvRound(in->width/2), 0);
		path[0].slope = (float)(path[0].farEnd.y - path[0].nearEnd.y)/(float)(path[0].farEnd.x - path[0].nearEnd.x);
		x = (path[0].farEnd.x - path[0].nearEnd.x);
		y = (path[0].farEnd.y - path[0].nearEnd.y);
		path[0].length = sqrt((float)((x*x) + (y*y)));
	}
	return(0);
}
/*
	This function is used to determine the wall information. This information, 
	however is not used for anything more than drawing on the debug image
*/
int constructWall(vector<buoy> &buoys, vector<wall> &walls)
{
	if(buoys.size() > 0)
	{
		walls.resize(buoys.size()-1);
		for(unsigned int i = 0; i < buoys.size()-1; i++)
		{
			walls[i].nearEnd = cvPoint(cvRound(buoys[i].x), cvRound(buoys[i].y));
			walls[i].farEnd = cvPoint(cvRound(buoys[i+1].x), cvRound(buoys[i+1].y));
		}
		return(1);
	}
	else return(0);
}

/*
	This function determines the threat level of the obstacle. If there are no gates 
	between the boat and the next gate, then the obstacle needs to be avoided and is an
	emediate threat. If it is not that close, then it is not a threat and does not need 
	to be considered yet. 
*/
bool checkForObstacle(vector<buoy> &greenBuoys, vector<buoy> &redBuoys, vector<buoy> &yellowBuoys)
{
	bool obsticle = false;
	//check if a yellow buoy has been detected
	if( (yellowBuoys.size() == 0))
	{
		obsticle = false;
	}
	//determine if the yellow buoy needs to be avoided
	else if(greenBuoys.size() == 0 && redBuoys.size() == 0)
	{
		obsticle = true;
	}
	else
	{
		if(greenBuoys.size() > 0)
		{
			if(yellowBuoys[0].y > greenBuoys[0].y){obsticle = true;}
			else {obsticle = false;}
		}
		else
		{
			if(yellowBuoys[0].y > redBuoys[0].y){obsticle = true;}
			else {obsticle = false;}
		}
	}
	return(obsticle);
}

/*
	This function provides a low pass filter for the control signal. 
	This is used to smooth a transition from one gate to the next or to maintain heading
	if one of the buoys is momentarily lost (not found or move out of FoV)
*/
CvPoint rollAverage(vector<CvPoint> &averageBuff, vector<path> &path)
{
	float X = 0.0;
	float Y = 0.0;
	//rotate out the old value
	for(unsigned char i = averageBuff.size()-1; i >0; i--)
	{
		averageBuff[i] = averageBuff[i-1];

	}
	//insert the new value
	averageBuff[0] = cvPoint(path[0].farEnd.x, path[0].farEnd.y);

	for(unsigned char i = 0; i < averageBuff.size(); i++)
	{
		X += averageBuff[i].x;
		Y += averageBuff[i].y;
	}
	X /= averageBuff.size();
	Y /= averageBuff.size();
	
	return(cvPoint((int)X, (int)Y));
}
void constructControl(CvPoint *start, CvPoint *end, path *control)
{
	int x, y;
	control->nearEnd = *start;
	control->farEnd = *end;
	control->slope = (float)(control->farEnd.y - control->nearEnd.y)/(float)(control->farEnd.x - control->nearEnd.x);
	x = (control->farEnd.x - control->nearEnd.x);
	y = (control->farEnd.y - control->nearEnd.y);
	control->length = sqrt((float)((x*x) + (y*y)));
}

void intigrator(CvPoint *sum, CvPoint *newPoint, float Ki, float width)
{
	sum->x = (int)((sum->x - width/2.0) + Ki*(newPoint->x - width/2.0));
	sum->x = (int)(sum->x + width/2.0);
	sum->y = (int)(newPoint->y) ;//+ Ki*(newPoint->y);
}

CvPoint differentiator(CvPoint *old, CvPoint *in, float Kd, float width)
{
	CvPoint out;

	out.x = (int)(Kd*(in->x - old->x - width));
	out.x += (int)(width/2.0);
	out.y = in->y;
	old->x = in->x;
	return(out);
}