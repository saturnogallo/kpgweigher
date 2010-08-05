// SerialKey.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

char kbuffer[100];

extern void RetrieveDriverID();
int _tmain(int argc, _TCHAR* argv[])
{
	int length,i,j,count;
	char data[100];

	RetrieveDriverID();
	char *kb2;
	kb2 = kbuffer;
	for(j=0;j<99;j++){
		if( ' ' != *kb2)
			break;
		kb2++;
	}

	length = strlen(kb2);
	while(length < 8)
	{
		kb2[length++]='1';
		kb2[length]='\0';
	}
	length = 8;
	for(j=0;j<8;j++)
	{
		count = 0;
		for(i=0;i<length;i++)
		{
			if(kb2[i]&(0x01<<j))
				count++;
		}
		count +=  kb2[j];
		if((count>>4) <= 9)
			data[2*j]='0'+(count>>4);
		else
			data[2*j]='A'+((count>>4)-0x0a);
		
		if((count&0x0f) <= 9)
			data[2*j+1]='0'+(count&0x0f);
		else
			data[2*j+1]='A'+((count&0x0f)-0x0a);
		
	}
	data[16]='\0';
	printf("%s",data);
	getchar();
	return 0;

}

