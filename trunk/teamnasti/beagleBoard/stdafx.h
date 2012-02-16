// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
// *****************************************************************
// Include this file in all other .cpp files in the program 

#pragma once

#include <stdlib.h>
#include <math.h>

#ifdef unix
#include </usr/include/opencv/cv.h>
#include </usr/include/opencv/highgui.h>

#else
#include <C:\opencv\include\opencv\cv.h>
#include <C:\opencv\include\opencv\highgui.h>

#endif

#define aftPort      0
#define forPort      1
#define drive1       2
#define drive2       3
#define aftStarboard 4
#define forStarboard 5

static int g_slider_position = 0;
static int inv  = 10;
static int filter;
static int maxPWM = 100;
static int minPWM = 20;

typedef struct
{
	float x;
	float y;
	float radius;
} buoy;

typedef struct
{
	CvPoint green;
	CvPoint red;
	CvPoint yellow;
	CvPoint goal;
} gate;

typedef struct
{
	CvPoint farEnd;
	CvPoint nearEnd;
} wall;

typedef struct
{
	CvPoint farEnd;
	CvPoint nearEnd;
	float slope;
	float length;
} path;


// TODO: reference additional headers your program requires here
//#include "GVARS.h"
#include "testFuncs.h"
#include "imageProc.h"
#include "roboboat.h"
#include "pathPlanning.h"
#include "locomotion.h"