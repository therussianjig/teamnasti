/*****************************************************************************************
* Author: Terry Max Christy
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Date written: 10/20/11 Last Modified: 1/26/12
*****************************************************************************************/
//include all additional header files in StdAfx.h
//then include StdAfx.h in all other cpp files. 

#ifdef unix
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#endif

#include "stdafx.h"
using namespace std;
using namespace cv;

#ifndef unix
CRITICAL_SECTION crSec;
#endif

int main()
{
	CvCapture* g_capture  = 0;
	CvCapture* g_capture2 = 0;

	int imgCount = 1;
	int key;
	//char key2;
	IplImage* img_full = 0;
	IplImage* img_full2 = 0;
	IplImage* img = 0;
	IplImage* img1 = 0;
	IplImage* img2 = 0;
	IplImage* out = 0;

	vector<float> motors;
	unsigned char motorschar[6];
	vector<buoy> greenBuoys;
	vector<buoy> redBuoys;
	vector<buoy> yellowBuoys;
	vector<buoy> blueBuoys;
	vector<gate> gates;
	vector<path> path;
	vector<wall> greenWall;
	vector<wall> redWall;
	vector<wall> blueWall;
	bool RedRightReturn = FALSE;
	bool oneCAM = FALSE;

	char lighting = 0;

	float closingOnGateDen = 1.0;
	float closingPWM  = 60.0;
	float PWMoffset = 60.0;
	float maxThrottle = 100.0;
	float diffCoef = 1.0;
	float leftOff = 1.0;
	float rightOff = 1.0;

	inputParams(closingOnGateDen, closingPWM, PWMoffset, maxThrottle, diffCoef, leftOff, rightOff);

	//cout<<endl<<"One Camera? ";
	//cin>>key2;
	//if(key2 == 'y') oneCAM = TRUE;
	//else oneCAM = FALSE;
	oneCAM = TRUE;

	g_capture  = cvCaptureFromCAM(-1);
	//if(oneCAM == FALSE) g_capture2 = cvCaptureFromCAM(1);
	//else g_capture2 = 0x0;
	 
	//g_capture = cvCreateFileCapture("highTight.avi");
	//cvSetCaptureProperty( g_capture, CV_CAP_PROP_FRAME_WIDTH, 160 );
	//cvSetCaptureProperty( g_capture, CV_CAP_PROP_FRAME_HEIGHT, 140 );
	
	#ifndef unix
	//assert(g_capture); //assert is a windows ONLY macro
	#endif

	motors.resize(6);
	
	int cport_nr=16;       // /dev/ttyS2 (COM3 on windows)
    int bdrate=9600;
	OpenComport(cport_nr, bdrate);

    if(!cvGrabFrame(g_capture) /*|| !cvGrabFrame(g_capture2)*/) // capture a frame
	{             
            printf("Could not grab a frame\n\7");
            exit(0);
    }
#ifdef debug
	 // create a window
	cvNamedWindow( "in",CV_WINDOW_AUTOSIZE);
	cvNamedWindow( "out",CV_WINDOW_AUTOSIZE);
#endif
	//cvCreateTrackbar("Position","in",&g_slider_position,inv,onTrackbarSlide);
	//have to put this outside the loop, or you will leak memory all over the floor
	while(1==1)
	{   
	//SendByte(cport_nr, 'J');
		// retrieve the captured frame and display it in a window
		//img = cvLoadImage("balls.jpeg"); //from image
		//IplImage* img1 =cvRetrieveFrame(g_capture);   //from camera
		//if( !img1 ) break;
		
		img_full = cvQueryFrame(g_capture);       //from video/camera
		if( !img_full ) break; 
		img =  cvCreateImage(cvSize(320,240), img_full->depth, img_full->nChannels);
		cvResize(img_full,img);
//#ifdef debug
//		cvNamedWindow( "show",CV_WINDOW_AUTOSIZE);
//		cvShowImage( "show", img );
//#endif

		//(Insert) Using two Cameras 3/29/2012

#ifdef debug
		cvShowImage( "in", img ); 
#endif
		//do stuff in here****************************************************
		//********************************************************************
		int horizon = img->height/3;

		out =  cvCreateImage(cvGetSize(img), img->depth, img->nChannels);

		cvCopy(img, out, NULL);

		
		
		//find the green buoys
		findBuoy(img, horizon, 'g', greenBuoys, lighting);

		//find the red buoys
		img2 = findBuoy(img, horizon, 'r', redBuoys, lighting);

		//find the yellow buoys
		//findBuoy(img, horizon, 'y', yellowBuoys, lighting);

		//find the blue buoys
		//findBuoy(img, horizon, 'b', blueBuoys, lighting);

		//construct the gates
		constructGates(greenBuoys, redBuoys, yellowBuoys, gates);
		
		//determine leaving port/return to port
		RedRightReturn = redRightReturn(gates);

		//build the walls
		constructWall(greenBuoys, greenWall);
		constructWall(redBuoys, redWall);
		constructWall(blueBuoys, blueWall);
		 
		//find the path
		findPath(img, gates, path);

		//Determine motor signals
		navigateChannel(path, motors, closingOnGateDen, closingPWM, PWMoffset, maxThrottle, diffCoef, leftOff, rightOff);
		
		for(unsigned int i = 0; i < motors.size(); i++)
		{
			//motors[i] = 5;
			cout<<(int)motors[i]<<"   ";
			//SendByte(cport_nr,char(motors[i]));
		}
		cout<<endl;

//#ifdef debug
		
		SendByte(cport_nr,'*');
		SendByte(cport_nr,'*');
		SendByte(cport_nr,'*');
		pwm2uchar(motors, motorschar);
		//SendByte(cport_nr,255);
		//SendByte(cport_nr,255);
		//SendByte(cport_nr,0);
		//SendByte(cport_nr,0);
		//SendByte(cport_nr,255);
		//SendByte(cport_nr,255);
		SendBuf(cport_nr, motorschar, 6);

		/** Drawing Stuff **********************************************************/
		drawStuff(out, greenBuoys, redBuoys, yellowBuoys, blueBuoys, gates, path,	greenWall, redWall, blueWall);
		/****************************************************************************/

		
		char fName[50];
		char str[10];

		strcpy(fName, "test"); /* copy name into the new var */
		
		if (imgCount < 10) {
			strcat(fName, "000");
		}

		else if (imgCount < 100) {
			strcat(fName, "00");
		}

		else if (imgCount < 1000) {
			strcat(fName, "0");
		}

		#ifdef unix
		sprintf(str,"%d",imgCount);
		
		#else
		itoa(imgCount, str, 10); // 10 - decimal; 
		#endif
		
		strcat(fName, str);
		strcat(fName, ".jpeg"); /* add the extension */
		imgCount++;

#ifdef unix
		cvSaveImage(fName, img2);
		cvSaveImage(strcat(strcat("in",fName), ".jpeg"), img);
#endif
		//Show altered image in another window
#ifdef debug
		cvShowImage( "out", out );
#endif
		cvReleaseImage( &out );//clean up after thyself
		cvReleaseImage(&img);
		cvReleaseImage(&img1);
		cvReleaseImage(&img2);
		cvReleaseImage(&img_full);
//#endif
		// wait for a key arg = pos, wait that long, =0 or neg wait indeff
		key=cvWaitKey(1);
		if (key == 32) {break;}  // Press 'space' to exit program
		// wait for a key arg = pos, wait that long, =0 or neg wait indeff
		//key2=cvWaitKey(-1);
		//if (key2 == 110) {NULL;}  // Press 'n' to move to next frame
		//else if (key2 == 32) {break;}  // Press 'space' to exit program
	}
#ifdef debug
	cvDestroyWindow( "in" ); //good practice to destroy the windows you create
	cvDestroyWindow("out");
	cvDestroyWindow( "show" );
	//cvReleaseImage(&img); //I guess I don't need to do this 
#endif
	cvReleaseCapture(&g_capture); //Must release capture device or mem leak
	//cvReleaseCapture(&g_capture2);
	//Do not release the trackbar before the capture. The code will break. 
	return 0;
}

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

void drawStuff(IplImage* out, vector<buoy> &greenBuoys, vector<buoy> &redBuoys, vector<buoy> &yellowBuoys, vector<buoy> &blueBuoys, vector<gate> &gates, vector<path> &path,	vector<wall> &greenWall, vector<wall> &redWall, vector<wall> &blueWall)
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
	for(unsigned int i = 0; i < path.size(); i++)
	{
		cvLine(out, path[i].nearEnd, path[i].farEnd, CV_RGB(0, 0, 0), 3);	
	//	cout<<path[i].length<<"  "<<path[i].slope<<endl;
	}
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