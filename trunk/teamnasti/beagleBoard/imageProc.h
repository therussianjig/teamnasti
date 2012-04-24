/*****************************************************************************************
* Author: Terry Max Christy
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Date written: 10/20/11 Last Modified: 4/24/12
*****************************************************************************************/
using namespace std;
using namespace cv;

IplImage* findBuoy(IplImage* in, int horizon, char color, vector<buoy> &buoys, char lighting);

void inputParams(float &closingOnGateDen, float &closingPWM, float &PWMoffset, float &maxThrottle, float &diffCoef, float &leftOff, float &rightOff);
void drawStuff(IplImage* out, vector<buoy> &greenBuoys, vector<buoy> &redBuoys, vector<buoy> &yellowBuoys, vector<buoy> &blueBuoys, vector<gate> &gates, vector<path> &path,	vector<wall> &greenWall, vector<wall> &redWall, vector<wall> &blueWall, CvPoint target);

void onTrackbarSlide(int pos) ;

IplImage* doCanny(IplImage* in,double lowThresh,double highThresh,double aperture) ;
