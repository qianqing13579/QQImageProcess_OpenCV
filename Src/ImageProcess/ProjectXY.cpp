#define DLLAPI_EXPORTS
#include "ProjectXY.h"
#include "IntegralImage.h"
#include <time.h>
#include <iostream>
using namespace std;
namespace QQ
{

#define RESULT	"D:/Image/Projects/WuHu/Result/"

//2015-3-4 16:36:18,by QQ
//��ֱͶӰ(��X�᷽��ͶӰ)
//height_ProjectYImage:��ֱͶӰͼ�ĸ߶�
void ProjectX(const Mat &srcImage,int height_ProjectXImage)
{

	////////////////////////��ʼ��Y����ͶӰ����//////////////////////////////////////////////////
	int width_Src=srcImage.cols;
	int height_Src=srcImage.rows;

	int *projectY=new int[width_Src];
	memset(projectY,0,sizeof(int)*width_Src);

	double time1=getTickCount();
#ifndef INTEGRALIMAGE
	/////////////////////////ͶӰ,����ͼ��ķ�ʽ(�Ȼ���ͼ��ʽЧ�ʸ�)/////////////////////////////////////////////////
	int widthStep=width_Src*srcImage.channels();
	//��һ��
	uchar *row=srcImage.data;
	for (int y=0;y<=height_Src-1;++y)
	{
		//��
		uchar *col=row;
		for (int x=0;x<=width_Src-1;++x)
		{
			//��
			//ͳ��ͶӰ����
			projectY[x]+=col[0];

			//��һ������
			col++;

		}
		//��һ��
		row+=widthStep;

	}

#else
	////////////////////////ͶӰ,ʹ�û���ͼ//////////////////////////////////////////////////
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
	
	//����ʱ�亯������clock()����׼ȷ
	double executeTime_ms=(time2-time1)*1000./getTickFrequency();//��λ����
	cout<<"time:"<<executeTime_ms<<endl;


	////////////////////////����ֱ����ͶӰͼ//////////////////////////////////////////////////
	Mat image_ProjectY;
	DrawProjectYImage(image_ProjectY,height_ProjectXImage,projectY,width_Src);
	
	
	////////////////////////ƴ�Ӳ�����//////////////////////////////////////////////////
	Mat image_Connect;
	ConnectTwoImage_UpDown(srcImage,image_ProjectY,image_Connect);
	imwrite(string(RESULT)+"ProjectY.bmp",image_Connect);
}

//2015-3-4 16:27:21,by QQ
//ˮƽͶӰ(������Y�᷽��ͶӰ)
//width_ProjectXImage:ͶӰͼ�Ŀ��
void ProjectY(const Mat &srcImage,int width_ProjectYImage)
{

	/////////////////////////��ʼ��X����ͶӰ����/////////////////////////////////////////////////
	int width_Src=srcImage.cols;
	int height_Src=srcImage.rows;
	int *projectX=new int[height_Src];
	memset(projectX,0,sizeof(int)*height_Src);

	double time1=getTickCount();

#ifndef INTEGRALIMAGE
	/////////////////////////ͶӰ,����ͼ��ķ�ʽ/////////////////////////////////////////////////
	int widthStep=width_Src*srcImage.channels();

	//��һ��
	uchar *row=srcImage.data;
	for (int y=0;y<=height_Src-1;++y)
	{
		//��
		uchar *col=row;
		for (int x=0;x<=width_Src-1;++x)
		{
			//ͳ��ͶӰ����
			projectX[y]+=col[0];

			//��һ������
			col++;
		}
		//��һ��
		row+=widthStep;
	}
#else

	/////////////////////ͶӰ,ʹ�û���ͼ/////////////////////////////////////////////////////
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
	double executeTime_ms=(time2-time1)*1000./getTickFrequency();//��λ����
	cout<<"time:"<<executeTime_ms<<endl;


	////////////////////////��ͶӰͼ//////////////////////////////////////////////////
	Mat image_ProjectX;
	DrawProjectXImage(image_ProjectX,width_ProjectYImage,projectX,height_Src);


	////////////////////////ƴ�Ӳ�����//////////////////////////////////////////////
	Mat image_Connect;
	ConnectTwoImage_LeftRight(srcImage,image_ProjectX,image_Connect);
	imwrite(string(RESULT)+"ProjectX.bmp",image_Connect);

}
//2015-3-4 17:46:55,by QQ
//����ֱͶӰͼ
//height_ProjectYImage:��ֱͶӰͼ�߶�
void DrawProjectXImage(Mat &image_ProjectX,int height_ProjectXImage,int *projectX,int projectXSize)
{
	/////////////////////////////step 1.���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	//��ͼ��Ĵ�С
	int width_Dst=projectXSize;
	int height_Dst=height_ProjectXImage;
	image_ProjectX.create(Size(width_Dst,height_Dst),CV_8UC1);//������·��䣬֮ǰ�Ŀռ���ӵ�
	image_ProjectX.setTo(Scalar(0));
	

	/////////////////////////////step 2.����ͶӰ����Mat/////////////////////////////////////////////
	Mat mat_ProjectY(1,projectXSize,CV_32SC1,Scalar(0));
	
	//��ͶӰ����projectY��ֵ��mat_ProjectY
	int *data=(int*)mat_ProjectY.data;
	for (int i=0;i<=projectXSize-1;++i)
	{
		data[0]=projectX[i];
		
		//��һ��Ԫ��
		data++;
	}


	///////////////////////////////step 2.��һ��///////////////////////////////////////////
	normalize(mat_ProjectY,mat_ProjectY,0,height_ProjectXImage,NORM_MINMAX,-1);//


	//////////////////////////step 3.��ʼ��ͼ////////////////////////////////////////////////
	int channelCount=image_ProjectX.step1(1);//ÿ��Ԫ��ͨ�������ƶ���ָ��
	int widthStep=image_ProjectX.step1(0);//ÿ��ͨ�������ƶ���ָ��
	
	//���һ��
	uchar *row_Project=image_ProjectX.data+(image_ProjectX.rows-1)*widthStep;
	
	//ͶӰ�����һ��Ԫ��
	int *row_MatProject=(int *)mat_ProjectY.data;
	for(int y=height_Dst-1;y>=0;--y)
	{
		//��
		uchar *col_Project=row_Project;
		int *col_MatProject=row_MatProject;
		for (int x=0;x<=width_Dst-1;++x)
		{
			
			//û�а�ɫ���ص�,ֱ������
			if (col_MatProject[x]!=0)
			{
				//���ͶӰ���鲻Ϊ�㣬�ͻ���
				col_Project[0]=255;
				col_MatProject[x]--;
			}
			//��һ������
			col_Project++;

		}
		//��һ��
		row_Project-=widthStep;

	}
	
}
//2015-3-4 17:47:06,by QQ
//��ˮƽͶӰͼ
//
void DrawProjectYImage(Mat &image_ProjectY,int width_ProjectYImage,int *projectY,int projectYSize)
{
	/////////////////////////////step 1.���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	//��ͼ��Ĵ�С
	int width_Dst = width_ProjectYImage;
	int height_Dst = projectYSize;
	image_ProjectY.create(Size(width_Dst, height_Dst), CV_8UC1);//������·��䣬֮ǰ�Ŀռ���ӵ�

	// OpenCV4.0֮��֧��IplImage
	// IplImage iplImage = image_ProjectY;
	// cvSetZero(&iplImage);


	/////////////////////////////step 2.����ͶӰ����Mat/////////////////////////////////////////////
	//Mat mat_ProjectX(projectXSize,1,CV_32SC1,Scalar(0));
	Mat mat_ProjectX(1, projectYSize, CV_32SC1, Scalar(0));

	//��ͶӰ����projectX��ֵ��mat_ProjectX
	int *data=(int*)mat_ProjectX.data;
	for (int i = 0; i <= projectYSize - 1; ++i)
	{
		data[i] = projectY[i];
	}

	///////////////////////////////step 2.��һ��///////////////////////////////////////////
	normalize(mat_ProjectX, mat_ProjectX, 0, width_ProjectYImage, 32, -1);//

	//////////////////////////step 3.��ʼ��ͼ////////////////////////////////////////////////
	//ÿ��Ԫ��ͨ�������ƶ���ָ��
	int channelCount = image_ProjectY.channels();
	
	//ÿ��ͨ�������ƶ���ָ��
	int widthStep=channelCount*width_Dst;
	
	//��һ��
	uchar *row_Project = image_ProjectY.data;
	int *data_MatProjectY=(int *)mat_ProjectX.data;
	
	//���б���
	for(int y=0;y<=height_Dst-1;++y)
	{
		//�������Ϊ0��ֱ����һ��
		if (data_MatProjectY[y]!=0)
		{
			//���һ��
			uchar *col_ProjectY=row_Project+widthStep-1;
			for (int x=width_Dst-1;x>=0;--x)
			{
				if (data_MatProjectY[y]!=0)
				{
					col_ProjectY[0]=255;
					data_MatProjectY[y]--;
					//��һ������
					col_ProjectY--;
				}
				else
					break;
			}
		}
		//��һ��
		row_Project+=widthStep;
	}
	

}


//�����������ͬ��ͼƬ������ƴ��
//˳�����
void ConnectTwoImage_UpDown(const Mat &srcImage1,const Mat &srcImage2,Mat &image_Dst)
{
	/////////////////////////////step 1.���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	//��ͼ��Ĵ�С
	int width_Dst=srcImage1.cols;
	int height_Dst=srcImage1.rows+srcImage2.rows;
	image_Dst.create(Size(width_Dst,height_Dst),srcImage1.type());//������·��䣬֮ǰ�Ŀռ���ӵ�


	//////////////////////////////step 2.����ͼ��1////////////////////////////////////////////
	//����
	//���ص����
	int pixelCount_Src1=srcImage1.rows*srcImage1.cols;
	
	//ÿ��Ԫ��ͨ�������ƶ���ָ��
	int channelCount_Src1=srcImage1.step1(1);
	int channelCount_Dst=image_Dst.step1(1);
	
	//��һ������
	uchar *imageData_Src1=srcImage1.data;
	uchar *imageData_Dst=image_Dst.data;
	for (int i=0;i<=pixelCount_Src1-1;++i)
	{
		//����ÿ�����ص�
		for (int k=0;k<=channelCount_Src1-1;++k)
		{
			imageData_Dst[k]=imageData_Src1[k];

		}

		//��һ�����ص�
		imageData_Src1+=channelCount_Src1;
		imageData_Dst+=channelCount_Dst;
	}

	//////////////////////////////step 3.����ͼ��2////////////////////////////////////////////
	//����
	//���ص����
	int pixelCount_Src2=srcImage2.rows*srcImage2.cols;
	
	//ÿ��Ԫ�ص�ͨ�������ƶ���ָ��
	int channelCount_Src2=srcImage2.step1(1);
	
	//��һ������
	uchar *imageData_Src2=srcImage2.data;
	uchar *imageData_Dst2=image_Dst.data+pixelCount_Src1*channelCount_Src1;
	for (int i=0;i<=pixelCount_Src2-1;++i)
	{
		//����ÿ�����ص�
		for (int k=0;k<=channelCount_Src2-1;++k)
		{
			imageData_Dst2[k]=imageData_Src2[k];

		}

		//��һ�����ص�
		imageData_Src2+=channelCount_Src2;
		imageData_Dst2+=channelCount_Dst;
	}
}
//����˳�����
void ConnectTwoImage_LeftRight(const Mat &srcImage1,const Mat &srcImage2,Mat &image_Dst)//����ƴ������ͼ��
{
	/////////////////////////////step 1.���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	//��ͼ��Ĵ�С
	int width_Dst=srcImage1.cols+srcImage2.cols;
	int height_Dst=srcImage1.rows;
	image_Dst.create(Size(width_Dst,height_Dst),srcImage1.type());//������·��䣬֮ǰ�Ŀռ���ӵ�
	

	/////////////////////////////step 2.����ͼƬ1/////////////////////////////////////////////
	//����
	//ÿ��ͨ�������ƶ���ָ��
	int widthStep_Src=srcImage1.step1(0);
	int widthStep_Dst=image_Dst.step1(0);
	
	//ÿ��Ԫ��ͨ�������ƶ���ָ��
	int channelCount_Src=srcImage1.step1(1);
	int channelCount_Dst=image_Dst.step1(1);
	
	//��һ��
	uchar *row_Src1=srcImage1.data;
	uchar *row_Dst=image_Dst.data;
	for (int y=0;y<=srcImage1.rows-1;++y)
	{
		//��
		uchar *col_Src1=row_Src1;
		uchar *col_Dst=row_Dst;
		for (int x=0;x<=srcImage1.cols-1;++x)
		{
			
			//����ÿ��ͨ��
			for (int k=0;k<=channelCount_Src-1;++k)
			{
				col_Dst[k]=col_Src1[k];

			}
			//��һ������
			col_Src1+=channelCount_Src;
			col_Dst+=channelCount_Dst;

		}

		//��һ��
		row_Src1+=widthStep_Src;
		row_Dst+=widthStep_Dst;
	}


	/////////////////////////////step 3.����ͼƬ2/////////////////////////////////////////////
	
	//����
	//ÿ��ͨ�������ƶ���ָ��
	int widthStep_Src2=srcImage2.step1(0);
	
	//ÿ��Ԫ��ͨ�������ƶ���ָ��
	int channelCount_Src2=srcImage2.step1(1);
	
	//��һ��
	uchar *row_Src2=srcImage2.data;
	uchar *row_Dst2=image_Dst.data+widthStep_Src;
	for (int y=0;y<=srcImage2.rows-1;++y)
	{
		//��
		uchar *col_Src2=row_Src2;
		uchar *col_Dst=row_Dst2;
		for (int x=0;x<=srcImage2.cols-1;++x)
		{

			//����ÿ��ͨ��
			for (int k=0;k<=channelCount_Src2-1;++k)
			{
				col_Dst[k]=col_Src2[k];

			}
			//��һ������
			col_Src2+=channelCount_Src2;
			col_Dst+=channelCount_Dst;

		}

		//��һ��
		row_Src2+=widthStep_Src2;
		row_Dst2+=widthStep_Dst;
	}
}

}//namespace QQ



