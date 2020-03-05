#define DLLAPI_EXPORTS

#include "Filter.h"
#include "Histogram.h"

namespace QQ
{

//计算窗口像素平均值
static int CalculateMeanValue(uchar *imageData, int pixelCount)
{
	int sum = 0;
	for (int i = 0; i <= pixelCount - 1; ++i)
	{
		sum += imageData[i];
	}
	return sum / pixelCount;
}

//计算亮度中值和灰度<=中值的像素点个数
static void CalculateImage_MedianGray_PixelCount(const Mat &histogram, int pixelCount, int &medianValue, int &pixleCountLowerMedian)
{
	float  *data = (float *)histogram.data;//直方图
	int sum = 0;
	for (int i = 0; i <= 255; ++i)
	{
		//
		sum += data[i];
		if (2 * sum>pixelCount)
		{
			medianValue = i;
			pixleCountLowerMedian = sum;
			break;
		}
	}
}

// 用于比较
static uchar GetMedianValue(const Mat& image)
{
	double time1 = getTickCount();
	////////////////////////求直方图////////////////////////////////////
	Mat histogram;//直方图
	int channels[] = { 0 };//需要计算的通道
	int histogramSize[] = { 256 };////每个直方图bin的个数	

	//每个直方图灰度级范围
	float range[] = { 0, 256 };//多个直方图，需要设置多个数组 ，参考 Learn_calcHist_minMaxLoc_Function_Demo3()   
	const float* histRange[] = { range };

	//计算直方图
	calcHist(&image, //Mat 数组，这里只计算一副图像
		1,//Mat的个数
		channels,//使用哪些单通道的图像来计算目标直方图，通道数：0,1,2...
		Mat(),//Mat()返回一个空值，表示不用mask,
		histogram, //直方图
		1, //直方图的维数，如果计算2个直方图，就为2
		histogramSize, //每一个直方图bin的个数,也就是每列的行数
		histRange//灰度级变化范围
		);
	double time2 = getTickCount();
	double elapse_ms = (time2 - time1)*1000. / getTickFrequency();
	std::cout << "OpenCV time:" << elapse_ms << std::endl;


	////////////////////////求中值////////////////////////////////////
	float *data = (float *)histogram.data;
	int sum = 0;
	int medianValue;
	int pixelCount = image.cols*image.rows;
	for (int i = 0; i <= 255; ++i)
	{
		sum += data[i];
		if (sum >= pixelCount / 2)
		{
			medianValue = i;
			break;
		}

	}
	return medianValue;
}

//2015-1-1 20:23:53，by QQ
//使用copyMakeBorder扩充图像边界，处理滤波边界
void Blur(const Mat &srcImage, Mat &dstImage, Size size_Aperture)
{
	/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
	//新图像的大小
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	dstImage.create(Size(width_Dst,height_Dst),CV_8UC1);//如果重新分配，之前的空间会扔掉

	//////////////////////////////step 2.创建一副新图像(对源图像加以扩充)////////////////////////////////////////////
	//扩充的大小
	int width_Extend=size_Aperture.width>>1;
	int height_Extend=size_Aperture.height>>1;

	//创建新图像
	Mat image_New;
	copyMakeBorder(srcImage,image_New,height_Extend,height_Extend,width_Extend,width_Extend,BORDER_DEFAULT);


	////////////////////////////step 3.滤波//////////////////////////////////////////////
	//新图像参数
	int width_New=image_New.cols;
	int height_New=image_New.rows;
	int widthStep_New=width_New;
	
	//滑动窗口
	int width_Aperture=size_Aperture.width;
	int height_Aperture=size_Aperture.height;
	int pixelCount=width_Aperture*height_Aperture;

	//计算需要滤波像素的起点坐标
	int startX=width_Aperture>>1;
	int startY=height_Aperture>>1;

	//每列的灰度值和
	int *sum_PerCol=new int[width_New];

	//对新图像做滤波处理
	uchar *row_New=image_New.data+startY*widthStep_New+startX;
	uchar *row_Dst=dstImage.data;//第一行
	uchar *row_Aperture_New=image_New.data;
	for (int y=startY;y<=height_New-startY-1;++y)
	{
		//列
		uchar *col_Dst=row_Dst;
		uchar *col_Aperture_New=row_Aperture_New;

		//计算每列height_Aperture个像素的灰度值和
		//第一行，计算所有列的和
		if (y==startY)
		{

			for (int k=0;k<=width_New-1;++k)
			{
				sum_PerCol[k]=0;
				//每列第一个指针
				uchar *col_PerLine=col_Aperture_New+k;
				for (int t=0;t<=height_Aperture-1;++t)
				{
					sum_PerCol[k]+=col_PerLine[0];
					col_PerLine+=widthStep_New;//下一行
				}

			}
		}
		else//非第一行
		{
			for (int k=0;k<=width_New-1;++k)
			{
				//每列第一个指针
				uchar *col_=col_Aperture_New+k;
				sum_PerCol[k]-=col_[0-widthStep_New];//减上面
				sum_PerCol[k]+=col_[0+(height_Aperture-1)*widthStep_New];//加下面
			}

		}

		//计算width_Aperture行的列总和
		int sum_Aperture=0;
		for (int x=startX;x<=width_New-startX-1;++x)
		{
			//每行第一个元素，求width_Aperture个列和
			if (x==startX)
			{
				for (int k=0;k<=width_Aperture-1;++k)
				{
					sum_Aperture+=sum_PerCol[k];
				}
			}
			else//非第一个元素
			{
				//减去左边
				sum_Aperture-=sum_PerCol[x-startX-1];

				//加上右边
				sum_Aperture+=sum_PerCol[x+startX];
			}

			//求均值
			uchar meanValue=sum_Aperture/pixelCount;
			col_Dst[0]=meanValue;

			//滑动一个像素
			col_Dst++;
			//col_New++;//这里不需要对源图像指针进行滑动
		}

		//下一行
		row_Dst+=width_Dst;
		row_Aperture_New+=width_New;
	}
}

//思路：将原图向外扩充一个滑动窗口大小(设为黑色)，由于均值滤波算的是和，这样就可以将所有像素的操作都统一起来
//只是边界的窗口的像素点个数不一样
void Blur_3(const Mat &srcImage, Mat &dstImage, Size size_Aperture)
{
	/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
	//新图像的大小
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	dstImage.create(Size(width_Dst,height_Dst),CV_8UC1);//如果重新分配，之前的空间会扔掉

	//////////////////////////////step 2.创建一副新图像(对源图像加以扩充)////////////////////////////////////////////
	//新图像大小
	int width_Extend=size_Aperture.width>>1;
	int height_Extend=size_Aperture.height>>1;
	int width_New=width_Dst+2*width_Extend;//宽度左右各扩充width_Extend
	int height_New=height_Dst+2*height_Extend;//高度上下各扩充height_Extend

	//创建新图像
	Mat image_New(Size(width_New,height_New),CV_8UC1,Scalar(0));
	
	//拷贝源图像到新图像
	int widthStep_New=image_New.cols;//新图像的步长
	int widthStep_Src=srcImage.cols;//源图像的步长
	int width_Src=width_Dst;
	int height_Src=height_Dst;
	uchar *row_Src=srcImage.data;
	uchar *row_New=image_New.data+height_Extend*widthStep_New+width_Extend;
	for (int y=0;y<=height_Src-1;++y)//纵坐标
	{
		//列
		uchar *col_Src=row_Src;
		uchar *col_New=row_New;
		for (int x=0;x<=width_Src-1;++x)//横坐标
		{
			//处理每个像素
			col_New[0]=col_Src[0];

			//下一个像素
			col_Src++;
			col_New++;
		}

		//下一行
		row_Src+=widthStep_Src;
		row_New+=widthStep_New;
	}

	////////////////////////////step 3.滤波//////////////////////////////////////////////
	//滑动窗口
	int width_Aperture=size_Aperture.width;
	int height_Aperture=size_Aperture.height;

	//计算需要滤波像素的起点坐标
	int startX=width_Aperture>>1;
	int startY=height_Aperture>>1;

	//每列的灰度值和
	int *sum_PerCol=new int[width_New];
	
	//对新图像做滤波处理
	row_New=image_New.data+startY*widthStep_New+startX;
	uchar *row_Dst=dstImage.data;//第一行
	uchar *row_Aperture_New=image_New.data;
	for (int y=startY;y<=height_New-startY-1;++y)
	{
		//列
		//uchar *col_New=row_New;//这里不需要对源图像指针进行滑动
		uchar *col_Dst=row_Dst;
		uchar *col_Aperture_New=row_Aperture_New;

		//计算每列height_Aperture个像素的灰度值和
		//第一行，计算所有列的和
		if (y==startY)
		{

			for (int k=0;k<=width_New-1;++k)
			{
				sum_PerCol[k]=0;
				//每列第一个指针
				uchar *col_PerLine=col_Aperture_New+k;
				for (int t=0;t<=height_Aperture-1;++t)
				{
					sum_PerCol[k]+=col_PerLine[0];
					col_PerLine+=widthStep_New;//下一行
				}

			}
		}
		else//非第一行
		{
			for (int k=0;k<=width_New-1;++k)
			{
				//每列第一个指针
				uchar *col_=col_Aperture_New+k;
				sum_PerCol[k]-=col_[0-widthStep_New];//减上面
				sum_PerCol[k]+=col_[0+(height_Aperture-1)*widthStep_New];//加下面
			}

		}

		//计算width_Aperture行的列总和
		int sum_Aperture=0;
		for (int x=startX;x<=width_New-startX-1;++x)
		{
			//每行第一个元素，求width_Aperture个列和
			if (x==startX)
			{
				for (int k=0;k<=width_Aperture-1;++k)
				{
					sum_Aperture+=sum_PerCol[k];
				}
			}
			else//非第一个元素
			{
				//减去左边
				sum_Aperture-=sum_PerCol[x-startX-1];

				//加上右边
				sum_Aperture+=sum_PerCol[x+startX];
			}

			///////////////////////////////////边界处理:计算滑动窗口内灰度值非零的像素点个数///////////////////////////////////////
			//中间区域：窗口内所有像素点
			int pixelCount=width_Aperture*height_Aperture;
			
			//上边界:部分像素点
			if (y<=2*startY)
			{
				int w=(x+1)<width_Aperture?(x+1):width_Aperture;
				pixelCount=(y+1)*w;
			}
			
			//左边界
			if(x<=2*startX)
			{
				int h=(y+1)<height_Aperture?(y+1):height_Aperture;
				pixelCount=(x+1)*h;
			}
			
			//下边界
			if (y>=height_New-2*startY)
			{
				int w=(x+1)<width_Aperture?(x+1):width_Aperture;
				pixelCount=(height_New-y)*w;
			}

			//右边界
			if(x>=width_New-2*startX)
			{
				int h=(y+1)<=height_Aperture?(y+1):height_Aperture;
				pixelCount=(width_New-x)*h;
				
				//最右下角(宽度和高度都在减小):这个角特别注意，需要特殊处理
				if (y>=height_New-2*startY)
				{
					int h=height_New-y;
					pixelCount=(width_New-x)*h;

				}
			}
			
			//求均值
			uchar meanValue=sum_Aperture/pixelCount;
			col_Dst[0]=meanValue;

			//滑动一个像素
			col_Dst++;
			//col_New++;//这里不需要对源图像指针进行滑动
		}

		//下一行
		//row_New+=width_New;//这里不需要对源图像指针进行滑动
		row_Dst+=width_Dst;
		row_Aperture_New+=width_New;
	}
}

//！！边界没有处理
//高效均值滤波
void Blur_2(const Mat &srcImage, Mat &dstImage, Size size_Aperture)
{
	/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
	//新图像的大小
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	dstImage.create(Size(width_Dst,height_Dst),CV_8UC1);//如果重新分配，之前的空间会扔掉
	dstImage.setTo(Scalar(0));

	//滑动窗口
	int width_Aperture=size_Aperture.width;
	int height_Aperture=size_Aperture.height;
	int pixelCount=width_Aperture*height_Aperture;
	int *sum_PerCol=new int[width_Dst];//每列的灰度值


	//计算起点坐标
	//被滤波的点
	int startX=width_Aperture>>1;
	int startY=height_Aperture>>1;

	//第一行
	//三个关键的指针，这三个指针绑定在一起，一起滑动
	//1.2:需要被处理的像素
	//3:滑动窗口第一个元素,用来操作滑动窗口
	uchar *row_Src=srcImage.data+startY*width_Dst+startX;
	uchar *row_Dst=dstImage.data+startY*width_Dst+startX;
	uchar *row_Aperture_Src=srcImage.data;
	for (int y=startY;y<=height_Dst-startY-1;++y)
	{
		//列
		uchar *col_Src=row_Src;
		uchar *col_Dst=row_Dst;
		uchar *col_Aperture_Src=row_Aperture_Src;
		
		//计算每列height_Aperture个像素的灰度值和
		//第一行，计算所有列的和
		if (y==startY)
		{
			
			for (int k=0;k<=width_Dst-1;++k)
			{
				sum_PerCol[k]=0;
				//每列第一个指针
				uchar *col_PerLine=col_Aperture_Src+k;
				for (int t=0;t<=height_Aperture-1;++t)
				{
					sum_PerCol[k]+=col_PerLine[0];
					col_PerLine+=width_Dst;//下一行
				}

			}
		}
		else//非第一行
		{
			for (int k=0;k<=width_Dst-1;++k)
			{
				//每列第一个指针
				uchar *col_=col_Aperture_Src+k;
				sum_PerCol[k]-=col_[0-width_Dst];//减上面
				sum_PerCol[k]+=col_[0+(height_Aperture-1)*width_Dst];//加下面
			}

		}
		
		//计算width_Aperture行的列总和
		int sum_Aperture=0;
		for (int x=startX;x<=width_Dst-startX-1;++x)
		{
			//每行第一个元素，求width_Aperture个列和
			if (x==startX)
			{
				for (int k=x-startX;k<=x+startX;++k)
				{
					sum_Aperture+=sum_PerCol[k];
				}
			}
			else//非第一个元素
			{
				//减去左边
				sum_Aperture-=sum_PerCol[x-1-startX];
				
				//加上右边
				sum_Aperture+=sum_PerCol[x+startX];
			}
			
			//求均值
			uchar meanValue=sum_Aperture/pixelCount;
			col_Dst[0]=meanValue;

			//滑动一个像素
			col_Dst++;
			col_Src++;
		}
		//下一行
		row_Dst+=width_Dst;
		row_Src+=width_Dst;
		row_Aperture_Src+=width_Dst;
	}
}




//边界没处理
//窗口高度和宽度都必须为正奇数
//均值滤波
void Blur_1(const Mat &srcImage, Mat &dstImage,Size size_Aperture)
{
	/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
	//新图像的大小
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	dstImage.create(Size(width_Dst,height_Dst),CV_8UC1);//如果重新分配，之前的空间会扔掉
	dstImage.setTo(Scalar(0));

	//滑动窗口
	int width_Aperture=size_Aperture.width;
	int height_Aperture=size_Aperture.height;
	int pixelCount=width_Aperture*height_Aperture;//窗口内像素总个数
	Mat image_Aperture(1,pixelCount,CV_8UC1);//滑动窗口图像

	//计算起点坐标
	int startX=width_Aperture/2;
	int startY=height_Aperture/2;

	//行
	uchar *row_Src=srcImage.data+startY*width_Dst+startX;
	uchar *row_Dst=dstImage.data+startY*width_Dst+startX;
	for (int y=startY;y<=height_Dst-startY-1;++y)
	{
		//列
		uchar *col_Src=row_Src;
		uchar *col_Dst=row_Dst;
		for (int x=startX;x<=width_Dst-startX-1;++x)
		{
			//滑动窗口左上角点
			uchar *row_LeftUp=col_Src-startY*width_Dst-startX;

			//提取滑动窗口图像
			for (int k=0;k<=height_Aperture-1;++k)
			{
				//列
				for (int w=0;w<=width_Aperture-1;++w)
				{
					image_Aperture.data[k*width_Aperture+w]=row_LeftUp[k*width_Dst+w];
				}
			}

			//求均值
			uchar meanValue=CalculateMeanValue(image_Aperture.data,pixelCount);
			col_Dst[0]=meanValue;

			//滑动一个像素
			col_Dst++;
			col_Src++;
		}
		//下一行
		row_Dst+=width_Dst;
		row_Src+=width_Dst;
	}
}



//2015-4-22 19:51:29,by QQ
//改进的高效中值滤波：没有采用OpenCV求直方图
void FastMedianBlur(const Mat &srcImage, Mat &dstImage, int width_Aperture)
{
	/////////////////////////////重新分配图像(如果需要)/////////////////////////////////////////////
	int width_Dst = srcImage.cols;
	int height_Dst = srcImage.rows;
	dstImage.create(Size(width_Dst, height_Dst), CV_8UC1);//如果重新分配，之前的空间会扔掉
	dstImage.setTo(Scalar(0));//置为0

	//滑动窗口
	int pixelCount = width_Aperture*width_Aperture;//窗口内像素总个数
	Mat image_Aperture(width_Aperture, width_Aperture, CV_8UC1);//滑动窗口图像
	int thresholdValue = pixelCount / 2 + 1;//step1.设置阈值(步骤参考：图像的高效编程要点之四)

	//计算起点坐标
	int startX = width_Aperture / 2;
	int startY = width_Aperture / 2;

	///////////////////////////////扩充图像边界///////////////////////////////////////////
	int height_Extend = startY;
	int width_Extend = startX;
	Mat image_New;
	copyMakeBorder(srcImage, image_New, height_Extend, height_Extend, width_Extend, width_Extend, BORDER_DEFAULT);//默认采用BORDER_REFLECT
	int height_New = image_New.rows;
	int width_New = image_New.cols;
	//第一行
	//这里需要设置3个指针,一起滑动
	//1.源图像中被处理的像素 
	//2.目标图像被处理的像素 
	//3.源图像滑动窗口
	uchar *row_New = image_New.data + startY*width_New + startX;//新图像
	uchar *row_Dst = dstImage.data;//目标图像
	uchar *row_Aperture_New = image_New.data;//源图像中的滑动窗口

	for (int y = startY; y <= height_New - startY - 1; ++y)
	{
		//列
		uchar *col_New = row_New;
		uchar *col_Dst = row_Dst;
		uchar *col_Aperture_New = row_Aperture_New;//操作整个滑动窗口

		///////////////////////////////对滑动窗口操作///////////////////////////////////////////
		//计算每行第一个滑动窗口直方图
		//提取滑动窗口图像
		uchar *row_Aperture = image_Aperture.data;//滑动窗口图像
		uchar *row_Aperture_New_2 = col_Aperture_New;
		for (int k = 0; k <= width_Aperture - 1; ++k)
		{
			//列
			uchar *col_ApertureImage = row_Aperture;
			uchar *col_Aperture_New_2 = row_Aperture_New_2;

			for (int w = 0; w <= width_Aperture - 1; ++w)
			{
				//处理每个像素
				col_ApertureImage[0] = col_Aperture_New_2[0];

				//下一个像素
				col_ApertureImage++;
				col_Aperture_New_2++;
			}

			//下一行
			row_Aperture += width_Aperture;
			row_Aperture_New_2 += width_New;
		}

		//step 2.确定中值，并记录亮度<=中值的像素点个数
		//求直方图
		int histogram[256];
		GetHistogram(image_Aperture, histogram);

		//求亮度中值和<=中值的像素点个数
		int medianValue,pixelCountLowerMedian;
		GetMedianValue_AndPixelCountLowerMedian(image_Aperture, medianValue, pixelCountLowerMedian);
		//////////////////////////////滑动窗口操作结束////////////////////////////////////////////

		//滤波
		col_Dst[0] = (uchar)medianValue;

		//滑动一个像素(三个指针在一起移动)
		col_Dst++;
		col_New++;
		col_Aperture_New++;
		for (int x = startX + 1; x <= width_New - startX - 1; ++x)//从每行第二个滤波像素开始
		{
			//////////////////////////////////维持滑动窗口直方图////////////////////////////////////////
			//step 3.去掉左侧
			uchar *col_Left = col_Aperture_New - 1;
			for (int k = 0; k <= width_Aperture - 1; ++k)
			{
				int gray = col_Left[0];
				histogram[gray] -= 1;
				if (gray <= medianValue)
				{
					pixelCountLowerMedian--;
				}
				col_Left += width_New;
			}

			//step 4.增加右侧
			uchar *col_Right = col_Aperture_New + width_Aperture - 1;
			for (int k = 0; k <= width_Aperture - 1; ++k)
			{
				int gray = col_Right[0];
				histogram[gray] += 1;
				if (gray <= medianValue)
				{
					pixelCountLowerMedian++;
				}
				col_Right += width_New;
			}

			//搜索新的中值
			if (pixelCountLowerMedian>thresholdValue)//step 6.
			{
				while (1)
				{
					pixelCountLowerMedian -= histogram[medianValue];
					medianValue--;
					if (pixelCountLowerMedian <= thresholdValue)
					{
						break;
					}
				}

			}
			else
			{
				while (pixelCountLowerMedian<thresholdValue)//step 5
				{
					medianValue++;
					pixelCountLowerMedian += histogram[medianValue];

				}

			}

			//滤波
			col_Dst[0] = (uchar)medianValue;

			//滑动一个像素
			col_New++;
			col_Dst++;
			col_Aperture_New++;
		}//end of x

		//下一行
		row_New += width_New;
		row_Dst += width_Dst;
		row_Aperture_New += width_New;
	}//end of y

}

//！！边界没有处理
//中值滤波，使用的时候，注意修改直方图类型，修改为float
//窗口大小为width_Aperture*width_Aperture的正方形
void FastMedianBlur_1(const Mat &srcImage, Mat &dstImage, int width_Aperture)
{
	/////////////////////////////重新分配图像(如果需要)/////////////////////////////////////////////
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	dstImage.create(Size(width_Dst,height_Dst),CV_8UC1);//如果重新分配，之前的空间会扔掉
	dstImage.setTo(Scalar(0));//置为0

	//滑动窗口
	int pixelCount=width_Aperture*width_Aperture;//窗口内像素总个数
	Mat image_Aperture(width_Aperture,width_Aperture,CV_8UC1);//滑动窗口图像

	//直方图
	Mat histogram;
	int histogramSize=256;//灰度等级
	int thresholdValue=pixelCount/2+1;//step1.设置阈值(步骤参考：图像的高效编程要点之四)

	//计算起点坐标
	int startX=width_Aperture/2;
	int startY=width_Aperture/2;

	//第一行
	//这里需要设置3个指针,一起滑动
	//1.源图像中被处理的像素 
	//2.目标图像被处理的像素 
	//3.源图像滑动窗口
	uchar *row_Src=srcImage.data+startY*width_Dst+startX;//源图像
	uchar *row_Dst=dstImage.data+startY*width_Dst+startX;//目标图像
	uchar *row_Aperture_Src=srcImage.data;//源图像中的滑动窗口
	
	for (int y=startY;y<=height_Dst-startY-1;++y)
	{
		//列
		uchar *col_Src=row_Src;
		uchar *col_Dst=row_Dst;
		uchar *col_Aperture_Src=row_Aperture_Src;//操作整个滑动窗口
		
		///////////////////////////////对滑动窗口操作///////////////////////////////////////////
		//计算每行第一个滑动窗口直方图
		//double time1 = getTickCount();
		Mat image_Aperture = srcImage(Rect(0, y - startY, width_Aperture, width_Aperture));
		//提取滑动窗口图像
		//uchar *row_Aperture=image_Aperture.data;//滑动窗口图像
		//uchar *row_Aperture_Src_2=col_Aperture_Src;
		//for (int k=0;k<=width_Aperture-1;++k)
		//{
		//	//列
		//	uchar *col_ApertureImage=row_Aperture;
		//	uchar *col_Aperture_Src_2=row_Aperture_Src_2;
		//	
		//	for (int w=0;w<=width_Aperture-1;++w)
		//	{
		//		//处理每个像素
		//		col_ApertureImage[0]=col_Aperture_Src_2[0];
		//		
		//		//下一个像素
		//		col_ApertureImage++;
		//		col_Aperture_Src_2++;
		//	}

		//	//下一行
		//	row_Aperture+=width_Aperture;
		//	row_Aperture_Src_2+=width_Dst;
		//}

		//double time2 = getTickCount();
		//double elapse_ms = (time2 - time1)*1000. / getTickFrequency();
		//std::cout << "ROI time:" << elapse_ms << std::endl;


		//step 2.确定中值，并记录亮度<=中值的像素点个数
		//求直方图
		calcHist(&image_Aperture,
			1,//Mat的个数
			0,//用来计算直方图的通道索引，通道索引依次排开
			Mat(),//Mat()返回一个空值，表示不用mask,
			histogram, //直方图
			1, //直方图的维数，如果计算2个直方图，就为2
			&histogramSize, //直方图的等级数(如灰度等级),也就是每列的行数
			0//分量的变化范围
			);
		
		//求亮度中值和<=中值的像素点个数
		int medianValue,pixleCountLowerMedian;
		CalculateImage_MedianGray_PixelCount(histogram,pixelCount,medianValue,pixleCountLowerMedian);
		//////////////////////////////滑动窗口操作结束////////////////////////////////////////////
		
		//滤波
		col_Dst[0]=(uchar)medianValue;
		
		//滑动一个像素(三个指针在一起移动)
		col_Dst++;
		col_Src++;
		col_Aperture_Src++;
		for (int x=startX+1;x<=width_Dst-startX-1;++x)//从每行第二个滤波像素开始
		{
			//////////////////////////////////维持滑动窗口直方图////////////////////////////////////////
			//step 3.去掉左侧
			uchar *col_Left=col_Aperture_Src-1;
			float *data=(float*)histogram.data;
			for (int k=0;k<=width_Aperture-1;++k)
			{
				int gray=col_Left[0];
				data[gray]-=1.0;
				if (gray<=medianValue)
				{
					pixleCountLowerMedian--;
				}
				col_Left+=width_Dst;
			}
			
			//step 4.增加右侧
			uchar *col_Right=col_Aperture_Src+width_Aperture-1;
			for (int k=0;k<=width_Aperture-1;++k)
			{
				int gray=col_Right[0];
				data[gray]+=1.0;
				if (gray<=medianValue)
				{
					pixleCountLowerMedian++;
				}
				col_Right+=width_Dst;
			}

			//搜索新的中值
			if (pixleCountLowerMedian>thresholdValue)//step 6.
			{
				while(1)
				{
					pixleCountLowerMedian-=data[medianValue];
					medianValue--;
					if (pixleCountLowerMedian<=thresholdValue)
					{
						break;
					}
				}
				
			}
			else
			{
				while(pixleCountLowerMedian<thresholdValue)//step 5
				{
					medianValue++;
					pixleCountLowerMedian+=data[medianValue];
				}

			}

			//滤波
			col_Dst[0]=(uchar)medianValue;

			//滑动一个像素
			col_Src++;
			col_Dst++;
			col_Aperture_Src++;
		}//end of x

		//下一行
		row_Src+=width_Dst;
		row_Dst+=width_Dst;
		row_Aperture_Src+=width_Dst;
	}//end of y

}



//边界没处理
//窗口大小为width_Aperture*width_Aperture
//窗口宽度：width_Aperture必须为奇数且为正
void MedianBlur(const Mat &srcImage, Mat &dstImage, int width_Aperture)
{

	/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
	//新图像的大小
	int width_Dst = srcImage.cols;
	int height_Dst = srcImage.rows;
	dstImage.create(Size(width_Dst, height_Dst), CV_8UC1);//如果重新分配，之前的空间会扔掉
	dstImage.setTo(Scalar(0));

	//滑动窗口
	int pixelCount = width_Aperture*width_Aperture;//窗口内像素总个数
	Mat image_Aperture(width_Aperture, width_Aperture, CV_8UC1);//滑动窗口图像


	//计算起点坐标
	int startX = width_Aperture / 2;
	int startY = width_Aperture / 2;

	//行
	//这里要设置3个值，源图像，目标图像处理的像素，源图像的滑动窗口图像,这几个值是需要绑定在一起移动的
	uchar *row_Src = srcImage.data + startY*width_Dst + startX;
	uchar *row_Dst = dstImage.data + startY*width_Dst + startX;
	uchar *row_Aperture_Src = srcImage.data;//原图像中的滑动窗口
	for (int y = startY; y <= height_Dst - startY - 1; ++y)
	{
		//列
		uchar *col_Src = row_Src;
		uchar *col_Dst = row_Dst;
		uchar *col_Aperture_Src = row_Aperture_Src;//每个滑动窗口第一个元素
		for (int x = startX; x <= width_Dst - startX - 1; ++x)
		{
			//////////////////////////////对滑动窗口操作////////////////////////////////////////////
			//提取滑动窗口图像
			
			//采用OpenCV提取，效率太低
			//3.0RC1版本已不支持该方式
			//Rect ROI(x - startX, y - startY, width_Aperture, width_Aperture);
			//Mat ROIImage = srcImage(ROI);
			//IplImage iplImage = ROIImage;
			//Mat image_Aperture(&iplImage, true);

			//uchar *row_ApertureImage = image_Aperture.data;
			//uchar *row_Aperture_Src_2 = col_Aperture_Src;
			//for (int k = 0; k <= width_Aperture - 1; ++k)//一共m行
			//{
			//	//列
			//	uchar *col_ApertureImage = row_ApertureImage;
			//	uchar *col_Aperture_Src_2 = row_Aperture_Src_2;
			//	for (int w = 0; w <= width_Aperture - 1; ++w)
			//	{
			//		col_ApertureImage[0] = col_Aperture_Src_2[0];

			//		//下一个像素
			//		col_Aperture_Src_2++;
			//		col_ApertureImage++;
			//	}
			//	//下一行
			//	row_ApertureImage += width_Aperture;
			//	row_Aperture_Src_2 += width_Dst;
			//}

			//求亮度中值
			int medianValue;
			GetMedianValue(image_Aperture, medianValue);
			
			//采取OpenCV计算简单直方图，效率低
			//medianValue = GetMedianValue(image_Aperture);

			//滤波
			col_Dst[0] = medianValue;

			//滑动一个像素
			col_Dst++;
			col_Src++;
			col_Aperture_Src++;
		}

		//下一行
		row_Dst += width_Dst;
		row_Src += width_Dst;
		row_Aperture_Src += width_Dst;
	}

}



// 2016-10-4,by QQ
void GaussianBlur(const Mat &srcImage, Mat &dstImage, double sigma)
{

	// compute the size of gaussian kernel
	int kSize = cvRound(sigma * 3) << 1 + 1;// must be odd

	
	double coefficient = -1 / (2 *sigma*sigma);
	double coefficient2 = -1 * coefficient / CV_PI;

	// the center of gaussian kernel
	int x0 = (kSize - 1) >> 1;
	int y0 = (kSize - 1) >> 1;

	// compute gaussian kernel
	Mat gaussianKernel(kSize, kSize,CV_32FC1);
	float *rowOfKernel = (float *)gaussianKernel.data;
	double sum = 0;// for normalization
	for (int y = 0; y <= kSize - 1; ++y,rowOfKernel+=kSize)
	{
		float *colOfKernel = rowOfKernel;
		for (int x = 0; x <= kSize - 1; ++x,++colOfKernel)
		{
			double distance = (x - x0)*(x - x0) + (y - y0)*(y - y0);
			colOfKernel[0] = coefficient2*exp(coefficient*(distance));
			sum += colOfKernel[0];
		}

	}

	// normalize the gaussian kernel
	int numberOfPixels = kSize*kSize;
	float *dataOfKernel = (float *)gaussianKernel.data;
	for (int i = 0; i <= numberOfPixels - 1; ++i)
	{
		dataOfKernel[i] /= sum;
	}

	// smooth
	cv::filter2D(srcImage, dstImage, CV_8U, gaussianKernel);

}// gaussian blur

// 2017-1-11 by QQ
// 卷积运算,Kernel正方形卷积核且边长为奇数
void Convolution1(const Mat &srcImage, const Mat &kernel, Mat &dstImage)
{
	CV_Assert(srcImage.type() == CV_8UC1&&kernel.type()==CV_32FC1);

	// 目标图像大小
	int stride = 1;// 默认滑动窗口步长为1
	int kernelSize = kernel.rows;
	int widthOfDst = (srcImage.cols + kernelSize - 1 - kernelSize) / stride + 1;
	int heightOfDst = (srcImage.rows + kernelSize - 1 - kernelSize) / stride + 1;
	dstImage.create(heightOfDst, widthOfDst,CV_8UC1);

	// 扩充原图
	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, kernelSize / 2, kernelSize / 2, kernelSize / 2, kernelSize / 2, BORDER_DEFAULT);

	// 生成滑动窗口矩阵
	int numberOfElementOfKernel = kernelSize*kernelSize;
	int numberOfPixelsOfDst = widthOfDst*heightOfDst;
	Mat slidingWindowMat(numberOfElementOfKernel, numberOfPixelsOfDst, CV_8UC1);
	uchar *dataOfSlidingWindowMat = slidingWindowMat.data;
	uchar *dataOfExtendImage = extendedImage.data;

	// 获取卷积核每个元素在原图中滑动的时候对应的所有像素，形成滑动窗口矩阵中的一行（滑动窗口矩阵也可以称为patch矩阵）
	/* 卷积核的该元素在图像中滑动的时候对应的每个像素的偏移由两部分组成(这里只考虑单通道图像)： 
		1. 该元素在滑动窗口内的偏移(每次遍历的时候是固定的),注：卷积核滑动的时候称为滑动窗口
		2. 滑动窗口在图像中的偏移(滑动窗口第一个元素在图像中的偏移) 
	*/
	for (int c = 0; c <= numberOfElementOfKernel - 1; ++c)
	{
		// 计算该元素在滑动窗口中y方向上和x方向上的偏移
		int offsetOfY_1 = (c / kernelSize) % kernelSize;
		int offsetOfX_1 = c % kernelSize;

		// 计算滑动窗口在图像中的偏移
		for (int y = 0; y <= heightOfDst - 1; ++y)
		{
			// 滑动窗口在y方向上的偏移(滑动窗口第一个元素在y方向上的偏移)
			int offsetOfY_2 = y*stride;

			for (int x = 0; x <= widthOfDst - 1; ++x)
			{
				// 滑动窗口在方向上的偏移(滑动窗口第一个元素在x方向上的偏移)
				int offsetOfX_2 = x*stride;

				// 最后的偏移
				int offsetOfY = offsetOfY_1 + offsetOfY_2;
				int offsetOfX = offsetOfX_1 + offsetOfX_2;

				dataOfSlidingWindowMat[c*numberOfPixelsOfDst + y*widthOfDst + x] = dataOfExtendImage[offsetOfY*extendedImage.cols + offsetOfX];
			}
		}

	}

	// 卷积核与滑动窗口矩阵相乘，这部分可以使用矩阵运算优化(比如Eigen)
	float *dataOfKernel = (float *)kernel.data;
	uchar *dataOfDst = dstImage.data;
	for (int x = 0; x <= slidingWindowMat.cols - 1; ++x)
	{
		float sum = 0; // 注意，这里不能使用int sum=0;每次都会有精度损失
		for (int y = 0; y <= numberOfElementOfKernel - 1; ++y)
		{
			sum += (dataOfKernel[y] * slidingWindowMat.at<uchar>(y, x));
		}

		// 卷积结果赋值为结果图像,注意溢出的处理！
		dataOfDst[x] = SATURATE((int)sum);
	}

} // Convolution1

// 2017-3-20 by QQ
void Convolution12(const Mat &srcImage, const Mat &kernel, Mat &dstImage)
{
	CV_Assert(srcImage.type() == CV_8UC1&&kernel.type() == CV_32FC1);

	// 目标图像大小
	int stride = 1;// 默认滑动窗口步长为1
	int kernelSize = kernel.rows;
	int widthOfDst = (srcImage.cols + kernelSize - 1 - kernelSize) / stride + 1;
	int heightOfDst = (srcImage.rows + kernelSize - 1 - kernelSize) / stride + 1;
	dstImage.create(heightOfDst, widthOfDst, CV_8UC1);

	// 扩充原图
	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, kernelSize / 2, kernelSize / 2, kernelSize / 2, kernelSize / 2, BORDER_DEFAULT);

	// 生成滑动窗口矩阵
	int numberOfElementOfKernel = kernelSize*kernelSize;
	int numberOfPixelsOfDst = widthOfDst*heightOfDst;
	Mat slidingWindowMat(numberOfElementOfKernel, numberOfPixelsOfDst, CV_8UC1);
	uchar *dataOfSlidingWindowMat = slidingWindowMat.data;
	uchar *dataOfExtendImage = extendedImage.data;

	///////////////////////////  优化部分 ///////////////////////////
	// 获取卷积核每个元素在原图中滑动的时候对应的所有像素，形成滑动窗口矩阵中的一行（滑动窗口矩阵也可以称为patch矩阵）
	uchar *rowOfSlidingWindowMat = dataOfSlidingWindowMat;// 指向滑动窗口矩阵的一行
	int stepOfExtendedImage = extendedImage.cols*stride;
	for (int c = 0; c <= numberOfElementOfKernel - 1; ++c, rowOfSlidingWindowMat += numberOfPixelsOfDst)
	{
		// 计算该元素在滑动窗口中y方向上和x方向上的偏移
		int offsetOfY_1 = (c / kernelSize) % kernelSize;
		int offsetOfX_1 = c % kernelSize;
		
		// 滑动窗口的列地址参与两层循环的计算
		uchar *colOfSlidingWindowMat = rowOfSlidingWindowMat;

		// data2OfExtendImage指向每一次滑动窗口滑动的时候的初始位置，也就是第一个滑动窗口该元素在图像中的位置
		uchar *data2OfExtendImage = dataOfExtendImage+offsetOfY_1*extendedImage.cols + offsetOfX_1;
		uchar *rowOfExtendImage = data2OfExtendImage;
		for (int y = 0; y <= heightOfDst - 1; ++y, rowOfExtendImage += stepOfExtendedImage, colOfSlidingWindowMat += widthOfDst)
		{
			uchar *col2OfSlidingWindowMat = colOfSlidingWindowMat;
			uchar *colOfExtendImage = rowOfExtendImage;
			for (int x = 0; x <= widthOfDst - 1; ++x, colOfExtendImage += stride, ++col2OfSlidingWindowMat)
			{

				col2OfSlidingWindowMat[0] = colOfExtendImage[0];
			}
		}

	}

	// 卷积核与滑动窗口矩阵相乘，这部分可以使用矩阵运算优化(比如Eigen)
	float *dataOfKernel = (float *)kernel.data;
	uchar *dataOfDst = dstImage.data;
	for (int x = 0; x <= slidingWindowMat.cols - 1; ++x)
	{
		float sum = 0; // 注意，这里不能使用int sum=0;每次都会有精度损失
		for (int y = 0; y <= numberOfElementOfKernel - 1; ++y)
		{
			sum += (dataOfKernel[y] * slidingWindowMat.at<uchar>(y, x));
		}

		// 卷积结果赋值为结果图像,注意溢出的处理！
		dataOfDst[x] = SATURATE((int)sum);
	}

}

// 2017-1-27 by QQ
// 卷积运算,Kernel正方形卷积核且边长为奇数
void Convolution2(const Mat &srcImage, const Mat &kernel, Mat &dstImage)
{

	CV_Assert(srcImage.type() == CV_8UC1&&kernel.type() == CV_32FC1);

	// 目标图像大小
	int stride = 1;// 默认滑动窗口步长为1
	int kernelSize = kernel.rows;
	int widthOfDst = (srcImage.cols + kernelSize - 1 - kernelSize) / stride + 1;
	int heightOfDst = (srcImage.rows + kernelSize - 1 - kernelSize) / stride + 1;
	dstImage.create(heightOfDst, widthOfDst, CV_8UC1);

	// 扩充原图
	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, kernelSize / 2, kernelSize / 2, kernelSize / 2, kernelSize / 2, BORDER_DEFAULT);

	uchar *rowOfDst = dstImage.data;
	for (int y = 0; y <= heightOfDst - 1; ++y, rowOfDst += widthOfDst)
	{
		uchar *colOfDst = rowOfDst;

		// 滑动窗口(第一个元素)在y方向上的偏移
		int offsetOfY = y*stride;

		for (int x = 0; x <= widthOfDst - 1; ++x, ++colOfDst)
		{
			// 滑动窗口(第一个元素)在x方向上的偏移
			int offsetOfX = x*stride;

			// 计算卷积
			uchar *rowOfROI = extendedImage.data + offsetOfY*extendedImage.cols + offsetOfX;
			float *rowOfKernel = (float *)kernel.data;
			float sum = 0;
			for (int y0 = 0; y0 <= kernelSize - 1; ++y0, rowOfROI += extendedImage.cols, rowOfKernel += kernelSize)
			{
				uchar *colOfROI = rowOfROI;
				float *colOfKernel = rowOfKernel;
				for (int x0 = 0; x0 <= kernelSize - 1; ++x0, ++colOfROI, ++colOfKernel)
				{
					sum += (colOfROI[0] * colOfKernel[0]);
				}
			}

			// 卷积结果赋值为结果图像,注意溢出的处理！
			colOfDst[0] = SATURATE((int)sum);
		}
	}


}// Convolution2

// 卷积运算,Kernel正方形卷积核且边长为奇数
void Convolution22(const Mat &srcImage, const Mat &kernel, Mat &dstImage)
{
	CV_Assert(srcImage.type() == CV_8UC1&&kernel.type() == CV_32FC1);

	// 目标图像大小
	int stride = 1;// 默认滑动窗口步长为1
	int kernelSize = kernel.rows;
	int widthOfDst = (srcImage.cols + kernelSize - 1 - kernelSize) / stride + 1;
	int heightOfDst = (srcImage.rows + kernelSize - 1 - kernelSize) / stride + 1;
	dstImage.create(heightOfDst, widthOfDst, CV_8UC1);

	// 扩充原图
	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, kernelSize / 2, kernelSize / 2, kernelSize / 2, kernelSize / 2, BORDER_DEFAULT);

	// 构建滑动窗口像素查找表,计算滑动窗口中每个像素在图像中相对于滑动窗口指针偏移
	std::vector<int> pixelOffset;
	pixelOffset.resize(kernelSize*kernelSize);
	memset(&pixelOffset[0], 0, pixelOffset.size()*sizeof(int));
	int index = 0;
	for (int y = 0; y < kernelSize; ++y)
	{
		int yOffset = y*extendedImage.cols;
		for (int x = 0; x < kernelSize; ++x)
		{
			// 计算每个像素的偏移
			pixelOffset[index++] = yOffset + x;
		}
	}

	// 计算卷积
	uchar *rowOfDst = dstImage.data;
	for (int y = 0; y <= heightOfDst - 1; ++y, rowOfDst += widthOfDst)
	{
		uchar *colOfDst = rowOfDst;

		// 滑动窗口(第一个元素)在y方向上的偏移
		int offsetOfY = y*stride;

		for (int x = 0; x <= widthOfDst - 1; ++x, ++colOfDst)
		{
			// 计算滑动窗口在图像中的偏移,并转化为指向该滑动窗口的指针
			int offsetOfX = x*stride;
			uchar *dataOfROI = extendedImage.data + offsetOfY*extendedImage.cols + offsetOfX;
			
			// 卷积核
			float *dataOfKernel = (float *)kernel.data;
			float sum = 0;
			for (int i = 0; i < pixelOffset.size(); ++i)
			{
				sum += (dataOfROI[pixelOffset[i]] * dataOfKernel[i]);
			}
			

			// 卷积结果赋值为结果图像,注意溢出的处理！
			colOfDst[0] = SATURATE((int)sum);
		}
	}
}// Convolution22



}//end of namespace QQ
