#define DLLAPI_EXPORTS
#include "IntegralImage.h"
#include <time.h>

namespace QQ
{


//2015-3-4 16:53:57，by QQ
//改进的计算像素灰度值积分图像
//srcImage:灰度图
//image_Integral：srcImage大小相同的CV_32SC1类型
//方法：Integral(y,x) = Integral(y-1,x) + rowSum(y);
void CalculateIntegralImage(const Mat &srcImage,Mat &image_Integral)
{
	/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
	//新图像的大小
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	image_Integral.create(Size(width_Dst,height_Dst),CV_32SC1);//如果重新分配，之前的空间会扔掉
	image_Integral.setTo(Scalar(0));

	/////////////////////////////step 2.计算积分图/////////////////////////////////////////////
	int width_Src=srcImage.cols;
	int height_Src=srcImage.rows;
	
	//每个像素的通道数
	int channelCount_Src=srcImage.channels();
	int channelCount_Integral=image_Integral.channels();//每个像素的通道数
	
	//行的通道数
	int widthStep_Src=channelCount_Src*width_Src;
	int widthStep_Integral=channelCount_Integral*width_Src;
	
	//第一行
	uchar *row_Src=srcImage.data;
	int *row_Integral=(int *)image_Integral.data;//注意指针的转换
	for (int y=0;y<=height_Src-1;++y)
	{
		int sum=0;//当前行的累加和
		//列
		uchar *col_Src=row_Src;
		int *col_Integral=row_Integral;
		for (int x=0;x<=width_Src-1;++x)
		{
			//该行的累加
			sum+=col_Src[0];

			//计算第0行,第一行单独处理
			if (y==0)
			{
				col_Integral[0]=sum;
			}
			else
			{
				//非第0行
				//当前行累加和+同列的上一个元素的值
				col_Integral[0]=sum+col_Integral[0-widthStep_Integral];//下标
				//col_Integral[0]=sum+*(col_Integral-image_Integral.cols);//指针移动

			}

			//下一个像素
			col_Src++;
			col_Integral++;

		}
		//下一行
		row_Src+=widthStep_Src;
		row_Integral+=widthStep_Integral;
	}
	//临时访问用
	//printf("2:%d\n",image_Integral.at<int>(srcImage.rows-1,srcImage.cols-1));
	//printf("2:%d\n",*((int *)image_Integral.data+image_Integral.cols*image_Integral.rows*image_Integral.channels()-1));

}


//计算像素灰度值积分图像
//srcImage:灰度图
//image_Integral：srcImage大小相同的CV_32SC1类型
//方法：Integral(y,x) = Integral(y,x-1) + Integral(y-1,x) - Integral(y-1,x-1) + Image(y,x);
void CalculateIntegralImage_Old(const Mat &srcImage,Mat &image_Integral)
{
	/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
	//新图像的大小
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	image_Integral.create(Size(width_Dst,height_Dst),CV_32SC1);//如果重新分配，之前的空间会扔掉
	IplImage iplImage=image_Integral;
	cvSetZero(&iplImage);

	/////////////////////////////step 2.计算积分图/////////////////////////////////////////////
	for (int y=0;y<=srcImage.rows-1;++y)
	{
		for (int x=0;x<=srcImage.cols-1;++x)
		{
			//第0行,单独处理
			if (y==0)
			{
				//加左边和下边
				if (x!=0)
				{
					//只有左边
					image_Integral.at<int>(y,x)+=image_Integral.at<int>(y,x-1);//使用矩阵坐标的形式，访问更快

				}
				//加Image(x,y)
				image_Integral.at<int>(y,x)+=srcImage.at<uchar>(y,x);

			}
			else
			{
				//第0列
				if (x==0)
				{
					//加左边和下边
					image_Integral.at<int>(y,x)+=image_Integral.at<int>(y-1,x);//只有下边

					//加Image(x,y)
					image_Integral.at<int>(y,x)+=srcImage.at<uchar>(y,x);
				}
				else
				{
					//非第0行，非第0列
					//加周左边和下边
					image_Integral.at<int>(y,x)+=image_Integral.at<int>(y-1,x);//下边
					image_Integral.at<int>(y,x)+=image_Integral.at<int>(y,x-1);//左边

					//减去重复的！
					image_Integral.at<int>(y,x)-=image_Integral.at<int>(y-1,x-1);

					//加Image(x,y)
					image_Integral.at<int>(y,x)+=srcImage.at<uchar>(y,x);

				}

			}

		}
	}
	//printf("old:%d\n",image_Integral.at<int>(srcImage.rows-1,srcImage.cols-1));

}


//改进的计算像素灰度值积分图像
//srcImage:灰度图
//image_Integral：srcImage大小相同的CV_32SC1类型
//方法：Integral(y,x) = Integral(y-1,x) + rowSum(y);
void CalculateIntegralImage_2(const Mat &srcImage,Mat &image_Integral)
{
	/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
	//新图像的大小
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	image_Integral.create(Size(width_Dst,height_Dst),CV_32SC1);//如果重新分配，之前的空间会扔掉
	IplImage iplImage=image_Integral;
	cvSetZero(&iplImage);

	/////////////////////////////step 2.计算积分图/////////////////////////////////////////////
	for (int y=0;y<=srcImage.rows-1;++y)
	{
		int sum=0;//当前行的累加和
		uchar *row_Src=srcImage.data+y*srcImage.step[0];
		int *row_Integral=(int *)(image_Integral.data+y*image_Integral.step[0]);
		for (int x=0;x<=srcImage.cols-1;++x)
		{
			//列
			uchar *col_Src=row_Src+x*srcImage.channels();//函数调用非常低效
			int *col_Integral=row_Integral+x*image_Integral.channels();
			//该行的累加
			sum+=col_Src[0];

			//计算第0行,第一行单独处理
			if (y==0)
			{
				col_Integral[0]=sum;
			}
			else
			{
				//非第0行
				//当前行累加和+同列的上一个元素的值
				col_Integral[0]=sum+col_Integral[0-image_Integral.cols];//下标
				//col_Integral[0]=sum+*(col_Integral-image_Integral.cols);//指针移动

			}

		}
	}
	//printf("2:%d\n",image_Integral.at<int>(srcImage.rows-1,srcImage.cols-1));

}


//改进的计算像素灰度值积分图像
//srcImage:灰度图
//image_Integral：srcImage大小相同的CV_32SC1类型
//方法：Integral(y,x) = Integral(y-1,x) + rowSum(y);
void CalculateIntegralImage_1(const Mat &srcImage,Mat &image_Integral)
{
	/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
	//新图像的大小
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	image_Integral.create(Size(width_Dst,height_Dst),CV_32SC1);//如果重新分配，之前的空间会扔掉
	IplImage iplImage=image_Integral;
	cvSetZero(&iplImage);

	/////////////////////////////step 2.计算积分图/////////////////////////////////////////////
	for (int y=0;y<=srcImage.rows-1;++y)
	{
		int sum=0;//当前行的累加和
		for (int x=0;x<=srcImage.cols-1;++x)
		{
			//该行的累加
			sum+=srcImage.at<uchar>(y,x);

			//计算第0行,第一行单独处理
			if (y==0)
			{
				image_Integral.at<int>(y,x)=sum;
			}
			else
			{
				//非第0行
				//当前行累加和+同列的上一个元素的值
				image_Integral.at<int>(y,x)=sum+image_Integral.at<int>(y-1,x);
			}

		}
	}
	//printf("2:%d\n",image_Integral.at<int>(srcImage.rows-1,srcImage.cols-1));

}



}//namespace QQ