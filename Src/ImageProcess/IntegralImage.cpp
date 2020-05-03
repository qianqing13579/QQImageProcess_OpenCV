#define DLLAPI_EXPORTS
#include "IntegralImage.h"
#include <time.h>

namespace QQ
{


//2015-3-4 16:53:57,by QQ
//�Ľ��ļ������ػҶ�ֵ����ͼ��
//srcImage:�Ҷ�ͼ
//image_Integral��srcImage��С��ͬ��CV_32SC1����
//������Integral(y,x) = Integral(y-1,x) + rowSum(y);
void CalculateIntegralImage(const Mat &srcImage,Mat &image_Integral)
{
	/////////////////////////////step 1.���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	//��ͼ��Ĵ�С
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	image_Integral.create(Size(width_Dst,height_Dst),CV_32SC1);//������·��䣬֮ǰ�Ŀռ���ӵ�
	image_Integral.setTo(Scalar(0));

	/////////////////////////////step 2.�������ͼ/////////////////////////////////////////////
	int width_Src=srcImage.cols;
	int height_Src=srcImage.rows;
	
	//ÿ�����ص�ͨ����
	int channelCount_Src=srcImage.channels();
	int channelCount_Integral=image_Integral.channels();//ÿ�����ص�ͨ����
	
	//�е�ͨ����
	int widthStep_Src=channelCount_Src*width_Src;
	int widthStep_Integral=channelCount_Integral*width_Src;
	
	//��һ��
	uchar *row_Src=srcImage.data;
	int *row_Integral=(int *)image_Integral.data;//ע��ָ���ת��
	for (int y=0;y<=height_Src-1;++y)
	{
		int sum=0;//��ǰ�е��ۼӺ�
		//��
		uchar *col_Src=row_Src;
		int *col_Integral=row_Integral;
		for (int x=0;x<=width_Src-1;++x)
		{
			//���е��ۼ�
			sum+=col_Src[0];

			//�����0��,��һ�е�������
			if (y==0)
			{
				col_Integral[0]=sum;
			}
			else
			{
				//�ǵ�0��
				//��ǰ���ۼӺ�+ͬ�е���һ��Ԫ�ص�ֵ
				col_Integral[0]=sum+col_Integral[0-widthStep_Integral];//�±�
				//col_Integral[0]=sum+*(col_Integral-image_Integral.cols);//ָ���ƶ�

			}

			//��һ������
			col_Src++;
			col_Integral++;

		}
		//��һ��
		row_Src+=widthStep_Src;
		row_Integral+=widthStep_Integral;
	}
	//��ʱ������
	//printf("2:%d\n",image_Integral.at<int>(srcImage.rows-1,srcImage.cols-1));
	//printf("2:%d\n",*((int *)image_Integral.data+image_Integral.cols*image_Integral.rows*image_Integral.channels()-1));

}


//�������ػҶ�ֵ����ͼ��
//srcImage:�Ҷ�ͼ
//image_Integral��srcImage��С��ͬ��CV_32SC1����
//������Integral(y,x) = Integral(y,x-1) + Integral(y-1,x) - Integral(y-1,x-1) + Image(y,x);
void CalculateIntegralImage_Old(const Mat &srcImage,Mat &image_Integral)
{
	/////////////////////////////step 1.���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	//��ͼ��Ĵ�С
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	image_Integral.create(Size(width_Dst,height_Dst),CV_32SC1);//������·��䣬֮ǰ�Ŀռ���ӵ�
	
	// OpenCV4.0֮��֧��IplImage
	// IplImage iplImage=image_Integral;
	// cvSetZero(&iplImage);

	/////////////////////////////step 2.�������ͼ/////////////////////////////////////////////
	for (int y=0;y<=srcImage.rows-1;++y)
	{
		for (int x=0;x<=srcImage.cols-1;++x)
		{
			//��0��,��������
			if (y==0)
			{
				//����ߺ��±�
				if (x!=0)
				{
					//ֻ�����
					image_Integral.at<int>(y,x)+=image_Integral.at<int>(y,x-1);//ʹ�þ����������ʽ�����ʸ���

				}
				//��Image(x,y)
				image_Integral.at<int>(y,x)+=srcImage.at<uchar>(y,x);

			}
			else
			{
				//��0��
				if (x==0)
				{
					//����ߺ��±�
					image_Integral.at<int>(y,x)+=image_Integral.at<int>(y-1,x);//ֻ���±�

					//��Image(x,y)
					image_Integral.at<int>(y,x)+=srcImage.at<uchar>(y,x);
				}
				else
				{
					//�ǵ�0�У��ǵ�0��
					//������ߺ��±�
					image_Integral.at<int>(y,x)+=image_Integral.at<int>(y-1,x);//�±�
					image_Integral.at<int>(y,x)+=image_Integral.at<int>(y,x-1);//���

					//��ȥ�ظ��ģ�
					image_Integral.at<int>(y,x)-=image_Integral.at<int>(y-1,x-1);

					//��Image(x,y)
					image_Integral.at<int>(y,x)+=srcImage.at<uchar>(y,x);

				}

			}

		}
	}
	//printf("old:%d\n",image_Integral.at<int>(srcImage.rows-1,srcImage.cols-1));

}


//�Ľ��ļ������ػҶ�ֵ����ͼ��
//srcImage:�Ҷ�ͼ
//image_Integral��srcImage��С��ͬ��CV_32SC1����
//������Integral(y,x) = Integral(y-1,x) + rowSum(y);
void CalculateIntegralImage_2(const Mat &srcImage,Mat &image_Integral)
{
	/////////////////////////////step 1.���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	//��ͼ��Ĵ�С
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	image_Integral.create(Size(width_Dst,height_Dst),CV_32SC1);//������·��䣬֮ǰ�Ŀռ���ӵ�
	
	// OpenCV4.0֮��֧��IplImage
	// IplImage iplImage=image_Integral;
	// cvSetZero(&iplImage);

	/////////////////////////////step 2.�������ͼ/////////////////////////////////////////////
	for (int y=0;y<=srcImage.rows-1;++y)
	{
		int sum=0;//��ǰ�е��ۼӺ�
		uchar *row_Src=srcImage.data+y*srcImage.step[0];
		int *row_Integral=(int *)(image_Integral.data+y*image_Integral.step[0]);
		for (int x=0;x<=srcImage.cols-1;++x)
		{
			//��
			uchar *col_Src=row_Src+x*srcImage.channels();//�������÷ǳ���Ч
			int *col_Integral=row_Integral+x*image_Integral.channels();
			//���е��ۼ�
			sum+=col_Src[0];

			//�����0��,��һ�е�������
			if (y==0)
			{
				col_Integral[0]=sum;
			}
			else
			{
				//�ǵ�0��
				//��ǰ���ۼӺ�+ͬ�е���һ��Ԫ�ص�ֵ
				col_Integral[0]=sum+col_Integral[0-image_Integral.cols];//�±�
				//col_Integral[0]=sum+*(col_Integral-image_Integral.cols);//ָ���ƶ�

			}

		}
	}
	//printf("2:%d\n",image_Integral.at<int>(srcImage.rows-1,srcImage.cols-1));

}


//�Ľ��ļ������ػҶ�ֵ����ͼ��
//srcImage:�Ҷ�ͼ
//image_Integral��srcImage��С��ͬ��CV_32SC1����
//������Integral(y,x) = Integral(y-1,x) + rowSum(y);
void CalculateIntegralImage_1(const Mat &srcImage,Mat &image_Integral)
{
	/////////////////////////////step 1.���·���ͼ��(�����Ҫ)/////////////////////////////////////////////
	//��ͼ��Ĵ�С
	int width_Dst=srcImage.cols;
	int height_Dst=srcImage.rows;
	image_Integral.create(Size(width_Dst,height_Dst),CV_32SC1);//������·��䣬֮ǰ�Ŀռ���ӵ�
	
	// OpenCV4.0֮��֧��IplImage
	// IplImage iplImage=image_Integral;
	// cvSetZero(&iplImage);

	/////////////////////////////step 2.�������ͼ/////////////////////////////////////////////
	for (int y=0;y<=srcImage.rows-1;++y)
	{
		int sum=0;//��ǰ�е��ۼӺ�
		for (int x=0;x<=srcImage.cols-1;++x)
		{
			//���е��ۼ�
			sum+=srcImage.at<uchar>(y,x);

			//�����0��,��һ�е�������
			if (y==0)
			{
				image_Integral.at<int>(y,x)=sum;
			}
			else
			{
				//�ǵ�0��
				//��ǰ���ۼӺ�+ͬ�е���һ��Ԫ�ص�ֵ
				image_Integral.at<int>(y,x)=sum+image_Integral.at<int>(y-1,x);
			}

		}
	}
	//printf("2:%d\n",image_Integral.at<int>(srcImage.rows-1,srcImage.cols-1));

}



}//namespace QQ