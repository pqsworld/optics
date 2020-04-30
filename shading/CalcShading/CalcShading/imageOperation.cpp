//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "imageOperation.h"
//#include "DefineParameter.h"

static int Load_Img(const char filename[], unsigned char *ucImg);
static int Load_Bmp(const char filename[], unsigned char *ucImg, int *h, int *w);

int GFPLoadImage(const char filename[], unsigned char *ucImage, int *h, int *w)
{
	int i;
	char str[50] = { 0 };
	int ImgSize = 0;

	*h = 0;
	*w = 0;

	for (i = strlen(filename) - 1; i >= 0; i--)
	{
		if (filename[i] == '.')
		{
			strcpy_s(str, filename + i + 1);
			break;
		}
	}
	if (str == "")
		return ImgSize;

	_strlwr_s(str);//转小写

	if (strcmp(str, "img") == 0 || strcmp(str, "dat") == 0)
	{
		ImgSize = Load_Img(filename, ucImage);

		return ImgSize;
	}

	if (strcmp(str, "bmp") == 0)
	{
		ImgSize = Load_Bmp(filename, ucImage, h, w);

		return ImgSize;
	}

	return ImgSize;
}

static int Load_Img(const char filename[], unsigned char *ucImg)
{
	FILE *fp;
	unsigned char *p;
	int ImgSize = -1;

	fopen_s(&fp, filename, "rb");

	if (fp == NULL)
	{
		return ImgSize;
	}

	p = ucImg;

	while (!feof(fp))
	{
		*p++ = fgetc(fp);
		ImgSize++;
	}

	fclose(fp);

	return ImgSize;
}

static int Load_Bmp(const char filename[], unsigned char *ucImg, int *h, int *w)
{
	int i, j, k, l, LineBytes;
	unsigned char c;
	int ImgSize = -1;

	FILE *fp;

	WL_BITMAPFILEHEADER		bmfHdr;
	WL_BITMAPINFOHEADER		bmiHdr;
	unsigned int			palNum;
	int errnum = 0;
	//errno = 0;
	fopen_s(&fp, filename, "rb");
	/*errnum = errno;
	printf("%s\n error = %d", filename,errnum);*/
	
	if (fp == NULL)
		return ImgSize;

	fread(&bmfHdr, 1, sizeof(WL_BITMAPFILEHEADER), fp);
	if (bmfHdr.bfType != BMP_TYPE)
	{
		return ImgSize;
	}

	fread(&bmiHdr, 1, sizeof(WL_BITMAPINFOHEADER), fp);
	if (bmiHdr.biSize != sizeof(WL_BITMAPINFOHEADER))
	{
		return ImgSize;
	}

	switch (bmiHdr.biBitCount)
	{
	case 1:
	case 8:
		palNum = (bmiHdr.biClrUsed != 0) ? bmiHdr.biClrUsed : (1 << bmiHdr.biBitCount);
		break;
	case 24:
	case 32:
		palNum = 0;
		break;
	default:
		return ImgSize;
	}

	ImgSize = bmiHdr.biHeight * bmiHdr.biWidth;

	*h = bmiHdr.biHeight;
	*w = bmiHdr.biWidth;

	//fseek(fp,sizeof(WL_RGBQUAD)*palNum,SEEK_CUR);
	fseek(fp, 1024 + 54, SEEK_SET);
	switch (bmiHdr.biBitCount)
	{
	case 1:
		LineBytes = WIDTHBYTES(bmiHdr.biWidth);
		for (i = 0; i<bmiHdr.biHeight; i++)
		for (j = 0; j<LineBytes; j++)
		{
			c = fgetc(fp);
			k = j * 8;
			for (l = 0; l<8; l++)
			if (k + l<bmiHdr.biWidth)
				ucImg[(bmiHdr.biHeight - 1 - i)*bmiHdr.biWidth + k + l] = (c >> (7 - l)) & 1;
		}
		break;

	case 8:  // 假设为8位灰度图，不考虑8位索引图
		LineBytes = WIDTHBYTES(bmiHdr.biWidth * 8);
		for (i = 0; i<bmiHdr.biHeight; i++)
		for (j = 0; j<LineBytes; j++)
		{
			c = fgetc(fp);

			if (j<bmiHdr.biWidth)
				ucImg[(bmiHdr.biHeight - 1 - i)*bmiHdr.biWidth + j] = c;
		}
		break;

	case 24:
		LineBytes = WIDTHBYTES(bmiHdr.biWidth * 3 * 8);
		for (i = 0; i<bmiHdr.biHeight; i++)
		{
			for (j = 0; j<bmiHdr.biWidth; j++)  // 0.2989 * R + 0.5870 * G + 0.1140 * B
			{
				unsigned char r, g, b;

				b = fgetc(fp);
				g = fgetc(fp);
				r = fgetc(fp);

				ucImg[(bmiHdr.biHeight - 1 - i)*bmiHdr.biWidth + j] = (unsigned char)(0.2989*r + 0.5870*g + 0.1140*b + 0.5);
			}

			for (j = 3 * (bmiHdr.biWidth); j<LineBytes; j++) fgetc(fp);
		}
		break;
	case 32:
		LineBytes = WIDTHBYTES(bmiHdr.biWidth * 3 * 8);
		for (i = 0; i<bmiHdr.biHeight; i++)
		{
			for (j = 0; j<bmiHdr.biWidth; j++)  // 0.2989 * R + 0.5870 * G + 0.1140 * B
			{
				unsigned char r, g, b;

				b = fgetc(fp);
				g = fgetc(fp);
				r = fgetc(fp);
				fgetc(fp);

				ucImg[(bmiHdr.biHeight - 1 - i)*bmiHdr.biWidth + j] = (unsigned char)(0.2989*r + 0.5870*g + 0.1140*b + 0.5);
			}
		}
	}

	fclose(fp);

	return ImgSize;
}





void GFP_Write_Bmp_8(const char filename[], unsigned char *ucImg, int h, int w)
{
	FILE *fp;
	int i, j, LineBytes, palNum = 256;

	WL_BITMAPFILEHEADER  bmfHdr;
	WL_BITMAPINFOHEADER  bmiHdr;
	LPWL_RGBQUAD         pPalette;

	fopen_s(&fp, filename, "wb");

	if (fp == NULL)
	{
		printf("save file error!\n");
		return;
	}

	// WL_BITMAPFILEHEADER
	bmfHdr.bfType = 0x4d42;
	bmfHdr.bfSize = sizeof(WL_BITMAPFILEHEADER)+
		sizeof(WL_BITMAPINFOHEADER)+
		sizeof(WL_RGBQUAD)*palNum +
		WIDTHBYTES(w * 8)*h;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = sizeof(WL_BITMAPFILEHEADER)+
		sizeof(WL_BITMAPINFOHEADER)+
		sizeof(WL_RGBQUAD)*palNum;
	fwrite(&bmfHdr, 1, sizeof(WL_BITMAPFILEHEADER), fp);

	// WL_BITMAPINFOHEADER
	bmiHdr.biSize = 40;
	bmiHdr.biWidth = w;
	bmiHdr.biHeight = h;
	bmiHdr.biPlanes = 1;
	bmiHdr.biBitCount = 8;
	bmiHdr.biCompression = BI_RGB;
	bmiHdr.biSizeImage = WIDTHBYTES(w * 8)*h;
	bmiHdr.biXPelsPerMeter = 0;
	bmiHdr.biYPelsPerMeter = 0;
	bmiHdr.biClrUsed = 0;
	bmiHdr.biClrImportant = 0;
	fwrite(&bmiHdr, 1, sizeof(WL_BITMAPINFOHEADER), fp);

	// PALETTE
	pPalette = (LPWL_RGBQUAD)malloc(sizeof(WL_RGBQUAD)*palNum);
	for (i = 0; i<palNum; i++)
	{
		pPalette[i].rgbBlue = i;
		pPalette[i].rgbGreen = i;
		pPalette[i].rgbRed = i;
		pPalette[i].rgbReserved = 0;
	}
	fwrite((void*)pPalette, sizeof(WL_RGBQUAD), palNum, fp);
	//计算图像每行像素所占的字节数（必须是4的倍数）
	LineBytes = WIDTHBYTES(w * 8);

	//PIXEL DATA
	for (i = 0; i<h; i++)
	for (j = 0; j<LineBytes; j++)
	if (j<w)
		fputc(ucImg[(h - 1 - i)*w + j], fp);
	else
		fputc('0', fp);

	fclose(fp);
	free(pPalette);

	return;
}









