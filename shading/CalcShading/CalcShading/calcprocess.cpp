#include "calcprocess.h"
#include <stdio.h>

int GetShading(unsigned char * imageBuf, int height, int width)
{
	if (imageBuf == NULL || width <= 0 || height <= 0)
	{
		printf("--- PARAM_ERROR! --- \nFile : %s,\nFunction :  %s,\nLine[%d],\nTimestemp :  %s\n", __FILE__, __FUNCTION__, __LINE__, __TIMESTAMP__);
		return PARAM_ERROR;
	}
	int i, j;
	int par = 7;
	int heightPar = height / par;
	int widthPar = width / par;
	int shading = -1;
	int  mean[5] = { 0, 0, 0, 0, 0 };
	
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			if (i > heightPar * 3 && i < heightPar * 4 && j > widthPar * 3 && j < widthPar * 4)//center
			{
				mean[0] += imageBuf[j + i * width];
			}
			
			if (i > heightPar * 1 && i < heightPar* 2 && j > widthPar * 1 && j < widthPar* 2)//leftup
			{
				mean[1] += imageBuf[j + i * width];
			}
			if (i > heightPar * 1 && i < heightPar * 2 && j > widthPar * 5 && j < widthPar * 6)//rightup
			{
				mean[2] += imageBuf[j + i * width];
			}
			if (i > heightPar * 5 && i < heightPar * 6 && j > widthPar * 1 && j < widthPar * 2)//leftdown
			{
				mean[3] += imageBuf[j + i * width];
			}
			if (i > heightPar * 5 && i < heightPar * 6 && j > widthPar * 5 && j < widthPar * 6)//rightdown
			{
				mean[4] += imageBuf[j + i * width];
			}
		}
	}

	i = 0;
	do
	{
		mean[i] /= (widthPar * heightPar);
		++i;
	} while (i < 5);
	
	return shading = (mean[1] + mean[2] + mean[3] + mean[4]) * 100 / (mean[0] * 4);;
}

