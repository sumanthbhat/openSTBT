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
#include <stdbool.h>

#define WIDTH 720
#define HEIGHT 480

bool enable_masking = false;

struct Rectangle
{
	int top_left_corner_x;
	int top_left_corner_y;
	int bottom_right_corner_x;
	int bottom_right_corner_y;	
};

typedef struct Rectangle Rectangle;

//Masking details obtained from factory image

void initMask(Rectangle* FrameMask)
{
	FrameMask->top_left_corner_x = 58;
	FrameMask->top_left_corner_y = 128;
	FrameMask->bottom_right_corner_x = 90;
	FrameMask->bottom_right_corner_y = 614;
}

void ParseArguments(int argv,char** args)
{
	if(argv < 3 || argv>5)
	{
		printf("Usage : PSNR path-to-reference-image.yuv path-to-captured-image.yuv [options]\n");
		printf("Options: -m 1 for enabling masking");
		exit(0);
	}
    
    if(argv>3)
    {
    	if((strcmp(args[3],"-m") || strcmp(args[4],"1"))==0)
    	{
    		enable_masking = true;	
    		printf("Enabling default masking options\n");
    	}
    	else
    	{
    		printf("Error in CLI arguments\n");
    		exit(0);
    	}
    }
}

bool is_pixel_inside_masking_region(Rectangle FrameMask,int rowIndex, int columnIndex)
{
	if( columnIndex >= FrameMask.top_left_corner_y)
	{
		if(rowIndex >= FrameMask.top_left_corner_x)
		{
			if(columnIndex < FrameMask.bottom_right_corner_y)
			{
				if(rowIndex <= FrameMask.bottom_right_corner_x)
				{
					return true;
				}
			}
		}
	}

	return false;
}


int main(int argv,char** args)
{
	FILE *fSource,*fNoisySource,*fMaskedRegion;
	long unsigned int byteCount = 0;

	long int PSNR=0;
	float PSNR_db;
	int Sum,Temp;
	int rowIndex,columnIndex, maskedPixelCount=0;

	Rectangle FrameMask;

	initMask(&FrameMask);

	ParseArguments(argv,args);

	if(enable_masking)
	{
		fMaskedRegion = fopen("Masked_Source_Region.yuv","w+");
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
			if(enable_masking)
			{
				if(is_pixel_inside_masking_region(FrameMask,rowIndex,columnIndex))
				{
					fputc(fgetc(fSource),fMaskedRegion);
					fputc(fgetc(fSource),fMaskedRegion);
					fputc(fgetc(fSource),fMaskedRegion);
					fputc(fgetc(fSource),fMaskedRegion);
					// 4 bytes correspond to 2 pixels
					maskedPixelCount += 4;
					columnIndex++;
					if(columnIndex == WIDTH)
					{
						columnIndex = 0;
						rowIndex++;
						if(rowIndex == HEIGHT)
						{
							break;
						}
					}
					fgetc(fNoisySource)	;
					fgetc(fNoisySource)	;
					fgetc(fNoisySource)	;
					fgetc(fNoisySource)	;
					continue;
				}
			}
			Temp = 0;
			Sum = (fgetc(fSource) - fgetc(fNoisySource));
			Sum *= Sum ;
			Temp += Sum;	
	
			Sum = (fgetc(fSource) - fgetc(fNoisySource));
			Sum *= Sum ;
			Temp += Sum;	
			PSNR += Temp;

			byteCount += 2;
		}
	}
	
	PSNR_db = (float)PSNR / (HEIGHT*WIDTH*2);
	if(PSNR>0)
	{
		PSNR_db = (float)((20*log10((double)255))-(10*log10((double)PSNR_db)));
		printf("\nPSNR = %f dB\n",PSNR_db);
	}
	else
		printf("\nPSNR = infinity\n");

	printf("ByteCount = %ld\nmaskedPixelCount = %d\nTotal = %ld\n",byteCount,maskedPixelCount, byteCount + (long int)maskedPixelCount);

	fclose(fSource);
	fclose(fNoisySource);
	if(enable_masking)
		fclose(fMaskedRegion);
	return 0;
}

