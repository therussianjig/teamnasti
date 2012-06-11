/*****************************************************************************************
* Author: Terry Max Christy
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Date written: 10/20/11 Last Modified: 4/24/12
* 
* Description:
* The imageProc file contains the function that is used to identify the buoys in the image
* as well as the functions to take the input paramaters in and draw on the image for debug
* purposes.
*****************************************************************************************/
#ifdef unix
#include <iostream>

#endif

#include "stdafx.h"
using namespace std;
using namespace cv; 

/*
	This function takes an image, the y coordinate of the horizon, a char representation of 
	the color that is to be located, a vector to put the informoation in, and a lighting as 
	input paramaters and will return an Iplimage. The color char is used in an if tree to 
	determine the threshold values that are to be used when performing the threshold function
	on the HSV image. The image is converted from RGB color space to HSV color space before 
	processing is performed because the HSV color space is less responsive to various light
	conditions.
*/
IplImage* findBuoy(IplImage* in, int horizon, char color, vector<buoy> &buoys, char lighting)
{	
	/* the following section uses the horizon information to create a region of interest */
	IplImage* out =  cvCreateImage(cvGetSize(in), in->depth, in->nChannels);
	cvCopy(in, out, NULL);
	int x = out->origin; 
	int y = 0;
	int width = out->width;
	int height = out->height + y;
	int add = 100;

	if(horizon < out ->height){y = out->origin + horizon;}
	else{y = out->origin;}
	//Make a recatangle with the full x length of the image and the height from the bottom of the image to the 
	//horizon point
	CvRect water = cvRect(x,horizon,width,height);
	cvSetImageROI( in, water);
	/***********************************************************************************/
	IplImage* hsvImg       = cvCreateImage(cvGetSize(in), IPL_DEPTH_8U, 3);
	IplImage* thresholded  = cvCreateImage(cvGetSize(in), IPL_DEPTH_8U, 1);
	IplImage* thresholded2 = cvCreateImage(cvGetSize(in), IPL_DEPTH_8U, 1);
	IplImage* thresholded3  = cvCreateImage(cvGetSize(in), IPL_DEPTH_8U, 1);
	/* 
		The HSV colorspace uses 0-360 to represent the color values(hue) in a circular chart. 
		In doing this, it splits the red spectrum down the middle so anything that contains 
		red needs to be searched for on the low and high end of the hue values. This is the reason
		for the three thresholded images. thresholded and thresholded2 can be used to search multiple
		location on the hsv chart and thresholded3 is the or'ed together image. 
	
		To make things even more interesting, openCV uses chars to represent the values for hue, 
		saturation, and value. Rather than scaling 0-360 to 0-255 they chose to cut the resolution
		in half so hue is represented from 0 to 180.
	*/

	CvScalar hsv_min;
	CvScalar hsv_max;
	CvScalar hsv_min2;
	CvScalar hsv_max2;

	//for low light, decrease minimum value  (3rd input)
	//for high light, decrease minimum saturation (2nd input)
	if(color == 'g')
	{
		hsv_min  = cvScalar( 50, 255, 255, 0);
		hsv_max  = cvScalar(70, 255, 255, 0);
		hsv_min2 = cvScalar(50, 200, 50, 0);
		hsv_max2 = cvScalar(70, 255, 255, 0);
	}
	else if (color == 'r')
	{
		hsv_min  = cvScalar( 0, 200, 200, 0);
		hsv_max  = cvScalar(30, 255, 255, 0);
		hsv_min2 = cvScalar(110, 200, 110, 0);
		hsv_max2 = cvScalar(180, 255, 255, 0);
		//hsv_min  = cvScalar( 0, 150, 100, 0);
		//hsv_max  = cvScalar(30, 255, 255, 0);
		//hsv_min2 = cvScalar(100,  150, 100, 0);
		//hsv_max2 = cvScalar(180, 255, 255, 0);
	}
	else if (color == 'y')
	{
		//hsv_min  = cvScalar( 0, 5, 245, 0);
		//hsv_max  = cvScalar(60, 10, 250, 0);
		//hsv_min2 = cvScalar(60,  100, 200, 0);
		//hsv_max2 = cvScalar(100, 255, 255, 0);
		hsv_min  = cvScalar( 0, 5, 245, 0);
		hsv_max  = cvScalar(60, 10, 250, 0);
		hsv_min2 = cvScalar(60,  100, 200, 0);
		hsv_max2 = cvScalar(100, 255, 230, 0);
	}
	else if (color == 'b')
	{
		hsv_min  = cvScalar(40, 0, 0, 0);
		hsv_max  = cvScalar(20, 255, 255, 0);
		hsv_min2 = cvScalar(0,  100, 100, 0);
		hsv_max2 = cvScalar(20, 255, 255, 0);
	}
	else 
	{
		hsv_min  = cvScalar( 40, 255, 255, 0);
		hsv_max  = cvScalar(70, 255, 255, 0);
		hsv_min2 = cvScalar(40,  100, 100, 0);
		hsv_max2 = cvScalar(70, 255, 255, 0);
	}

	//converter image from RGB -> HSV
	cvCvtColor(in, hsvImg, CV_RGB2HSV);

	//thresholed and or images together
	cvInRangeS(hsvImg,  hsv_min,  hsv_max,  thresholded);
	cvInRangeS(hsvImg, hsv_min2, hsv_max2, thresholded2);
	cvOr(thresholded, thresholded2, thresholded3);

	//Atempt to doctor the image so that the circles are found easier
	if(color != 'y'){ cvSmooth(thresholded3, thresholded3, CV_GAUSSIAN, 3, 3); }

	cvErode(thresholded3, thresholded3, NULL, 1); //remove speckles (noise)
	//cvSmooth(thresholded, thresholded, CV_BLUR, 9, 9); //heavy blur
	cvDilate(thresholded3, thresholded3, NULL, 3); //return blobs that are left to proper size
	cvSmooth(thresholded3, thresholded3, CV_GAUSSIAN, 3, 3);

#ifdef debug
	//make an image so I can see what is happening durring the edge detection
	//IplImage* edge = doCanny(thresholded, 100,200, 3);
	//cvNamedWindow( "edge",CV_WINDOW_AUTOSIZE);
	//cvShowImage( "edge", edge );
	//cvReleaseImage(&edge);	
#endif

	/* Use the blob detection method, much more reliable then hough circle transform*/
	CBlobResult blobs;
	blobs = CBlobResult( thresholded3, NULL, 0);
	
	buoys.resize(blobs.GetNumBlobs());
	int k = 0;
	for(int i = 0; i < (blobs.GetNumBlobs()); i++)
	{
		//determine buoy parameters from blob information
		float x = (float)(blobs.GetBlob(i)->MinX() + (( blobs.GetBlob(i)->MaxX() - blobs.GetBlob(i)->MinX() ) / 2.0));
		float y = (float)(blobs.GetBlob(i)->MinY() + (( blobs.GetBlob(i)->MaxY() - blobs.GetBlob(i)->MinY() ) / 2.0));
		float radius =  (float)(blobs.GetBlob(i)->MaxY() - blobs.GetBlob(i)->MinY())/2;
		float diameter = 2*radius;
		if(radius > 9) //use only the larger 'buoys'. if they are too small they probably arent buoys at all
		{
			buoys[k].x= x;
			buoys[k].y = y;
			buoys[k].radius = radius;
			k++;
		}
	}

	//sort the vector of buoys so that the first listed buoys are the 'closest' buoys
	buoys.resize(k);
	buoy buoyTemp;
	for(int j = 0; j < k-1; j++)
	{
		for(int i = 0; i < k-1; i++)
		{
			if( buoys[i].y < buoys[i+1].y )
			{
				buoyTemp = buoys[i+1];
				buoys[i+1] = buoys[i];
				buoys[i] = buoyTemp;
			}
		}
	}
	//adjust the buoy y location because the ROI sets a new origin at the top left of ROI
	//the adjustment is needed so the buoys are locates properly relative to the whole image
	for(int j = 0; j < k; j++)
	{
		buoys[j].y = buoys[j].y + horizon;
	}

#ifdef debug
	cvNamedWindow( "HSV",CV_WINDOW_AUTOSIZE);
	cvShowImage( "HSV", thresholded );
	cvNamedWindow( "thresholded",CV_WINDOW_AUTOSIZE);
	cvShowImage( "thresholded", thresholded3 );
	cvNamedWindow( "thresholded2",CV_WINDOW_AUTOSIZE);
	cvShowImage( "thresholded2", thresholded2 );
#endif
	
	cvReleaseImage(&thresholded);
	cvReleaseImage(&thresholded2);
	cvReleaseImage(&hsvImg);
	cvReleaseImage(&out);
	return(thresholded3);
}

/*
	This function is used for the purposes of debuging and presenting. 
	It will take an image in as well as the buoy parameters and draw circles and lines on the image to represent the additional information.
*/
void drawStuff(IplImage* out, vector<buoy> &greenBuoys, vector<buoy> &redBuoys, vector<buoy> &yellowBuoys, vector<buoy> &blueBuoys, vector<gate> &gates, vector<path> &paths,	vector<wall> &greenWall, vector<wall> &redWall, vector<wall> &blueWall, CvPoint target)
{
	//draw the green buoys
	for(unsigned int i = 0; i < greenBuoys.size(); i++)
	{
		CvPoint pt = cvPoint(cvRound(greenBuoys[i].x), cvRound(greenBuoys[i].y));
		cvCircle(out, pt, cvRound(greenBuoys[i].radius), CV_RGB(0, 128, 0), 3);
		//cout<<greenBuoys[i].x<<"     "<<greenBuoys[i].y<<"  "<<i<<endl;
	}

	//draw the red buoys
	for(unsigned int i = 0; i < redBuoys.size(); i++)
	{
		CvPoint pt = cvPoint(cvRound(redBuoys[i].x), cvRound(redBuoys[i].y));
		cvCircle(out, pt, cvRound(redBuoys[i].radius), CV_RGB(255, 0, 0), 3);
		//cout<<redBuoys[i].y<<"  b  "<<i<<endl;
	}

	//draw the yellow buoys
	for(unsigned int i = 0; i < yellowBuoys.size(); i++)
	{
		CvPoint pt = cvPoint(cvRound(yellowBuoys[i].x), cvRound(yellowBuoys[i].y));
		cvCircle(out, pt, cvRound(yellowBuoys[i].radius), CV_RGB(255, 255, 0), 3);
	}

	////draw the blue buoys
	for(unsigned int i = 0; i < blueBuoys.size(); i++)
	{
		CvPoint pt = cvPoint(cvRound(blueBuoys[i].x), cvRound(blueBuoys[i].y));
		cvCircle(out, pt, cvRound(blueBuoys[i].radius), CV_RGB(0, 0, 255), 3);
	}

	//draw the gates
	for(unsigned int i = 0; i < gates.size(); i++)
	{
		cvLine(out, gates[i].green, gates[i].red, CV_RGB(255, 255, 255), 3);
		//cout<<"Yellow buoy"<<gates[i].yellow.x<<endl;
	}

	//draw the target path
	for(unsigned int i = 0; i < paths.size(); i++)
	{
		cvLine(out, paths[i].nearEnd, paths[i].farEnd, CV_RGB(0, 0, 0), 3);	
	//	cout<<paths[i].length<<"  "<<paths[i].slope<<endl;
	}

	//draw control path 
	cvLine(out, paths[0].nearEnd, target, CV_RGB(150, 0, 40), 3);
	//cout<<endl;
	////draw the walls
	for(unsigned int i = 0; i < greenWall.size(); i++)
	{
		cvLine(out, greenWall[i].nearEnd, greenWall[i].farEnd, CV_RGB(0, 128, 0), 3);
	}
	for(unsigned int i = 0; i < redWall.size(); i++)
	{
		cvLine(out, redWall[i].nearEnd, redWall[i].farEnd, CV_RGB(255, 0, 0), 3);
	}
	for(unsigned int i = 0; i < blueWall.size(); i++)
	{
		cvLine(out, blueWall[i].nearEnd, blueWall[i].farEnd, CV_RGB(0, 0, 255), 3);
	}
}

/*
	This function can be used to read the tuning parameters in from the command prompt
*/
void inputParams(float &closingOnGateDen, float &closingPWM, float &PWMoffset, float &maxThrottle, float &diffCoef, float &leftOff, float &rightOff)
{
	cout<<"Closing on Gate Denominator: ";
	cin>>closingOnGateDen;
	if(closingOnGateDen <=1 ) closingOnGateDen = 1;
	else if (closingOnGateDen > 100) closingOnGateDen = 100;
	else closingOnGateDen = closingOnGateDen;

	cout<<endl<<"Closing PWM: ";
	cin>>closingPWM;
	if(closingPWM <=0 ) closingPWM = 10;
	else if (closingPWM > 100) closingPWM = 100;
	else closingPWM = closingPWM;

	cout<<endl<<"PWM offset: ";
	cin>>PWMoffset;
	if(PWMoffset <=0 ) PWMoffset = 10;
	else if (PWMoffset > 100) PWMoffset = 100;
	else PWMoffset = PWMoffset;

	cout<<endl<<"Max Throttle PWM: ";
	cin>>maxThrottle;
	if(maxThrottle <=0 ) maxThrottle = 10;
	else if (maxThrottle > 100) maxThrottle = 100;
	else maxThrottle = maxThrottle;

	cout<<endl<<"Differential Coefficient: ";
	cin>>diffCoef;
	if(diffCoef <=0 ) diffCoef = 0;
	else if (diffCoef > 100) diffCoef = 100;
	else diffCoef = diffCoef;

	cout<<endl<<"Left Drive Motor Offset: ";
	cin>>leftOff;
	if(leftOff <=0 ) leftOff = 0;
	else if (leftOff > 100) leftOff = 100;
	else leftOff = leftOff;

	cout<<endl<<"Right Drive Motor Offset: ";
	cin>>rightOff;
	if(leftOff <=0 ) rightOff = 0;
	else if (leftOff > 100) rightOff = 100;
	else rightOff = rightOff;
}

/* use this function to add a trackbar */
void onTrackbarSlide(int pos) 
{
	inv = pos;
}

/*
	this is a wrapper function to perform a canny edge detection on an image. It will 
	converter the input image to grayscale before performing the edge detection and 
	output the edge image
*/
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
