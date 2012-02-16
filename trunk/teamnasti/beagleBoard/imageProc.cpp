/*****************************************************************************************
// Author: Terry Max Christy
// Bradley University 
// Nautical Autonomous System with Task Integration (code name NASTI)
// Date written: 10/20/11 Last Modified: 10/20/11
*****************************************************************************************/
#ifdef unix
#include <iostream>

#endif

#include "stdafx.h"
using namespace std;
using namespace cv; 

void onTrackbarSlide(int pos) 
{
	inv = pos;
}

IplImage* doCanny(IplImage* in, double lowThresh, double highThresh, double aperture) 
{
	IplImage* out  = cvCreateImage(cvGetSize(in),IPL_DEPTH_8U,1);
	IplImage* grey = cvCreateImage(cvGetSize(in),IPL_DEPTH_8U,1);
	if(in->nChannels != 1)//Canny only handles gray scale images
	{
		cout<<"Canny - Image not grey";
		cvCvtColor(in, grey, CV_RGB2GRAY); 
		cvCanny( grey, out, lowThresh, highThresh, (int)aperture );
	}
	else 
	{
		cvCanny( in, out, lowThresh, highThresh, (int)aperture );
	}
	cvReleaseImage(&grey);
	return( out );
}

CvHistogram* RGB2HS_HShist(IplImage* in, int h_bins, int s_bins, float h_max, float s_max, int fact)
{
	IplImage* hsv = cvCreateImage(cvGetSize(in),8,3);
	cvCvtColor(in,hsv,CV_RGB2HSV);

	IplImage* h_plane = cvCreateImage(cvGetSize(in),8,1);
	IplImage* s_plane = cvCreateImage(cvGetSize(in),8,1);
	IplImage* v_plane = cvCreateImage(cvGetSize(in),8,1);
	IplImage* planes[] = {h_plane,s_plane};
	cvCvtPixToPlane(hsv, h_plane, s_plane, v_plane, 0);

		// Build the histogram and compute its contents.
	CvHistogram* hist;
	{
		int hist_size[] = { h_bins, s_bins };
		float h_ranges[] = { 0, h_max }; // hue full range [0,180]
		float s_ranges[] = { 0, s_max }; // sat full range [0, 255] for depth = 8 bit
		float* ranges[] = { h_ranges, s_ranges };
		hist = cvCreateHist(2,hist_size,CV_HIST_ARRAY,ranges,1);
	}

	//Compute histogram
	cvCalcHist( planes, hist, 0, 0 );

	if(fact > 0) {cvNormalizeHist( hist, fact);}

	cvReleaseImage(&h_plane);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
	cvReleaseImage(&hsv);
	return(hist);
}

