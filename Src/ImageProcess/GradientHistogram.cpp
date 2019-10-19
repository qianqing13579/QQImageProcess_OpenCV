#define DLLAPI_EXPORTS

#include"GradientHistogram.h"

namespace QQ
{
	// 2017-8-3 17:16:16,by QQ
	void ComputeGradient(const Mat &srcImage, Mat &gradient, Mat &gradientAngle, const int numberOfBins, bool signedGradient)
	{

		CV_Assert(srcImage.depth() == CV_8U);

		// 计算fx,fy
		Mat fx(srcImage.size(), CV_32SC1);
		Mat fy(srcImage.size(), CV_32SC1);

		gradient.create(srcImage.size(), CV_32FC1);
		gradientAngle.create(srcImage.size(), CV_32FC1);

		Mat enlargedImage;
		copyMakeBorder(srcImage, enlargedImage, 1, 1, 1, 1, cv::BORDER_REFLECT_101);

		float angleScale = signedGradient ?360 : 180;

		// 计算fx,fy
		int stepOfEnlargedImage = enlargedImage.cols*enlargedImage.channels();
		int stepOfFx = fx.cols;
		int numberOfChannels = enlargedImage.channels();
		uchar *rowOfEnlargedImage = enlargedImage.data + stepOfEnlargedImage + enlargedImage.channels();
		int *rowOfFx = (int *)fx.data;
		int *rowOfFy = (int *)fy.data;
		float *rowOfGradient = (float *)gradient.data;
		float *rowOfGradientAngle = (float *)gradientAngle.data;
		int height = srcImage.rows;
		int width = srcImage.cols;

		// debug
		//std::ofstream outFile1("gradient.txt", ios::out);

		for (int y = 0; y <= height - 1; ++y, rowOfEnlargedImage += stepOfEnlargedImage, rowOfFx += stepOfFx, rowOfFy += stepOfFx,rowOfGradient+=stepOfFx,rowOfGradientAngle+=stepOfFx)
		{
			uchar *colOfEnlargedImage = rowOfEnlargedImage;
			int *colOfFx = rowOfFx;
			int *colOfFy = rowOfFy;
			float *colOfGradient = rowOfGradient;
			float *colOfGradientAngle = rowOfGradientAngle;
			for (int x = 0; x <= width - 1; ++x, colOfEnlargedImage += numberOfChannels, ++colOfFx, ++colOfFy,++colOfGradient,++colOfGradientAngle)
			{
				if (numberOfChannels == 1)
				{
					// 使用Sobel模板
					// fx
					//colOfFx[0] = colOfEnlargedImage[1 - stepOfEnlargedImage] + 2 * colOfEnlargedImage[1] + colOfEnlargedImage[1 + stepOfEnlargedImage] -
					//	colOfEnlargedImage[-1 - stepOfEnlargedImage] - 2 * colOfEnlargedImage[-1] - colOfEnlargedImage[-1 + stepOfEnlargedImage];

					//// fy
					//colOfFy[0] = colOfEnlargedImage[stepOfEnlargedImage - 1] + 2 * colOfEnlargedImage[stepOfEnlargedImage] + colOfEnlargedImage[stepOfEnlargedImage + 1] -
					//	colOfEnlargedImage[-stepOfEnlargedImage - 1] - 2 * colOfEnlargedImage[-stepOfEnlargedImage] - colOfEnlargedImage[-stepOfEnlargedImage + 1];

					// 使用最简单的对称模板[-1,0,1](HOG中使用)
					colOfFx[0] = colOfEnlargedImage[1] - colOfEnlargedImage[-1];
					colOfFy[0] = colOfEnlargedImage[stepOfEnlargedImage] - colOfEnlargedImage[-stepOfEnlargedImage];

				}
				else
				{
					int maxGraident = -1;
					int maxFx = 0, maxFy = 0;
					for (int k = 0; k <= numberOfChannels - 1; ++k)
					{
						// 使用Sobel模板
						// fx
						//int fx0 = colOfEnlargedImage[numberOfChannels - stepOfEnlargedImage + k] + 2 * colOfEnlargedImage[numberOfChannels + k] + colOfEnlargedImage[numberOfChannels + stepOfEnlargedImage + k] -
						//	colOfEnlargedImage[-numberOfChannels - stepOfEnlargedImage + k] - 2 * colOfEnlargedImage[-numberOfChannels + k] - colOfEnlargedImage[-numberOfChannels + stepOfEnlargedImage + k];

						//// fy
						//int fy0 = colOfEnlargedImage[stepOfEnlargedImage - numberOfChannels + k] + 2 * colOfEnlargedImage[stepOfEnlargedImage + k] + colOfEnlargedImage[stepOfEnlargedImage + numberOfChannels + k] -
						//	colOfEnlargedImage[-stepOfEnlargedImage - numberOfChannels + k] - 2 * colOfEnlargedImage[-stepOfEnlargedImage + k] - colOfEnlargedImage[-stepOfEnlargedImage + numberOfChannels + k];

						// 使用最简单的对称模板[-1,0,1](HOG中使用)
						int fx0 = colOfEnlargedImage[numberOfChannels+k] - colOfEnlargedImage[-numberOfChannels+k];
						int fy0 = colOfEnlargedImage[stepOfEnlargedImage+k] - colOfEnlargedImage[-stepOfEnlargedImage+k];

						int gradient = fx0*fx0 + fy0*fy0;

						// 取梯度大小最大的那个通道
						if (gradient > maxGraident)
						{
							maxGraident = gradient;
							maxFx = fx0;
							maxFy = fy0;
						}
					}

					colOfFx[0] = maxFx;
					colOfFy[0] = maxFy;

					

				}

				// 计算梯度大小和梯度方向
				colOfGradient[0] = sqrt(colOfFx[0] * colOfFx[0] + colOfFy[0] * colOfFy[0]);

				// 是否有方向
				if (signedGradient)
				{
					// 转化为角度
					colOfGradientAngle[0] = cv::fastAtan2(colOfFy[0], colOfFx[0]);
				}
				else
				{
					// 转化为角度
					colOfGradientAngle[0] = cv::fastAtan2(colOfFy[0], colOfFx[0]) - 180.0*(colOfFy[0]<0);
				}

				// 转化为bin的序号
				colOfGradientAngle[0] = colOfGradientAngle[0] / angleScale * numberOfBins;
				if (colOfGradientAngle[0] >= numberOfBins)
				{
					colOfGradientAngle[0] -= numberOfBins;
				}
				
				// debug
				//outFile1 << colOfFx[0] << " " << colOfFy[0] << " " << colOfGradient[0] << " " << colOfGradientAngle[0] << endl;
			}
		}

		// debug
		//outFile1.close();
		
	}// ComputeGradient

	void ComputeGradient2(const Mat &srcImage, Mat &gradient, Mat &gradientAngle, const int numberOfBins, bool signedGradient)
	{

		CV_Assert(srcImage.depth() == CV_8U);

		// 计算fx,fy
		Mat fx(srcImage.size(), CV_32SC1);
		Mat fy(srcImage.size(), CV_32SC1);

		gradient.create(srcImage.size(), CV_32FC2);
		gradientAngle.create(srcImage.size(), CV_32FC2);

		Mat enlargedImage;
		copyMakeBorder(srcImage, enlargedImage, 1, 1, 1, 1, cv::BORDER_REFLECT_101);

		float angleScale = signedGradient ? (1.0*numberOfBins) / 360 : (1.0*numberOfBins)/180;

		// 计算fx,fy
		int stepOfEnlargedImage = enlargedImage.cols*enlargedImage.channels();
		int stepOfFx = fx.cols;
		int stepOfGradient = 2 * stepOfFx;
		int numberOfChannels = enlargedImage.channels();
		uchar *rowOfEnlargedImage = enlargedImage.data + stepOfEnlargedImage + enlargedImage.channels();
		int *rowOfFx = (int *)fx.data;
		int *rowOfFy = (int *)fy.data;
		float *rowOfGradient = (float *)gradient.data;
		float *rowOfGradientAngle = (float *)gradientAngle.data;
		int height = srcImage.rows;
		int width = srcImage.cols;

		// debug
		//std::ofstream outFile1("gradient.txt", ios::out);

		for (int y = 0; y <= height - 1; ++y, rowOfEnlargedImage += stepOfEnlargedImage, rowOfFx += stepOfFx, rowOfFy += stepOfFx, rowOfGradient += stepOfGradient, rowOfGradientAngle += stepOfGradient)
		{
			uchar *colOfEnlargedImage = rowOfEnlargedImage;
			int *colOfFx = rowOfFx;
			int *colOfFy = rowOfFy;
			float *colOfGradient = rowOfGradient;
			float *colOfGradientAngle = rowOfGradientAngle;
			for (int x = 0; x <= width - 1; ++x, colOfEnlargedImage += numberOfChannels, ++colOfFx, ++colOfFy, colOfGradient+=2, colOfGradientAngle+=2)
			{
				if (numberOfChannels == 1)
				{
					// 使用Sobel模板
					// fx
					//colOfFx[0] = colOfEnlargedImage[1 - stepOfEnlargedImage] + 2 * colOfEnlargedImage[1] + colOfEnlargedImage[1 + stepOfEnlargedImage] -
					//	colOfEnlargedImage[-1 - stepOfEnlargedImage] - 2 * colOfEnlargedImage[-1] - colOfEnlargedImage[-1 + stepOfEnlargedImage];

					//// fy
					//colOfFy[0] = colOfEnlargedImage[stepOfEnlargedImage - 1] + 2 * colOfEnlargedImage[stepOfEnlargedImage] + colOfEnlargedImage[stepOfEnlargedImage + 1] -
					//	colOfEnlargedImage[-stepOfEnlargedImage - 1] - 2 * colOfEnlargedImage[-stepOfEnlargedImage] - colOfEnlargedImage[-stepOfEnlargedImage + 1];

					// 使用最简单的对称模板[-1,0,1](HOG中使用)
					colOfFx[0] = colOfEnlargedImage[1] - colOfEnlargedImage[-1];
					colOfFy[0] = colOfEnlargedImage[stepOfEnlargedImage] - colOfEnlargedImage[-stepOfEnlargedImage];

				}
				else
				{
					int maxGraident = -1;
					int maxFx = 0, maxFy = 0;
					for (int k = 0; k <= numberOfChannels - 1; ++k)
					{
						// 使用Sobel模板
						// fx
						//int fx0 = colOfEnlargedImage[numberOfChannels - stepOfEnlargedImage + k] + 2 * colOfEnlargedImage[numberOfChannels + k] + colOfEnlargedImage[numberOfChannels + stepOfEnlargedImage + k] -
						//	colOfEnlargedImage[-numberOfChannels - stepOfEnlargedImage + k] - 2 * colOfEnlargedImage[-numberOfChannels + k] - colOfEnlargedImage[-numberOfChannels + stepOfEnlargedImage + k];

						//// fy
						//int fy0 = colOfEnlargedImage[stepOfEnlargedImage - numberOfChannels + k] + 2 * colOfEnlargedImage[stepOfEnlargedImage + k] + colOfEnlargedImage[stepOfEnlargedImage + numberOfChannels + k] -
						//	colOfEnlargedImage[-stepOfEnlargedImage - numberOfChannels + k] - 2 * colOfEnlargedImage[-stepOfEnlargedImage + k] - colOfEnlargedImage[-stepOfEnlargedImage + numberOfChannels + k];

						// 使用最简单的对称模板[-1,0,1](HOG中使用)
						int fx0 = colOfEnlargedImage[numberOfChannels + k] - colOfEnlargedImage[-numberOfChannels + k];
						int fy0 = colOfEnlargedImage[stepOfEnlargedImage + k] - colOfEnlargedImage[-stepOfEnlargedImage + k];

						int gradient = fx0*fx0 + fy0*fy0;

						// 取梯度大小最大的那个通道
						if (gradient > maxGraident)
						{
							maxGraident = gradient;
							maxFx = fx0;
							maxFy = fy0;
						}
					}

					colOfFx[0] = maxFx;
					colOfFy[0] = maxFy;

				}

				// 计算梯度大小和梯度方向
				float magnitude= sqrt(colOfFx[0] * colOfFx[0] + colOfFy[0] * colOfFy[0]);

				// 是否有方向
				float angle;
				if (signedGradient)
				{
					// 转化为角度
					angle = cv::fastAtan2(colOfFy[0], colOfFx[0]);
				}
				else
				{
					// 转化为角度
					angle = cv::fastAtan2(colOfFy[0], colOfFx[0]) - 180.0*(colOfFy[0]<0);
				}

				// 转化为bin的序号
				angle = angle*angleScale - 0.5;
				int indexOfFirstBin = cvFloor(angle);
				float weightOfSecondBin = angle - indexOfFirstBin;
				float weightOfFirstBin = (1 - weightOfSecondBin);
				colOfGradient[0] = magnitude*weightOfFirstBin;
				colOfGradient[1] = magnitude*weightOfSecondBin;
				if (indexOfFirstBin < 0)
				{
					indexOfFirstBin += numberOfBins;
				}
				if (indexOfFirstBin >= numberOfBins)
				{
					indexOfFirstBin -= numberOfBins;
				}
				colOfGradientAngle[0] = indexOfFirstBin;
				int indexOfSecondBin = indexOfFirstBin + 1;
				if (indexOfSecondBin >= numberOfBins)
				{
					indexOfSecondBin = 0;
				}
				colOfGradientAngle[1] = indexOfSecondBin;

				// debug
				//outFile1 << colOfFx[0] << " " << colOfFy[0] << " " << colOfGradient[0] << " " << colOfGradient[1] << " " << colOfGradientAngle[0] << " " << colOfGradientAngle[1] << endl;
			}
		}

		// debug
		//outFile1.close();

	}// ComputeGradient2


	void ComputeGradient_HOG(const Mat& img, Mat& grad, Mat& qangle, int nbins, bool signedGradient, bool gammaCorrection, Size paddingTL, Size paddingBR)
	{

		CV_Assert(img.type() == CV_8U || img.type() == CV_8UC3);

		Size gradsize(img.cols + paddingTL.width + paddingBR.width,
			img.rows + paddingTL.height + paddingBR.height);

		// 注意是双通道，需要记录相邻两个bin以及对应的权值
		grad.create(gradsize, CV_32FC2);  // <magnitude*(1-alpha), magnitude*alpha>
		qangle.create(gradsize, CV_8UC2); // [0..nbins-1] - quantized gradient orientation

		// 如果img是ROI，计算原图像的大小以及ROI的起点
		Size wholeSize;
		Point roiofs;
		img.locateROI(wholeSize, roiofs);

		int i, x, y;
		int cn = img.channels();

		Mat_<float> _lut(1, 256);
		const float* lut = &_lut(0, 0);

		if (gammaCorrection)
		for (i = 0; i < 256; i++)
			_lut(0, i) = std::sqrt((float)i);
		else
		for (i = 0; i < 256; i++)
			_lut(0, i) = (float)i;

		AutoBuffer<int> mapbuf(gradsize.width + gradsize.height + 4);
		int* xmap = (int*)mapbuf + 1;
		int* ymap = xmap + gradsize.width + 2;

		/*borderType为边界扩充后所填充像素点的方式
		Various border types, image boundaries are denoted with '|'

		* BORDER_REPLICATE:     aaaaaa|abcdefgh|hhhhhhh
		* BORDER_REFLECT:       fedcba|abcdefgh|hgfedcb
		* BORDER_REFLECT_101:   gfedcb|abcdefgh|gfedcba
		* BORDER_WRAP:          cdefgh|abcdefgh|abcdefg
		* BORDER_CONSTANT:      iiiiii|abcdefgh|iiiiiii  with some specified 'i'
		*/
		const int borderType = (int)BORDER_REFLECT_101;

		/*int borderInterpolate(int p, int len, int borderType)
		其中
		p：扩充图像的像素在原图中的坐标；
		len：对应原图像的一个坐标轴的长度；
		borderType：扩充类型
		比如，如果要计算扩充图像x方向每个像素的坐标对应的原图中的坐标，那么p就是扩充后的图像的每个像素在原图的坐标，而len就是原图的宽
		所以这个函数的作用是计算扩充图像的像素点在原图中的坐标
		如果img不是ROI,那么xmap和ymap就是原图的坐标
		*/
		for (x = -1; x < gradsize.width + 1; x++)
		{
			xmap[x] = borderInterpolate(x - paddingTL.width + roiofs.x,
				wholeSize.width, borderType) - roiofs.x;
			
			//printf("%d\n", xmap[x]);
		}

		//printf("\n");

		for (y = -1; y < gradsize.height + 1; y++)
		{
			ymap[y] = borderInterpolate(y - paddingTL.height + roiofs.y,
				wholeSize.height, borderType) - roiofs.y;
			
			// printf("%d\n", ymap[y]);

		}

		// x- & y- derivatives for the whole row
		int width = gradsize.width;
		AutoBuffer<float> _dbuf(width * 4);
		float* dbuf = _dbuf;

		// 每一行的Dx,Dy,Mag,Angle.注意，这四个对象内存是连续的
		Mat Dx(1, width, CV_32F, dbuf);
		Mat Dy(1, width, CV_32F, dbuf + width);
		Mat Mag(1, width, CV_32F, dbuf + width * 2);
		Mat Angle(1, width, CV_32F, dbuf + width * 3);

		int _nbins = nbins;

		// 最新的3.2源码,区分有符号还是无符号，这里默认为无符号
		// 有符号应该计算角度占据360的比例，而无符号应该计算角度占据180的比例
		float angleScale = signedGradient ? (float)(nbins/(2.0*CV_PI)) : (float)(nbins/CV_PI); 
		//float angleScale = (float)(_nbins / CV_PI);

		// debug
		//std::ofstream outFile11("gradient_HOG.txt", ios::out);

		// 计算每行，每个像素的梯度大小和方向，使用的模板是[-1,0,1](也是HOG论文中采用的模板)
		for (y = 0; y < gradsize.height; y++)
		{
			const uchar* imgPtr = img.data + img.step*ymap[y];
			const uchar* prevPtr = img.data + img.step*ymap[y - 1];
			const uchar* nextPtr = img.data + img.step*ymap[y + 1];

			float* gradPtr = (float*)grad.ptr(y);
			uchar* qanglePtr = (uchar*)qangle.ptr(y);

			if (cn == 1)
			{
				for (x = 0; x < width; x++)
				{
					int x1 = xmap[x];

					dbuf[x] = (float)(lut[imgPtr[xmap[x + 1]]] - lut[imgPtr[xmap[x - 1]]]);
					dbuf[width + x] = (float)(lut[nextPtr[x1]] - lut[prevPtr[x1]]);

				}
			}
			else
			{
				for (x = 0; x < width; x++)
				{
					int x1 = xmap[x] * 3;
					float dx0, dy0, dx, dy, mag0, mag;

					const uchar* p2 = imgPtr + xmap[x + 1] * 3;
					const uchar* p0 = imgPtr + xmap[x - 1] * 3;

					dx0 = lut[p2[2]] - lut[p0[2]];
					dy0 = lut[nextPtr[x1 + 2]] - lut[prevPtr[x1 + 2]];
					mag0 = dx0*dx0 + dy0*dy0;

					dx = lut[p2[1]] - lut[p0[1]];
					dy = lut[nextPtr[x1 + 1]] - lut[prevPtr[x1 + 1]];
					mag = dx*dx + dy*dy;

					if (mag0 < mag)
					{
						dx0 = dx;
						dy0 = dy;
						mag0 = mag;
					}

					dx = lut[p2[0]] - lut[p0[0]];
					dy = lut[nextPtr[x1]] - lut[prevPtr[x1]];
					mag = dx*dx + dy*dy;

					if (mag0 < mag)
					{
						dx0 = dx;
						dy0 = dy;
						mag0 = mag;
					}

					dbuf[x] = dx0;
					dbuf[x + width] = dy0;
				}
			}

			cartToPolar(Dx, Dy, Mag, Angle, false);

			for (x = 0; x < width; x++)
			{

				/* 首先计算梯度大小在梯度方向上的插值（梯度方向与相邻两个bin中心的距离）
				qangle记录该点对应的两个的bin,grad记录了该点对应的两个bin对应的权值(每个bin应该加多少权值)
				如何计算该像素梯度方向所在的两个bin和对应的权值呢?
				由于是与两个bin的中点比较，这里可以巧妙的运用一个技巧：
				首先将原始角度减去0.5，就可以得到第一个bin，此时该角度与第一个bin的距离就是第二个bin的权值，也就可以计算出对第一个bin的权值(1-x)
				同时我们也就得到了对应的bin，但是这里需要注意两个特殊情况：
				对于角度范围为[0,0.5]之间的值，第一个bin是个-1，说明第一个bin是最后一个bin
				对于角度范围为[8.5,9]之间的值，第二个bin越界了，说明第二个bin就是第一个bin

				为什么梯度方向要减去0.5呢？
				因为梯度方向是按照该点梯度方向与两个bin中点距离进行插值，将角度减去0.5
				为了将该角度移动到第一个bin中，且此时该角度与第一个bin的距离就是对第二个bin的权值

				计算示例：
				假设原始角度1.3,减去0.5后angle=0.8,hidx=0
				angle=0.8
				gradPtr[x*2]->(1-angle)=0.2
				gradPtr[x*2+1]->angle=0.8
				qanglePtr[x*2]->0
				qanglePtr[x*2+1]->1
				假设原始角度为1.8，减去0.5后angle=1.3,hidx=1
				angle=0.3
				gradPtr[x*2]->(1-angle)=0.7
				gradPtr[x*2+1]->angle=0.3
				qanglePtr[x*2]->1
				qanglePtr[x*2+1]->2
				!假设原始角度为0.3，减去0.5后angle=-0.2,hidx=-1
				angle=0.8
				gradPtr[x*2]->(1-angle)=0.2
				gradPtr[x*2+1]->angle=0.8
				qanglePtr[x*2]->8
				qanglePtr[x*2+1]->0
				!假设原始角度为8.8，减去0.5后angle=8.3,hidx=8
				angle=0.3
				gradPtr[x*2]->(1-angle)=0.7
				gradPtr[x*2+1]->angle=0.3
				qanglePtr[x*2]->8
				qanglePtr[x*2+1]->0
				对于0.5,1.5这样的临界值，都只对一个bin有贡献，另一个bin贡献度为0

				这里要注意：
				如果是无符号角度，计算angle = dbuf[x + width * 3] * angleScale - 0.5f,如果角度大于180，会超过nbin,所以后面需要
				if (hidx >= _nbins)
					hidx -= _nbins
				如果想要angle计算出来就是真实的值，应该angle = (dbuf[x + width * 3] - CV_PI*(dbuf[x + width]<0))* angleScale - 0.5f
				*/
				float mag = dbuf[x + width * 2], angle = dbuf[x + width * 3] * angleScale - 0.5f;
				int hidx = cvFloor(angle); // cvFloor()返回不大于参数的最大整数
				angle -= hidx;
				gradPtr[x * 2] = mag*(1.f - angle);
				gradPtr[x * 2 + 1] = mag*angle;

				// 对于角度范围为[0,0.5]之间的值，该像素对应的梯度方向的第一个bin就是最后一个bin
				// 对于角度范围为[8.5,9]之间的值，该像素对应的梯度方向的第二个bin就是第一个bin
				// 如果hindex为-1，hidx设置为8
				if (hidx < 0)
					hidx += _nbins; // 8
				else if (hidx >= _nbins)
					hidx -= _nbins;
				assert((unsigned)hidx < (unsigned)_nbins);

				qanglePtr[x * 2] = (uchar)hidx; // 8
				hidx++; // 9

				//
				//-1在补码中的表示为11111111,与-1相与的话就是自己本身了
				// 0在补码中的表示为00000000,与0相与的结果就是0了
				hidx &= hidx < _nbins ? -1 : 0; // 0
				qanglePtr[x * 2 + 1] = (uchar)hidx; // 0

				//// debug
				// dbuf[x + width * 3]是实际的角度(弧度)
				// 对于无符号角度(dbuf[x + width * 3] - CV_PI*(dbuf[x + width]<0))* angleScale 才是实际的bin的序号
				// 对于有符号角度 (dbuf[x + width * 3])* angleScale
				//outFile11 << (float)dbuf[x] << " " << (float)dbuf[x + width] << " " << (float)gradPtr[x * 2] << " " << (float)gradPtr[x * 2 + 1] << " " << (int)qanglePtr[x * 2] << " " << (int)qanglePtr[x * 2+1] << endl;
				
			}
		}

		// debug
		//outFile11.close();

	}// ComputeGradient2


}// namespace QQ