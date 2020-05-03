#include <stdio.h>
int	GetFov(unsigned char * imageBuf, int height, int width);
/*
实现图像FOV直径的计算
问题描述
在工厂装配模组时，可能由于装配的偏差导致镜头偏心或者模组与屏幕距离超出误差范围。这些误差会在校准图像上有所反映。如下：亮校准图像上有四个暗角，他们是镜头成像的极限位置，请根据这些暗角的边缘计算成像面积的直径大小，并得到成像的光心位置。


输入描述
Test数据中所有校准得到的亮基准图；
数据路径：“\\192.168.1.45\file - input\常用软件 + 培训资料\【培训文档】\光学” silead - fpdata - caldata - 0402压缩包
输出描述
.txt 输出n张校准图像对应的成像直径和光心坐标；
*/

int GetFov(unsigned char * imageBuf, int height, int width)
{



	
	printf("height = %d", height);
	return 0;
}
