#define DLLAPI_EXPORTS
#include "ProjectXY.h"
#include "IntegralImage.h"
#include <time.h>
#include <iostream>
using namespace std;
namespace QQ
{

#define RESULT	"D:/Image/Result/"

//2015-3-4 16:36:18,by QQ
//垂直投影(向X轴方向投影)
//height_ProjectYImage:垂直投影图的高度
void ProjectX(const Mat &srcImage,int height_ProjectXImage)
{

	////////////////////////初始化Y方向投影数组//////////////////////////////////////////////////
	int width_Src=srcImage.cols;
	int height_Src=srcImage.rows;

	int *projectY=new int[width_Src];
	memset(projectY,0,sizeof(int)*width_Src);

	double time1=getTickCount();
#ifndef INTEGRALIMAGE
	/////////////////////////投影,遍历图像的方式(比积分图方式效率高)/////////////////////////////////////////////////
	int widthStep=width_Src*srcImage.channels();
	//第一行
	uchar *row=srcImage.data;
	for (int y=0;y<=height_Src-1;++y)
	{
		//列
		uchar *col=row;
		for (int x=0;x<=width_Src-1;++x)
		{
			//列
			//统计投影数组
			projectY[x]+=col[0];

			//下一个像素
			col++;

		}
		//下一行
		row+=widthStep;

	}

#else
	////////////////////////投影,使用积分图//////////////////////////////////////////////////
	Mat image_Sum;
	CalculateIntegralImage(srcImage,image_Sum);
	for (int x=0;x<=width_Src-1;++x)
	{
		if (x==0)
		{
			projectY[x]=image_Sum.at<int>(image_Sum.rows-1,0);
			continue;
		}
		int s2=image_Sum.at<int>(image_Sum.rows-1,x);
		int s1=image_Sum.at<int>(image_Sum.rows-1,x-1);
		projectY[x]=s2-s1;
	}
#endif
	double time2=getTickCount();
	
	//计算时间函数，比clock()更加准确
	double executeTime_ms=(time2-time1)*1000./getTickFrequency();//单位毫秒
	cout<<"time:"<<executeTime_ms<<endl;


	////////////////////////画垂直方向投影图//////////////////////////////////////////////////
	Mat image_ProjectY;
	DrawProjectYImage(image_ProjectY,height_ProjectXImage,projectY,width_Src);
	
	
	////////////////////////拼接并保存//////////////////////////////////////////////////
	Mat image_Connect;
	ConnectTwoImage_UpDown(srcImage,image_ProjectY,image_Connect);
	imwrite(string(RESULT)+"ProjectY.bmp",image_Connect);
}

//2015-3-4 16:27:21,by QQ
//水平投影(按行向Y轴方向投影)
//width_ProjectXImage:投影图的宽度
void ProjectY(const Mat &srcImage,int width_ProjectYImage)
{

	/////////////////////////初始化X方向投影数组/////////////////////////////////////////////////
	int width_Src=srcImage.cols;
	int height_Src=srcImage.rows;
	int *projectX=new int[height_Src];
	memset(projectX,0,sizeof(int)*height_Src);

	double time1=getTickCount();

#ifndef INTEGRALIMAGE
	/////////////////////////投影,遍历图像的方式/////////////////////////////////////////////////
	int widthStep=width_Src*srcImage.channels();

	//第一行
	uchar *row=srcImage.data;
	for (int y=0;y<=height_Src-1;++y)
	{
		//列
		uchar *col=row;
		for (int x=0;x<=width_Src-1;++x)
		{
			//统计投影数组
			projectX[y]+=col[0];

			//下一个像素
			col++;
		}
		//下一行
		row+=widthStep;
	}
#else

	/////////////////////投影,使用积分图/////////////////////////////////////////////////////
	Mat image_Sum(srcImage.size(),CV_32SC1,Scalar(0));
	CalculateIntegralImage(srcImage,image_Sum);
	for (int y=0;y<=height_Src-1;++y)
	{
		if (y==0)
		{
			projectX[y]=image_Sum.at<int>(y,image_Sum.cols-1);
			continue;
		}
		int s2=image_Sum.at<int>(y,image_Sum.cols-1);
		int s1=image_Sum.at<int>(y-1,image_Sum.cols-1);
		projectX[y]=s2-s1;
	}
#endif
	double time2=getTickCount();
	double executeTime_ms=(time2-time1)*1000./getTickFrequency();//单位毫秒
	cout<<"time:"<<executeTime_ms<<endl;


	////////////////////////画投影图//////////////////////////////////////////////////
	Mat image_ProjectX;
	DrawProjectXImage(image_ProjectX,width_ProjectYImage,projectX,height_Src);


	////////////////////////拼接并保存//////////////////////////////////////////////
	Mat image_Connect;
	ConnectTwoImage_LeftRight(srcImage,image_ProjectX,image_Connect);
	imwrite(string(RESULT)+"ProjectX.bmp",image_Connect);

}
//2015-3-4 17:46:55,by QQ
//画垂直投影图
//height_ProjectYImage:垂直投影图高度
void DrawProjectXImage(Mat &image_ProjectX,int height_ProjectXImage,int *projectX,int projectXSize)
{
	/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
	//新图像的大小
	int width_Dst=projectXSize;
	int height_Dst=height_ProjectXImage;
	image_ProjectX.create(Size(width_Dst,height_Dst),CV_8UC1);//如果重新分配，之前的空间会扔掉
	image_ProjectX.setTo(Scalar(0));
	

	/////////////////////////////step 2.创建投影数组Mat/////////////////////////////////////////////
	Mat mat_ProjectY(1,projectXSize,CV_32SC1,Scalar(0));
	
	//将投影数组projectY赋值给mat_ProjectY
	int *data=(int*)mat_ProjectY.data;
	for (int i=0;i<=projectXSize-1;++i)
	{
		data[0]=projectX[i];
		
		//下一个元素
		data++;
	}


	///////////////////////////////step 2.归一化///////////////////////////////////////////
	normalize(mat_ProjectY,mat_ProjectY,0,height_ProjectXImage,NORM_MINMAX,-1);//


	//////////////////////////step 3.开始画图////////////////////////////////////////////////
	int channelCount=image_ProjectX.step1(1);//每个元素通道数，移动列指针
	int widthStep=image_ProjectX.step1(0);//每行通道数，移动行指针
	
	//最后一行
	uchar *row_Project=image_ProjectX.data+(image_ProjectX.rows-1)*widthStep;
	
	//投影数组第一个元素
	int *row_MatProject=(int *)mat_ProjectY.data;
	for(int y=height_Dst-1;y>=0;--y)
	{
		//列
		uchar *col_Project=row_Project;
		int *col_MatProject=row_MatProject;
		for (int x=0;x<=width_Dst-1;++x)
		{
			
			//没有白色像素点,直接跳过
			if (col_MatProject[x]!=0)
			{
				//如果投影数组不为零，就画点
				col_Project[0]=255;
				col_MatProject[x]--;
			}
			//下一个像素
			col_Project++;

		}
		//上一行
		row_Project-=widthStep;

	}
	
}
//2015-3-4 17:47:06,by QQ
//画水平投影图
//
void DrawProjectYImage(Mat &image_ProjectY,int width_ProjectYImage,int *projectY,int projectYSize)
{
	/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
	//新图像的大小
	int width_Dst = width_ProjectYImage;
	int height_Dst = projectYSize;
	image_ProjectY.create(Size(width_Dst, height_Dst), CV_8UC1);//如果重新分配，之前的空间会扔掉

	// OpenCV4.0之后不支持IplImage
	// IplImage iplImage = image_ProjectY;
	// cvSetZero(&iplImage);


	/////////////////////////////step 2.创建投影数组Mat/////////////////////////////////////////////
	//Mat mat_ProjectX(projectXSize,1,CV_32SC1,Scalar(0));
	Mat mat_ProjectX(1, projectYSize, CV_32SC1, Scalar(0));

	//将投影数组projectX赋值给mat_ProjectX
	int *data=(int*)mat_ProjectX.data;
	for (int i = 0; i <= projectYSize - 1; ++i)
	{
		data[i] = projectY[i];
	}

	///////////////////////////////step 2.归一化///////////////////////////////////////////
	normalize(mat_ProjectX, mat_ProjectX, 0, width_ProjectYImage, 32, -1);//

	//////////////////////////step 3.开始画图////////////////////////////////////////////////
	//每个元素通道数，移动列指针
	int channelCount = image_ProjectY.channels();
	
	//每行通道数，移动行指针
	int widthStep=channelCount*width_Dst;
	
	//第一行
	uchar *row_Project = image_ProjectY.data;
	int *data_MatProjectY=(int *)mat_ProjectX.data;
	
	//按行遍历
	for(int y=0;y<=height_Dst-1;++y)
	{
		//如果该行为0，直接下一行
		if (data_MatProjectY[y]!=0)
		{
			//最后一列
			uchar *col_ProjectY=row_Project+widthStep-1;
			for (int x=width_Dst-1;x>=0;--x)
			{
				if (data_MatProjectY[y]!=0)
				{
					col_ProjectY[0]=255;
					data_MatProjectY[y]--;
					//下一个像素
					col_ProjectY--;
				}
				else
					break;
			}
		}
		//下一行
		row_Project+=widthStep;
	}
	

}


//将两幅宽度相同的图片，上下拼接
//顺序遍历
void ConnectTwoImage_UpDown(const Mat &srcImage1,const Mat &srcImage2,Mat &image_Dst)
{
	/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
	//新图像的大小
	int width_Dst=srcImage1.cols;
	int height_Dst=srcImage1.rows+srcImage2.rows;
	image_Dst.create(Size(width_Dst,height_Dst),srcImage1.type());//如果重新分配，之前的空间会扔掉


	//////////////////////////////step 2.拷贝图像1////////////////////////////////////////////
	//参数
	//像素点个数
	int pixelCount_Src1=srcImage1.rows*srcImage1.cols;
	
	//每个元素通道数，移动列指针
	int channelCount_Src1=srcImage1.step1(1);
	int channelCount_Dst=image_Dst.step1(1);
	
	//第一个像素
	uchar *imageData_Src1=srcImage1.data;
	uchar *imageData_Dst=image_Dst.data;
	for (int i=0;i<=pixelCount_Src1-1;++i)
	{
		//处理每个像素点
		for (int k=0;k<=channelCount_Src1-1;++k)
		{
			imageData_Dst[k]=imageData_Src1[k];

		}

		//下一个像素点
		imageData_Src1+=channelCount_Src1;
		imageData_Dst+=channelCount_Dst;
	}

	//////////////////////////////step 3.拷贝图像2////////////////////////////////////////////
	//参数
	//像素点个数
	int pixelCount_Src2=srcImage2.rows*srcImage2.cols;
	
	//每个元素的通道数，移动列指针
	int channelCount_Src2=srcImage2.step1(1);
	
	//第一个像素
	uchar *imageData_Src2=srcImage2.data;
	uchar *imageData_Dst2=image_Dst.data+pixelCount_Src1*channelCount_Src1;
	for (int i=0;i<=pixelCount_Src2-1;++i)
	{
		//处理每个像素点
		for (int k=0;k<=channelCount_Src2-1;++k)
		{
			imageData_Dst2[k]=imageData_Src2[k];

		}

		//下一个像素点
		imageData_Src2+=channelCount_Src2;
		imageData_Dst2+=channelCount_Dst;
	}
}
//不能顺序遍历
void ConnectTwoImage_LeftRight(const Mat &srcImage1,const Mat &srcImage2,Mat &image_Dst)//横向拼接两幅图像
{
	/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
	//新图像的大小
	int width_Dst=srcImage1.cols+srcImage2.cols;
	int height_Dst=srcImage1.rows;
	image_Dst.create(Size(width_Dst,height_Dst),srcImage1.type());//如果重新分配，之前的空间会扔掉
	

	/////////////////////////////step 2.拷贝图片1/////////////////////////////////////////////
	//参数
	//每行通道数，移动行指针
	int widthStep_Src=srcImage1.step1(0);
	int widthStep_Dst=image_Dst.step1(0);
	
	//每个元素通道数，移动列指针
	int channelCount_Src=srcImage1.step1(1);
	int channelCount_Dst=image_Dst.step1(1);
	
	//第一行
	uchar *row_Src1=srcImage1.data;
	uchar *row_Dst=image_Dst.data;
	for (int y=0;y<=srcImage1.rows-1;++y)
	{
		//列
		uchar *col_Src1=row_Src1;
		uchar *col_Dst=row_Dst;
		for (int x=0;x<=srcImage1.cols-1;++x)
		{
			
			//遍历每个通道
			for (int k=0;k<=channelCount_Src-1;++k)
			{
				col_Dst[k]=col_Src1[k];

			}
			//下一个像素
			col_Src1+=channelCount_Src;
			col_Dst+=channelCount_Dst;

		}

		//下一行
		row_Src1+=widthStep_Src;
		row_Dst+=widthStep_Dst;
	}


	/////////////////////////////step 3.拷贝图片2/////////////////////////////////////////////
	
	//参数
	//每行通道数，移动行指针
	int widthStep_Src2=srcImage2.step1(0);
	
	//每个元素通道数，移动列指针
	int channelCount_Src2=srcImage2.step1(1);
	
	//第一行
	uchar *row_Src2=srcImage2.data;
	uchar *row_Dst2=image_Dst.data+widthStep_Src;
	for (int y=0;y<=srcImage2.rows-1;++y)
	{
		//列
		uchar *col_Src2=row_Src2;
		uchar *col_Dst=row_Dst2;
		for (int x=0;x<=srcImage2.cols-1;++x)
		{

			//遍历每个通道
			for (int k=0;k<=channelCount_Src2-1;++k)
			{
				col_Dst[k]=col_Src2[k];

			}
			//下一个像素
			col_Src2+=channelCount_Src2;
			col_Dst+=channelCount_Dst;

		}

		//下一行
		row_Src2+=widthStep_Src2;
		row_Dst2+=widthStep_Dst;
	}
}

}//namespace QQ



