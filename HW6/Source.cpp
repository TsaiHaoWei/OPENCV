#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
# include <opencv2/opencv.hpp>  
#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;
using namespace cv;
/*函数功能：求两条直线交点*/
/*输入：两条Vec4i类型直线*/
/*返回：Point2f类型的点*/
Mat photo(Mat dist, int i)
{
	Mat dist_8u;
	dist.convertTo(dist_8u, CV_8U);
	// Find total markers
	vector<vector<Point> > contours;
	findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	// Create the marker image for the watershed algorithm
	Mat markers = Mat::zeros(dist.size(), CV_32S);
	// Draw the foreground markers
	for (size_t i = 0; i < contours.size(); i++)
	{
		drawContours(markers, contours, static_cast<int>(i), Scalar(static_cast<int>(i) + 1), -1);
	}
	// Draw the background marker
	circle(markers, Point(5, 5), 3, Scalar(255), -1);
	//imshow("test Markers"+i, markers * 10000);
	// Perform the watershed algorithm
	//watershed(imgResult, markers);
	Mat mark;
	markers.convertTo(mark, CV_8U);
	bitwise_not(mark, mark);
	// imshow("Markers_v2", mark); // uncomment this if you want to see how the mark
	 // image looks like at that point
	 // Generate random colors
	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int b = theRNG().uniform(0, 256);
		int g = theRNG().uniform(0, 256);
		int r = theRNG().uniform(0, 256);
		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}
	// Create the result image
	Mat dst = Mat::zeros(markers.size(), CV_8UC3);
	// Fill labeled objects with random colors
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()))
			{
				dst.at<Vec3b>(i, j) = colors[index - 1];
			}
		}
	}
	// Visualize the final image
	//imshow("Final Result"+i, dst);
	return dst;
}

int main(int argc, char* argv[])
{
	// Load the image
	char strModule[256];
	GetModuleFileName(NULL, strModule, 256); //得到當前模塊路徑

	string a;
	a.assign(strModule);

	a.append("//..//");   //設置為當前工作路徑為當時的上一級
	//a=a+"..//";
	SetCurrentDirectory(a.c_str()); //設置
	GetCurrentDirectory(1000, strModule);

	Mat src = imread("圖.jpg");
	if (src.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		cout << "Usage: " << argv[0] << " <Input image>" << endl;
		return -1;
	}	imshow("原圖", src);
	///灰階圖
	Mat grayImg;
	cvtColor(src, grayImg, CV_BGR2GRAY);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (src.at<Vec3b>(i, j)[0] >= 200)
			{
				src.at<Vec3b>(i, j)[0] = 0;
				src.at<Vec3b>(i, j)[1] = 0;
				src.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}imshow("背景黑色", src);


	Mat canny_image;	Canny(src, canny_image, 150, 200, 3);
	imshow("Canny 圖", canny_image);
	///Laplace/////////////////////////////////////////
	Mat kernel = (Mat_<float>(3, 3) <<
		1, 1, 1,
		1, -8, 1,
		1, 1, 1);
	Mat imgLaplacian;
	filter2D(src, imgLaplacian, CV_32F, kernel);
	imshow("imgLaplacian 圖 ", imgLaplacian);
	Mat sharp;
	src.convertTo(sharp, CV_32F);
	Mat imgResult = sharp - imgLaplacian;

	imgResult.convertTo(imgResult, CV_8UC3);
	imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
	imshow("套上原圖", imgResult);

	///2值化/////////////////////////////
	Mat bw;
	Mat be_erode;
	cvtColor(imgResult, bw, COLOR_BGR2GRAY);
	threshold(bw, bw, 10, 255, THRESH_BINARY);
	Mat element = getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
	dilate(bw, be_erode, element);
	erode(be_erode, be_erode, element);
	//threshold(bw, bw, 40, 255, THRESH_BINARY | THRESH_OTSU);
	//imshow("2值化", bw);
	imshow("OPEN_2值化", be_erode);

	/*霍夫直线检测*/
	/*vector<Vec4i> Lines;
	HoughLinesP(be_erode, Lines, 1, CV_PI / 360, 200, 100, 10);
	Vec4i LineStand = Lines[0];
	Vec4i LineAnother;
	double ka = (double)(LineStand[1] - LineStand[3]) / (double)(LineStand[0] - LineStand[2]);
	double kb;
	for (int i = 1; i < Lines.size(); i++)
	{
		double ki = (double)(Lines[i][1] - Lines[i][3]) / (double)(Lines[i][0] - Lines[i][2]);
		if (ki * ka < 0)
		{
			LineAnother = Lines[i];
			kb = ki;
		}
	}
	/*画出两条直线*/
	/*Mat result;
	result = src.clone();
	line(result, Point(LineStand[0], LineStand[1]), Point(LineStand[2], LineStand[3]), Scalar(0, 255, 0), 2, 8);
	line(result, Point(LineAnother[0], LineAnother[1]), Point(LineAnother[2], LineAnother[3]), Scalar(0, 0, 255), 2, 8);
	cout << "直线A过点(" << LineStand[0] << "," << LineStand[1] << ")以及点(" << LineStand[2] << "，" << LineStand[3] << ")；斜率为：" << ka << endl;
	cout << "直线B过点(" << LineAnother[0] << "，" << LineAnother[1] << ")以及点(" << LineAnother[2] << "，" << LineAnother[3] << ")；斜率为：" << kb << endl;

	imshow("result", result);*/

	/////distanceTransform///////////////////////////////
// Perform the distance transform algorithm
	Mat dist;
	distanceTransform(be_erode, dist, DIST_L2, 3);

	normalize(dist, dist, 0, 1.0, NORM_MINMAX);
	imshow("Distance Transform Image", dist);
	/////抓節////////////////////////////////////////
	Mat element1 = getStructuringElement(cv::MORPH_CROSS, cv::Size(10, 10));
	Mat eroded, temp;
	Mat canny_2;
	Canny(be_erode, canny_2, 50, 100, 3);
	erode(be_erode, eroded, element1);
	dilate(eroded, temp, element1);
	//imshow("New1", temp);
	Mat stem = be_erode - temp;
	stem = stem - canny_2;
	//imshow("New2", stem);
	Mat element3 = getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 10));
	erode(stem, stem, element3);
	dilate(stem, stem, element3);
	dilate(stem, stem, element3);
	erode(stem, stem, element3);
	//	imshow("New3", stem);
	Mat fin1 = photo(temp, 1);
	imshow("判斷幾個節", fin1);
	int stem_count = 0;
	int stem_rember[] = { 0 };
	for (int i = 0; i < fin1.rows; i++) {
		for (int j = 0; j < fin1.cols; j++) {
			if (fin1.at<Vec3b>(i, j)[0] != 0)
			{
				int test = fin1.at<Vec3b>(i, j)[0];
				//初始
				if (stem_rember[0] == 0)
					stem_rember[0] = test;
				int repeat = 0;
				for (int x = 0; x < stem_count; x++)
				{
					if (stem_rember[x] == test)
					{
						repeat++;
					}
				}
				if (repeat == 0)
				{
					stem_rember[stem_count] = test;
					stem_count++;
				}
			}
		}
	}
	cout << "這張值物照片有" << stem_count - 1 << "個分節點\n";

	Mat fin2 = photo(stem, 2);
	imshow("判斷莖粗度", fin2);
	int stem_weight1 = 0, stem_weight2 = 0;
	int stem_weight[] = { 0 };
	int stem_weightx[] = { 0 };
	int stem_weight_count = 0;
	for (int i = 0; i < fin2.rows; i++) {
		stem_weight_count = 0;
		for (int j = 0; j < fin2.cols; j++) {
			if (fin2.at<Vec3b>(i, j)[0] != 0)
			{
				//cout << i <<"__有" << j<< "點\n";
				stem_weight[stem_weight_count] = j;
				stem_weight_count++;
				if (stem_weight_count > sizeof(stem_weightx))
				{
					for (int g = 0; g < sizeof(stem_weight); g++)
					{
						stem_weightx[g] = stem_weight[g];
					}
				}
				/*cout << "植物莖的粗度為" << j << "pixel\n";
				if (stem_weight_count == 0)
				{
					stem_weight1 = j;
					stem_weight_count++;
				}
				else
				{
					stem_weight2 = j;
				}*/
			}
		}
	}
	for (int g = 0; g < sizeof(stem_weightx); g++)
	{
		//cout << "植物莖的粗度為" << stem_weightx[g] << "pixel\n";
		if (g == 0)
			stem_weight1 = stem_weightx[g];
		else
			stem_weight2 = stem_weightx[g];
	}
	cout << "植物莖的粗度為" << stem_weight2 - stem_weight1 + 1 << "pixel\n";

	//骨架//////////////////////////////////////////////////////
	/*
	Mat skel(bw.size(), CV_8UC1, cv::Scalar(1));
	Mat temp;
	Mat eroded;
	//Mat element = getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));

	bool done;
	do
	{
		erode(be_erode, eroded, element);
		dilate(eroded, temp, element);
		subtract(be_erode, temp, temp);
		bitwise_or(skel, temp, skel);
		eroded.copyTo(be_erode);

		done = (countNonZero(be_erode) == 0);
	} while (!done);

	imshow("Skeleton", skel);*/





	///2值化膨脹

	threshold(dist, dist, 0.55, 1.0, THRESH_BINARY);
	// Dilate a bit the dist image
	Mat kernel1 = Mat::ones(7, 7, CV_8U);
	dilate(dist, dist, kernel1);
	imshow("Peaks", dist);


	Mat dist_8u;
	dist.convertTo(dist_8u, CV_8U);
	// Find total markers
	vector<vector<Point> > contours;
	findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	// Create the marker image for the watershed algorithm
	Mat markers = Mat::zeros(dist.size(), CV_32S);
	// Draw the foreground markers
	for (size_t i = 0; i < contours.size(); i++)
	{
		drawContours(markers, contours, static_cast<int>(i), Scalar(static_cast<int>(i) + 1), -1);
	}
	// Draw the background marker
	circle(markers, Point(5, 5), 3, Scalar(255), -1);
	imshow("Markers", markers * 10000);
	// Perform the watershed algorithm
	//watershed(imgResult, markers);
	Mat mark;
	markers.convertTo(mark, CV_8U);
	bitwise_not(mark, mark);
	// imshow("Markers_v2", mark); // uncomment this if you want to see how the mark
	 // image looks like at that point
	 // Generate random colors
	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int b = theRNG().uniform(0, 256);
		int g = theRNG().uniform(0, 256);
		int r = theRNG().uniform(0, 256);
		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}
	// Create the result image
	Mat dst = Mat::zeros(markers.size(), CV_8UC3);
	// Fill labeled objects with random colors
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()))
			{
				dst.at<Vec3b>(i, j) = colors[index - 1];
			}
		}
	}
	// Visualize the final image
	imshow("Final Result", dst);
	//尋找幾個葉片

	int red_count = 1;
	int red_rember[] = { 0 };
	for (int i = 0; i < dst.rows; i++) {
		for (int j = 0; j < dst.cols; j++) {
			if (dst.at<Vec3b>(i, j)[0] != 0 & dst.at<Vec3b>(i, j)[1] != 0 & dst.at<Vec3b>(i, j)[2] != 0)
			{
				int test = dst.at<Vec3b>(i, j)[0];
				//初始
				if (red_rember[0] == 0)
					red_rember[0] = test;
				int repeat = 0;
				for (int x = 0; x < red_count; x++)
				{
					if (red_rember[x] == test)
					{
						repeat++;
					}
				}
				if (repeat == 0)
				{
					red_rember[red_count] = test;
					red_count++;
				}
			}
		}
	}
	cout << "葉片有" << red_count << "片\n";
	//找大小

	int height1 = 0, height2 = 0;
	int sch = 0;
	//找高
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (src.at<Vec3b>(i, j)[0] != 0)
			{
				if (sch == 0)
				{
					height1 = i;
					sch++;
				}
				else {
					if (height2 < i)
					{
						height2 = i;
						//cout << height2;
					}
				}

			}
		}
	}cout << "高度度為" << height2 - height1 << "pixel\n";

	//找寬
	int weith1 = 0, weigh2 = 0;
	sch = 0;
	for (int j = 0; j < src.cols; j++) {
		for (int i = 0; i < src.rows; i++) {
			if (src.at<Vec3b>(i, j)[0] != 0)
			{
				if (sch == 0)
				{
					weith1 = j;
					sch++;
				}
				else {
					if (weigh2 < j)
					{
						weigh2 = j;
						//cout << weigh2;
					}
				}

			}

		}
	}
	cout << "寬度為" << weigh2 - weith1 << "pixel\n";
	Mat image_src;					//原图	
	Mat imageROI;						//ROI区域
	Mat TempImg;						//裁取出的区域存储为Mat
	int x_begin = weith1, y_begin = height1, width = weigh2 - weith1, height = height2 - height1;		//裁取区域的坐标及大小

	image_src = src.clone();
	imageROI = image_src(Rect(x_begin, y_begin, width, height));
	imageROI.convertTo(TempImg, TempImg.type());
	imshow("剪過後", TempImg);
	waitKey();
	return 0;
}




