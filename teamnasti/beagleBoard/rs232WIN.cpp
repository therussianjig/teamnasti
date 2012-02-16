

#include "stdafx.h"
using namespace std;
using namespace cv;

int OpenComport(int, int){return (0);}
int PollComport(int, unsigned char *, int){return (0);}
int SendByte(int, unsigned char){return (0);}
int SendBuf(int, unsigned char *, int){return (0);}
void CloseComport(int){}
void cprintf(int, const char *){}
int IsCTSEnabled(int){return (0);}