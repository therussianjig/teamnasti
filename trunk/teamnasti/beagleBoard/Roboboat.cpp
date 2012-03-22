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
	int imgCount = 1;
	int key;
	int key2;
	IplImage* img_full = 0;
	IplImage* img_full2 = 0;
	IplImage* img = 0;
	IplImage* img2 = 0;

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
	//CvCapture* g_capture  = cvCaptureFromCAM(-1);
	//cvWaitKey(1000);
	//CvCapture* g_capture2 = cvCaptureFromCAM(0);
	CvCapture* g_capture = cvCreateFileCapture("highTight.avi");
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
	SendByte(cport_nr, 'J');
		// retrieve the captured frame and display it in a window 
		//IplImage* img =cvRetrieveFrame(g_capture);   //from camera

		img_full = cvQueryFrame(g_capture);       //from video
		if( !img_full ) break; 
		img =  cvCreateImage(cvSize(320,240), img_full->depth, img_full->nChannels);
		cvResize(img_full,img);

		//img_full2 = cvQueryFrame(g_capture2);       //from video
		//if( !img_full2 ) break; 
		// img2 =  cvCreateImage(cvSize(320,240), img_full2->depth, img_full2->nChannels);
		//cvResize(img_full2,img2);

		//IplImage* img = cvLoadImage("presentation.jpg"); //from image
		 //if( !img1 || !img2 ) break; 
		 
		 //CvSize size = cvSize(2*cvGetSize(img1).width, cvGetSize(img1).height);
		 //IplImage* img =  cvCreateImage(size, img1->depth, img1->nChannels);
		 //cvSetImageROI(img, cvRect(0,0, cvGetSize(img1).width, cvGetSize(img1).height));
		 //cvCopy(img1, img, NULL);
		 //cvResetImageROI(img);
		 //cvSetImageROI(img, cvRect(cvGetSize(img1).width,0, cvGetSize(img2).width, cvGetSize(img2).height));
		 //cvCopy(img2, img, NULL);
		 //cvResetImageROI(img);
#ifdef debug
		cvShowImage( "in", img ); 
#endif
		//do stuff in here****************************************************
		//********************************************************************
		int horizon = img->height/2;

		IplImage* out =  cvCreateImage(cvGetSize(img), img->depth, img->nChannels);

		cvCopy(img, out, NULL);

		
		
		//find the green buoys
		findBuoy(img, horizon, 'g', greenBuoys);

		//find the red buoys
		findBuoy(img, horizon, 'r', redBuoys);

		//find the yellow buoys
		//findBuoy(img, horizon, 'y', yellowBuoys);

		//find the blue buoys
		//findBuoy(img, horizon, 'b', blueBuoys);

		//construct the gates
		constructGates(greenBuoys, redBuoys, yellowBuoys, gates);
		
		//build the walls
		constructWall(greenBuoys, greenWall);
		constructWall(redBuoys, redWall);
		constructWall(blueBuoys, blueWall);
		 
		//find the path
		findPath(img, gates, path);

		//Determine motor signals
		navigateChannel(path, motors);
		
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
		SendByte(cport_nr,255);
		SendByte(cport_nr,255);
		SendByte(cport_nr,0);
		SendByte(cport_nr,0);
		SendByte(cport_nr,255);
		SendByte(cport_nr,255);
		//SendBuf(cport_nr, motorschar, 6);

		/** Drawing Stuff **********************************************************/
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
		/****************************************************************************/
		//********************************************************************
		//********************************************************************
		
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

		cvSaveImage(fName, out);
		//Show altered image in another window
		//cvShowImage( "out", out );
		cvReleaseImage( &out );//clean up after thyself
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
	//cvReleaseImage(&img); //I guess I don't need to do this 
#endif
	cvReleaseCapture(&g_capture); //Must release capture device or mem leak
	//cvReleaseCapture(&g_capture2);
	//Do not release the trackbar before the capture. The code will break. 
	return 0;
}
