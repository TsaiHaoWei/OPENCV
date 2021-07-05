#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

int main(int argc, char** argv) {

	//create a gui window:
	namedWindow("Output", 1);

	//initialize a 120X350 matrix of black pixels:
	Mat output = Mat::zeros(120, 550, CV_8UC3);

	//write text on the matrix:
	putText(output,
		"Hello Tsai_Hao_Wei",
		Point(15, 70),
		FONT_HERSHEY_PLAIN,
		3,
		Scalar(0, 255, 0),
		4);


	imshow("Tsai Hao Wei", output);


	waitKey();

	return 0;

}