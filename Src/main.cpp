#include "ImageProcessTest.h"
#include<time.h>
#include<iostream>
#include<fstream>
using namespace std;


int main()
{
	//AccessMat_CV_8UC1_1();
	//AccessMat_CV_8UC1_2();
	//AccessMat_ByCol();
	ImageProcessTest imageProcessTest;
	imageProcessTest.TestRotate();
	/*LBP lbp;
	lbp.Test();*/

	/*Mat src(300, 300, CV_8UC1);
	Scalar value(0, 128, 255);
	src.setTo(value);

	imshow("src", src);
	waitKey(0);*/
	return 0;
}