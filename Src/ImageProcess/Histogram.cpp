#define DLLAPI_EXPORTS
#include "Histogram.h"

namespace QQ
{
//不支持ROI
void GetHistogram(const Mat &image, int *histogram)
{
	memset(histogram, 0, 256 * sizeof(int));

	//计算直方图
	int pixelCount = image.cols*image.rows;
	uchar *imageData = image.data;
	for (int i = 0; i <= pixelCount - 1; ++i)
	{
		int gray = imageData[i];
		histogram[gray]++;
	}
}
void GetMedianValue(const Mat &image, int &medianValue)
{
	//获取直方图
	int histogram[256];
	GetHistogram(image, histogram);

	int sum=0;
	int pixelCount = image.cols*image.rows;
	for (int i=0;i<=255;++i)
	{
		//
		sum += histogram[i];
		if (2 * sum>pixelCount)//少用除法(sum>pixelCount/2)
		{
			medianValue = i;
			break;
		}
	}

}
void GetMedianValue_AndPixelCountLowerMedian(const Mat &image, int &medianValue, int &pixelCountLowerMedian)
{
	//获取直方图
	int histogram[256];
	GetHistogram(image, histogram);

	pixelCountLowerMedian = 0;
	int pixelCount = image.cols*image.rows;
	for (int i = 0; i <= 255; ++i)
	{
		//
		pixelCountLowerMedian += histogram[i];
		if (2 * pixelCountLowerMedian>pixelCount)//少用除法(sum>pixelCount/2)
		{
			medianValue = i;
			break;
		}
	}

}

//计算图像最大和最小亮度
void GetMaxMinValue(const Mat &image, int &maxValue, int &minValue)//计算亮最大亮度和最小亮度
{
	//获取直方图
	int histogram[256];
	GetHistogram(image, histogram);

	//求最大最小
	for (int i=0;i<=255;++i)
	{
		if (histogram[i] != 0)
		{
			minValue=i;
			break;
		}
	}
	for (int i=255;i>=0;--i)
	{
		if (histogram[i] != 0)
		{
			maxValue=i;
			break;
		}
	}
}



//注意：image_Histogram的宽度要>=256
void DrawHistogram(const Mat &histogram, Mat &image_Historam)
{
	//
	image_Historam.setTo(Scalar(255, 255, 255));

	//获取图像的参数
	int width = image_Historam.cols;
	int height = image_Historam.rows;

	//直方图归一化
	Mat hist_Normalize(histogram);//拷贝构造
	normalize(histogram, hist_Normalize, 0, height, NORM_MINMAX, -1);

	//开始画图
	float *data = (float *)hist_Normalize.data;
	int step = width / 256; //每个灰度级的宽度
	for (int i = 0; i < 255; i++)
	{
		rectangle(image_Historam,
			Point(i*step, height - 1),
			Point((i + 1)*step, height - cvRound(hist_Normalize.at<float>(i))),
			Scalar::all(0), -1, 8, 0);
	}

}







}//namespace QQ
