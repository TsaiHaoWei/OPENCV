#include < stdio.h >
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

using namespace cv;

int main()
{
	
	char strModule[256];
	GetModuleFileName(NULL, strModule, 256); //得到當前模塊路徑
	
	string a;
	a.assign(strModule);
	
	a.append("//..//");   //設置為當前工作路徑為當時的上一級
	//a=a+"..//";
	SetCurrentDirectory(a.c_str()); //設置
	GetCurrentDirectory(1000, strModule);

	Mat src = imread("4.jpg");

	Mat out;

	namedWindow("原圖");
	imshow("原圖", src);
	Mat gray_image;
	Mat g_src;//用高斯滤波降噪
	GaussianBlur(src, g_src, Size(3, 3), 0, 0);
	cvtColor(g_src, gray_image, COLOR_RGB2GRAY);//灰度图像
	

	Mat binary;
	threshold(gray_image, binary, 100, 255, CV_THRESH_BINARY);

	threshold(gray_image, out, 150, 255, CV_THRESH_TRUNC);

	imshow("Original image", src);
	imshow("gray image", gray_image);
	imshow("Binary", binary);
	//imshow("truncate", truncate);
	   	
	waitKey();
	return 0;
}

