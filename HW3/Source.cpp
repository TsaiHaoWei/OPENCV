#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;
#include <stdio.h>


using namespace std;
using namespace cv;

int main() {
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

	
	
	   
	Mat gray_image;
	cvtColor(src, gray_image, CV_BGR2GRAY);
	Mat binary;
	threshold(gray_image, binary, 100, 255, CV_THRESH_BINARY);
	threshold(gray_image, out, 100,255, CV_THRESH_TRUNC);
	//imshow("Original image", src);
	//imshow("gray image", gray_image);
	//imshow("TRUNC", out);
	//imshow("Global binary", binary);

	Mat otsu, adaptive_img, adaptive_img_mean;
	threshold(gray_image, otsu, 0, 255,  CV_THRESH_OTSU);
	imshow("OTSU image", otsu);
	Mat otsu_guess,otsu_guess_otsu;

	GaussianBlur(otsu, otsu_guess, Size(3, 3), 0, 0);
	//threshold(otsu_guess, otsu_guess_otsu, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	threshold(otsu_guess, otsu_guess_otsu, 0, 255, CV_THRESH_OTSU);
	imshow("otsu_guess_otsu", otsu_guess_otsu);

	adaptiveThreshold(gray_image, adaptive_img_mean, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 7, 11);
	adaptiveThreshold(gray_image, adaptive_img, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 7, 11);

	//imshow("Gaussian", adaptive_img);
	//imshow("adaptive_img_mean", adaptive_img_mean);


	
	waitKey(0);
	return 0;
}