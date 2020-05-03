#include"ImageProcessTest.h"
#include<time.h>
#include<string>
#include"All.h"
#include"CommonUtility.h"
#include"Filesystem.h"
#include"SimpleLog.h"

using namespace std;
using namespace QQ;

// ͼ��·��
#define LENA_GRAY		"../../../../Resource/Image/Gray/Lena512.bmp"
#define LENA_COLOR		"../../../../Resource/Image/Color/Lena800.bmp"
#define BEAUTY_COLOR	"../../../../Resource/Image/Color/Beauty.bmp"
#define BEAUTY_GRAY		"../../../../Resource/Image/Gray/Beauty.bmp"
#define CAMERA_GRAY		"../../../../Resource/Image/Gray/Camera256.bmp"
#define CAMERA_COLOR	"../../../../Resource/Image/Color/Camera256.bmp"

#define COLOR	"./"
#define GRAY	"./"
#define RESULT	"./"

void ImageProcessTest::TestConnectImage()
{
	Mat mat1=imread(string(COLOR)+"White51.bmp",-1);
	Mat mat2=imread(string(COLOR)+"Yellow51.bmp",-1);
	Mat mat3;
	clock_t t1=clock();
	ConnectTwoImage_LeftRight(mat1,mat2,mat3);
	clock_t t2=clock();
	printf("%d\n",t2-t1);
	imwrite("D:/1.bmp",mat3);
	namedWindow("Connect",1);
	imshow("Connect",mat3);
	waitKey(0);
}

void ImageProcessTest::TestProject()
{
	//����ˮƽͶӰ
	//Mat image_Src=imread("D:/Image/Projects/WuHu/Src/2.jpg",-1);
	
	//�ҶȻ�
	Mat image_Gray;
	//cvtColor(image_Src,image_Gray,COLOR_BGR2GRAY);
	//ProjectX(image_Gray,100);

	//���Դ�ֱͶӰ
	Mat image_Src=imread("D:/Image/Projects/WuHu/Src/Test01.jpg",0);
	ProjectY(image_Src,80);
	
}

//����Mat��IplImage �Ƿ���4�ֽڶ���
//����ͼƬ,9*7��ͨ���Ҷ�ͼ
void ImageProcessTest::TestMat_IplImage4ALigned()//����Mat�Ƿ�4�ֽڶ���
{
	
	// Mat mat=imread(string(GRAY)+"White_Small.bmp",-1);
	// int widthStep_Mat=mat.step[0];//9

	// OpenCV4.0֮��֧��IplImage
	// IplImage *iplImage=cvLoadImage((string(GRAY)+"White_Small.bmp").c_str(),-1);
	// int widthStep_Ipl=iplImage->widthStep;//12

	// int pixelCount=mat.cols*mat.rows;
	
	// //��ӡ��Mat
	// uchar *imageData=mat.data;
	// printf("Mat\n");
	// for (int i=0;i<=pixelCount-1;++i)
	// {
	// 	printf("%d,",*imageData++);//������ӡ������û����������
	// }
	// printf("\n\n");
	
	// //��ӡ��IplImage
	// uchar *imageData_Ipl=(uchar *)iplImage->imageData;
	// printf("IplImage\n");
	// for (int i=0;i<=pixelCount-1;++i)
	// {
	// 	printf("%d,",*imageData_Ipl++);//������ӡ��������������
	// }
	// printf("\n\n");
	
	// ////////////////////////////IplImageתΪMat//////////////////////////////////////////////
	
	// //��4�ֽڶ����IplImageת��ΪMat
	// Mat ipl2Mat_True = cvarrToMat(iplImage, true);
	// int withStep3=ipl2Mat_True.step[0];//9
	// uchar *imageData2=ipl2Mat_True.data;
	// printf("Mat ipl2Mat_True(iplImage,true)\n");
	// for (int i=0;i<=pixelCount-1;++i)
	// {
	// 	printf("%d,",*imageData2++);//������ӡ��������������
	// }
	// printf("\n\n");

	
	// //��4�ֽڶ����IplImageת��ΪMat
	// Mat ipl2Mat_false=cvarrToMat(iplImage, false);
	// int withStep4=ipl2Mat_false.step[0];//12
	// uchar *imageData3=ipl2Mat_false.data;
	// printf("Mat ipl2Mat_false(iplImage,false)\n");
	// for (int i=0;i<=pixelCount-1;++i)
	// {
	// 	printf("%d,",*imageData3++);//������ӡ��������������
	// }

}

//�����㷨ִ��ƽ��ʱ��
void ImageProcessTest::TestMeanTime()
{
	double sum=0;
	int numberOfTest = 10;
	Mat srcImage = imread(BEAUTY_COLOR, -1);
	Mat dstImage;
	for (int i = 1; i <= numberOfTest; ++i)
	{
		double t1 = getTickCount();

		//Rotate_Nearest(srcImage, dstImage, Point(srcImage.cols / 2, srcImage.rows / 2), Size(2500, 2500), 30.0, 2); 
		//Rotate_Bilinear(srcImage, dstImage, Point(srcImage.cols / 2, srcImage.rows / 2), Size(2500, 2500),30.0, 2); 
		// Rotate_Bilinear1(srcImage, dstImage, Point(srcImage.cols / 2, srcImage.rows / 2), Size(2500, 2500), 30.0, 1); 
		//Rotate_Bilinear2(srcImage, dstImage, Point(srcImage.cols / 2, srcImage.rows / 2), Size(2500, 2500), 30.0, 2); 
		//Rotate_Bilinear2(srcImage, dstImage, Point(srcImage.cols / 2, srcImage.rows / 2), Size(2500, 2500), 60.0, 2); 

		double t2 = getTickCount();
		sum += (t2 - t1);

	}
	imwrite("RotatedImage.jpg", dstImage);
	printf("average time:%f", (sum/ numberOfTest)*1000.0 / getTickFrequency());
}

void ImageProcessTest::TestDrawHist()
{
	Mat image=imread(string(LENA_GRAY),-1);

	//��ֱ��ͼ
	Mat histogram;
	int histSize=256;//�Ҷȵȼ�
	calcHist(&image, 
		1,//Mat�ĸ���
		0,//��������ֱ��ͼ��ͨ��������ͨ�����������ſ�
		Mat(),//mask�����ʾ����mask,
		histogram, //ֱ��ͼ
		1, //ֱ��ͼ��ά�����������2��ֱ��ͼ����Ϊ2
		&histSize, //ֱ��ͼ�ĵȼ���(��Ҷȵȼ�),Ҳ����ÿ�е�����
		0//�����ı仯��Χ
		);
	
	//��ֱ��ͼ
	Mat image_Hist(100,256,CV_8UC1);
	DrawHistogram(histogram,image_Hist);
	namedWindow("Hist",1);
	imshow("Hist",image_Hist);

	//ֱ��ͼ���⻯
	Mat image_Equalize;
	equalizeHist(image,image_Equalize);
	//����ֱ��ͼ
	calcHist(&image_Equalize, 
		1,//Mat�ĸ���
		0,//��������ֱ��ͼ��ͨ��������ͨ�����������ſ�
		Mat(),//mask�����ʾ����mask,
		histogram, //ֱ��ͼ
		1, //ֱ��ͼ��ά�����������2��ֱ��ͼ����Ϊ2
		&histSize, //ֱ��ͼ�ĵȼ���(��Ҷȵȼ�),Ҳ����ÿ�е�����
		0//�����ı仯��Χ
		);
	//��ֱ��ͼ
	Mat histogramImage(100,256,CV_8UC1);
	DrawHistogram(histogram,histogramImage);
	namedWindow("EqualizeHist",1);
	imshow("EqualizeHist",histogramImage);

	waitKey(0);

}
void ImageProcessTest::TestImageMedianMinMax()//����ͼ���������ֵ�����ֵ����Сֵ	
{
	Mat image=imread(string(LENA_GRAY),-1);

	//��ֱ��ͼ
	Mat histogram;
	int histSize=256;//�Ҷȵȼ�
	calcHist(&image, 
		1,//Mat�ĸ���
		0,//��������ֱ��ͼ��ͨ��������ͨ�����������ſ�
		Mat(),//mask�����ʾ����mask,
		histogram, //ֱ��ͼ
		1, //ֱ��ͼ��ά�����������2��ֱ��ͼ����Ϊ2
		&histSize, //ֱ��ͼ�ĵȼ���(��Ҷȵȼ�),Ҳ����ÿ�е�����
		0//�����ı仯��Χ
		);
	//����������ֵ
	int medianValue=0;
	//CalcImageMedianGray(histogram,image.cols*image.rows,medianValue);

	//�����������ֵ����Сֵ
	int maxValue,minValue;
	//CalcImageMaxMinGray(histogram,maxValue,minValue);

	//printf("Median:%d\nMin:%d\nMax:%d\n",medianValue,minValue,maxValue);
}

void ImageProcessTest::TestSmooth()//�����Լ����˲���
{
	//���Ծ�ֵ�˲�
	Mat image_Src=imread(string(BEAUTY_GRAY),-1);
	Mat image_Blur;
	clock_t t1;
	clock_t t2;
	int sum=0;
	for (int i=1;i<=16;i++)
	{
		 t1=clock();
		Blur(image_Src,image_Blur,Size(5,3));
		t2=clock();
		sum+=(t2-t1);

	}
	printf("My Blur time:%d ms\n",sum>>4);
	imwrite(string(RESULT)+"meanBlur.bmp",image_Blur);

	Mat image_Blur_OpenCV;
	sum=0;
	for (int i=1;i<=16;i++)
	{
		t1=clock();
		blur(image_Src,image_Blur_OpenCV,Size(5,3));
		t2=clock();
		sum+=(t2-t1);

	}
	printf("OpenCV blur time:%d ms\n",sum>>4);
	imwrite(string(RESULT)+"meanBlur_OpenCV.bmp",image_Blur_OpenCV);

	//������ֵ�˲�
	sum=0;
	for (int i=1;i<=2;i++)
	{
		t1=clock();
		MedianBlur(image_Src,image_Blur,5);
		t2=clock();
		sum+=(t2-t1);

	}
	printf("\n");
	printf("My MedianBlur time:%d ms\n",sum>>1);
	imwrite(string(RESULT)+"medianBlur.bmp",image_Blur);
	//OpenCV
	sum=0;
	for (int i=1;i<=16;i++)
	{
		t1=clock();
		medianBlur(image_Src,image_Blur_OpenCV,5);
		t2=clock();
		sum+=(t2-t1);

	}
	printf("OpenCV medianBlur time:%d ms\n",sum>>4);
	imwrite(string(RESULT)+"medianBlur_OpenCV.bmp",image_Blur_OpenCV);
}

#define NUM_LOOP 100
void ImageProcessTest::TestConvolution()
{
	Mat srcImage = imread(BEAUTY_GRAY, -1);
	// ��
	Mat kernel = (Mat_<float>(3, 3) << -1, -1, -1,
		-1, 9, -1,
		-1, -1, -1.);

	Mat dstImage;
	double time1, time2, sum = 0;
	for (int i = 0; i < NUM_LOOP; ++i)
	{
		time1 = getTickCount();

		// ���Ի�����Core i5-6200U,12G,Release
		//Convolution1(srcImage, kernel, dstImage);// 24
		Convolution12(srcImage, kernel, dstImage);// 21

		//Convolution2(srcImage, kernel, dstImage);// 17
		//Convolution22(srcImage, kernel, dstImage);// 13,�Ż�����㷨�ٶȽϿ�

		time2 = getTickCount();
		sum += (time2 - time1)*1000.0 / getTickFrequency();
	}

	imwrite("Result.jpg", dstImage);
	printf("average time:%f\n", sum / NUM_LOOP);
	
}

void ImageProcessTest::TestEdge()//���Ա�Ե���
{
	Mat image_Src=imread(string(LENA_GRAY),-1);
	Mat image_Robert;
	clock_t t1;
	clock_t t2;
	int sum=0;
	//Robert
	for (int i=1;i<=16;i++)
	{
		t1=clock();
		Robert(image_Src,image_Robert);
		t2=clock();
		sum+=(t2-t1);

	}
	printf("Robert:%d ms\n",sum>>4);
	imwrite(string(RESULT)+"Robert.bmp",image_Robert);

	//Sobel
	//��OpenCV��������Ľ������Ҫ��
	Mat image_Sobel;
	for (int i=1;i<=16;i++)
	{
		t1=clock();
		Sobel(image_Src,image_Sobel);
		t2=clock();
		sum+=(t2-t1);

	}
	printf("Sobel:%d ms\n",sum>>4);
	imwrite(string(RESULT)+"Sobel.bmp",image_Sobel);

	//ShenJun
	Mat image_ShenJun;
	for (int i=1;i<=16;i++)
	{
		t1=clock();
		ShenJun(image_Src,image_ShenJun);
		t2=clock();
		sum+=(t2-t1);

	}
	printf("ShenJun:%d ms\n",sum>>4);
	imwrite(string(RESULT)+"ShenJun.bmp",image_ShenJun);
}
#define NUM 32
#define LOW 50
#define HIGH 150
void ImageProcessTest::TestCanny()
{
	double t1;
	double t2;
	double sum = 0;
	Mat srcImage = imread(string(LENA_GRAY), -1);
	Mat canny1, canny, canny2, canny3;
	for (int i = 1; i <= NUM; i++)
	{
		t1 = clock();
		Canny1(srcImage, canny1, LOW, HIGH, 3, false);
		t2 = clock();
		sum += (t2 - t1);

	}
	printf("Canny1:%f ms\n", sum / NUM);
	sum = 0;

	for (int i = 1; i <= NUM; i++)
	{
		t1 = clock();
		Canny2(srcImage, canny2, LOW, HIGH, 3, false);
		t2 = clock();
		sum += (t2 - t1);

	}
	printf("Canny2:%f ms\n", sum / NUM);
	sum = 0;

	for (int i = 1; i <= NUM; i++)
	{
		t1 = clock();
		Canny3(srcImage, canny3, LOW, HIGH, 3, false);
		t2 = clock();
		sum += (t2 - t1);

	}
	printf("Canny3:%f ms\n", sum / NUM);
	sum = 0;

	for (int i = 1; i <= NUM; i++)
	{
		t1 = clock();
		Canny(srcImage, canny, LOW, HIGH, 3, false);
		t2 = clock();
		sum += (t2 - t1);

	}
	printf("canny:%f ms\n", sum / NUM);
	
	imwrite(string(RESULT) + "Canny.bmp", canny);
	imwrite(string(RESULT) + "Canny1.bmp", canny1);
	imwrite(string(RESULT) + "Canny2.bmp", canny2);
	imwrite(string(RESULT) + "Canny3.bmp", canny3);

}

void ImageProcessTest::TestGeometryTransformation()
{
	//˫���Բ�ֵ
	Mat image_Src=imread(string(BEAUTY_COLOR),-1);
	Mat image_Dst;
	BilinearInterpolation(image_Src,image_Dst,Size(800,400));
	imwrite(string(RESULT)+"BilinearInterpolation.bmp",image_Dst);

	//����ڲ�ֵ(ͼ��������״��Ե)
	NearestInterpolation(image_Src,image_Dst,Size(800,400));
	imwrite(string(RESULT)+"NearestInterpolation.bmp",image_Dst);
	
	// ��ֱ��ת
	VerticalFlip(image_Src, image_Dst);
	imwrite(string(RESULT) + "VerticalFlip.bmp", image_Dst);

	// ����ˮƽ��ת
	Mat dst2;
	HorizontalFlip(image_Src, dst2);
	imwrite(string(RESULT) + "HorizontalFlip.bmp", dst2);
}

void ImageProcessTest::TestEqualiz()
{
	Mat srcImage = imread(BEAUTY_GRAY, -1);
	Mat dstImage;
	EqualizeHistogram(srcImage, dstImage);
	imwrite(string(RESULT) + "Equalized.bmp", dstImage);
}

void ImageProcessTest::TestThin()
{
	Mat srcImage = imread("D:/Image/Gray/thin.bmp", 0);
	Mat dstImage;
	QQ::thin1(srcImage, dstImage, 200); // �����Ĵ�������Ŀ��Ĳ�ͬ���仯
	//thin2(srcImage, dstImage); 
	imwrite(string(RESULT) + "Thin.bmp", dstImage);

}

void ImageProcessTest::TestCvtColor()
{
	Mat srcImage = imread(BEAUTY_COLOR, -1);
	Mat grayImage;
	RGB2Gray(srcImage, grayImage);

	imwrite(string(RESULT) + "Gray.bmp", grayImage);


}

void ImageProcessTest::TestRotate()
{
	Mat srcImage = imread(BEAUTY_COLOR, -1);
	Mat rotatedImage;
	Rotate_Nearest(srcImage, Point(srcImage.cols / 2, srcImage.rows / 2), 90, 1,Size(srcImage.rows, srcImage.cols), rotatedImage);
	//QQ::Rotate(srcImage, rotatedImage, Point(srcImage.cols / 2, srcImage.rows / 2), DEGREE2RADIAN(30), Size(1024, 1024),0.5);
	imshow("Src", srcImage);
	imshow("Rotate", rotatedImage);
	waitKey(0);

}
void ImageProcessTest::TestGaussianBlur()
{
	Mat srcImage = imread(BEAUTY_GRAY, -1);
	Mat dstImage;
	QQ::GaussianBlur(srcImage, dstImage, 0.84089642);
	imwrite(string(RESULT) + "1.bmp", dstImage);

}

void ImageProcessTest::TestGradientHist()
{
	Mat srcImage = imread(LENA_GRAY, -1);
	Mat gradient, gradientAngle;
	
	double time1, time2, sum = 0;
	for (int i = 0; i < NUM_LOOP; ++i)
	{
		time1 = getTickCount();

		// ���Ի�����Core i5-4460,8G,Release,����ͼƬLENA_GRAY
		ComputeGradient2(srcImage, gradient, gradientAngle, 9, false);// 12.860420 ms

		// ����HOG
		//ComputeGradient_HOG(srcImage, gradient, gradientAngle, 9, false);// 6.196427

		time2 = getTickCount();
		sum += (time2 - time1)*1000.0 / getTickFrequency();
	}

	printf("average time:%f\n", sum / NUM_LOOP);

	/*std::ofstream outFile1("gradient.txt", ios::out);
	std::ofstream outFile2("angle.txt", ios::out);
	float *dataOfGradient = (float *)gradient.data;
	float *dataOfAngle = (float *)gradientAngle.data;
	int numberOfPixels = srcImage.cols*srcImage.rows;
	for (int i = 0; i <= numberOfPixels - 1; ++i)
	{
		outFile1 << dataOfGradient[i] << endl;
		outFile2 << dataOfAngle[i] << endl;
	}

	outFile1.close();
	outFile2.close();*/
	//imwrite("Graident.jpg", gradient);
	//imwrite("GraidentAngle.jpg", gradientAngle);


	/*Mat fx, fy;
	cv::Sobel(srcImage, fx, CV_32F, 1, 0, 3);
	cv::Sobel(srcImage, fy, CV_32F, 0, 1, 3);
	Mat magnitude, angle;
	cartToPolar(fx, fy, magnitude, angle, true);
	std::ofstream outFile11("gradient2.txt", ios::out);
	std::ofstream outFile22("angle2.txt", ios::out);
	dataOfGradient = (float *)magnitude.data;
	dataOfAngle = (float *)angle.data;
	for (int i = 0; i <= numberOfPixels - 1; ++i)
	{
		outFile11 << dataOfGradient[i] << endl;
		outFile22 << dataOfAngle[i] << endl;
	}
	outFile11.close();
	outFile22.close();
	imwrite("Graident2.jpg", magnitude);
	imwrite("GraidentAngle2.jpg", angle);*/

}

// LBP�Ĳ���
#define CELLSIZE_LBP  16  // LBP�Ĵ��ڴ�С��4,8��16
#define PATH "D:/Image/Texture/Example_02/"

// LBP�ȼ�ģʽ + SVM
void ImageProcessTest::LBP_Uniform_SVM()
{
	// ����ѵ������·�������
	vector<string> fileNameListOfTrainData;
	vector<int> labelsOfTrainData;
	string line;
	int numberOfLine = 0;
	std::ifstream trainData(string(PATH)+"TrainData.txt", ios::in);
	while (getline(trainData, line))
	{
		if (line.empty())
			continue;

		++numberOfLine;
		if (numberOfLine % 2 == 0)//�����������
		{
			labelsOfTrainData.push_back(atoi(line.c_str()));
		}
		else
		{
			fileNameListOfTrainData.push_back(line);

		}

	}
	trainData.close();

	// ��������LBP�������������������
	LOG_INFO(stdout, "Computing LBP Feature...\n");
	
	// ���ȼ������������ĳ���
	LBP lbp;
	Mat srcImage = imread(fileNameListOfTrainData[0], 0);// ����Ҷ�ͼ
	Mat featureVector;
	lbp.ComputeLBPFeatureVector_Uniform(srcImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVector);
	int lengthOfFeatureVector = featureVector.size[1]; // size:ÿһάԪ�صĸ���
	
	Mat allfeatureVectors(fileNameListOfTrainData.size(), lengthOfFeatureVector, CV_32FC1); // ������������������
	Mat classOfSample(fileNameListOfTrainData.size(), 1, CV_32SC1);
	for (int i = 0; i < fileNameListOfTrainData.size(); ++i)
	{
		// ����ͼƬ
		Mat srcImage = imread(fileNameListOfTrainData[i], -1);

		// ��������LBP��������
		lbp.ComputeLBPFeatureVector_Uniform(srcImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVector);

		for (int j = 0; j <= lengthOfFeatureVector - 1; ++j)
		{
			allfeatureVectors.at<float>(i, j) = featureVector.at<float>(0, j);
		}

		classOfSample.at<int>(i, 0) = labelsOfTrainData[i];

		// log
		double process = (1.0*(i + 1) / fileNameListOfTrainData.size()) * 100;
		LOG_INFO(stdout, "%s  done! %f%\n", GetFileName(fileNameListOfTrainData[i]).c_str(), process);
	}
	LOG_INFO(stdout, "Computing LBP Feature done!\n");

	// ʹ��SVM������ѵ��
	LOG_INFO(stdout, "Traing SVM...\n");
	Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
	svm->setType(cv::ml::SVM::C_SVC);
	svm->setKernel(cv::ml::SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	svm->train(allfeatureVectors, cv::ml::ROW_SAMPLE, classOfSample);
	svm->save(string(PATH)+"Classifier.xml");
	LOG_INFO(stdout, "Traing done!\n");

	// ʹ��ѵ���õķ���������ʶ��
	LOG_INFO(stdout, "Testing...\n");
	vector<string> fileNameListOfTestData;
	ifstream testData(string(PATH) + "TestData.txt", ios::in);// ע��Ҫȥ�����һ�еĻ��У��������һ��ͼƬ���������ǿյ�
	while (getline(testData, line))
	{
		if (line.empty())
			continue;

		fileNameListOfTestData.push_back(line);
	}
	testData.close();
	ofstream resultOfPrediction(string(PATH) + "PredictResult.txt", ios::out);
	for (int i = 0; i < fileNameListOfTestData.size(); ++i)
	{
		Mat testImage = imread(fileNameListOfTestData[i], 0);// ����Ҷ�ͼ

		// ����LBP��������
		Mat featureVectorOfTestImage;
		lbp.ComputeLBPFeatureVector_Uniform(testImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVectorOfTestImage);

		int predict = svm->predict(featureVectorOfTestImage);
		char result[512];
		sprintf(result, "%s %d\n", fileNameListOfTestData[i].c_str(), predict);
		resultOfPrediction << result;

		// log
		double process = (1.0*(i + 1) / fileNameListOfTestData.size()) * 100;
		LOG_INFO(stdout, "%s -> %d %f%\n", GetFileName(fileNameListOfTestData[i]).c_str(), predict, process);
	}
	LOG_INFO(stdout, "Testing done!\n");
	resultOfPrediction.close();
}
// LBP��ת���䣬�ȼ�ģʽ+SVM
void ImageProcessTest::LBP_Rotation_Uniform_SVM()
{
	// ����ѵ������·�������
	vector<string> fileNameListOfTrainData;
	vector<int> labelsOfTrainData;
	string line;
	int numberOfLine = 0;
	std::ifstream trainData(string(PATH) + "TrainData.txt", ios::in);
	while (getline(trainData, line))
	{
		if (line.empty())
			continue;

		++numberOfLine;
		if (numberOfLine % 2 == 0)//�����������
		{
			labelsOfTrainData.push_back(atoi(line.c_str()));
		}
		else
		{
			fileNameListOfTrainData.push_back(line);

		}

	}
	trainData.close();

	// ��������LBP�������������������
	LOG_INFO(stdout, "Computing LBP Feature...\n");

	// ���ȼ������������ĳ���
	LBP lbp;
	Mat srcImage = imread(fileNameListOfTrainData[0], 0);// ����Ҷ�ͼ
	Mat featureVector;
	lbp.ComputeLBPFeatureVector_Rotation_Uniform(srcImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVector);
	int lengthOfFeatureVector = featureVector.size[1]; // size:ÿһάԪ�صĸ���

	Mat allfeatureVectors(fileNameListOfTrainData.size(), lengthOfFeatureVector, CV_32FC1); // ������������������
	Mat classOfSample(fileNameListOfTrainData.size(), 1, CV_32SC1);
	for (int i = 0; i < fileNameListOfTrainData.size(); ++i)
	{
		// ����ͼƬ
		Mat srcImage = imread(fileNameListOfTrainData[i], -1);

		// ��������LBP��������
		lbp.ComputeLBPFeatureVector_Rotation_Uniform(srcImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVector);

		for (int j = 0; j <= lengthOfFeatureVector - 1; ++j)
		{
			allfeatureVectors.at<float>(i, j) = featureVector.at<float>(0, j);
		}

		classOfSample.at<int>(i, 0) = labelsOfTrainData[i];

		// log
		double process = (1.0*(i + 1) / fileNameListOfTrainData.size()) * 100;
		LOG_INFO(stdout, "%s  done! %f%\n", GetFileName(fileNameListOfTrainData[i]).c_str(), process);
	}
	LOG_INFO(stdout, "Computing LBP Feature done!\n");

	// ʹ��SVM������ѵ��
	LOG_INFO(stdout, "Traing SVM...\n");
	Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
	svm->setType(cv::ml::SVM::C_SVC);
	svm->setKernel(cv::ml::SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	svm->train(allfeatureVectors, cv::ml::ROW_SAMPLE, classOfSample);
	svm->save(string(PATH) + "Classifier.xml");
	LOG_INFO(stdout, "Traing done!\n");

	// ʹ��ѵ���õķ���������ʶ��
	LOG_INFO(stdout, "Testing...\n");
	vector<string> fileNameListOfTestData;
	ifstream testData(string(PATH) + "TestData.txt", ios::in);// ע��Ҫȥ�����һ�еĻ��У��������һ��ͼƬ���������ǿյ�
	while (getline(testData, line))
	{
		if (line.empty())
			continue;

		fileNameListOfTestData.push_back(line);
	}
	testData.close();
	ofstream resultOfPrediction(string(PATH) + "PredictResult.txt", ios::out);
	for (int i = 0; i < fileNameListOfTestData.size(); ++i)
	{
		Mat testImage = imread(fileNameListOfTestData[i], 0);// ����Ҷ�ͼ

		// ����LBP��������
		Mat featureVectorOfTestImage;
		lbp.ComputeLBPFeatureVector_Rotation_Uniform(testImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVectorOfTestImage);

		int predict = svm->predict(featureVectorOfTestImage);
		char result[512];
		sprintf(result, "%s %d\n", fileNameListOfTestData[i].c_str(), predict);
		resultOfPrediction << result;

		// log
		double process = (1.0*(i + 1) / fileNameListOfTestData.size()) * 100;
		LOG_INFO(stdout, "%s -> %d %f%\n", GetFileName(fileNameListOfTestData[i]).c_str(), predict, process);
	}
	LOG_INFO(stdout, "Testing done!\n");
	resultOfPrediction.close();
}
void ImageProcessTest::TestLBP()
{
	LBP lbp;
	Mat srcImage = imread(BEAUTY_GRAY, -1);

    // ����LBPͼ��
    Mat lbpImage;
    lbp.ComputeLBPImage_Uniform(srcImage,lbpImage);
    imwrite("LBPImage.jpg",lbpImage);

	// ����LBP��������
//    Mat featureVector;
//	double time1, time2, sum = 0;
//	for (int i = 0; i < NUM_LOOP; ++i)
//	{
//		time1 = getTickCount();

//		// ���Ի�����Core i5-6200U,12G,Release
//		//lbp.ComputeLBPFeatureVector_256_O_2(srcImage, Size(16, 16), featureVector);// 16.706699
//		//lbp.ComputeLBPFeatureVector_256_O(srcImage, Size(16, 16), featureVector);// 17.877541
//		//lbp.ComputeLBPFeatureVector_256(srcImage, Size(16, 16), featureVector);// 17.640340
//		//lbp.ComputeLBPFeatureVector_Uniform(srcImage, Size(16, 16), featureVector); // 18.424251
//		lbp.ComputeLBPFeatureVector_Uniform_O(srcImage, Size(16, 16), featureVector); // 18.333606

//		time2 = getTickCount();
//		sum += (time2 - time1)*1000.0 / getTickFrequency();
//	}
//	printf("average time:%f\n", sum / NUM_LOOP);

//	float *dataOfFeature = (float *)featureVector.data;
//	int dimensionOfFeature = featureVector.rows*featureVector.cols;
//	std::ofstream outFile("D:/Feature.txt", ios::out);

//	outFile << "dimension:" << dimensionOfFeature << endl;
//	for (int i = 0; i <= dimensionOfFeature - 1;++i)
//	{
//		outFile << dataOfFeature[i] << endl;
//	}

//	outFile.close();



}


