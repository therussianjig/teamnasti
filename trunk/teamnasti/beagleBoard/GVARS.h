/*****************************************************************************************
* Author: Terry Max Christy
* Bradley University 
* Nautical Autonomous System with Task Integration (code name NASTI)
* Date written: 10/20/11 Last Modified: 1/26/12
*****************************************************************************************/

static int g_slider_position = 0;
static int inv  = 10;
static int filter;

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