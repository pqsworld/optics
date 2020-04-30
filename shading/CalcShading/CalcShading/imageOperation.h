#ifndef _H_IMAGEOPERATION_H_
#define _H_IMAGEOPERATION_H_


#define BI_RGB              0L
#define BI_RLE8             1L
#define BI_RLE4             2L
#define BMP_TYPE            0x4d42
#define WIDTHBYTES(bits)    (((bits)+31)/32*4)
#define MIN(x,y)            (x < y ? x : y)



#pragma   pack(1)  // 让编译器按1字节对齐


typedef struct _BITMAPFILEHEADER_
{

	unsigned short		bfType;
	unsigned long		bfSize;
	unsigned short		bfReserved1;
	unsigned short		bfReserved2;
	unsigned long		bfOffBits;

} WL_BITMAPFILEHEADER, *LPWL_BITMAPFILEHEADER;

typedef struct _BITMAPINFOHEADER_
{

	unsigned long		biSize;
	long				biWidth;
	long				biHeight;
	unsigned short      biPlanes;
	unsigned short      biBitCount;
	unsigned long		biCompression;
	unsigned long		biSizeImage;
	long				biXPelsPerMeter;
	long				biYPelsPerMeter;
	unsigned long		biClrUsed;
	unsigned long		biClrImportant;

} WL_BITMAPINFOHEADER, *LPWL_BITMAPINFOHEADER;

typedef struct _RGBQUAD_
{

	unsigned char    rgbBlue;
	unsigned char    rgbGreen;
	unsigned char    rgbRed;
	unsigned char    rgbReserved;

} WL_RGBQUAD, *LPWL_RGBQUAD;


#pragma   pack()  // 还原





#ifdef __cplusplus
extern "C" {
#endif


	int GFPLoadImage(const char filename[], unsigned char *ucImage, int *h, int *w);
	void GFP_Write_Bmp_1(const char filename[], unsigned char *ucImg, int h, int w);
	void GFP_Write_Bmp_8(const char filename[], unsigned char *ucImg, int h, int w);
	void GFP_Write_Bmp_24(const char filename[], WL_RGBQUAD *ucImg, int h, int w);
	int GFPLoadBin(const char filename[], unsigned char *ucImage, int h, int w);
	void GFP_Bmp_Change_8(unsigned char *imageChange, unsigned char *image, int h, int w, int c_h, int c_w);

	void SaveWSQ(char *name, unsigned char *data, int len);
	int WritePGM(char *PGMFileName, unsigned char *idata, int height, int width);
	double ImageComparePSNR(unsigned char *idata1, int height1, int width1,
		unsigned char *idata2, int height2, int width2);





#ifdef __cplusplus
}
#endif

#endif //结束第一个#ifndef


