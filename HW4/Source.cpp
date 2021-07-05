#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <string>

using namespace std;
using namespace cv;

Mat g_srcImage, g_dstImage, g_midImage;//原始图、中间图和效果图
vector<Vec4i> g_lines;//定义一个矢量结构g_lines用于存放得到的线段矢量集合
//变量接收的TrackBar位置参数
int g_nthreshold = 100;

static void on_HoughLines(int, void*);//回调函数
static void ShowHelpText();

class WatershedSegmenter {
private:

	Mat markers;

public:

	void setMarkers(const Mat& markerImage) {

		// Convert to image of ints
		markerImage.convertTo(markers, CV_32S);
	}

	Mat process(const Mat& image) {

		// Apply watershed
		watershed(image, markers);

		return markers;
	}

	// Return result in the form of an image
	Mat getSegmentation() {

		Mat tmp;
		// all segment with label higher than 255
		// will be assigned value 255
	markers.convertTo(tmp, CV_8U); 

			return tmp;
	}

	// Return watershed in the form of an image
	Mat getWatersheds() {

		Mat tmps;
		//在設定標記影象，即執行process（）後，maskers邊緣的畫素會被賦值為-1，其他的用正整數表示  
		//下面的這個轉換可以讓邊緣畫素變為-1*255+255=0，即黑色，其餘的溢位，賦值為255，即白色。
		markers.convertTo(tmps, CV_8U, 255, 255);
			return tmps;
	}
};
int main() {
	char strModule[256];
	GetModuleFileName(NULL, strModule, 256); //得到當前模塊路徑

	string a;
	a.assign(strModule);

	a.append("//..//");   //設置為當前工作路徑為當時的上一級
	//a=a+"..//";
	SetCurrentDirectory(a.c_str()); //設置
	GetCurrentDirectory(1000, strModule);

	Mat src = imread("D:\\opencv_3.4.7\\hw\\4.jpg");
	Mat out;

	
	imshow("【原始圖】", src);
	Rect ccomp;
	floodFill(src, Point(40, 20), Scalar(155, 255, 255), &ccomp, Scalar(10, 10, 10), Scalar(10,10, 10));
	imshow("【效果圖】", src);
	
		
	src = imread("D:\\opencv_3.4.7\\hw\\4.jpg");
	
	Mat binary;
	cvtColor(src, binary, CV_BGR2GRAY);
	threshold(binary, binary, 80, 255, CV_THRESH_BINARY);  //閾值化操作
	imshow("Binary Image", binary);
	// Eliminate noise and smaller objects  
	Mat fg, bg;
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	erode(binary, fg, element);
	
	dilate(binary, bg, element);
	threshold(bg, bg, 1, 128, THRESH_BINARY_INV);
	
	// Show markers image  
	Mat marker(binary.size(), CV_8U, Scalar(0));
	marker = fg + bg;
	

	// Create watershed segmentation object  
	WatershedSegmenter segmenter;
	// Set marker and process  
	segmenter.setMarkers(marker);
	segmenter.process(src);
	// Display segmentation result  
	//imshow("Segmentation", segmenter.getSegmentation());
	// Display watersheds  
	imshow("Watersheds", segmenter.getWatersheds());


	

		Mat g_srcImage = imread("D:\\opencv_3.4.7\\hw\\4.jpg");  //工程目录下应该有一张名为1.jpg的素材图
		Mat midImage, dstImage;//临时变量和目标图的定义

		//【2】进行边缘检测和转化为灰度图
		/*Canny(srcImage, midImage, 10, 200, 3);//进行一此canny边缘检测
		cvtColor(midImage, dstImage, CV_GRAY2BGR);//转化边缘检测后的图为灰度图
		*/
		
		Canny(g_srcImage, g_midImage, 50, 200, 3);//进行一次canny边缘检测
		cvtColor(g_midImage, g_dstImage, CV_GRAY2BGR);//转化边缘检测后的图为灰度图

		//调用一次回调函数，调用一次HoughLinesP函数
		on_HoughLines(g_nthreshold, 0);
		HoughLinesP(g_midImage, g_lines, 1, CV_PI / 180, 5, 50, 10);

		//显示效果图  
		imshow("【效果图】", g_dstImage);

		
		

	

	
	waitKey(0);
	return 0;
}

static void on_HoughLines(int, void*)
{
	//定义局部变量储存全局变量
	Mat dstImage = g_dstImage.clone();
	Mat midImage = g_midImage.clone();

	//调用HoughLinesP函数
	vector<Vec4i> mylines;
	HoughLinesP(midImage, mylines, 1, CV_PI / 180, g_nthreshold + 1, 50, 10);

	//循环遍历绘制每一条线段
	for (size_t i = 0; i < mylines.size(); i++)
	{
		Vec4i l = mylines[i];
		line(dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186, 88, 255), 1, CV_AA);
	//	line(dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(223, 180, 55), 1, CV_AA);
	}
	//显示图像
	imshow("【xxx】", dstImage);
}

//-----------------------------------【ShowHelpText( )函数】----------------------------------
//		描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//输出一些帮助信息
	printf("\n\n\n\t请调整滚动条观察图像效果~\n\n");
	printf("\n\n\t\t\t\t\t\t\t\t by浅墨"
	);
}

