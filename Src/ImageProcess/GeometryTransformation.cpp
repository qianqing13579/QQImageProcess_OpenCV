#define DLLAPI_EXPORTS

#include "GeometryTransformation.h"

using namespace std;

namespace QQ
{

//2015-3-5 15:33:20,by QQ
//最近邻插值
void NearestInterpolation(const Mat &image_Src,Mat &image_Dst,Size size_Dst)
{
	//重新分配
	image_Dst.create(size_Dst,CV_8UC1);
	
	//
	int height=size_Dst.height;
	int width=size_Dst.width;

	//缩放的倍数
	//1.不要忘了，否则就是整型了
	float ratioX=1.*image_Src.cols/image_Dst.cols;
	float ratioY=1.*image_Src.rows/image_Dst.rows;

	//
	for(int y=0;y<=height-1;++y)
	{
		//对应原图坐标(浮点坐标)
		float y1=y*ratioY;
		
		//取整得到最近邻像素
		int y2=(int)y1;
		
		for (int x=0;x<=width-1;++x)
		{
			//对应的原图坐标
			//最近邻与双线性的区别就是，双线性插值在获得原图的浮点坐标后，利用周围
			//4个点坐标，而最近邻直接把浮点数坐标取整，获得最近邻像素
			float x1=x*ratioX;
			
			//取整得到最近邻像素
			int x2=(int)x1;
			image_Dst.at<uchar>(y,x)=image_Src.at<uchar>(y2,x2);

		}
	}

}


//2015-3-5 13:27:24,by QQ
// 双线性插值
// size_Dst:目标图像的大小
void BilinearInterpolation(const Mat &image_Src,Mat &image_Dst,Size size_Dst)
{
	// 重新分配
	image_Dst.create(size_Dst,CV_8UC1);

	// 原图放大，便于处理边界
	Mat image_Extend;
	copyMakeBorder(image_Src,image_Extend,1,1,1,1,BORDER_DEFAULT);

	int height=size_Dst.height;
	int width=size_Dst.width;

	//缩放的倍数
	//1.不要忘了,否则就是整型了
	float ratioX=1.*image_Src.cols/image_Dst.cols;//最近邻和双线性都是通过缩放的倍数
	float ratioY=1.*image_Src.rows/image_Dst.rows;
	
	//
	for(int y=0;y<=height-1;++y)
	{
		//对应原图的坐标(浮点数坐标)
		float y1=y*ratioY;

		// 在扩充图中的坐标
		y1++;

		//取整,也就是最近邻的像素
		//最近邻与双线性的区别就是，双线性插值在获得原图的浮点坐标后，利用周围
		//4个点坐标，而最近邻直接把浮点数坐标取整，获得最近邻像素
		int y2=(int)y1;

		// 差值
		int dy=(y1-y2)*(1 << SHIFT);
		for (int x=0;x<=width-1;++x)
		{
			//对应的原图坐标
			float x1 = x*ratioX;

			// 在扩充图中的坐标 
			x1++;

			//取整,也就是最近邻的像素
			int x2=(int)x1;

			// 差值
			int dx=(x1-x2)*(1 << SHIFT);

			//周围4个点
			//a就是最近邻像素
			//a   b
			//  p
			//c   d
			uchar a=image_Extend.at<uchar>(y2,x2);
			uchar b=image_Extend.at<uchar>(y2,x2+1);
			uchar c=image_Extend.at<uchar>(y2+1,x2);
			uchar d=image_Extend.at<uchar>(y2+1,x2+1);
			
			// 双线性插值
			// 不要做除法
			//int p = (a*((1 << SHIFT) - dx)*((1 << SHIFT) - dy) + b*dx*((1 << SHIFT) - dy) + c*((1 << SHIFT) - dx)*dy + d*dx*dy)/(1<<(2*SHIFT));
			int p=a*((1 << SHIFT) -dx)*((1 << SHIFT) -dy)+b*dx*((1 << SHIFT) -dy)+c*((1 << SHIFT)-dx)*dy+d*dx*dy;
			p = DESCALE(p, 2 * SHIFT);
			image_Dst.at<uchar>(y,x)=(uchar)p;

		}
	}
}

// 2015-5-16 22:21:28,by QQ
// Updated:2016-5-17 11:38:14,by QQ
void VerticalFlip(const Mat &srcImage, Mat &dstImage)
{
	// 分配内存
	CV_Assert(srcImage.depth() == CV_8U);
	dstImage.create(srcImage.size(), srcImage.type());

	// 垂直翻转
	int width = srcImage.cols;
	int height = srcImage.rows;
	int widthStep = srcImage.cols*srcImage.channels();

	uchar *dataOfSrc = srcImage.data;
	uchar *dataOfDst = dstImage.data + widthStep*(dstImage.rows - 1);
	int limit = (height - 1) / 2;
	for (int y = 0; y <= height - 1; ++y,dataOfSrc+=widthStep,dataOfDst-=widthStep)
	{
		// 拷贝一行
		memcpy(dataOfDst, dataOfSrc, widthStep);
	}
}

// 2016-5-17 11:40:18,by QQ
void HorizontalFlip(const Mat &srcImage, Mat &dstImage)
{
	// 分配内存
	CV_Assert(srcImage.depth() == CV_8U);
	dstImage.create(srcImage.size(), srcImage.type());

	// 每一行建立查找表(对每一个通道建立索引)
	int width = srcImage.cols;
	int channelCount = srcImage.channels();
	int widthStep = width*channelCount;
	int *table = new int[widthStep];
	for (int i = 0; i <= width - 1; ++i)
	{
		int index1 = i*channelCount;
		int index2 = (width - 1 - i)*channelCount;
		for (int j = 0; j <= channelCount - 1; ++j)
			table[index1 + j] = index2 + j;
	}

	
	uchar *dataOfSrc = srcImage.data;
	uchar *dataOfDst = dstImage.data;
	int limit = ((width + 1) >>1)*channelCount; // 注意运算符优先级,为什么((width - 1) >>1)*channelCount 不对？提示:因为多通道的原因，对半使用(x+1)/2
	for (int y = 0; y <= srcImage.rows - 1; ++y, dataOfSrc += widthStep, dataOfDst += widthStep)
	{
		for (int x = 0; x <= limit; ++x)
		{
			int index = table[x];
			dataOfDst[index] = dataOfSrc[x];
			dataOfDst[x] = dataOfSrc[index];
		}
	}

	delete[] table;

}

// 2016-8-11 15:49:14,by QQ
/*	center:原图像的旋转中心
	dstSize:旋转后图像的大小
	theta:旋转角度，单位弧度，顺时针为正
	scale:缩放，scale>1表示放大  <1表示缩小
*/ 
void Rotate_Nearest(const Mat &srcImage, Point center, double theta, double scale, Size dstSize, Mat &dstImage)
{
	CV_Assert(srcImage.depth() == CV_8U);
	dstImage.create(dstSize, srcImage.type());

	// 原图像的旋转中心
	int x0 = center.x;
	int y0 = center.y;
	theta = DEGREE2RADIAN(theta);

	// dx,dy就是dstImage中心点与srcImage的旋转中心的距离，也就是平移的距离
	int dx = dstImage.cols / 2 - x0;
	int dy = dstImage.rows / 2 - y0;
	int numberOfChannels = srcImage.channels();

	int widthOfDst = dstImage.cols;
	int heightOfDst = dstImage.rows;
	
	for (int y = 0; y <= heightOfDst - 1; ++y)
	{
		for (int x = 0; x <= widthOfDst - 1; ++x)
		{
			float srcX = ((x - x0 - dx)*cos(theta) + (y - y0 - dy)*sin(theta))/scale + x0;
			float srcY = ((x - x0 - dx)*(-sin(theta)) + (y - y0 - dy)*cos(theta))/scale + y0;

			// get the nearest coordinate of src
			int x1 = (int)srcX;
			int y1 = (int)srcY;
			if (numberOfChannels == 1)
			{
				if ((x1 >= 0 && x1 <= srcImage.cols - 1) && (y1 >= 0 && y1 <= srcImage.rows - 1))
				{
					dstImage.at<uchar>(y, x) = srcImage.at<uchar>(y1, x1);
				}
				else
				{
					// 越界赋值0
					dstImage.at<uchar>(y, x) = 0;
				}
			}
			else
			{
				if ((x1 >= 0 && x1 <= srcImage.cols - 1) && (y1 >= 0 && y1 <= srcImage.rows - 1))
				{
					dstImage.at<cv::Vec3b>(y, x) = srcImage.at<cv::Vec3b>(y1, x1);
				}
				else
				{
					dstImage.at<cv::Vec3b>(y, x) = cv::Vec3b(0,0,0);
				}

			}
		}
	}

}

void Rotate_Bilinear(const Mat &srcImage, Point center, double theta, double scale, Size dstSize, Mat &dstImage)
{
	CV_Assert(srcImage.depth() == CV_8U);
	dstImage.create(dstSize, srcImage.type());
	dstImage.setTo(Scalar(0, 0, 0));

	// 原图像的旋转中心
	int x0 = center.x;
	int y0 = center.y;
	theta = DEGREE2RADIAN(theta);

	// dx,dy就是dstImage中心点与srcImage的旋转中心的距离，也就是平移的距离
	int dx = dstImage.cols / 2 - x0;
	int dy = dstImage.rows / 2 - y0;
	int numberOfChannels = srcImage.channels();

	int widthOfDst = dstImage.cols;
	int heightOfDst = dstImage.rows;

	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1, BORDER_CONSTANT, Scalar(0, 0, 0)); // 使用0填充边界

	for (int y = 0; y <= heightOfDst - 1; ++y)
	{
		for (int x = 0; x <= widthOfDst - 1; ++x)
		{
			// 按照原来的方式计算原图坐标
			float srcX = ((x - x0 - dx)*cos(theta) + (y - y0 - dy)*sin(theta)) / scale + x0;
			float srcY = ((x0 + dx - x)*sin(theta) + (y - y0 - dy)*cos(theta)) / scale + y0;
			
			// 加1,得到在extendedImage中的坐标
			srcX++; 
			srcY++;
			 
			// get the nearest coordinate of src
			int x1 = (int)(srcX); 
			int y1 = (int)(srcY);

			// 浮点转化为整数
			int dx1 = (srcX - x1)*(1<< SHIFT);
			int dy1 = (srcY - y1)*(1<< SHIFT);

			if (numberOfChannels == 1)
			{
				// !！！注意这里的范围，在extendedImage中，原图的范围就是1~cols - 2了
				if ((x1 >= 1 && x1 <= extendedImage.cols - 2) && (y1 >= 1 && y1 <= extendedImage.rows - 2))
				{	
					//双线性插值
					//周围4个点
					//a就是最近邻像素
					//a   b
					//  p
					//c   d
					uchar a = extendedImage.at<uchar>(y1, x1);
					uchar b = extendedImage.at<uchar>(y1, x1 + 1);
					uchar c = extendedImage.at<uchar>(y1 + 1, x1);
					uchar d = extendedImage.at<uchar>(y1 + 1, x1 + 1);

					// 不要做除法
					//int p = (a*((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b*dx1*((1 << SHIFT) - dy1) + c*((1 << SHIFT) - dx1)*dy1 + d*dx1*dy1)/(1<<(2* SHIFT));
					
					int p = a*((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b*dx1*((1 << SHIFT) - dy1) + c*((1 << SHIFT) - dx1)*dy1 + d*dx1*dy1;
					p = DESCALE(p, 2*SHIFT);

					dstImage.at<uchar>(y, x) = p;
				}
				else
				{
					// 越界赋值0
					dstImage.at<uchar>(y, x) = 0;
				}
			}
			else
			{
				if ((x1 >= 1 && x1 <= extendedImage.cols - 2) && (y1 >= 1 && y1 <= extendedImage.rows - 2))
				{
					//双线性插值
					//周围4个点
					//a就是最近邻像素
					//a   b
					//  p
					//c   d
					Vec3b a = extendedImage.at<Vec3b>(y1, x1);
					Vec3b b = extendedImage.at<Vec3b>(y1, x1 + 1);
					Vec3b c = extendedImage.at<Vec3b>(y1 + 1, x1);
					Vec3b d = extendedImage.at<Vec3b>(y1 + 1, x1 + 1);

					// 不要做除法
					/*int p1 = (a[0] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[0] * dx1*((1 << SHIFT) - dy1) + c[0] * ((1 << SHIFT) - dx1)*dy1 + d[0] * dx1*dy1)/(1<<(2*SHIFT));
					int p2 = (a[1] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[1] * dx1*((1 << SHIFT) - dy1) + c[1] * ((1 << SHIFT) - dx1)*dy1 + d[1] * dx1*dy1)/ (1 << (2 * SHIFT));
					int p3 = (a[2] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[2] * dx1*((1 << SHIFT) - dy1) + c[2] * ((1 << SHIFT) - dx1)*dy1 + d[2] * dx1*dy1)/ (1 << (2 * SHIFT));*/

					int p1 = a[0]*((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[0]*dx1*((1 << SHIFT) - dy1) + c[0]*((1 << SHIFT) - dx1)*dy1 + d[0]*dx1*dy1;
					p1 = DESCALE(p1, 2 * SHIFT);
					int p2 = a[1]*((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[1]*dx1*((1 << SHIFT) - dy1) + c[1]*((1 << SHIFT) - dx1)*dy1 + d[1] *dx1*dy1;
					p2 = DESCALE(p2, 2 * SHIFT);
					int p3 = a[2]*((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[2]*dx1*((1 << SHIFT) - dy1) + c[2]*((1 << SHIFT) - dx1)*dy1 + d[2] *dx1*dy1;
					p3 = DESCALE(p3, 2 * SHIFT);


					dstImage.at<cv::Vec3b>(y, x) = Vec3b(p1,p2,p3);
				}
				else
				{
					dstImage.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
				}

			}
		}
	}
}

// 2016-8-14 16:16:20,by QQ
// 优化：1. 将Rotate_Bilinear循环内的不变量提取出来
void Rotate_Bilinear1(const Mat &srcImage, Point center, double theta, double scale, Size dstSize, Mat &dstImage)
{
	CV_Assert(srcImage.depth() == CV_8U);
	dstImage.create(dstSize, srcImage.type());
	dstImage.setTo(Scalar(0, 0, 0));

	// 原图像的旋转中心
	int x0 = center.x;
	int y0 = center.y;
	theta = DEGREE2RADIAN(theta);

	// dx,dy就是dstImage中心点与srcImage的旋转中心的距离，也就是平移的距离
	int dx = dstImage.cols / 2 - x0;
	int dy = dstImage.rows / 2 - y0;
	int numberOfChannels = srcImage.channels();

	int widthOfDst = dstImage.cols;
	int heightOfDst = dstImage.rows;

	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1, BORDER_CONSTANT, Scalar(0, 0, 0)); // 使用0填充边界

	////////////////////////////////// 优化部分/////////////////////////////
	// 将循环内的不变量提取出来
	double sinTheta = sin(theta);
	double cosTheta = cos(theta);
	scale = 1.0 / scale;
	
	for (int y = 0; y <= heightOfDst - 1; ++y)
	{
		// 将内部循环不变量提取出来
		double temp1 = (y - y0 - dy)*sinTheta;
		double temp2 = (y - y0 - dy)*cosTheta;

		for (int x = 0; x <= widthOfDst - 1; ++x)
		{
			// 按照原来的方式计算原图坐标
			double srcX =  ((x - x0 - dx)*cosTheta + temp1)*scale + x0;
			double srcY = (-(x-x0 - dx)*sinTheta + temp2)*scale + y0;

			// 加1,得到在extendedImage中的坐标
			srcX++;
			srcY++;

			// get the nearest coordinate of src
			int x1 = (int)(srcX);
			int y1 = (int)(srcY);

			// 浮点转化为定点数
			int dx1 = (srcX - x1)*(1 << SHIFT);
			int dy1 = (srcY - y1)*(1 << SHIFT);

			if (numberOfChannels == 1)
			{
				// !！！注意这里的范围，在extendedImage中，原图的范围就是1~cols - 2了
				if ((x1 >= 1 && x1 <= extendedImage.cols - 2) && (y1 >= 1 && y1 <= extendedImage.rows - 2))
				{
					//双线性插值
					//周围4个点
					//a就是最近邻像素
					//a   b
					//  p
					//c   d
					uchar a = extendedImage.at<uchar>(y1, x1);
					uchar b = extendedImage.at<uchar>(y1, x1 + 1);
					uchar c = extendedImage.at<uchar>(y1 + 1, x1);
					uchar d = extendedImage.at<uchar>(y1 + 1, x1 + 1);

					// 不要做除法
					//int p = (a*((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b*dx1*((1 << SHIFT) - dy1) + c*((1 << SHIFT) - dx1)*dy1 + d*dx1*dy1)/(1<<(2* SHIFT));

					int p = a*((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b*dx1*((1 << SHIFT) - dy1) + c*((1 << SHIFT) - dx1)*dy1 + d*dx1*dy1;
					p = DESCALE(p, 2 * SHIFT);

					dstImage.at<uchar>(y, x) = p;
				}
				else
				{
					// 越界赋值0
					dstImage.at<uchar>(y, x) = 0;
				}
			}
			else
			{
				if ((x1 >= 1 && x1 <= extendedImage.cols - 2) && (y1 >= 1 && y1 <= extendedImage.rows - 2))
				{
					//双线性插值
					//周围4个点
					//a就是最近邻像素
					//a   b
					//  p
					//c   d
					Vec3b a = extendedImage.at<Vec3b>(y1, x1);
					Vec3b b = extendedImage.at<Vec3b>(y1, x1 + 1);
					Vec3b c = extendedImage.at<Vec3b>(y1 + 1, x1);
					Vec3b d = extendedImage.at<Vec3b>(y1 + 1, x1 + 1);

					// 不要做除法
					/*int p1 = (a[0] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[0] * dx1*((1 << SHIFT) - dy1) + c[0] * ((1 << SHIFT) - dx1)*dy1 + d[0] * dx1*dy1)/(1<<(2*SHIFT));
					int p2 = (a[1] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[1] * dx1*((1 << SHIFT) - dy1) + c[1] * ((1 << SHIFT) - dx1)*dy1 + d[1] * dx1*dy1)/ (1 << (2 * SHIFT));
					int p3 = (a[2] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[2] * dx1*((1 << SHIFT) - dy1) + c[2] * ((1 << SHIFT) - dx1)*dy1 + d[2] * dx1*dy1)/ (1 << (2 * SHIFT));*/

					int p1 = a[0] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[0] * dx1*((1 << SHIFT) - dy1) + c[0] * ((1 << SHIFT) - dx1)*dy1 + d[0] * dx1*dy1;
					p1 = DESCALE(p1, 2 * SHIFT);
					int p2 = a[1] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[1] * dx1*((1 << SHIFT) - dy1) + c[1] * ((1 << SHIFT) - dx1)*dy1 + d[1] * dx1*dy1;
					p2 = DESCALE(p2, 2 * SHIFT);
					int p3 = a[2] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[2] * dx1*((1 << SHIFT) - dy1) + c[2] * ((1 << SHIFT) - dx1)*dy1 + d[2] * dx1*dy1;
					p3 = DESCALE(p3, 2 * SHIFT);


					dstImage.at<cv::Vec3b>(y, x) = Vec3b(p1, p2, p3);
				}
				else
				{
					dstImage.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
				}

			}
		}
	}
}
// 2016-8-14 16:41:51,by QQ
//  优化：在Rotate_Bilinear1的基础上，进一步提取不变量，改变循环变量自增方式
void Rotate_Bilinear2(const Mat &srcImage, Point center, double theta, double scale, Size dstSize, Mat &dstImage)
{
	CV_Assert(srcImage.depth() == CV_8U);
	dstImage.create(dstSize, srcImage.type());
	dstImage.setTo(Scalar(0, 0, 0));

	// 原图像的旋转中心
	int x0 = center.x;
	int y0 = center.y;
	theta = DEGREE2RADIAN(theta);

	// dx,dy就是dstImage中心点与srcImage的旋转中心的距离，也就是平移的距离
	int dx = dstImage.cols / 2 - x0;
	int dy = dstImage.rows / 2 - y0;
	int numberOfChannels = srcImage.channels();

	int widthOfDst = dstImage.cols;
	int heightOfDst = dstImage.rows;

	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1, BORDER_CONSTANT, Scalar(0, 0, 0)); // 使用0填充边界

	////////////////////////////////// 优化部分/////////////////////////////
	// 将循环内的不变量提取出来
	double sinTheta = sin(theta);
	double cosTheta = cos(theta);
	scale = 1.0 / scale;

	// 改变了循环内部增量的方式
	double temp1= (0 - y0 - dy)*sinTheta;
	double temp2 = (0 - y0 - dy)*cosTheta;
	double dtemp1 = sinTheta;
	double dtemp2 = cosTheta;

	for (int y = 0; y <= heightOfDst - 1; ++y,temp1+=dtemp1,temp2+=dtemp2)
	{
		// 改变了循环内部增量的方式
		double temp3= ((0 - x0 - dx)*cosTheta + temp1)*scale + x0;
		double temp4= (-(0 - x0 - dx)*sinTheta + temp2)*scale + y0;
		double dtemp3 = (cosTheta)*scale;
		double dtemp4= (-sinTheta)*scale;
		for (int x = 0; x <= widthOfDst - 1; ++x,temp3+=dtemp3,temp4+=dtemp4)
		{
			// 计算原图坐标
			double srcX = temp3;
			double srcY = temp4;

			// 加1,得到在extendedImage中的坐标
			srcX++;
			srcY++;

			// get the nearest coordinate of src
			int x1 = (int)(srcX);
			int y1 = (int)(srcY);

			// 浮点转化为整数
			int dx1 = (srcX - x1)*(1 << SHIFT);
			int dy1 = (srcY - y1)*(1 << SHIFT);

			if (numberOfChannels == 1)
			{
				// !！！注意这里的范围，在extendedImage中，原图的范围就是1~cols - 2了
				if ((x1 >= 1 && x1 <= extendedImage.cols - 2) && (y1 >= 1 && y1 <= extendedImage.rows - 2))
				{
					//双线性插值
					//周围4个点
					//a就是最近邻像素
					//a   b
					//  p
					//c   d
					uchar a = extendedImage.at<uchar>(y1, x1);
					uchar b = extendedImage.at<uchar>(y1, x1 + 1);
					uchar c = extendedImage.at<uchar>(y1 + 1, x1);
					uchar d = extendedImage.at<uchar>(y1 + 1, x1 + 1);

					// 不要做除法
					//int p = (a*((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b*dx1*((1 << SHIFT) - dy1) + c*((1 << SHIFT) - dx1)*dy1 + d*dx1*dy1)/(1<<(2* SHIFT));

					int p = a*((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b*dx1*((1 << SHIFT) - dy1) + c*((1 << SHIFT) - dx1)*dy1 + d*dx1*dy1;
					p = DESCALE(p, 2 * SHIFT);

					dstImage.at<uchar>(y, x) = p;
				}
				else
				{
					// 越界赋值0
					dstImage.at<uchar>(y, x) = 0;
				}
			}
			else
			{
				if ((x1 >= 1 && x1 <= extendedImage.cols - 2) && (y1 >= 1 && y1 <= extendedImage.rows - 2))
				{
					//双线性插值
					//周围4个点
					//a就是最近邻像素
					//a   b
					//  p
					//c   d
					Vec3b a = extendedImage.at<Vec3b>(y1, x1);
					Vec3b b = extendedImage.at<Vec3b>(y1, x1 + 1);
					Vec3b c = extendedImage.at<Vec3b>(y1 + 1, x1);
					Vec3b d = extendedImage.at<Vec3b>(y1 + 1, x1 + 1);

					// 不要做除法
					/*int p1 = (a[0] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[0] * dx1*((1 << SHIFT) - dy1) + c[0] * ((1 << SHIFT) - dx1)*dy1 + d[0] * dx1*dy1)/(1<<(2*SHIFT));
					int p2 = (a[1] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[1] * dx1*((1 << SHIFT) - dy1) + c[1] * ((1 << SHIFT) - dx1)*dy1 + d[1] * dx1*dy1)/ (1 << (2 * SHIFT));
					int p3 = (a[2] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[2] * dx1*((1 << SHIFT) - dy1) + c[2] * ((1 << SHIFT) - dx1)*dy1 + d[2] * dx1*dy1)/ (1 << (2 * SHIFT));*/

					int p1 = a[0] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[0] * dx1*((1 << SHIFT) - dy1) + c[0] * ((1 << SHIFT) - dx1)*dy1 + d[0] * dx1*dy1;
					p1 = DESCALE(p1, 2 * SHIFT);
					int p2 = a[1] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[1] * dx1*((1 << SHIFT) - dy1) + c[1] * ((1 << SHIFT) - dx1)*dy1 + d[1] * dx1*dy1;
					p2 = DESCALE(p2, 2 * SHIFT);
					int p3 = a[2] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[2] * dx1*((1 << SHIFT) - dy1) + c[2] * ((1 << SHIFT) - dx1)*dy1 + d[2] * dx1*dy1;
					p3 = DESCALE(p3, 2 * SHIFT);


					dstImage.at<cv::Vec3b>(y, x) = Vec3b(p1, p2, p3);
				}
				else
				{
					dstImage.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
				}

			}
		}
	}
} // Rotate_Bilinear2

  // 2016-9-11 15:57:13,by QQ
  //  优化：在Rotate_Bilinear2的基础上，进一步提取不变量,至此循环体内循环变量的自增已经没有乘法
void Rotate_Bilinear3(const Mat &srcImage, Point center, double theta, double scale, Size dstSize, Mat &dstImage)
{
	CV_Assert(srcImage.depth() == CV_8U);
	dstImage.create(dstSize, srcImage.type());
	dstImage.setTo(Scalar(0, 0, 0));

	// 原图像的旋转中心
	int x0 = center.x;
	int y0 = center.y;
	theta = DEGREE2RADIAN(theta);

	// dx,dy就是dstImage中心点与srcImage的旋转中心的距离，也就是平移的距离
	int dx = dstImage.cols / 2 - x0;
	int dy = dstImage.rows / 2 - y0;
	int numberOfChannels = srcImage.channels();

	int widthOfDst = dstImage.cols;
	int heightOfDst = dstImage.rows;

	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1, BORDER_CONSTANT, Scalar(0, 0, 0)); // 使用0填充边界

	////////////////////////////////// 优化部分/////////////////////////////
	// 将循环内的不变量提取出来
	double sinTheta = sin(theta);
	double cosTheta = cos(theta);
	scale = 1.0 / scale;

	// 改变了循环内部增量的方式
	double temp1 = (0 - y0 - dy)*sinTheta;
	double temp2 = (0 - y0 - dy)*cosTheta;
	double dtemp1 = sinTheta;
	double dtemp2 = cosTheta;

	double dtemp3 = (cosTheta)*scale;
	double dtemp4 = (-sinTheta)*scale;

	// 改变了循环内部增量的方式
	double temp3 = ((0 - x0 - dx)*cosTheta + temp1)*scale + x0- (dtemp1*scale);
	double temp4 = (-(0 - x0 - dx)*sinTheta + temp2)*scale + y0- (dtemp2*scale);

	double dtemp5 = (dtemp1*scale);
	double dtemp6 = (dtemp2*scale);
	for (int y = 0; y <= heightOfDst - 1; ++y)
	{
		// 改变了循环内部增量的方式
		//double temp3 = ((0 - x0 - dx)*cosTheta + temp1)*scale + x0;
		//double temp4 = (-(0 - x0 - dx)*sinTheta + temp2)*scale + y0;
		temp3 += dtemp5;
		temp4 += dtemp6;
		
		// 注意这里一定要创建临时变量，如果直接使用temp3,temp4就错误了，因为temp3和temp4在内循环中值被修改了
		double temp5 = temp3;
		double temp6 = temp4;
		for (int x = 0; x <= widthOfDst - 1; ++x, temp5 += dtemp3, temp6 += dtemp4)
		{
			// 计算原图坐标
			double srcX = temp5;
			double srcY = temp6;

			// 加1,得到在extendedImage中的坐标
			srcX++;
			srcY++;

			// get the nearest coordinate of src
			int x1 = (int)(srcX);
			int y1 = (int)(srcY);

			// 浮点转化为整数
			int dx1 = (srcX - x1)*(1 << SHIFT);
			int dy1 = (srcY - y1)*(1 << SHIFT);

			if (numberOfChannels == 1)
			{
				// !！！注意这里的范围，在extendedImage中，原图的范围就是1~cols - 2了
				if ((x1 >= 1 && x1 <= extendedImage.cols - 2) && (y1 >= 1 && y1 <= extendedImage.rows - 2))
				{
					//双线性插值
					//周围4个点
					//a就是最近邻像素
					//a   b
					//  p
					//c   d
					uchar a = extendedImage.at<uchar>(y1, x1);
					uchar b = extendedImage.at<uchar>(y1, x1 + 1);
					uchar c = extendedImage.at<uchar>(y1 + 1, x1);
					uchar d = extendedImage.at<uchar>(y1 + 1, x1 + 1);

					// 不要做除法
					//int p = (a*((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b*dx1*((1 << SHIFT) - dy1) + c*((1 << SHIFT) - dx1)*dy1 + d*dx1*dy1)/(1<<(2* SHIFT));

					int p = a*((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b*dx1*((1 << SHIFT) - dy1) + c*((1 << SHIFT) - dx1)*dy1 + d*dx1*dy1;
					p = DESCALE(p, 2 * SHIFT);

					dstImage.at<uchar>(y, x) = p;
				}
				else
				{
					// 越界赋值0
					dstImage.at<uchar>(y, x) = 0;
				}
			}
			else
			{
				if ((x1 >= 1 && x1 <= extendedImage.cols - 2) && (y1 >= 1 && y1 <= extendedImage.rows - 2))
				{
					//双线性插值
					//周围4个点
					//a就是最近邻像素
					//a   b
					//  p
					//c   d
					Vec3b a = extendedImage.at<Vec3b>(y1, x1);
					Vec3b b = extendedImage.at<Vec3b>(y1, x1 + 1);
					Vec3b c = extendedImage.at<Vec3b>(y1 + 1, x1);
					Vec3b d = extendedImage.at<Vec3b>(y1 + 1, x1 + 1);

					// 不要做除法
					/*int p1 = (a[0] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[0] * dx1*((1 << SHIFT) - dy1) + c[0] * ((1 << SHIFT) - dx1)*dy1 + d[0] * dx1*dy1)/(1<<(2*SHIFT));
					int p2 = (a[1] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[1] * dx1*((1 << SHIFT) - dy1) + c[1] * ((1 << SHIFT) - dx1)*dy1 + d[1] * dx1*dy1)/ (1 << (2 * SHIFT));
					int p3 = (a[2] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[2] * dx1*((1 << SHIFT) - dy1) + c[2] * ((1 << SHIFT) - dx1)*dy1 + d[2] * dx1*dy1)/ (1 << (2 * SHIFT));*/

					int p1 = a[0] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[0] * dx1*((1 << SHIFT) - dy1) + c[0] * ((1 << SHIFT) - dx1)*dy1 + d[0] * dx1*dy1;
					p1 = DESCALE(p1, 2 * SHIFT);
					int p2 = a[1] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[1] * dx1*((1 << SHIFT) - dy1) + c[1] * ((1 << SHIFT) - dx1)*dy1 + d[1] * dx1*dy1;
					p2 = DESCALE(p2, 2 * SHIFT);
					int p3 = a[2] * ((1 << SHIFT) - dx1)*((1 << SHIFT) - dy1) + b[2] * dx1*((1 << SHIFT) - dy1) + c[2] * ((1 << SHIFT) - dx1)*dy1 + d[2] * dx1*dy1;
					p3 = DESCALE(p3, 2 * SHIFT);


					dstImage.at<cv::Vec3b>(y, x) = Vec3b(p1, p2, p3);
				}
				else
				{
					dstImage.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
				}

			}
		}
	}
}  // Rotate_Bilinear3




}//namespace QQ
