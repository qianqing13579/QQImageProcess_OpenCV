#define DLLAPI_EXPORTS

#include "Filter.h"
#include "Histogram.h"

namespace QQ
{

//���㴰������ƽ��ֵ
static int CalculateMeanValue(uchar *imageData, int pixelCount)
{
	int sum = 0;
	for (int i = 0; i <= pixelCount - 1; ++i)
	{
		sum += imageData[i];
	}
	return sum / pixelCount;
}

//����������ֵ�ͻҶ�<=��ֵ�����ص����
static void CalculateImage_MedianGray_PixelCount(const Mat &histogram, int pixelCount, int &medianValue, int &pixleCountLowerMedian)
{
	float  *data = (float *)histogram.data;//ֱ��ͼ
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

// ���ڱȽ�
static uchar GetMedianValue(const Mat& image)
{
	double time1 = getTickCount();
	////////////////////////��ֱ��ͼ////////////////////////////////////
	Mat histogram;//ֱ��ͼ
	int channels[] = { 0 };//��Ҫ�����ͨ��
	int histogramSize[] = { 256 };////ÿ��ֱ��ͼbin�ĸ���	

	//ÿ��ֱ��ͼ�Ҷȼ���Χ
	float range[] = { 0, 256 };//���ֱ��ͼ����Ҫ���ö������ ���ο� Learn_calcHist_minMaxLoc_Function_Demo3()   
	const float* histRange[] = { range };

	//����ֱ��ͼ
	calcHist(&image, //Mat ���飬����ֻ����һ��ͼ��
		1,//Mat�ĸ���
		channels,//ʹ����Щ��ͨ����ͼ��������Ŀ��ֱ��ͼ��ͨ������0,1,2...
		Mat(),//Mat()����һ����ֵ����ʾ����mask,
		histogram, //ֱ��ͼ
		1, //ֱ��ͼ��ά�����������2��ֱ��ͼ����Ϊ2
		histogramSize, //ÿһ��ֱ��ͼbin�ĸ���,Ҳ����ÿ�е�����
		histRange//�Ҷȼ��仯��Χ
		);
	double time2 = getTickCount();
	double elapse_ms = (time2 - time1)*1000. / getTickFrequency();
	std::cout << "OpenCV time:" << elapse_ms << std::endl;


	////////////////////////����ֵ////////////////////////////////////
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

//2015-1-1 20:23:53,by QQ
//ʹ��copyMakeBorder����ͼ��߽磬�����˲��߽�
void Blur(const Mat &srcImage, Mat &dstImage, Size size_Aperture)
{
	/////////////////////////////step 1.���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	//��ͼ��Ĵ�С
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	dstImage.create(Size(width_Dst,height_Dst),CV_8UC1);//������·��䣬֮ǰ�Ŀռ���ӵ�

	//////////////////////////////step 2.����һ����ͼ��(��Դͼ���������)////////////////////////////////////////////
	//����Ĵ�С
	int width_Extend=size_Aperture.width>>1;
	int height_Extend=size_Aperture.height>>1;

	//������ͼ��
	Mat image_New;
	copyMakeBorder(srcImage,image_New,height_Extend,height_Extend,width_Extend,width_Extend,BORDER_DEFAULT);


	////////////////////////////step 3.�˲�//////////////////////////////////////////////
	//��ͼ�����
	int width_New=image_New.cols;
	int height_New=image_New.rows;
	int widthStep_New=width_New;
	
	//��������
	int width_Aperture=size_Aperture.width;
	int height_Aperture=size_Aperture.height;
	int pixelCount=width_Aperture*height_Aperture;

	//������Ҫ�˲����ص��������
	int startX=width_Aperture>>1;
	int startY=height_Aperture>>1;

	//ÿ�еĻҶ�ֵ��
	int *sum_PerCol=new int[width_New];

	//����ͼ�����˲�����
	uchar *row_New=image_New.data+startY*widthStep_New+startX;
	uchar *row_Dst=dstImage.data;//��һ��
	uchar *row_Aperture_New=image_New.data;
	for (int y=startY;y<=height_New-startY-1;++y)
	{
		//��
		uchar *col_Dst=row_Dst;
		uchar *col_Aperture_New=row_Aperture_New;

		//����ÿ��height_Aperture�����صĻҶ�ֵ��
		//��һ�У����������еĺ�
		if (y==startY)
		{

			for (int k=0;k<=width_New-1;++k)
			{
				sum_PerCol[k]=0;
				//ÿ�е�һ��ָ��
				uchar *col_PerLine=col_Aperture_New+k;
				for (int t=0;t<=height_Aperture-1;++t)
				{
					sum_PerCol[k]+=col_PerLine[0];
					col_PerLine+=widthStep_New;//��һ��
				}

			}
		}
		else//�ǵ�һ��
		{
			for (int k=0;k<=width_New-1;++k)
			{
				//ÿ�е�һ��ָ��
				uchar *col_=col_Aperture_New+k;
				sum_PerCol[k]-=col_[0-widthStep_New];//������
				sum_PerCol[k]+=col_[0+(height_Aperture-1)*widthStep_New];//������
			}

		}

		//����width_Aperture�е����ܺ�
		int sum_Aperture=0;
		for (int x=startX;x<=width_New-startX-1;++x)
		{
			//ÿ�е�һ��Ԫ�أ���width_Aperture���к�
			if (x==startX)
			{
				for (int k=0;k<=width_Aperture-1;++k)
				{
					sum_Aperture+=sum_PerCol[k];
				}
			}
			else//�ǵ�һ��Ԫ��
			{
				//��ȥ���
				sum_Aperture-=sum_PerCol[x-startX-1];

				//�����ұ�
				sum_Aperture+=sum_PerCol[x+startX];
			}

			//���ֵ
			uchar meanValue=sum_Aperture/pixelCount;
			col_Dst[0]=meanValue;

			//����һ������
			col_Dst++;
			//col_New++;//���ﲻ��Ҫ��Դͼ��ָ����л���
		}

		//��һ��
		row_Dst+=width_Dst;
		row_Aperture_New+=width_New;
	}
}

//˼·����ԭͼ��������һ���������ڴ�С(��Ϊ��ɫ)�����ھ�ֵ�˲�����Ǻͣ������Ϳ��Խ��������صĲ�����ͳһ����
//ֻ�Ǳ߽�Ĵ��ڵ����ص������һ��
void Blur_3(const Mat &srcImage, Mat &dstImage, Size size_Aperture)
{
	/////////////////////////////step 1.���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	//��ͼ��Ĵ�С
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	dstImage.create(Size(width_Dst,height_Dst),CV_8UC1);//������·��䣬֮ǰ�Ŀռ���ӵ�

	//////////////////////////////step 2.����һ����ͼ��(��Դͼ���������)////////////////////////////////////////////
	//��ͼ���С
	int width_Extend=size_Aperture.width>>1;
	int height_Extend=size_Aperture.height>>1;
	int width_New=width_Dst+2*width_Extend;//������Ҹ�����width_Extend
	int height_New=height_Dst+2*height_Extend;//�߶����¸�����height_Extend

	//������ͼ��
	Mat image_New(Size(width_New,height_New),CV_8UC1,Scalar(0));
	
	//����Դͼ����ͼ��
	int widthStep_New=image_New.cols;//��ͼ��Ĳ���
	int widthStep_Src=srcImage.cols;//Դͼ��Ĳ���
	int width_Src=width_Dst;
	int height_Src=height_Dst;
	uchar *row_Src=srcImage.data;
	uchar *row_New=image_New.data+height_Extend*widthStep_New+width_Extend;
	for (int y=0;y<=height_Src-1;++y)//������
	{
		//��
		uchar *col_Src=row_Src;
		uchar *col_New=row_New;
		for (int x=0;x<=width_Src-1;++x)//������
		{
			//����ÿ������
			col_New[0]=col_Src[0];

			//��һ������
			col_Src++;
			col_New++;
		}

		//��һ��
		row_Src+=widthStep_Src;
		row_New+=widthStep_New;
	}

	////////////////////////////step 3.�˲�//////////////////////////////////////////////
	//��������
	int width_Aperture=size_Aperture.width;
	int height_Aperture=size_Aperture.height;

	//������Ҫ�˲����ص��������
	int startX=width_Aperture>>1;
	int startY=height_Aperture>>1;

	//ÿ�еĻҶ�ֵ��
	int *sum_PerCol=new int[width_New];
	
	//����ͼ�����˲�����
	row_New=image_New.data+startY*widthStep_New+startX;
	uchar *row_Dst=dstImage.data;//��һ��
	uchar *row_Aperture_New=image_New.data;
	for (int y=startY;y<=height_New-startY-1;++y)
	{
		//��
		//uchar *col_New=row_New;//���ﲻ��Ҫ��Դͼ��ָ����л���
		uchar *col_Dst=row_Dst;
		uchar *col_Aperture_New=row_Aperture_New;

		//����ÿ��height_Aperture�����صĻҶ�ֵ��
		//��һ�У����������еĺ�
		if (y==startY)
		{

			for (int k=0;k<=width_New-1;++k)
			{
				sum_PerCol[k]=0;
				//ÿ�е�һ��ָ��
				uchar *col_PerLine=col_Aperture_New+k;
				for (int t=0;t<=height_Aperture-1;++t)
				{
					sum_PerCol[k]+=col_PerLine[0];
					col_PerLine+=widthStep_New;//��һ��
				}

			}
		}
		else//�ǵ�һ��
		{
			for (int k=0;k<=width_New-1;++k)
			{
				//ÿ�е�һ��ָ��
				uchar *col_=col_Aperture_New+k;
				sum_PerCol[k]-=col_[0-widthStep_New];//������
				sum_PerCol[k]+=col_[0+(height_Aperture-1)*widthStep_New];//������
			}

		}

		//����width_Aperture�е����ܺ�
		int sum_Aperture=0;
		for (int x=startX;x<=width_New-startX-1;++x)
		{
			//ÿ�е�һ��Ԫ�أ���width_Aperture���к�
			if (x==startX)
			{
				for (int k=0;k<=width_Aperture-1;++k)
				{
					sum_Aperture+=sum_PerCol[k];
				}
			}
			else//�ǵ�һ��Ԫ��
			{
				//��ȥ���
				sum_Aperture-=sum_PerCol[x-startX-1];

				//�����ұ�
				sum_Aperture+=sum_PerCol[x+startX];
			}

			///////////////////////////////////�߽紦��:���㻬�������ڻҶ�ֵ��������ص����///////////////////////////////////////
			//�м����򣺴������������ص�
			int pixelCount=width_Aperture*height_Aperture;
			
			//�ϱ߽�:�������ص�
			if (y<=2*startY)
			{
				int w=(x+1)<width_Aperture?(x+1):width_Aperture;
				pixelCount=(y+1)*w;
			}
			
			//��߽�
			if(x<=2*startX)
			{
				int h=(y+1)<height_Aperture?(y+1):height_Aperture;
				pixelCount=(x+1)*h;
			}
			
			//�±߽�
			if (y>=height_New-2*startY)
			{
				int w=(x+1)<width_Aperture?(x+1):width_Aperture;
				pixelCount=(height_New-y)*w;
			}

			//�ұ߽�
			if(x>=width_New-2*startX)
			{
				int h=(y+1)<=height_Aperture?(y+1):height_Aperture;
				pixelCount=(width_New-x)*h;
				
				//�����½�(��Ⱥ͸߶ȶ��ڼ�С):������ر�ע�⣬��Ҫ���⴦��
				if (y>=height_New-2*startY)
				{
					int h=height_New-y;
					pixelCount=(width_New-x)*h;

				}
			}
			
			//���ֵ
			uchar meanValue=sum_Aperture/pixelCount;
			col_Dst[0]=meanValue;

			//����һ������
			col_Dst++;
			//col_New++;//���ﲻ��Ҫ��Դͼ��ָ����л���
		}

		//��һ��
		//row_New+=width_New;//���ﲻ��Ҫ��Դͼ��ָ����л���
		row_Dst+=width_Dst;
		row_Aperture_New+=width_New;
	}
}

//�����߽�û�д���
//��Ч��ֵ�˲�
void Blur_2(const Mat &srcImage, Mat &dstImage, Size size_Aperture)
{
	/////////////////////////////step 1.���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	//��ͼ��Ĵ�С
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	dstImage.create(Size(width_Dst,height_Dst),CV_8UC1);//������·��䣬֮ǰ�Ŀռ���ӵ�
	dstImage.setTo(Scalar(0));

	//��������
	int width_Aperture=size_Aperture.width;
	int height_Aperture=size_Aperture.height;
	int pixelCount=width_Aperture*height_Aperture;
	int *sum_PerCol=new int[width_Dst];//ÿ�еĻҶ�ֵ


	//�����������
	//���˲��ĵ�
	int startX=width_Aperture>>1;
	int startY=height_Aperture>>1;

	//��һ��
	//�����ؼ���ָ�룬������ָ�����һ��һ�𻬶�
	//1.2:��Ҫ�����������
	//3:�������ڵ�һ��Ԫ��,����������������
	uchar *row_Src=srcImage.data+startY*width_Dst+startX;
	uchar *row_Dst=dstImage.data+startY*width_Dst+startX;
	uchar *row_Aperture_Src=srcImage.data;
	for (int y=startY;y<=height_Dst-startY-1;++y)
	{
		//��
		uchar *col_Src=row_Src;
		uchar *col_Dst=row_Dst;
		uchar *col_Aperture_Src=row_Aperture_Src;
		
		//����ÿ��height_Aperture�����صĻҶ�ֵ��
		//��һ�У����������еĺ�
		if (y==startY)
		{
			
			for (int k=0;k<=width_Dst-1;++k)
			{
				sum_PerCol[k]=0;
				//ÿ�е�һ��ָ��
				uchar *col_PerLine=col_Aperture_Src+k;
				for (int t=0;t<=height_Aperture-1;++t)
				{
					sum_PerCol[k]+=col_PerLine[0];
					col_PerLine+=width_Dst;//��һ��
				}

			}
		}
		else//�ǵ�һ��
		{
			for (int k=0;k<=width_Dst-1;++k)
			{
				//ÿ�е�һ��ָ��
				uchar *col_=col_Aperture_Src+k;
				sum_PerCol[k]-=col_[0-width_Dst];//������
				sum_PerCol[k]+=col_[0+(height_Aperture-1)*width_Dst];//������
			}

		}
		
		//����width_Aperture�е����ܺ�
		int sum_Aperture=0;
		for (int x=startX;x<=width_Dst-startX-1;++x)
		{
			//ÿ�е�һ��Ԫ�أ���width_Aperture���к�
			if (x==startX)
			{
				for (int k=x-startX;k<=x+startX;++k)
				{
					sum_Aperture+=sum_PerCol[k];
				}
			}
			else//�ǵ�һ��Ԫ��
			{
				//��ȥ���
				sum_Aperture-=sum_PerCol[x-1-startX];
				
				//�����ұ�
				sum_Aperture+=sum_PerCol[x+startX];
			}
			
			//���ֵ
			uchar meanValue=sum_Aperture/pixelCount;
			col_Dst[0]=meanValue;

			//����һ������
			col_Dst++;
			col_Src++;
		}
		//��һ��
		row_Dst+=width_Dst;
		row_Src+=width_Dst;
		row_Aperture_Src+=width_Dst;
	}
}




//�߽�û����
//���ڸ߶ȺͿ�ȶ�����Ϊ������
//��ֵ�˲�
void Blur_1(const Mat &srcImage, Mat &dstImage,Size size_Aperture)
{
	/////////////////////////////step 1.���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	//��ͼ��Ĵ�С
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	dstImage.create(Size(width_Dst,height_Dst),CV_8UC1);//������·��䣬֮ǰ�Ŀռ���ӵ�
	dstImage.setTo(Scalar(0));

	//��������
	int width_Aperture=size_Aperture.width;
	int height_Aperture=size_Aperture.height;
	int pixelCount=width_Aperture*height_Aperture;//�����������ܸ���
	Mat image_Aperture(1,pixelCount,CV_8UC1);//��������ͼ��

	//�����������
	int startX=width_Aperture/2;
	int startY=height_Aperture/2;

	//��
	uchar *row_Src=srcImage.data+startY*width_Dst+startX;
	uchar *row_Dst=dstImage.data+startY*width_Dst+startX;
	for (int y=startY;y<=height_Dst-startY-1;++y)
	{
		//��
		uchar *col_Src=row_Src;
		uchar *col_Dst=row_Dst;
		for (int x=startX;x<=width_Dst-startX-1;++x)
		{
			//�����������Ͻǵ�
			uchar *row_LeftUp=col_Src-startY*width_Dst-startX;

			//��ȡ��������ͼ��
			for (int k=0;k<=height_Aperture-1;++k)
			{
				//��
				for (int w=0;w<=width_Aperture-1;++w)
				{
					image_Aperture.data[k*width_Aperture+w]=row_LeftUp[k*width_Dst+w];
				}
			}

			//���ֵ
			uchar meanValue=CalculateMeanValue(image_Aperture.data,pixelCount);
			col_Dst[0]=meanValue;

			//����һ������
			col_Dst++;
			col_Src++;
		}
		//��һ��
		row_Dst+=width_Dst;
		row_Src+=width_Dst;
	}
}



//2015-4-22 19:51:29,by QQ
//�Ľ��ĸ�Ч��ֵ�˲���û�в���OpenCV��ֱ��ͼ
void FastMedianBlur(const Mat &srcImage, Mat &dstImage, int width_Aperture)
{
	/////////////////////////////���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	int width_Dst = srcImage.cols;
	int height_Dst = srcImage.rows;
	dstImage.create(Size(width_Dst, height_Dst), CV_8UC1);//������·��䣬֮ǰ�Ŀռ���ӵ�
	dstImage.setTo(Scalar(0));//��Ϊ0

	//��������
	int pixelCount = width_Aperture*width_Aperture;//�����������ܸ���
	Mat image_Aperture(width_Aperture, width_Aperture, CV_8UC1);//��������ͼ��
	int thresholdValue = pixelCount / 2 + 1;//step1.������ֵ(����ο���ͼ��ĸ�Ч���Ҫ��֮��)

	//�����������
	int startX = width_Aperture / 2;
	int startY = width_Aperture / 2;

	///////////////////////////////����ͼ��߽�///////////////////////////////////////////
	int height_Extend = startY;
	int width_Extend = startX;
	Mat image_New;
	copyMakeBorder(srcImage, image_New, height_Extend, height_Extend, width_Extend, width_Extend, BORDER_DEFAULT);//Ĭ�ϲ���BORDER_REFLECT
	int height_New = image_New.rows;
	int width_New = image_New.cols;
	//��һ��
	//������Ҫ����3��ָ��,һ�𻬶�
	//1.Դͼ���б���������� 
	//2.Ŀ��ͼ�񱻴�������� 
	//3.Դͼ�񻬶�����
	uchar *row_New = image_New.data + startY*width_New + startX;//��ͼ��
	uchar *row_Dst = dstImage.data;//Ŀ��ͼ��
	uchar *row_Aperture_New = image_New.data;//Դͼ���еĻ�������

	for (int y = startY; y <= height_New - startY - 1; ++y)
	{
		//��
		uchar *col_New = row_New;
		uchar *col_Dst = row_Dst;
		uchar *col_Aperture_New = row_Aperture_New;//����������������

		///////////////////////////////�Ի������ڲ���///////////////////////////////////////////
		//����ÿ�е�һ����������ֱ��ͼ
		//��ȡ��������ͼ��
		uchar *row_Aperture = image_Aperture.data;//��������ͼ��
		uchar *row_Aperture_New_2 = col_Aperture_New;
		for (int k = 0; k <= width_Aperture - 1; ++k)
		{
			//��
			uchar *col_ApertureImage = row_Aperture;
			uchar *col_Aperture_New_2 = row_Aperture_New_2;

			for (int w = 0; w <= width_Aperture - 1; ++w)
			{
				//����ÿ������
				col_ApertureImage[0] = col_Aperture_New_2[0];

				//��һ������
				col_ApertureImage++;
				col_Aperture_New_2++;
			}

			//��һ��
			row_Aperture += width_Aperture;
			row_Aperture_New_2 += width_New;
		}

		//step 2.ȷ����ֵ������¼����<=��ֵ�����ص����
		//��ֱ��ͼ
		int histogram[256];
		GetHistogram(image_Aperture, histogram);

		//��������ֵ��<=��ֵ�����ص����
		int medianValue,pixelCountLowerMedian;
		GetMedianValue_AndPixelCountLowerMedian(image_Aperture, medianValue, pixelCountLowerMedian);
		//////////////////////////////�������ڲ�������////////////////////////////////////////////

		//�˲�
		col_Dst[0] = (uchar)medianValue;

		//����һ������(����ָ����һ���ƶ�)
		col_Dst++;
		col_New++;
		col_Aperture_New++;
		for (int x = startX + 1; x <= width_New - startX - 1; ++x)//��ÿ�еڶ����˲����ؿ�ʼ
		{
			//////////////////////////////////ά�ֻ�������ֱ��ͼ////////////////////////////////////////
			//step 3.ȥ�����
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

			//step 4.�����Ҳ�
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

			//�����µ���ֵ
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

			//�˲�
			col_Dst[0] = (uchar)medianValue;

			//����һ������
			col_New++;
			col_Dst++;
			col_Aperture_New++;
		}//end of x

		//��һ��
		row_New += width_New;
		row_Dst += width_Dst;
		row_Aperture_New += width_New;
	}//end of y

}

//�����߽�û�д���
//��ֵ�˲���ʹ�õ�ʱ��ע���޸�ֱ��ͼ���ͣ��޸�Ϊfloat
//���ڴ�СΪwidth_Aperture*width_Aperture��������
void FastMedianBlur_1(const Mat &srcImage, Mat &dstImage, int width_Aperture)
{
	/////////////////////////////���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	dstImage.create(Size(width_Dst,height_Dst),CV_8UC1);//������·��䣬֮ǰ�Ŀռ���ӵ�
	dstImage.setTo(Scalar(0));//��Ϊ0

	//��������
	int pixelCount=width_Aperture*width_Aperture;//�����������ܸ���
	Mat image_Aperture(width_Aperture,width_Aperture,CV_8UC1);//��������ͼ��

	//ֱ��ͼ
	Mat histogram;
	int histogramSize=256;//�Ҷȵȼ�
	int thresholdValue=pixelCount/2+1;//step1.������ֵ(����ο���ͼ��ĸ�Ч���Ҫ��֮��)

	//�����������
	int startX=width_Aperture/2;
	int startY=width_Aperture/2;

	//��һ��
	//������Ҫ����3��ָ��,һ�𻬶�
	//1.Դͼ���б���������� 
	//2.Ŀ��ͼ�񱻴�������� 
	//3.Դͼ�񻬶�����
	uchar *row_Src=srcImage.data+startY*width_Dst+startX;//Դͼ��
	uchar *row_Dst=dstImage.data+startY*width_Dst+startX;//Ŀ��ͼ��
	uchar *row_Aperture_Src=srcImage.data;//Դͼ���еĻ�������
	
	for (int y=startY;y<=height_Dst-startY-1;++y)
	{
		//��
		uchar *col_Src=row_Src;
		uchar *col_Dst=row_Dst;
		uchar *col_Aperture_Src=row_Aperture_Src;//����������������
		
		///////////////////////////////�Ի������ڲ���///////////////////////////////////////////
		//����ÿ�е�һ����������ֱ��ͼ
		//double time1 = getTickCount();
		Mat image_Aperture = srcImage(Rect(0, y - startY, width_Aperture, width_Aperture));
		//��ȡ��������ͼ��
		//uchar *row_Aperture=image_Aperture.data;//��������ͼ��
		//uchar *row_Aperture_Src_2=col_Aperture_Src;
		//for (int k=0;k<=width_Aperture-1;++k)
		//{
		//	//��
		//	uchar *col_ApertureImage=row_Aperture;
		//	uchar *col_Aperture_Src_2=row_Aperture_Src_2;
		//	
		//	for (int w=0;w<=width_Aperture-1;++w)
		//	{
		//		//����ÿ������
		//		col_ApertureImage[0]=col_Aperture_Src_2[0];
		//		
		//		//��һ������
		//		col_ApertureImage++;
		//		col_Aperture_Src_2++;
		//	}

		//	//��һ��
		//	row_Aperture+=width_Aperture;
		//	row_Aperture_Src_2+=width_Dst;
		//}

		//double time2 = getTickCount();
		//double elapse_ms = (time2 - time1)*1000. / getTickFrequency();
		//std::cout << "ROI time:" << elapse_ms << std::endl;


		//step 2.ȷ����ֵ������¼����<=��ֵ�����ص����
		//��ֱ��ͼ
		calcHist(&image_Aperture,
			1,//Mat�ĸ���
			0,//��������ֱ��ͼ��ͨ��������ͨ�����������ſ�
			Mat(),//Mat()����һ����ֵ����ʾ����mask,
			histogram, //ֱ��ͼ
			1, //ֱ��ͼ��ά�����������2��ֱ��ͼ����Ϊ2
			&histogramSize, //ֱ��ͼ�ĵȼ���(��Ҷȵȼ�),Ҳ����ÿ�е�����
			0//�����ı仯��Χ
			);
		
		//��������ֵ��<=��ֵ�����ص����
		int medianValue,pixleCountLowerMedian;
		CalculateImage_MedianGray_PixelCount(histogram,pixelCount,medianValue,pixleCountLowerMedian);
		//////////////////////////////�������ڲ�������////////////////////////////////////////////
		
		//�˲�
		col_Dst[0]=(uchar)medianValue;
		
		//����һ������(����ָ����һ���ƶ�)
		col_Dst++;
		col_Src++;
		col_Aperture_Src++;
		for (int x=startX+1;x<=width_Dst-startX-1;++x)//��ÿ�еڶ����˲����ؿ�ʼ
		{
			//////////////////////////////////ά�ֻ�������ֱ��ͼ////////////////////////////////////////
			//step 3.ȥ�����
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
			
			//step 4.�����Ҳ�
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

			//�����µ���ֵ
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

			//�˲�
			col_Dst[0]=(uchar)medianValue;

			//����һ������
			col_Src++;
			col_Dst++;
			col_Aperture_Src++;
		}//end of x

		//��һ��
		row_Src+=width_Dst;
		row_Dst+=width_Dst;
		row_Aperture_Src+=width_Dst;
	}//end of y

}



//�߽�û����
//���ڴ�СΪwidth_Aperture*width_Aperture
//���ڿ�ȣ�width_Aperture����Ϊ������Ϊ��
void MedianBlur(const Mat &srcImage, Mat &dstImage, int width_Aperture)
{

	/////////////////////////////step 1.���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	//��ͼ��Ĵ�С
	int width_Dst = srcImage.cols;
	int height_Dst = srcImage.rows;
	dstImage.create(Size(width_Dst, height_Dst), CV_8UC1);//������·��䣬֮ǰ�Ŀռ���ӵ�
	dstImage.setTo(Scalar(0));

	//��������
	int pixelCount = width_Aperture*width_Aperture;//�����������ܸ���
	Mat image_Aperture(width_Aperture, width_Aperture, CV_8UC1);//��������ͼ��


	//�����������
	int startX = width_Aperture / 2;
	int startY = width_Aperture / 2;

	//��
	//����Ҫ����3��ֵ��Դͼ��Ŀ��ͼ��������أ�Դͼ��Ļ�������ͼ��,�⼸��ֵ����Ҫ����һ���ƶ���
	uchar *row_Src = srcImage.data + startY*width_Dst + startX;
	uchar *row_Dst = dstImage.data + startY*width_Dst + startX;
	uchar *row_Aperture_Src = srcImage.data;//ԭͼ���еĻ�������
	for (int y = startY; y <= height_Dst - startY - 1; ++y)
	{
		//��
		uchar *col_Src = row_Src;
		uchar *col_Dst = row_Dst;
		uchar *col_Aperture_Src = row_Aperture_Src;//ÿ���������ڵ�һ��Ԫ��
		for (int x = startX; x <= width_Dst - startX - 1; ++x)
		{
			//////////////////////////////�Ի������ڲ���////////////////////////////////////////////
			//��ȡ��������ͼ��
			
			//����OpenCV��ȡ��Ч��̫��
			//3.0RC1�汾�Ѳ�֧�ָ÷�ʽ
			//Rect ROI(x - startX, y - startY, width_Aperture, width_Aperture);
			//Mat ROIImage = srcImage(ROI);
			//IplImage iplImage = ROIImage;
			//Mat image_Aperture(&iplImage, true);

			//uchar *row_ApertureImage = image_Aperture.data;
			//uchar *row_Aperture_Src_2 = col_Aperture_Src;
			//for (int k = 0; k <= width_Aperture - 1; ++k)//һ��m��
			//{
			//	//��
			//	uchar *col_ApertureImage = row_ApertureImage;
			//	uchar *col_Aperture_Src_2 = row_Aperture_Src_2;
			//	for (int w = 0; w <= width_Aperture - 1; ++w)
			//	{
			//		col_ApertureImage[0] = col_Aperture_Src_2[0];

			//		//��һ������
			//		col_Aperture_Src_2++;
			//		col_ApertureImage++;
			//	}
			//	//��һ��
			//	row_ApertureImage += width_Aperture;
			//	row_Aperture_Src_2 += width_Dst;
			//}

			//��������ֵ
			int medianValue;
			GetMedianValue(image_Aperture, medianValue);
			
			//��ȡOpenCV�����ֱ��ͼ��Ч�ʵ�
			//medianValue = GetMedianValue(image_Aperture);

			//�˲�
			col_Dst[0] = medianValue;

			//����һ������
			col_Dst++;
			col_Src++;
			col_Aperture_Src++;
		}

		//��һ��
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
// �������,Kernel�����ξ�����ұ߳�Ϊ����
void Convolution1(const Mat &srcImage, const Mat &kernel, Mat &dstImage)
{
	CV_Assert(srcImage.type() == CV_8UC1&&kernel.type()==CV_32FC1);

	// Ŀ��ͼ���С
	int stride = 1;// Ĭ�ϻ������ڲ���Ϊ1
	int kernelSize = kernel.rows;
	int widthOfDst = (srcImage.cols + kernelSize - 1 - kernelSize) / stride + 1;
	int heightOfDst = (srcImage.rows + kernelSize - 1 - kernelSize) / stride + 1;
	dstImage.create(heightOfDst, widthOfDst,CV_8UC1);

	// ����ԭͼ
	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, kernelSize / 2, kernelSize / 2, kernelSize / 2, kernelSize / 2, BORDER_DEFAULT);

	// ���ɻ������ھ���
	int numberOfElementOfKernel = kernelSize*kernelSize;
	int numberOfPixelsOfDst = widthOfDst*heightOfDst;
	Mat slidingWindowMat(numberOfElementOfKernel, numberOfPixelsOfDst, CV_8UC1);
	uchar *dataOfSlidingWindowMat = slidingWindowMat.data;
	uchar *dataOfExtendImage = extendedImage.data;

	// ��ȡ�����ÿ��Ԫ����ԭͼ�л�����ʱ���Ӧ���������أ��γɻ������ھ����е�һ�У��������ھ���Ҳ���Գ�Ϊpatch����
	/* ����˵ĸ�Ԫ����ͼ���л�����ʱ���Ӧ��ÿ�����ص�ƫ�������������(����ֻ���ǵ�ͨ��ͼ��)�� 
		1. ��Ԫ���ڻ��������ڵ�ƫ��(ÿ�α�����ʱ���ǹ̶���),ע������˻�����ʱ���Ϊ��������
		2. ����������ͼ���е�ƫ��(�������ڵ�һ��Ԫ����ͼ���е�ƫ��) 
	*/
	for (int c = 0; c <= numberOfElementOfKernel - 1; ++c)
	{
		// �����Ԫ���ڻ���������y�����Ϻ�x�����ϵ�ƫ��
		int offsetOfY_1 = (c / kernelSize) % kernelSize;
		int offsetOfX_1 = c % kernelSize;

		// ���㻬��������ͼ���е�ƫ��
		for (int y = 0; y <= heightOfDst - 1; ++y)
		{
			// ����������y�����ϵ�ƫ��(�������ڵ�һ��Ԫ����y�����ϵ�ƫ��)
			int offsetOfY_2 = y*stride;

			for (int x = 0; x <= widthOfDst - 1; ++x)
			{
				// ���������ڷ����ϵ�ƫ��(�������ڵ�һ��Ԫ����x�����ϵ�ƫ��)
				int offsetOfX_2 = x*stride;

				// ����ƫ��
				int offsetOfY = offsetOfY_1 + offsetOfY_2;
				int offsetOfX = offsetOfX_1 + offsetOfX_2;

				dataOfSlidingWindowMat[c*numberOfPixelsOfDst + y*widthOfDst + x] = dataOfExtendImage[offsetOfY*extendedImage.cols + offsetOfX];
			}
		}

	}

	// ������뻬�����ھ�����ˣ��ⲿ�ֿ���ʹ�þ��������Ż�(����Eigen)
	float *dataOfKernel = (float *)kernel.data;
	uchar *dataOfDst = dstImage.data;
	for (int x = 0; x <= slidingWindowMat.cols - 1; ++x)
	{
		float sum = 0; // ע�⣬���ﲻ��ʹ��int sum=0;ÿ�ζ����о�����ʧ
		for (int y = 0; y <= numberOfElementOfKernel - 1; ++y)
		{
			sum += (dataOfKernel[y] * slidingWindowMat.at<uchar>(y, x));
		}

		// ��������ֵΪ���ͼ��,ע������Ĵ���
		dataOfDst[x] = SATURATE((int)sum);
	}

} // Convolution1

// 2017-3-20 by QQ
void Convolution12(const Mat &srcImage, const Mat &kernel, Mat &dstImage)
{
	CV_Assert(srcImage.type() == CV_8UC1&&kernel.type() == CV_32FC1);

	// Ŀ��ͼ���С
	int stride = 1;// Ĭ�ϻ������ڲ���Ϊ1
	int kernelSize = kernel.rows;
	int widthOfDst = (srcImage.cols + kernelSize - 1 - kernelSize) / stride + 1;
	int heightOfDst = (srcImage.rows + kernelSize - 1 - kernelSize) / stride + 1;
	dstImage.create(heightOfDst, widthOfDst, CV_8UC1);

	// ����ԭͼ
	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, kernelSize / 2, kernelSize / 2, kernelSize / 2, kernelSize / 2, BORDER_DEFAULT);

	// ���ɻ������ھ���
	int numberOfElementOfKernel = kernelSize*kernelSize;
	int numberOfPixelsOfDst = widthOfDst*heightOfDst;
	Mat slidingWindowMat(numberOfElementOfKernel, numberOfPixelsOfDst, CV_8UC1);
	uchar *dataOfSlidingWindowMat = slidingWindowMat.data;
	uchar *dataOfExtendImage = extendedImage.data;

	///////////////////////////  �Ż����� ///////////////////////////
	// ��ȡ�����ÿ��Ԫ����ԭͼ�л�����ʱ���Ӧ���������أ��γɻ������ھ����е�һ�У��������ھ���Ҳ���Գ�Ϊpatch����
	uchar *rowOfSlidingWindowMat = dataOfSlidingWindowMat;// ָ�򻬶����ھ����һ��
	int stepOfExtendedImage = extendedImage.cols*stride;
	for (int c = 0; c <= numberOfElementOfKernel - 1; ++c, rowOfSlidingWindowMat += numberOfPixelsOfDst)
	{
		// �����Ԫ���ڻ���������y�����Ϻ�x�����ϵ�ƫ��
		int offsetOfY_1 = (c / kernelSize) % kernelSize;
		int offsetOfX_1 = c % kernelSize;
		
		// �������ڵ��е�ַ��������ѭ���ļ���
		uchar *colOfSlidingWindowMat = rowOfSlidingWindowMat;

		// data2OfExtendImageָ��ÿһ�λ������ڻ�����ʱ��ĳ�ʼλ�ã�Ҳ���ǵ�һ���������ڸ�Ԫ����ͼ���е�λ��
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

	// ������뻬�����ھ�����ˣ��ⲿ�ֿ���ʹ�þ��������Ż�(����Eigen)
	float *dataOfKernel = (float *)kernel.data;
	uchar *dataOfDst = dstImage.data;
	for (int x = 0; x <= slidingWindowMat.cols - 1; ++x)
	{
		float sum = 0; // ע�⣬���ﲻ��ʹ��int sum=0;ÿ�ζ����о�����ʧ
		for (int y = 0; y <= numberOfElementOfKernel - 1; ++y)
		{
			sum += (dataOfKernel[y] * slidingWindowMat.at<uchar>(y, x));
		}

		// ��������ֵΪ���ͼ��,ע������Ĵ���
		dataOfDst[x] = SATURATE((int)sum);
	}

}

// 2017-1-27 by QQ
// �������,Kernel�����ξ�����ұ߳�Ϊ����
void Convolution2(const Mat &srcImage, const Mat &kernel, Mat &dstImage)
{

	CV_Assert(srcImage.type() == CV_8UC1&&kernel.type() == CV_32FC1);

	// Ŀ��ͼ���С
	int stride = 1;// Ĭ�ϻ������ڲ���Ϊ1
	int kernelSize = kernel.rows;
	int widthOfDst = (srcImage.cols + kernelSize - 1 - kernelSize) / stride + 1;
	int heightOfDst = (srcImage.rows + kernelSize - 1 - kernelSize) / stride + 1;
	dstImage.create(heightOfDst, widthOfDst, CV_8UC1);

	// ����ԭͼ
	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, kernelSize / 2, kernelSize / 2, kernelSize / 2, kernelSize / 2, BORDER_DEFAULT);

	uchar *rowOfDst = dstImage.data;
	for (int y = 0; y <= heightOfDst - 1; ++y, rowOfDst += widthOfDst)
	{
		uchar *colOfDst = rowOfDst;

		// ��������(��һ��Ԫ��)��y�����ϵ�ƫ��
		int offsetOfY = y*stride;

		for (int x = 0; x <= widthOfDst - 1; ++x, ++colOfDst)
		{
			// ��������(��һ��Ԫ��)��x�����ϵ�ƫ��
			int offsetOfX = x*stride;

			// ������
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

			// ��������ֵΪ���ͼ��,ע������Ĵ���
			colOfDst[0] = SATURATE((int)sum);
		}
	}


}// Convolution2

// �������,Kernel�����ξ�����ұ߳�Ϊ����
void Convolution22(const Mat &srcImage, const Mat &kernel, Mat &dstImage)
{
	CV_Assert(srcImage.type() == CV_8UC1&&kernel.type() == CV_32FC1);

	// Ŀ��ͼ���С
	int stride = 1;// Ĭ�ϻ������ڲ���Ϊ1
	int kernelSize = kernel.rows;
	int widthOfDst = (srcImage.cols + kernelSize - 1 - kernelSize) / stride + 1;
	int heightOfDst = (srcImage.rows + kernelSize - 1 - kernelSize) / stride + 1;
	dstImage.create(heightOfDst, widthOfDst, CV_8UC1);

	// ����ԭͼ
	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, kernelSize / 2, kernelSize / 2, kernelSize / 2, kernelSize / 2, BORDER_DEFAULT);

	// ���������������ز��ұ�,���㻬��������ÿ��������ͼ��������ڻ�������ָ��ƫ��
	std::vector<int> pixelOffset;
	pixelOffset.resize(kernelSize*kernelSize);
	memset(&pixelOffset[0], 0, pixelOffset.size()*sizeof(int));
	int index = 0;
	for (int y = 0; y < kernelSize; ++y)
	{
		int yOffset = y*extendedImage.cols;
		for (int x = 0; x < kernelSize; ++x)
		{
			// ����ÿ�����ص�ƫ��
			pixelOffset[index++] = yOffset + x;
		}
	}

	// ������
	uchar *rowOfDst = dstImage.data;
	for (int y = 0; y <= heightOfDst - 1; ++y, rowOfDst += widthOfDst)
	{
		uchar *colOfDst = rowOfDst;

		// ��������(��һ��Ԫ��)��y�����ϵ�ƫ��
		int offsetOfY = y*stride;

		for (int x = 0; x <= widthOfDst - 1; ++x, ++colOfDst)
		{
			// ���㻬��������ͼ���е�ƫ��,��ת��Ϊָ��û������ڵ�ָ��
			int offsetOfX = x*stride;
			uchar *dataOfROI = extendedImage.data + offsetOfY*extendedImage.cols + offsetOfX;
			
			// �����
			float *dataOfKernel = (float *)kernel.data;
			float sum = 0;
			for (int i = 0; i < pixelOffset.size(); ++i)
			{
				sum += (dataOfROI[pixelOffset[i]] * dataOfKernel[i]);
			}
			

			// ��������ֵΪ���ͼ��,ע������Ĵ���
			colOfDst[0] = SATURATE((int)sum);
		}
	}
}// Convolution22



}//end of namespace QQ
