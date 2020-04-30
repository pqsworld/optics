#include <stdio.h>
#include <iostream>
#include <io.h>
#include <stdlib.h>
#include "calcprocess.h"
#include <afx.h>
#include "imageOperation.h"
#include <string>
#include <fstream>


using namespace std;
//-------------------------------------------声明------------------------------------------------
int MainCalc();
int DfsProcess(string folderPath,unsigned char * imageBuf);
int findBmp(string fileName, int bmpCount, string folderPath);

//-------------------------------------------全局变量--------------------------------------------
unsigned int imageCount = 0;
//-------------------------------------------主函数----------------------------------------------
int main()
{
	MainCalc();
	system("pause");
	return 0;
}

int MainCalc()
{
	unsigned char * imageBuf = (unsigned char *)malloc(sizeof(unsigned char)* MAX_HEIGHT * MAX_WIDTH);
	char *folderPath = ".\\silead-fpdata-caldata-0402";
	
	DfsProcess(folderPath,imageBuf);
	printf("Shading calculated, and saved to .txt in current directory.\n");
	return 0;
}

int DfsProcess(string folderPath,unsigned char * dfsBuf)
{
	//finddata结构体
	struct _finddata_t fileData;
	CString bmpname;
	string strFind = folderPath + "\\*.*";

	//all files
	long handle = _findfirst(strFind.c_str(), &fileData);
	long bmpHandle;
	int intHandle = 0;
	unsigned char * imageBuf = NULL;
	
	if (handle == -1L)
	{
		printf("Folder %s Path DOESNOT match!", fileData.name);
		return -1;
	}
	//DFS
	do
	{
		if (fileData.attrib & _A_SUBDIR)//if has subdir 
		{
			if ((strcmp(fileData.name, ".") != 0) && strcmp(fileData.name, "..") != 0) //if not . ..
			{
				string dfsPath = folderPath + "\\" + fileData.name;
				DfsProcess(dfsPath, dfsBuf);
			}
		}
		else//no subdir
		{
			
			ofstream shadingFile;
			string findSize = folderPath + "\\*.bmp";
			
			bmpHandle = _findfirst(findSize.c_str(), &fileData);
			if (bmpHandle == -1L)
			{
				return  -1;
			}
			do//calc process on all .bmps
			{
				char buffer[300];
				int height = 0;
				int width = 0;
				imageCount++;
				shadingFile.open(folderPath + "\\shading.txt",ios::app);

				imageBuf = (unsigned char *)malloc(sizeof(unsigned char)* MAX_WIDTH * MAX_HEIGHT);
				memset(imageBuf, 0, MAX_HEIGHT * MAX_WIDTH);
				if (imageBuf == NULL)
				{
				printf("--- MALLOC_ERROR! --- \nFile : %s,\nFunction :  %s,\nLine[%d],\nTimestemp :  %s\n", __FILE__, __FUNCTION__, __LINE__, __TIMESTAMP__);
				return MALLOC_ERROR;
				}

				int ret = GFPLoadImage((const char*)(folderPath + "\\" + (string)fileData.name).c_str(), imageBuf, &height, &width);
				if (ret == -1)
				{
				printf("--- FILE_ERROR! --- \nFile : %s,\nFunction :  %s,\nLine[%d],\nTimestemp :  %s\n", __FILE__, __FUNCTION__, __LINE__, __TIMESTAMP__);
				return -1;
				}
				ctime_s(buffer, _countof(buffer), &fileData.time_write);
				printf("FilePath :  %-12s\nFileName :  %-12s\nFileInfo :  %.24s %9ldB\n\n\n", folderPath.c_str(),fileData.name, buffer, fileData.size);

				int shading = GetShading(imageBuf, height, width);
				printf("Shading : %d%%				ImageCount : %d\n", shading,imageCount);
				shadingFile << folderPath << "\n" << fileData.name << "\nShading : " << shading << "%" << "		ImageCount : " << imageCount << "\n\n";
				free(imageBuf);
				shadingFile.close();

			} while (_findnext(bmpHandle, &fileData) == 0);
			return 0;
		}
	}while(_findnext(handle, &fileData) == 0);
	_findclose(handle);

	return 0;
}