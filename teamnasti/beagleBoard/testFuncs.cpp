/*****************************************************************************************
// Author: Terry Max Christy
// Bradley University 
// Nautical Autonomous System with Task Integration (code name NASTI)
// Date written: 10/20/11 Last Modified: 10/20/11
*****************************************************************************************/
//This file is my sandbox. I have functions that are in progress here, as well as notes, 
//old code that I dont want to throw away yet, ect. It is/will be messy. 

#include "stdafx.h"
using namespace std;
using namespace cv;

IplImage* doPyrDown(IplImage* in,int filter = IPL_GAUSSIAN_5x5) 
{
	// Best to make sure input image is divisible by two.
	//
	assert( in->width%2 == 0 && in->height%2 == 0 );
	IplImage* out = cvCreateImage(cvSize( in->width/2, in->height/2 ),in->depth,in->nChannels);
	cvPyrDown(in, out);
	return( out );
}

IplImage* addROI(IplImage* in, int horizon)
{	
	IplImage* out = cvCreateImage(cvGetSize(in), in->depth, in->nChannels);
	cvCopy(in, out, NULL);
	int x = 20;//out->origin; 
	int y = 100;

	int width = 20;//out->width;
	int height = 20;//out->height - y;
	int add = 32;

	//if(horizon < out ->height)
	//{
	//	y = out->origin + horizon;
	//}
	//else y = out->origin;

	CvRect water = cvRect(x,y,width,height);

	IplImage *sub_img = cvCreateImageHeader(cvSize(water.width,water.height), out->depth, out->nChannels);
	sub_img->origin = out->origin;
	sub_img->widthStep = out->widthStep;
	sub_img->imageData = out->imageData + water.y * out->widthStep + water.x * out->nChannels;
	cvAddS( sub_img, cvScalar(add), sub_img );
	cvReleaseImageHeader(&sub_img);

	// perform Canny edge detect
	// out = doCanny(in, 10, 100, 3);
	//out = doPyrDown(out, filter);
	return (out);
}


//functions to set ROI and stuff
//cvSetImageROI(src, cvRect(x,y,width,height));
//cvAddS(src, cvScalar(add),src);
//cvResetImageROI(src);

//****Histogram work
	////Input the image of the red buoy and then create a histogram of RGB values ********
	//IplImage* red = cvLoadImage( "horizon5.jpg" );
	////split the color pannel
	//IplImage* r = cvCreateImage( cvGetSize(red), IPL_DEPTH_8U, 1 );
	//IplImage* g = cvCreateImage( cvGetSize(red), IPL_DEPTH_8U, 1 );
	//IplImage* b = cvCreateImage( cvGetSize(red), IPL_DEPTH_8U, 1 );
	//IplImage* planes[] = {r , g, b};
	//cvSplit( red, r, g, b, NULL );

	//int rbins = 30, gbins = 32;
	//CvHistogram* hist;
	//{
	//	int hist_size[] = { rbins, gbins };
	//	float r_ranges[] = { 0, 255 }; 
	//	float g_ranges[] = { 0, 255 };
	//	float* ranges[] = { r_ranges, g_ranges };
	//	hist = cvCreateHist(2, hist_size, CV_HIST_ARRAY, ranges, 1 );
	//}
	//cvCalcHist( planes, hist, 0, 0 ); //Compute histogram
	//cvNormalizeHist( hist, 1.0 ); //Normalize it
	//// Create an image to use to visualize our histogram.
	////
	//int scale = 20;
	//IplImage* hist_img = cvCreateImage(cvSize( rbins * scale, gbins * scale ), IPL_DEPTH_8U, 3);
	//cvZero( hist_img );
	//// populate our visualization with little gray squares.
	////
	//float max_value = 0;
	//cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
	//for( int r = 0; r < rbins; r++ )
	//{
	//	for( int g = 0; g < gbins; g++ ) 
	//	{
	//		float bin_val = cvQueryHistValue_2D( hist, r, g );
	//		int intensity = cvRound( bin_val * 255 / max_value );
	//		cvRectangle(hist_img,cvPoint( r*scale, g*scale ),cvPoint( (r+1)*scale - 1, (g+1)*scale - 1),
	//			CV_RGB(intensity,intensity,intensity),CV_FILLED);
	//	}
	//}
//****
//********* binary threshold to find red buoy....didn't work
	////split the color pannel
	//IplImage* r = cvCreateImage( cvGetSize(red), IPL_DEPTH_8U, 1 );
	//IplImage* g = cvCreateImage( cvGetSize(red), IPL_DEPTH_8U, 1 );
	//IplImage* b = cvCreateImage( cvGetSize(red), IPL_DEPTH_8U, 1 );
	////IplImage* planes[] = {r , g, b};
	//cvSplit( red, r, g, b, NULL );

	////adaptive filter 
	//IplImage* newout = cvCreateImage( cvGetSize(red), IPL_DEPTH_8U, 1);
	//cvSmooth(newout, newout, CV_GAUSSIAN, 5, 5, 100);
	//cvAdaptiveThreshold(r, newout, 260, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY);
//**********************

// 11/5/11 horizon ROI stuff
	//cvCopy(in, out, NULL);
	//int x = out->origin; 
	//int y = 0;
	//if(horizon < out ->height)
	//{
	//	y = out->origin + horizon;
	//}
	//else y = out->origin;
	//int width = out->width;
	//int height = out->height - y;
	//int add = 100;
	////Make a recatangle of the water 
	//CvRect water = cvRect(x,y,width,height);
	////Make subimage of just the water, no point waisting processing power on the sky/trees
	//IplImage *sub_img = cvCreateImageHeader(cvSize(water.width,water.height), out->depth, out->nChannels);
	//sub_img->origin = out->origin;
	//sub_img->widthStep = out->widthStep;
	//sub_img->imageData = out->imageData + water.y * out->widthStep + water.x * out->nChannels;
	//
	////blur the image using the gaussian smoothing algorithm
	//cvSmooth(sub_img, sub_img, CV_GAUSSIAN, 5, 5, 100);

	////turn sub_img blue
	////cvAddS( sub_img, cvScalar(add), sub_img );
//****************************


//*************&&&&&&&&&&***************&&&&&&&&&&&**************&&&&&&&&&&*************&&&&&&&&&&&&&*************&&&&&&&&&&***
//// Program, all of wich is cointained here, that will detect faces...and eyes if you wanted it to. 
////It is the faster version using the lbp rather than the haar to compair to. I don't know what either of
////those are and I cannot find a good discription.
///**
// * @file objectDetection2.cpp
// * @author A. Huaman ( based in the classic facedetect.cpp in samples/c )
// * @brief A simplified version of facedetect.cpp, show how to load a cascade classifier and how to find objects (Face + eyes) in a video stream - Using LBP here
// */
//#include "StdAfx.h"
//#include "opencv2/objdetect/objdetect.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//
//#include <iostream>
//#include <stdio.h>
//
//using namespace std;
//using namespace cv;
//
///** Function Headers */
//void detectAndDisplay( Mat frame );
//
///** Global variables */
//String face_cascade_name = "lbpcascade_frontalface.xml";
//String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
//CascadeClassifier face_cascade;
//CascadeClassifier eyes_cascade;
//string window_name = "Capture - Face detection";
//
//RNG rng(12345);
//
///**
// * @function main
// */
//int main( int argc, const char** argv )
//{
//  CvCapture* capture;
//  Mat frame;
//
//  //-- 1. Load the cascade
//  if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
//  if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
//
//  //-- 2. Read the video stream
//  capture = cvCaptureFromCAM( -1 );
//  if( capture )
//  {
//    while( true )
//    {
//      frame = cvQueryFrame( capture );
//  
//      //-- 3. Apply the classifier to the frame
//      if( !frame.empty() )
//       { detectAndDisplay( frame ); }
//      else
//       { printf(" --(!) No captured frame -- Break!"); break; }
//      
//      int c = waitKey(1);
//      if( (char)c == ' ' ) { break; } 
//    }
//  }
//  return 0;
//}
//
///**
// * @function detectAndDisplay
// */
//void detectAndDisplay( Mat frame )
//{
//   std::vector<Rect> faces;
//   Mat frame_gray;
//
//   cvtColor( frame, frame_gray, CV_BGR2GRAY );
//   equalizeHist( frame_gray, frame_gray );
//
//   //-- Detect faces
//   face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0, Size(80, 80) );
//
//   for( int i = 0; i < faces.size(); i++ )
//    {
//      //Mat faceROI = frame_gray( faces[i] );
//      //std::vector<Rect> eyes;
//
//      ////-- In each face, detect eyes
//      //eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
//      //if( eyes.size() == 2)
//      //{
//         //-- Draw the face
//         Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
//         ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 0 ), 2, 8, 0 );
//
//      //   //for( int j = 0; j < eyes.size(); j++ )
//      //   // { //-- Draw the eyes
//      //   //   Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 ); 
//      //   //   int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
//      //   //   circle( frame, center, radius, Scalar( 255, 0, 255 ), 3, 8, 0 );
//      //   // }
//      // }
//
//    } 
//   //-- Show what you got
//   imshow( window_name, frame );
//}
//**********&&&&&&&&&&&&&&&******************&&&&&&&&&&&&&&&*****************&&&&&&&&&&&&&&&*****************&&&&&&&&&&&&&**
// HISTOGRAM MATCHING KINDA WORKS ---- HISTOGRAM MATCHING KINDA WORKS ---- HISTOGRAM MATCHING KINDA WORKS ---- HISTOGRAM MATCHING KINDA WORKS 
	//KEEP(11/5/11)
	//int hbins_test = 6, sbins_test = 6;
	//int hbins_target = 30, sbins_target = 32;
	//int hbins_final = hbins_target-hbins_test+1;
	//int sbins_final = sbins_target-sbins_test+1;
	////Create an HS histogram of test and target image
	//IplImage* greenBuoy = cvLoadImage("green.jpg"); //test
	////cvCopy(out, greenBuoy, NULL);
	////cvCvtColor(out, greenBuoy, CV_RGB2GRAY);
	//CvHistogram* hist = RGB2HS_HShist(greenBuoy, hbins_test, sbins_test, 180, 255, 1);
	//CvHistogram* hist_target = RGB2HS_HShist(out, hbins_target, sbins_target, 180, 255, 1);

	////perform a patch back projection on the target image%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//IplImage* buoys = cvCreateImage(cvSize(hbins_final, sbins_final) , IPL_DEPTH_16S, 1);
	//cvCalcBackProjectPatch(hist_target, buoys,cvSize(hbins_test, sbins_target), hist, CV_COMP_INTERSECT , 1);
	////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	////************************************************************************************************
	//// Create an image to use to visualize our histogram.
	//int scale = 10;
	//IplImage* hist_img = cvCreateImage(cvSize(hbins_test*scale,sbins_test*scale),8,3);
	//cvZero( hist_img );

	//// populate our visualization with little gray squares.
	//float max_value = 0;
	//cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
	//for(int h=0;h<hbins_test;h++)
	//{
	//	for(int s=0;s<sbins_test;s++)
	//	{
	//		float bin_val = cvQueryHistValue_2D( hist, h, s );
	//		int intensity = cvRound( bin_val*255 / max_value );
	//		cvRectangle(hist_img,cvPoint(h*scale,s*scale),cvPoint( (h+1)*scale-1,(s+1)*scale-1),
	//			CV_RGB(intensity,intensity,intensity),CV_FILLED);
	//	}
	//}
	////**********************************************************************************
	//// Create an image to use to visualize our histogram.
	//scale = 10;
	//IplImage* hist_img2 = cvCreateImage(cvSize(hbins_target*scale,sbins_target*scale),8,3);
	//cvZero( hist_img2 );

	//// populate our visualization with little gray squares.
	//max_value = 0;
	//cvGetMinMaxHistValue( hist_target, 0, &max_value, 0, 0 );
	//for(int h=0;h<hbins_target;h++)
	//{
	//	for(int s=0;s<sbins_target;s++)
	//	{
	//		float bin_val = cvQueryHistValue_2D( hist_target, h, s );
	//		int intensity = cvRound( bin_val*255 / max_value );
	//		cvRectangle(hist_img2,cvPoint(h*scale,s*scale),cvPoint( (h+1)*scale-1,(s+1)*scale-1),
	//			CV_RGB(intensity,intensity,intensity),CV_FILLED);
	//	}
	//}
	//cvNamedWindow( "target",CV_WINDOW_AUTOSIZE);
	//cvShowImage( "target", hist_img2 );
	////cvDestroyWindow( "target" );
	//cvReleaseImage(&hist_img2);
	////**********************************************************************************
	////// Create an image to use to visualize our histogram.
	////scale = 10;
	////IplImage* hist_img3 = cvCreateImage(cvSize(hbins_final*scale,sbins_final*scale),8,3);
	////cvZero( hist_img3 );

	////// populate our visualization with little gray squares.
	////max_value = 0;
	////cvGetMinMaxHistValue( hist_target, 0, &max_value, 0, 0 );
	////for(int h=0;h<hbins_target;h++)
	////{
	////	for(int s=0;s<sbins_target;s++)
	////	{
	////		float bin_val = cvQueryHistValue_2D( hist_target, h, s );
	////		int intensity = cvRound( bin_val*255 / max_value );
	////		cvRectangle(hist_img2,cvPoint(h*scale,s*scale),cvPoint( (h+1)*scale-1,(s+1)*scale-1),
	////			CV_RGB(intensity,intensity,intensity),CV_FILLED);
	////	}
	////}	
	//cvNamedWindow( "buoys",CV_WINDOW_AUTOSIZE);
	//cvShowImage( "buoys", buoys );
	//////cvDestroyWindow( "target" );
	//cvReleaseImage(&buoys);
	////****************************************************************************************
	//cvReleaseHist(&hist);
	//cvReleaseHist(&hist_target);
	////----
	////IplImage* circles = cvCreateImage(cvGetSize(out), out->depth, out->nChannels);
	////cvCopy(out, circles, NULL);
	////RGB split----------------------------------------------------
	////IplImage* r = cvCreateImage( cvGetSize(in), IPL_DEPTH_8U, 1 );
	////IplImage* g = cvCreateImage( cvGetSize(in), IPL_DEPTH_8U, 1 );
	////IplImage* b = cvCreateImage( cvGetSize(in), IPL_DEPTH_8U, 1 );
	////cvSplit( in, r, g, b, NULL );
	////--------------------------------------------------------------
	////convert to 16 bit image+++++++++++++++++++++++++++++++++++++++
	////cvCopy(circles, circles16, NULL);
	////cvCvtScale(circles, circles16, 1, 0);
	////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	////Find circles (the buoys should be pretty definite circles by now) ###########################
	////CvMemStorage* buoys = cvCreateMemStorage(0);
	////cvSmooth( circles, circles, CV_GAUSSIAN, 3);
	//////cvThreshold(g, g, 200, 255, CV_THRESH_BINARY);
	//////cvCanny( g, g, 100, 250, 3 );

	////CvSeq* results = cvHoughCircles(circles, buoys, CV_HOUGH_GRADIENT, 4, 50, 200, 150, 0, 20);

	////for(int i = 0; i < results->total; i++)
	////{
	////	float* p = (float*) cvGetSeqElem(results, i);
	////	CvPoint pt = cvPoint(cvRound(p[0]), cvRound(p[1]));
	////	cvCircle(circles, pt, cvRound(p[2]), CV_RGB(255, 255, 255));
	////}
	////##############################################################################################
	////***********************************************************************************
	////***********************************************************************************


	////cvReleaseImageHeader(&water_img);
	////cvReleaseImage( &red );
	////cvReleaseImage( &r );
	////cvReleaseImage( &g );
	////cvReleaseImage( &b );
	//cvReleaseImage( &out );
	////cvReleaseMemStorage( &buoys );
	//// perform Canny edge detect
	////cvSmooth(in,  in, CV_BLUR, 5, 5);
	////circCanny = doCanny(in, 100, 200, 3);
	//// out = doPyrDown(out, filter);
	//cvReleaseImage(&greenBuoy);
// HISTOGRAM MATCHING KINDA WORKS ---- HISTOGRAM MATCHING KINDA WORKS ---- HISTOGRAM MATCHING KINDA WORKS ---- HISTOGRAM MATCHING KINDA WORKS 

//using two cameras 3/29/2012
//*******************************************************************************************************************************************
		//if(oneCAM == FALSE)
		//{
		//	img_full2 = cvQueryFrame(g_capture2);       //from video/camera2 
		//	if( !img_full2 ) break; 
		//	img2 =  cvCreateImage(cvSize(320,240), img_full2->depth, img_full2->nChannels);
		//	cvResize(img_full2,img2);

		//	if( !img1 || !img2 ) break; 
		// 
		//	CvSize size = cvSize(2*cvGetSize(img1).width, cvGetSize(img1).height);
		//	img =  cvCreateImage(size, img1->depth, img1->nChannels);
		//	cvSetImageROI(img, cvRect(0,0, cvGetSize(img1).width, cvGetSize(img1).height));
		//	cvCopy(img1, img, NULL);
		//	cvResetImageROI(img);
		//	cvSetImageROI(img, cvRect(cvGetSize(img1).width,0, cvGetSize(img2).width, cvGetSize(img2).height));
		//	cvCopy(img2, img, NULL);
		//	cvResetImageROI(img);
		//}
		//else
		//{
		//	img =  cvCreateImage(cvGetSize(img1), img1->depth, img1->nChannels);
		//	cvCopy(img1, img, NULL);
		//}
//*******************************************************************************************************************************************

	//Atempt to narrow the found buoys down to actual buoys by checking for the existance of a bouy 
	//within the circle
	/*buoys.resize(circles->total);
	int k = 0;
	for(int i = (circles->total); i > 0; i--)
	{
		float* p = (float*) cvGetSeqElem(circles, i);
		float x = p[0];
		float y = p[1];
		float radius = p[2];
		float diameter = 2*radius;
		float n =0;
		float total = diameter*diameter;

//		for( int k = 0; k < 2*p[2]; k++)
//		{
			uchar* ptr = (uchar*)(thresholded->imageData  + (int)(y) * thresholded->widthStep);
		//	for(int f = 0; f < 2*p[2]; f++)
		//	{
		//		if(ptr[(int)(p[0]-p[2]+f)]) n++;
		//		else n = n;
		//	}
		//}
		if(ptr[(int)(x)])
		{
			buoys[k].x= p[0];
			buoys[k].y = p[1];
			buoys[k].radius = p[2];
			k++;
		}
	}
	*/