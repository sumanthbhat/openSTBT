/****************************************************************************
*  Program to calculate Peak Signal to Noise ratio 
*  Input Arguments : Two YUV 422 (UYVY) file names
*  Usage           : ./PSNR file-1-name file-2-name
*  File Size       : 720 x 480 resloution
*  Author          : Sumanth Bhat
*  Date            : 23 May 2017
*  E-mail          : sumanth@velankanigroup.com
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 720
#define HEIGHT 480

#define R 0
#define G 1
#define B 2

#define Y 0
#define U 1
#define V 2

int main(int argv,char** args)
{
	FILE *fSource,*fNoisySource;
	unsigned char c;
	long int PSNR=0;
	float PSNR_db;
	int Sum,Temp;
	int rowIndex,columnIndex;
	int count;

	if(argv < 3)
	{
		printf("Usage : PSNR path-to-reference-image.yuv path-to-captured-image.yuv\n");
		exit(0);
	}

	fSource = fopen(args[1],"r");
	fNoisySource=fopen(args[2],"r");

	if(fSource == NULL || fNoisySource == NULL)
	{
		printf("File Open error\n");
		exit(0);
	}
	for(rowIndex = 0 ;rowIndex<HEIGHT;rowIndex++)
	{
		for(columnIndex = 0;columnIndex<WIDTH;columnIndex++)
		{
			Temp = 0;
			Sum = (fgetc(fSource) - fgetc(fNoisySource));
			Sum *= Sum ;
			Temp += Sum;	
			Sum = (fgetc(fSource) - fgetc(fNoisySource));
			Sum *= Sum ;
			Temp += Sum;	
			Sum = (fgetc(fSource) - fgetc(fNoisySource));
			Sum *= Sum ;
			Temp += Sum;	
			PSNR += Temp;
		}
	}
	
	PSNR_db = (float)PSNR / (3*512*512);
	if(PSNR>0)
	{
		PSNR_db = (float)((20*log10((double)255))-(10*log10((double)PSNR_db)));
		printf("\nPSNR = %f dB\n",PSNR_db);
	}
	else
		printf("\nPSNR = infinity\n");
	fclose(fSource);
	fclose(fNoisySource);
	return 0;
}

