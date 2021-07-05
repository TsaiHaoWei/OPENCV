// ---------------------------------- - 【头文件包含部分】-------------------------------------- -
//     描述：包含程序所依赖的头文件
//----------------------------------------------------------------------------------------------
#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>

//-----------------------------------【命名空间声明部分】---------------------------------------
//     描述：包含程序所使用的命名空间
//----------------------------------------------------------------------------------------------- 
using namespace std;
using namespace cv;

int main()
{

	
	Mat image = imread("C:\\Users\\ASUS\\Desktop\\108-1\\影像辨識\\圖.jpg");
	Mat g_src;//用高斯滤波降噪
	GaussianBlur(image, g_src, Size(3, 3), 0, 0);
	
	imshow("ORIGINAL", image);
	//imshow("Gaussian", g_src);

	//灰階
	Mat gray_image;
	cvtColor(g_src, gray_image, COLOR_RGB2GRAY);//灰度图像


	Mat threshold_otsu;
	
	Mat canny_image;
	Canny(gray_image, canny_image, 50, 100, 3);
	threshold(gray_image, threshold_otsu, 220, 255, CV_THRESH_BINARY);
	imshow("灰階", gray_image);
	imshow("二值化", threshold_otsu);
	imshow("Canny", canny_image);
	Mat canny_dilate;
	Mat element = getStructuringElement(MORPH_RECT, Size(6, 6));
	 dilate(canny_image, canny_dilate, element);
	imshow("膨脹", canny_dilate);
	Mat out;
	

	morphologyEx(canny_image, out, MORPH_CLOSE, element);
	imshow("close", out);
	
	
			
	waitKey();
	return 0;
}
/*
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <opencv2\imgcodecs.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

using namespace cv;


void thinningIte(Mat& img, int pattern) {

	Mat del_marker = Mat::ones(img.size(), CV_8UC1);
	int x, y;

	for (y = 1; y < img.rows - 1; ++y) {

		for (x = 1; x < img.cols - 1; ++x) {

			int v9, v2, v3;
			int v8, v1, v4;
			int v7, v6, v5;

			v1 = img.data[y * img.step + x * img.elemSize()];
			v2 = img.data[(y - 1) * img.step + x * img.elemSize()];
			v3 = img.data[(y - 1) * img.step + (x + 1) * img.elemSize()];
			v4 = img.data[y * img.step + (x + 1) * img.elemSize()];
			v5 = img.data[(y + 1) * img.step + (x + 1) * img.elemSize()];
			v6 = img.data[(y + 1) * img.step + x * img.elemSize()];
			v7 = img.data[(y + 1) * img.step + (x - 1) * img.elemSize()];
			v8 = img.data[y * img.step + (x - 1) * img.elemSize()];
			v9 = img.data[(y - 1) * img.step + (x - 1) * img.elemSize()];

			int S = (v2 == 0 && v3 == 1) + (v3 == 0 && v4 == 1) +
				(v4 == 0 && v5 == 1) + (v5 == 0 && v6 == 1) +
				(v6 == 0 && v7 == 1) + (v7 == 0 && v8 == 1) +
				(v8 == 0 && v9 == 1) + (v9 == 0 && v2 == 1);

			int N = v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9;

			int m1 = 0, m2 = 0;

			if (pattern == 0) m1 = (v2 * v4 * v6);
			if (pattern == 1) m1 = (v2 * v4 * v8);

			if (pattern == 0) m2 = (v4 * v6 * v8);
			if (pattern == 1) m2 = (v2 * v6 * v8);

			if (S == 1 && (N >= 2 && N <= 6) && m1 == 0 && m2 == 0)
				del_marker.data[y * del_marker.step + x * del_marker.elemSize()] = 0;
		}
	}

	img &= del_marker;
}

void thinning(const Mat& src, Mat& dst) {
	dst = src.clone();
	dst /= 255;         // 0は0 , 1以上は1に変換される

	Mat prev = Mat::zeros(dst.size(), CV_8UC1);
	Mat diff;

	do {
		thinningIte(dst, 0);
		thinningIte(dst, 1);
		absdiff(dst, prev, diff);
		dst.copyTo(prev);
	} while (countNonZero(diff) > 0);

	dst *= 255;
}

int main() {
	Mat img = imread("C:\\Users\\ASUS\\Desktop\\108-1\\影像辨識\\圖1.jpg");
	imshow("src", img);
	Mat img2;
	cvtColor(img, img2, CV_BGR2GRAY);

	threshold(img2, img2, 50, 255, CV_THRESH_BINARY);
	imshow("二值化", img2);
	thinning(img2, img2);

	
	imshow("dst", img2);
	waitKey();
	return 0;
}



*/