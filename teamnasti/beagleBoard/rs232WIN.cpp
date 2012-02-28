

#include "stdafx.h"
using namespace std;
using namespace cv;

int OpenComport(int, int){return (0);}
int PollComport(int, unsigned char *, int){return (0);}
int SendByte(int, unsigned char){return (0);}
int SendBuf(int channel, unsigned char *buff, int size)
{
	cout<<"Serial Data: ";
	for(unsigned char i = 0; i < size; i++)
	{
		cout<<buff[i]<<" ";
	}
	cout<<endl;
	return (0);
}
void CloseComport(int){}
void cprintf(int, const char *){}
int IsCTSEnabled(int){return (0);}