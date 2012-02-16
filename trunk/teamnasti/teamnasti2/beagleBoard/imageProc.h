/*****************************************************************************************
// Author: Terry Max Christy
// Bradley University 
// Nautical Autonomous System with Task Integration (code name NASTI)
// Date written: 10/20/11 Last Modified: 10/20/11
*****************************************************************************************/
using namespace std;
using namespace cv;

void onTrackbarSlide(int pos) ;

IplImage* doCanny(IplImage* in,double lowThresh,double highThresh,double aperture) ;

CvHistogram* RGB2HS_HShist(IplImage* in, int h_bins, int s_bins, float h_max, float s_max, int fact=1.0);

