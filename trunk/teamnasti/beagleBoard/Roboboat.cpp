/*****************************************************************************************
* Author: Terry Max Christy
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Date written: 10/20/11 Last Modified: 4/24/12
* 
* Description:
* This file contains the main loop of the NASTI control system. The program has been written
* using a high level style of C++. The global variables and structures that are used can be
* found in the file stdafx.h and the functions for buoy detection can be found in imageProc.cpp
* The functions for the channel navigation algrothim can be found in pathPlanning.cpp and 
* the functions for controlling the motors can be found in locomotion.cpp
*
* This file is also where the image capture from the camera is defined and an image is read 
* in from the capture. These functions and my others used through the program for efficient
* image processing are from the openCV library. Once the image is acquired, the processing
* to locate the buoys is performed, followed by the path planning algorithm, then the motor
* control algorithm. There are many variables that can be altered to change gains throughout
* the control scheme, many of which are loaded into the program froma  text file called 
* params.txt or can be altered in the comand prompt before the main loop starts using the 
* function inputParams().
*****************************************************************************************/
//include all additional header files in StdAfx.h
//then include StdAfx.h in all other cpp files. 
//some librarys are specific to linux vs. windows, for this reason there are some things 
//included only in the linux build buy using the #ifdef statements as below. 

#ifdef unix
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include <fstream>
#endif

#include "stdafx.h"
#include <fstream>
using namespace std;
using namespace cv;

#ifndef unix
CRITICAL_SECTION crSec;
#endif

int main()
{
	CvCapture* g_capture  = 0;
	CvCapture* g_capture2 = 0; 
	/*
		^ Here we atempted to use two cameras, however the bandwidth of each of the cameras
		that were used was to larger for the BeagleBoard to handle so only one camera is 
		used. It would greatly increase the field of view using two cameras and thus increase
		the systems awareness greatly
	*/

	int imgCount = 1;
	int key;
	//char key2;
	IplImage* img_full = 0;
	IplImage* img_full2 = 0;
	IplImage* img = 0;
	IplImage* img1 = 0;
	IplImage* img2 = 0;
	IplImage* out = 0;

	vector<float> motors; //vector of motor values defined from 0-100 for pwm %
	unsigned char motorschar[6]; //character string for motor packet data after converting pwm
	vector<buoy> greenBuoys;
	vector<buoy> redBuoys;
	vector<buoy> yellowBuoys;
	vector<buoy> blueBuoys;
	vector<CvPoint> averageBuff; //buffer used for averaging the midpoint of the first gate. 
								 //this introduces a low pass filter into the control scheme
	vector<gate> gates;
	vector<path> paths;
	vector<wall> greenWall;
	vector<wall> redWall;
	vector<wall> blueWall;

	bool RedRightReturn = FALSE;
	bool oneCAM = FALSE;
	bool avoidYellow = false;

	char lighting = 0; //inteded to be used to change the HSV limits for buoy 
					   //detection based on lighting conditions, but time ran out 

	float closingOnGateDen = 1.0;
	float closingPWM  = 60.0;
	float PWMoffset = 60.0;
	float maxThrottle = 100.0;
	float diffCoef = 1.0;
	float leftOff = 1.0;
	float rightOff = 1.0;
	float optimalSlope = 1.0;
	float deadBand = 0.5;
	float yellowCoef = 10.0;
	int taps;
	float Ki = 0.0;
	float Kd = 0.0;
	float width = 0.0;
	CvPoint target;
	CvPoint oldPoint;
	path control; 

	//use this function if you want to read the customizable values in from the comand prompt
	//inputParams(closingOnGateDen, closingPWM, PWMoffset, maxThrottle, diffCoef, leftOff, rightOff);
	
	//use this block to read the paramaters in from the text file params.txt
	/*********get Params*********/
	ifstream pName("params.txt");
	float vals[12];
	float temp;
	int iLoop = 0;
	if(!pName) {
		cout << "You are missing your input File" << endl;
	}
	
	else {
		while(pName >> temp) {
		vals[iLoop] = temp;
		iLoop++;
		cout << temp << endl;
		}
	}
	
	closingOnGateDen = vals[0];
	closingPWM = vals[1];
	PWMoffset = vals[2];
	maxThrottle = vals[3];
	diffCoef = vals[4];
	leftOff = vals[5];
	rightOff = vals[6];
	taps = (int)vals[7];
	Ki = vals[8];
	Kd = vals[9];
	optimalSlope = vals[10];
	deadBand = vals[11];
	yellowCoef = vals[12];
	
	/************************/
	//uncomment if you wish to ask for two cameras, otherwise leave as it is
	//cout<<endl<<"One Camera? ";
	//cin>>key2;
	//if(key2 == 'y') oneCAM = TRUE;
	//else oneCAM = FALSE;
	oneCAM = TRUE;

	//capture from camera (as opposed to from video or image)
	g_capture  = cvCaptureFromCAM(-1);
	if(oneCAM == FALSE) g_capture2 = cvCaptureFromCAM(1);
	else g_capture2 = 0x0;

	//capture from a video
	//g_capture = cvCreateFileCapture("highTight.avi");
	//cvSetCaptureProperty( g_capture, CV_CAP_PROP_FRAME_WIDTH, 160 );
	//cvSetCaptureProperty( g_capture, CV_CAP_PROP_FRAME_HEIGHT, 140 );

	//initialize rolling average values
	averageBuff.resize(taps);
	img_full = cvQueryFrame(g_capture);       //from video/camera
	if( !img_full ) return(0); 
	img =  cvCreateImage(cvSize(320,240), img_full->depth, img_full->nChannels);
	cvResize(img_full,img);
	for(unsigned char i = 0; i < averageBuff.size(); i++)
	{
		averageBuff[i] = cvPoint(cvRound(img->width/2), 0);
	}
	target = cvPoint(cvRound(img->width/2), 0);
	oldPoint = cvPoint(cvRound(img->width/2), 0);
	#ifndef unix
	//assert(g_capture); //assert is a windows ONLY macro
	#endif

	//set the number of motors, currently there are 6, two main thrust and 4 side thrust in the 
	// following configuration
	/*
			0---------5
			.         .
	(side)	.         . (side)
			.         .	
			1---------4
			   2---3
			  (main)
	*/
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
	// Create windows to display the image read in and the image after the processing
	// has been performed. The drawing and display functions are only perfomed in 
	// debug mode so the time is not waisted on the BeagleBoard
	cvNamedWindow( "in",CV_WINDOW_AUTOSIZE);
	cvNamedWindow( "out",CV_WINDOW_AUTOSIZE);
#endif
	//have to put this outside the loop, or you will leak memory 
	//cvCreateTrackbar("Position","in",&g_slider_position,inv,onTrackbarSlide);
	while(1==1) //main loop*******************************************************
	{   
		// retrieve the captured frame and display it in a window
		//img = cvLoadImage("balls.jpeg");			    //from image
		//IplImage* img1 =cvRetrieveFrame(g_capture);   //from camera
		//if( !img1 ) break;
		
		img_full = cvQueryFrame(g_capture);       //from video/camera(works both BB and windows)
		if( !img_full ) break; 
		img =  cvCreateImage(cvSize(320,240), img_full->depth, img_full->nChannels);
		cvResize(img_full,img); //shrink the image to reduce processing time

		/*If two cameras are used, paste code that is titled Using two Cameras here from testFuncs*/
		//(Insert) Using two Cameras 3/29/2012 

#ifdef debug
		cvShowImage( "in", img ); 
#endif
		//********************************************************************
		//********************************************************************
		int horizon = img->height/3; //hard code horizon to be one third of the way down

		out =  cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
		cvCopy(img, out, NULL); //make out a copy of in
		
		//find the buoys
		img1 = findBuoy(img, horizon, 'g', greenBuoys, lighting);  //green
		img2 = findBuoy(img, horizon, 'r', redBuoys, lighting);    //red
		img2 = findBuoy(img, horizon, 'y', yellowBuoys, lighting); //yellow
		//img2 = findBuoy(img, horizon, 'b', blueBuoys, lighting); //blue
		/*
			currently, the code displayed the images as the processing is performed, if the 
			thresholded binary image wish to be views for a particullar color, you can just 
			comment out all other colors
		 */

		//check for an obstacle (yellow buoy)
		avoidYellow = checkForObstacle(greenBuoys, redBuoys, yellowBuoys);

		//construct the gates
		constructGates(greenBuoys, redBuoys, yellowBuoys, gates, avoidYellow);
		
		//determine leaving port/return to port
		RedRightReturn = redRightReturn(gates);

		//build the walls
		constructWall(greenBuoys, greenWall);
		constructWall(redBuoys, redWall);
		constructWall(blueBuoys, blueWall);
		 
		//find the path
		findPath(img, gates, paths);

		//determine a control signal (Gc)
		width = (float)img->width;
		target = rollAverage(averageBuff, paths);                        //low pass filter
		//intigrator(&target, &paths[0].farEnd, Ki, width);                //integrator
		//target = differentiator(&oldPoint, &paths[0].farEnd, Kd, width); //differentiator
		constructControl(&(paths[0].nearEnd), &target, &control);

		//Determine motor signals
		if(avoidYellow == false)
		{
			navigateChannel(&control, motors, paths[0].height,
				closingOnGateDen, closingPWM, PWMoffset, maxThrottle, diffCoef, leftOff, rightOff);
		}
		else
		{avoidObsticle(&control, paths, motors, paths[0].height, 
			PWMoffset, maxThrottle, diffCoef, yellowCoef, leftOff, rightOff, optimalSlope, deadBand);
		}
				
		for(unsigned int i = 0; i < motors.size(); i++)
		{
			cout<<(int)motors[i]<<"   "; //output motor char values
		}
		cout<<endl;
		
		//acknowledgement bytes
		SendByte(cport_nr,'*');
		SendByte(cport_nr,'*');
		SendByte(cport_nr,'*');
		pwm2uchar(motors, motorschar); //converter pwm(0-100) to packet equivalent
		SendBuf(cport_nr, motorschar, 6); //send the buffer of values to the MIM

		/** Drawing Stuff **********************************************************/
		drawStuff(out, greenBuoys, redBuoys, yellowBuoys, blueBuoys, gates, paths,	greenWall, redWall, blueWall, target);
		/****************************************************************************/

		
		char fName[50];
		char str[10];

		strcpy(fName, "test"); /* copy name into the new var */
		
		if (imgCount < 10) {strcat(fName, "000");}
		else if (imgCount < 100) {strcat(fName, "00");}
		else if (imgCount < 1000) {strcat(fName, "0");}

		#ifdef unix
		sprintf(str,"%d",imgCount);
		#else
		itoa(imgCount, str, 10); // 10 - decimal; 
		#endif
		
		strcat(fName, str);
		strcat(fName, ".jpeg"); /* add the extension */
		imgCount++;

#ifdef unix
		cvSaveImage(fName, out);
		//cvSaveImage(strcat(strcat("in",fName), ".jpeg"), img);
#endif
		//Show altered image in another window
#ifdef debug
		cvShowImage( "out", out );
#endif
		//release images to prevent memory leaks
		cvReleaseImage( &out );//clean up after thyself
		cvReleaseImage(&img);
		cvReleaseImage(&img1);
		cvReleaseImage(&img2);
		//cvReleaseImage(&img_full);

		// wait for a key arg = pos, wait that long, =0 or neg wait indeff
		key=cvWaitKey(1);
		if (key == 32) {break;}  // Press 'space' to exit program
		//use the following lines to step through a video frame by frame or grab from camera one at at time
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
	cvReleaseCapture(&g_capture2);
	//Do not release the trackbar before the capture. The code will break. 
	return 0;
 }
