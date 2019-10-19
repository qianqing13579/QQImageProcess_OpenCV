#define DLLAPI_EXPORTS
#include "Edge.h"

namespace QQ
{

	//边界处理
	void SetImageBoundary(Mat &srcImage, int color)
	{

		//处理第一个行和最后一行
		uchar *firstRow = srcImage.data;
		uchar *lastRow = srcImage.data + (srcImage.rows - 1)*srcImage.cols;
		int width_Src = srcImage.cols;
		int height_Src = srcImage.rows;
		for (int x = 0; x <= width_Src - 1; ++x)
		{
			firstRow[0] = color;
			lastRow[0] = color;

			//下一个像素
			firstRow++;
			lastRow++;
		}

		//处理第一列和最后一列
		uchar *firstCol = srcImage.data;
		uchar *lastCol = srcImage.data + width_Src - 1;

		for (int y = 0; y <= height_Src - 1; ++y)
		{
			firstCol[0] = color;
			lastCol[0] = color;

			//下一行
			firstCol += width_Src;
			lastCol += width_Src;
		}
	}
	void Robert(const Mat &srcImage, Mat &image_Robert)//梯度罗伯特
	{
		/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
		int width_Robert = srcImage.cols;
		int height_Robert = srcImage.rows;
		image_Robert.create(Size(width_Robert, height_Robert), CV_8UC1);
		image_Robert.setTo(Scalar(0));//处理边界，边界置为0

									  //////////////////////////////step 2.计算Robert梯度图////////////////////////////////////////////
		int width_Src = srcImage.cols;
		int height_Src = srcImage.rows;
		uchar *row_Src = srcImage.data + width_Src + 1;
		uchar *row_Robert = image_Robert.data + width_Robert + 1;

		for (int y = 1; y <= height_Src - 2; ++y)
		{
			//列
			uchar *col_Src = row_Src;
			uchar *col_Robert = row_Robert;
			for (int x = 1; x <= width_Src - 2; ++x)
			{
				//计算Robert算子
				int fx = col_Src[0] - col_Src[0 + width_Src + 1];
				int fy = col_Src[1] - col_Src[0 + width_Src];
				col_Robert[0] = max(abs(fx), abs(fy));
				//下一个像素
				col_Robert++;
				col_Src++;

			}

			//下一行
			row_Src += width_Src;
			row_Robert += width_Robert;
		}
	}
	void Sobel(const Mat &srcImage, Mat &image_Sobel)//梯度索伯
	{
		/////////////////////////////step 1.重新分配图像(如果需要)/////////////////////////////////////////////
		int width_Sobel = srcImage.cols;
		int height_Sobel = srcImage.rows;
		image_Sobel.create(Size(width_Sobel, height_Sobel), CV_8UC1);
		image_Sobel.setTo(Scalar(0));//处理边界，边界置为0

									 //////////////////////////////step 2.计算Sobel梯度图////////////////////////////////////////////
		int width_Src = srcImage.cols;
		int height_Src = srcImage.rows;
		int widthStep_Src = width_Src;
		int widthStep_Sobel = width_Sobel;
		uchar *row_Src = srcImage.data + width_Src + 1;
		uchar *row_Sobel = image_Sobel.data + width_Sobel + 1;

		for (int y = 1; y <= height_Src - 2; ++y)
		{
			//列
			uchar *col_Src = row_Src;
			uchar *col_Sobel = row_Sobel;
			for (int x = 1; x <= width_Src - 2; ++x)
			{
				//计算Sobel算子
				int fx = col_Src[0 - 1 - width_Src] + 2 * col_Src[0 - 1] + col_Src[0 - 1 + width_Src] -
					col_Src[0 + 1 - width_Src] - 2 * col_Src[0 + 1] - col_Src[0 + 1 + width_Src];

				int fy = col_Src[0 - width_Src - 1] + 2 * col_Src[0 - width_Src] + col_Src[0 - width_Src + 1] -
					col_Src[0 + width_Src - 1] - 2 * col_Src[0 + width_Src] - col_Src[0 + width_Src + 1];

				//采用|fx|+|fy|计算梯度幅度
				col_Sobel[0] = min(255, abs(fx) + abs(fy));

				//下一个像素
				col_Sobel++;
				col_Src++;

			}

			//下一行
			row_Src += widthStep_Src;
			row_Sobel += widthStep_Sobel;
		}

	}
	void ShenJun(const Mat &srcImage, Mat &ShenJunImage)
	{
		////////////////////////////////重新分配空间(如果需要)//////////////////////////////////////////
		ShenJunImage.create(Size(srcImage.cols, srcImage.rows), srcImage.type());
		ShenJunImage.setTo(Scalar(0));


		//通过修改alpha，调整边缘检测的精度
		Mat temp(srcImage.size(), srcImage.type());
		RmwShenJunEdge(srcImage.data, temp.data, srcImage.cols, srcImage.rows, 0.1, ShenJunImage.data);
	}

	void RmwShenJunEdge(BYTE *pGryImg, BYTE *pTmpImg, int width, int height, double alpha, BYTE *pResImg)
	{
		int i, x, y, j, pointer, size;
		double gLUT[512], *LUT;
		BYTE *cur, *pre, *pBuf1, *pBuf2, BLUT[256];

		alpha = min(0.99, alpha);
		alpha = max(0.01, alpha);
		// 1. step.1---------------------Shen Filter Image--------------------------------------//
		// init LUT
		for (i = 0; i<512; i++) gLUT[i] = alpha*(i - 256);//构造查找表
		LUT = gLUT + 256;
		// copy image
		memcpy(pResImg, pGryImg, width*height);
		memcpy(pTmpImg, pGryImg, width*height);
		// pass One : p1(i,j)=p1(i,j-1)+a*[p(i,j)-p1(i,j-1)];
		for (y = 0, pointer = y*width; y<height; y++, pointer += width)
			for (x = 1, cur = pTmpImg + pointer + x, pre = cur - 1; x<width; x++, cur++, pre++)
				*cur = (BYTE)(*pre + LUT[*cur - *pre]);
		// pass two : p2(i,j)=p2(i,j+1)-a*[p1(i,j)-p2(i,j+1)];
		for (y = 0, pointer = y*width; y<height; y++, pointer += width)
			for (x = width - 2, cur = pTmpImg + pointer + x, pre = cur + 1; x >= 0; x--, cur--, pre--)
				*cur = (BYTE)(*pre + LUT[*cur - *pre]);
		// passs three: p3(i,j)=p3(i-1,j)+a*[p2(i,j)-p3(i-1,j)];
		for (x = 0; x<width; x++)
			for (y = 1, cur = pTmpImg + y*width + x, pre = cur - width; y<height; y++, cur += width, pre += width)
				*cur = (BYTE)(*pre + LUT[*cur - *pre]);
		// pass four: p4(i,j)=p4(i+1,j)+a*[p3(i,j)-p4(i+1,j)];
		for (x = 0; x<width; x++)
			for (y = height - 2, cur = pTmpImg + y*width + x, pre = cur + width; y >= 0; y--, cur -= width, pre -= width)
				*cur = (BYTE)(*pre + LUT[*cur - *pre]);
		// 2. step.2---------------------Cross Zero points--------------------------------------//
		pBuf1 = pResImg; // orignal image
		pBuf2 = pTmpImg; // shen image
		size = width*height;
		for (pointer = 0; pointer<size; pointer++, pBuf1++, pBuf2++)
		{
			if (*pBuf1>*pBuf2) *pBuf1 = 255;
			else *pBuf1 = 0;
		}
		// cross Zero?==>128
		for (i = 1, pointer = i*width; i<height - 1; i++, pointer += width)
		{
			pBuf1 = pResImg + pointer;
			for (j = 1, pBuf1 += j; j<width - 1; j++, pBuf1++)
			{
				if (*pBuf1 == 0) continue;
				if (*(pBuf1 - 1) == 0 || *(pBuf1 + 1) == 0 || *(pBuf1 - width) == 0 || *(pBuf1 + width) == 0) *pBuf1 = 128;
			}
		}
		// edge points(128) to 255,others to 0.
		for (i = 0; i<256; i++) BLUT[i] = 0;
		BLUT[128] = 255;
		for (pointer = 0, pBuf1 = pResImg + pointer; pointer<size; pointer++) *(pBuf1++) = BLUT[*pBuf1];
		// clear image boundary
	}

	// 1.0版本
	// 采用自己分析的方法
	// 2016-6-15 02:30:17,by QQ
	// Last Update,2016-6-19 01:58:33,by QQ
	/*
	实现思路：计算fx方向导数图，计算fy方向导数图，计算强度图（扩充后的强度图，扩充处全部设置为0），
	然后遍历强度图，如果该点强度m>low,计算方向，继续双阈值和非极大抑制，建立一个标记图map（0表示可能是边缘，1表示不能是边缘，2表示确定是边缘），
	对于m>high的标记为2，并且将该地址放入队列，两者之间的标记为0，扩充的地方标记为1。
	全部遍历完成并标记好之后，开始便利队列，对于每个2的位置，搜索8领域，是否有0，如果有进入队列，队列再次出队，循环，直到队列为空，
	最后遍历map，形成最后的边缘图。

	边缘量化四个方向，对应边缘法线4个方向，对每一个边缘法线方向，左右各扩展22.5°，形成最后的4个法线区间，由于边缘方向在这里没有意义（虽然梯度有方向，但是表示的都是一个边缘方向）
	梯度的正负代表了一个边缘，所以，取梯度方向为正来计算（只计算180°区间，）
	我计算梯度方向的目的是非极大值抑制，无关无梯度的正负。
	对于3通道图，取梯度强度最大的通道
	*/
#define CANNY_SHIFT 16
#define TAN_225  (int)(0.4142135623730950488016887242097*(1 << CANNY_SHIFT));
#define TAN_675  (int)(2.4142135623730950488016887242097*(1 << CANNY_SHIFT));
	void Canny1(const Mat &srcImage, Mat &dstImage, double lowThreshold, double highThreshold, int sizeOfAperture, bool L2)
	{
		// 只支持灰度图
		CV_Assert(srcImage.type() == CV_8UC1);
		dstImage.create(srcImage.size(), srcImage.type());

		// L2范数计算边缘强度的时候，距离采用平方的方式，所以阈值也需要采用平方
		if (L2)
		{
			lowThreshold = std::min(32767.0, lowThreshold);
			highThreshold = std::min(32767.0, highThreshold);

			if (lowThreshold > 0) lowThreshold *= lowThreshold;
			if (highThreshold > 0) highThreshold *= highThreshold;
		}

		// 计算fx,fy,强度图
		Mat fx(srcImage.size(), CV_32SC1);
		Mat fy(srcImage.size(), CV_32SC1);
		Mat enlargedImage;
		Mat magnitudeImage(srcImage.rows + 2, srcImage.cols + 2, CV_32SC1);
		magnitudeImage.setTo(Scalar(0));
		copyMakeBorder(srcImage, enlargedImage, 1, 1, 1, 1, cv::BORDER_REPLICATE);
		int stepOfEnlargedImage = enlargedImage.cols;
		int stepOffx = fx.cols;
		int height = srcImage.rows;
		int width = srcImage.cols;
		uchar *rowOfEnlargedImage = enlargedImage.data + stepOfEnlargedImage + 1;
		int *rowOffx = (int *)fx.data;
		int *rowOffy = (int *)fy.data;
		int *rowOfMagnitudeImage = (int *)magnitudeImage.data + stepOfEnlargedImage + 1;
		for (int y = 0; y <= height - 1; ++y, rowOfEnlargedImage += stepOfEnlargedImage, rowOfMagnitudeImage += stepOfEnlargedImage, rowOffx += stepOffx, rowOffy += stepOffx)
		{
			uchar *colOfEnlargedImage = rowOfEnlargedImage;
			int *colOffx = rowOffx;
			int *colOffy = rowOffy;
			int *colOfMagnitudeImage = rowOfMagnitudeImage;
			for (int x = 0; x <= width - 1; ++x, ++colOfEnlargedImage, ++colOffx, ++colOffy, ++colOfMagnitudeImage)
			{
				// fx
				colOffx[0] = colOfEnlargedImage[1 - stepOfEnlargedImage] + 2 * colOfEnlargedImage[1] + colOfEnlargedImage[1 + stepOfEnlargedImage] -
					colOfEnlargedImage[-1 - stepOfEnlargedImage] - 2 * colOfEnlargedImage[-1] - colOfEnlargedImage[-1 + stepOfEnlargedImage];

				// fy
				colOffy[0] = colOfEnlargedImage[stepOfEnlargedImage - 1] + 2 * colOfEnlargedImage[stepOfEnlargedImage] + colOfEnlargedImage[stepOfEnlargedImage + 1] -
					colOfEnlargedImage[-stepOfEnlargedImage - 1] - 2 * colOfEnlargedImage[-stepOfEnlargedImage] - colOfEnlargedImage[-stepOfEnlargedImage + 1];

				// 计算边缘强度，由于只是用于比较，为了加快速度，只计算平方和
				if (L2)
				{
					colOfMagnitudeImage[0] = colOffx[0] * colOffx[0] + colOffy[0] * colOffy[0];
				}
				else
				{
					colOfMagnitudeImage[0] = std::abs(colOffx[0]) + std::abs(colOffy[0]);
				}

			}
		}


		// 非极大抑制，同时标记图做标记,双阈值处理
		//   0 - 可能是边缘
		//   1 - 不是边缘
		//   2 - 一定是边缘
		Mat labelImage(srcImage.rows + 2, srcImage.cols + 2, CV_8UC1);
		memset(labelImage.data, 1, labelImage.rows*labelImage.cols);
		rowOffx = (int *)fx.data;
		rowOffy = (int *)fy.data;
		rowOfMagnitudeImage = (int *)magnitudeImage.data + stepOfEnlargedImage + 1;
		uchar *rowOfLabelImage = labelImage.data + stepOfEnlargedImage + 1;
		queue<uchar*> queueOfEdgePixel;
		for (int y = 0; y <= height - 1; ++y, rowOfMagnitudeImage += stepOfEnlargedImage, rowOffx += stepOffx, rowOffy += stepOffx, rowOfLabelImage += stepOfEnlargedImage)
		{
			int *colOffx = rowOffx;
			int *colOffy = rowOffy;
			int *colOfMagnitudeImage = rowOfMagnitudeImage;
			uchar *colOfLabelImage = rowOfLabelImage;
			for (int x = 0; x <= width - 1; ++x, ++colOffx, ++colOffy, ++colOfMagnitudeImage, ++colOfLabelImage)
			{
				int fx = colOffx[0];
				int fy = colOffy[0];

				// 该像素点才有可能是边缘点
				if (colOfMagnitudeImage[0] > lowThreshold)
				{
					// 非极大抑制
					fy = fy*(1 << CANNY_SHIFT);

					int tan225 = fx * TAN_225;
					int tan675 = fx * TAN_675;

					// 梯度方向0
					if (fy>-1 * tan225 && fy < tan225)
					{
						// 极大值，有可能是边缘
						if (colOfMagnitudeImage[0] >= colOfMagnitudeImage[-1] && colOfMagnitudeImage[0] >= colOfMagnitudeImage[1])
						{
							// 大于高阈值，是边缘，标记为2
							if (colOfMagnitudeImage[0] > highThreshold)
							{
								// 进入队列，并设置标记
								colOfLabelImage[0] = 2;
								queueOfEdgePixel.push(colOfLabelImage);
							}
							else
							{
								// 有可能是边缘，标记为0
								colOfLabelImage[0] = 0;

							}
						}
					}

					// 梯度方向45
					if (fy > tan225&&fy < tan675)
					{
						// 极大值，有可能是边缘
						if (colOfMagnitudeImage[0] >= colOfMagnitudeImage[stepOfEnlargedImage + 1] && colOfMagnitudeImage[0] >= colOfMagnitudeImage[-stepOfEnlargedImage - 1])
						{
							// 大于高阈值，是边缘，标记为2
							if (colOfMagnitudeImage[0] > highThreshold)
							{
								// 进入队列，并设置标记
								colOfLabelImage[0] = 2;
								queueOfEdgePixel.push(colOfLabelImage);
							}
							else
							{
								// 有可能是边缘，标记为0
								colOfLabelImage[0] = 0;

							}

						}

					}

					// 梯度方向90
					if (fy>tan675 || fy<-tan675)
					{
						// 极大值，有可能是边缘
						if (colOfMagnitudeImage[0] >= colOfMagnitudeImage[stepOfEnlargedImage] && colOfMagnitudeImage[0] >= colOfMagnitudeImage[-stepOfEnlargedImage])
						{
							// 大于高阈值，是边缘，标记为2
							if (colOfMagnitudeImage[0] > highThreshold)
							{
								// 进入队列，并设置标记
								colOfLabelImage[0] = 2;
								queueOfEdgePixel.push(colOfLabelImage);
							}
							else
							{
								// 有可能是边缘，标记为0
								colOfLabelImage[0] = 0;

							}

						}

					}

					// 梯度方向135
					if (fy >= -1 * tan675&&fy <= -1 * tan225)
					{
						// 极大值，有可能是边缘
						if (colOfMagnitudeImage[0] >= colOfMagnitudeImage[stepOfEnlargedImage - 1] && colOfMagnitudeImage[0] >= colOfMagnitudeImage[-stepOfEnlargedImage + 1])
						{
							// 大于高阈值，是边缘，标记为2
							if (colOfMagnitudeImage[0] > highThreshold)
							{
								// 进入队列，并设置标记
								colOfLabelImage[0] = 2;
								queueOfEdgePixel.push(colOfLabelImage);
							}
							else
							{
								// 有可能是边缘，标记为0
								colOfLabelImage[0] = 0;

							}

						}

					}

				}
			}

		}

		// 连接性分析,这里采用队列实现，也可以采用栈实现
		// 注意，这里采用递归分析
		// 如果A像素为0，与2在8领域连接，那么标记为2，如果此时有像素值为0的像素与A相邻，则被标记为2,
		while (!queueOfEdgePixel.empty())
		{
			uchar *m = queueOfEdgePixel.front();
			queueOfEdgePixel.pop();

			// 在8领域搜索
			if (!m[-1])
			{
				m[-1] = 2;
				queueOfEdgePixel.push(m - 1);
			}

			if (!m[1])
			{
				m[1] = 2;
				queueOfEdgePixel.push(m + 1);
			}
			if (!m[-stepOfEnlargedImage - 1])
			{
				m[-stepOfEnlargedImage - 1] = 2;
				queueOfEdgePixel.push(m - stepOfEnlargedImage - 1);
			}
			if (!m[-stepOfEnlargedImage])
			{
				m[-stepOfEnlargedImage] = 2;
				queueOfEdgePixel.push(m - stepOfEnlargedImage);
			}
			if (!m[-stepOfEnlargedImage + 1])
			{
				m[-stepOfEnlargedImage + 1] = 2;
				queueOfEdgePixel.push(m - stepOfEnlargedImage + 1);
			}
			if (!m[stepOfEnlargedImage - 1])
			{
				m[stepOfEnlargedImage - 1] = 2;
				queueOfEdgePixel.push(m + stepOfEnlargedImage - 1);
			}
			if (!m[stepOfEnlargedImage])
			{
				m[stepOfEnlargedImage] = 2;
				queueOfEdgePixel.push(m + stepOfEnlargedImage);
			}
			if (!m[stepOfEnlargedImage + 1])
			{
				m[stepOfEnlargedImage + 1] = 2;
				queueOfEdgePixel.push(m + stepOfEnlargedImage + 1);
			}
		}

		// 最后生成边缘图
		rowOfLabelImage = labelImage.data + stepOfEnlargedImage + 1;
		uchar *rowOfDst = dstImage.data;
		for (int y = 0; y <= height - 1; ++y, rowOfLabelImage += stepOfEnlargedImage, rowOfDst += stepOffx)
		{
			uchar *colOfLabelImage = rowOfLabelImage;
			uchar *colOfDst = rowOfDst;
			for (int x = 0; x <= width - 1; ++x, ++colOfDst, ++colOfLabelImage)
			{
				if (colOfLabelImage[0] == 2)
					colOfDst[0] = 255;
				else
				{
					colOfDst[0] = 0;
				}
			}
		}

	}

	// 2.0版本
	// 完全采用OpenCV 2.4的方式处理角度,精度高，而且速度快！
	// Last Update,2016-6-19 01:59:06,by QQ
	void Canny2(const Mat &srcImage, Mat &dstImage, double lowThreshold, double highThreshold, int sizeOfAperture, bool L2)
	{
		// 只支持灰度图
		CV_Assert(srcImage.type() == CV_8UC1);
		dstImage.create(srcImage.size(), srcImage.type());

		// L2范数计算边缘强度的时候，距离采用平方的方式，所以阈值也需要采用平方
		if (L2)
		{
			lowThreshold = std::min(32767.0, lowThreshold);
			highThreshold = std::min(32767.0, highThreshold);

			if (lowThreshold > 0) lowThreshold *= lowThreshold;
			if (highThreshold > 0) highThreshold *= highThreshold;
		}

		// 计算fx,fy,强度图
		Mat fx(srcImage.size(), CV_32SC1);
		Mat fy(srcImage.size(), CV_32SC1);
		Mat enlargedImage;
		Mat magnitudeImage(srcImage.rows + 2, srcImage.cols + 2, CV_32SC1);
		magnitudeImage.setTo(Scalar(0));
		copyMakeBorder(srcImage, enlargedImage, 1, 1, 1, 1, cv::BORDER_REPLICATE);
		int stepOfEnlargedImage = enlargedImage.cols;
		int stepOffx = fx.cols;
		int height = srcImage.rows;
		int width = srcImage.cols;
		uchar *rowOfEnlargedImage = enlargedImage.data + stepOfEnlargedImage + 1;
		int *rowOffx = (int *)fx.data;
		int *rowOffy = (int *)fy.data;
		int *rowOfMagnitudeImage = (int *)magnitudeImage.data + stepOfEnlargedImage + 1;
		for (int y = 0; y <= height - 1; ++y, rowOfEnlargedImage += stepOfEnlargedImage, rowOfMagnitudeImage += stepOfEnlargedImage, rowOffx += stepOffx, rowOffy += stepOffx)
		{
			uchar *colOfEnlargedImage = rowOfEnlargedImage;
			int *colOffx = rowOffx;
			int *colOffy = rowOffy;
			int *colOfMagnitudeImage = rowOfMagnitudeImage;
			for (int x = 0; x <= width - 1; ++x, ++colOfEnlargedImage, ++colOffx, ++colOffy, ++colOfMagnitudeImage)
			{
				// fx
				colOffx[0] = colOfEnlargedImage[1 - stepOfEnlargedImage] + 2 * colOfEnlargedImage[1] + colOfEnlargedImage[1 + stepOfEnlargedImage] -
					colOfEnlargedImage[-1 - stepOfEnlargedImage] - 2 * colOfEnlargedImage[-1] - colOfEnlargedImage[-1 + stepOfEnlargedImage];

				// fy
				colOffy[0] = colOfEnlargedImage[stepOfEnlargedImage - 1] + 2 * colOfEnlargedImage[stepOfEnlargedImage] + colOfEnlargedImage[stepOfEnlargedImage + 1] -
					colOfEnlargedImage[-stepOfEnlargedImage - 1] - 2 * colOfEnlargedImage[-stepOfEnlargedImage] - colOfEnlargedImage[-stepOfEnlargedImage + 1];

				// 计算边缘强度，由于只是用于比较，为了加快速度，只计算平方和
				if (L2)
				{
					colOfMagnitudeImage[0] = colOffx[0] * colOffx[0] + colOffy[0] * colOffy[0];
				}
				else
				{
					colOfMagnitudeImage[0] = std::abs(colOffx[0]) + std::abs(colOffy[0]);
				}

			}
		}
#define CANNY_SHIFT 15
#define TG22  (int)(0.4142135623730950488016887242097*(1 << CANNY_SHIFT) + 0.5);

		// 遍历强度图，计算角度，并使用非极大抑制，同时标记图做标记
		//   0 - 可能是边缘
		//   1 - 不是边缘
		//   2 - 一定是边缘
		Mat labelImage(srcImage.rows + 2, srcImage.cols + 2, CV_8UC1);
		memset(labelImage.data, 1, labelImage.rows*labelImage.cols);
		rowOffx = (int *)fx.data;
		rowOffy = (int *)fy.data;
		rowOfMagnitudeImage = (int *)magnitudeImage.data + stepOfEnlargedImage + 1;
		uchar *rowOfLabelImage = labelImage.data + stepOfEnlargedImage + 1;
		queue<uchar*> queueOfEdgePixel;
		for (int y = 0; y <= height - 1; ++y, rowOfMagnitudeImage += stepOfEnlargedImage, rowOffx += stepOffx, rowOffy += stepOffx, rowOfLabelImage += stepOfEnlargedImage)
		{
			int *colOffx = rowOffx;
			int *colOffy = rowOffy;
			int *colOfMagnitudeImage = rowOfMagnitudeImage;
			uchar *colOfLabelImage = rowOfLabelImage;
			for (int x = 0; x <= width - 1; ++x, ++colOffx, ++colOffy, ++colOfMagnitudeImage, ++colOfLabelImage)
			{
				int xs = colOffx[0];
				int ys = colOffy[0];

				// 该像素点才有可能是边缘点
				if (colOfMagnitudeImage[0] > lowThreshold)
				{
					// 非极大抑制
					int x = std::abs(xs);
					int y = std::abs(ys) << CANNY_SHIFT;

					int tg22x = x * TG22;

					// 梯度方向0
					// |dy|/|dx|<0.414,计算出来-22.5<theta<22.5
					if (y < tg22x)
					{
						// 极大值，有可能是边缘
						if (colOfMagnitudeImage[0] >= colOfMagnitudeImage[-1] && colOfMagnitudeImage[0] >= colOfMagnitudeImage[1])
						{
							// 大于高阈值，是边缘，标记为2
							if (colOfMagnitudeImage[0] > highThreshold)
							{
								// 进入队列，并设置标记
								colOfLabelImage[0] = uchar(2);
								queueOfEdgePixel.push(colOfLabelImage);
							}
							else
							{
								// 有可能是边缘，标记为0
								colOfLabelImage[0] = 0;

							}
						}
					}
					else
					{
						// 梯度方向90
						int tg67x = tg22x + (x << (CANNY_SHIFT + 1));

						// 水平边缘|dy|/|dx|>tan67.5=2.414,注意tan函数曲线计算出来67.5<theta<112.5
						if (y > tg67x)
						{
							// 极大值，有可能是边缘
							if (colOfMagnitudeImage[0] >= colOfMagnitudeImage[stepOfEnlargedImage] && colOfMagnitudeImage[0] >= colOfMagnitudeImage[-stepOfEnlargedImage])
							{
								// 大于高阈值，是边缘，标记为2
								if (colOfMagnitudeImage[0] > highThreshold)
								{
									// 进入队列，并设置标记
									colOfLabelImage[0] = 2;
									queueOfEdgePixel.push(colOfLabelImage);
								}
								else
								{
									// 有可能是边缘，标记为0
									colOfLabelImage[0] = 0;

								}

							}
						}
						else
						{
							// 梯度方向 +45°/-45°
							int s = (xs ^ ys) < 0 ? -1 : 1;// ^:异或

														   // 极大值，有可能是边缘
							if (colOfMagnitudeImage[0] >= colOfMagnitudeImage[-stepOfEnlargedImage - s] && colOfMagnitudeImage[0] >= colOfMagnitudeImage[stepOfEnlargedImage + s])
							{
								// 大于高阈值，是边缘，标记为2
								if (colOfMagnitudeImage[0] > highThreshold)
								{
									// 进入队列，并设置标记
									colOfLabelImage[0] = 2;
									queueOfEdgePixel.push(colOfLabelImage);
								}
								else
								{
									// 有可能是边缘，标记为0
									colOfLabelImage[0] = 0;

								}

							}
						}
					}
				}
			}

		}

		// 连接性分析,这里采用队列实现，也可以采用栈实现
		// 注意，这里采用递归分析
		// 如果A像素为0，与2在8领域连接，那么标记为2，如果此时有像素值为0的像素与A相邻，则被标记为2,
		while (!queueOfEdgePixel.empty())
		{
			uchar *m = queueOfEdgePixel.front();
			queueOfEdgePixel.pop();

			// 在8领域搜索
			if (!m[-1])
			{
				m[-1] = 2;
				queueOfEdgePixel.push(m - 1);
			}

			if (!m[1])
			{
				m[1] = 2;
				queueOfEdgePixel.push(m + 1);
			}
			if (!m[-stepOfEnlargedImage - 1])
			{
				m[-stepOfEnlargedImage - 1] = 2;
				queueOfEdgePixel.push(m - stepOfEnlargedImage - 1);
			}
			if (!m[-stepOfEnlargedImage])
			{
				m[-stepOfEnlargedImage] = 2;
				queueOfEdgePixel.push(m - stepOfEnlargedImage);
			}
			if (!m[-stepOfEnlargedImage + 1])
			{
				m[-stepOfEnlargedImage + 1] = 2;
				queueOfEdgePixel.push(m - stepOfEnlargedImage + 1);
			}
			if (!m[stepOfEnlargedImage - 1])
			{
				m[stepOfEnlargedImage - 1] = 2;
				queueOfEdgePixel.push(m + stepOfEnlargedImage - 1);
			}
			if (!m[stepOfEnlargedImage])
			{
				m[stepOfEnlargedImage] = 2;
				queueOfEdgePixel.push(m + stepOfEnlargedImage);
			}
			if (!m[stepOfEnlargedImage + 1])
			{
				m[stepOfEnlargedImage + 1] = 2;
				queueOfEdgePixel.push(m + stepOfEnlargedImage + 1);
			}
		}

		// 最后生成边缘图
		rowOfLabelImage = labelImage.data + stepOfEnlargedImage + 1;
		uchar *rowOfDst = dstImage.data;
		for (int y = 0; y <= height - 1; ++y, rowOfLabelImage += stepOfEnlargedImage, rowOfDst += stepOffx)
		{
			uchar *colOfLabelImage = rowOfLabelImage;
			uchar *colOfDst = rowOfDst;
			for (int x = 0; x <= width - 1; ++x, ++colOfDst, ++colOfLabelImage)
			{
				if (colOfLabelImage[0] == 2)
					colOfDst[0] = 255;
				else
				{
					colOfDst[0] = 0;
				}
			}
		}
	}

#define CANNY_SHIFT 16
#define TAN_225  (int)(0.4142135623730950488016887242097*(1 << CANNY_SHIFT));
#define TAN_675  (int)(2.4142135623730950488016887242097*(1 << CANNY_SHIFT));
	// 对Canny1方法改进，参考OpenCV的方法转为绝对值求解
	void Canny3(const Mat &srcImage, Mat &dstImage, double lowThreshold, double highThreshold, int sizeOfAperture, bool L2)
	{
		// 只支持灰度图
		CV_Assert(srcImage.type() == CV_8UC1);
		dstImage.create(srcImage.size(), srcImage.type());

		// L2范数计算边缘强度的时候，距离采用平方的方式，所以阈值也需要采用平方
		if (L2)
		{
			lowThreshold = std::min(32767.0, lowThreshold);
			highThreshold = std::min(32767.0, highThreshold);

			if (lowThreshold > 0) lowThreshold *= lowThreshold;
			if (highThreshold > 0) highThreshold *= highThreshold;
		}

		// 计算fx,fy,强度图
		Mat fx(srcImage.size(), CV_32SC1);
		Mat fy(srcImage.size(), CV_32SC1);
		Mat enlargedImage;
		Mat magnitudeImage(srcImage.rows + 2, srcImage.cols + 2, CV_32SC1);
		magnitudeImage.setTo(Scalar(0));
		copyMakeBorder(srcImage, enlargedImage, 1, 1, 1, 1, cv::BORDER_REPLICATE);
		int stepOfEnlargedImage = enlargedImage.cols;
		int stepOffx = fx.cols;
		int height = srcImage.rows;
		int width = srcImage.cols;
		uchar *rowOfEnlargedImage = enlargedImage.data + stepOfEnlargedImage + 1;
		int *rowOffx = (int *)fx.data;
		int *rowOffy = (int *)fy.data;
		int *rowOfMagnitudeImage = (int *)magnitudeImage.data + stepOfEnlargedImage + 1;
		for (int y = 0; y <= height - 1; ++y, rowOfEnlargedImage += stepOfEnlargedImage, rowOfMagnitudeImage += stepOfEnlargedImage, rowOffx += stepOffx, rowOffy += stepOffx)
		{
			uchar *colOfEnlargedImage = rowOfEnlargedImage;
			int *colOffx = rowOffx;
			int *colOffy = rowOffy;
			int *colOfMagnitudeImage = rowOfMagnitudeImage;
			for (int x = 0; x <= width - 1; ++x, ++colOfEnlargedImage, ++colOffx, ++colOffy, ++colOfMagnitudeImage)
			{
				// fx
				colOffx[0] = colOfEnlargedImage[1 - stepOfEnlargedImage] + 2 * colOfEnlargedImage[1] + colOfEnlargedImage[1 + stepOfEnlargedImage] -
					colOfEnlargedImage[-1 - stepOfEnlargedImage] - 2 * colOfEnlargedImage[-1] - colOfEnlargedImage[-1 + stepOfEnlargedImage];

				// fy
				colOffy[0] = colOfEnlargedImage[stepOfEnlargedImage - 1] + 2 * colOfEnlargedImage[stepOfEnlargedImage] + colOfEnlargedImage[stepOfEnlargedImage + 1] -
					colOfEnlargedImage[-stepOfEnlargedImage - 1] - 2 * colOfEnlargedImage[-stepOfEnlargedImage] - colOfEnlargedImage[-stepOfEnlargedImage + 1];

				// 计算边缘强度，由于只是用于比较，为了加快速度，只计算平方和
				if (L2)
				{
					colOfMagnitudeImage[0] = colOffx[0] * colOffx[0] + colOffy[0] * colOffy[0];
				}
				else
				{
					colOfMagnitudeImage[0] = std::abs(colOffx[0]) + std::abs(colOffy[0]);
				}

			}
		}


		// 非极大抑制，同时标记图做标记,双阈值处理
		//   0 - 可能是边缘
		//   1 - 不是边缘
		//   2 - 一定是边缘
		Mat labelImage(srcImage.rows + 2, srcImage.cols + 2, CV_8UC1);
		memset(labelImage.data, 1, labelImage.rows*labelImage.cols);
		rowOffx = (int *)fx.data;
		rowOffy = (int *)fy.data;
		rowOfMagnitudeImage = (int *)magnitudeImage.data + stepOfEnlargedImage + 1;
		uchar *rowOfLabelImage = labelImage.data + stepOfEnlargedImage + 1;
		queue<uchar*> queueOfEdgePixel;
		for (int y = 0; y <= height - 1; ++y, rowOfMagnitudeImage += stepOfEnlargedImage, rowOffx += stepOffx, rowOffy += stepOffx, rowOfLabelImage += stepOfEnlargedImage)
		{
			int *colOffx = rowOffx;
			int *colOffy = rowOffy;
			int *colOfMagnitudeImage = rowOfMagnitudeImage;
			uchar *colOfLabelImage = rowOfLabelImage;
			for (int x = 0; x <= width - 1; ++x, ++colOffx, ++colOffy, ++colOfMagnitudeImage, ++colOfLabelImage)
			{
				int fx = colOffx[0];
				int fy = colOffy[0];

				// 取绝对值，转换为正数
				int abs_fx = std::abs(fx);
				int abs_fy = std::abs(fy);

				// 该像素点才有可能是边缘点
				if (colOfMagnitudeImage[0] > lowThreshold)
				{
					// 非极大抑制
					abs_fy = abs_fy << CANNY_SHIFT; // 对正数直接移位

					// 在乘以较小的浮点数的时候，需要进行数据类型转换
					int tan225 = abs_fx * TAN_225;
					int tan675 = abs_fx * TAN_675;

					// 梯度方向0°
					if (abs_fy< tan225)
					{
						// 极大值，有可能是边缘
						if (colOfMagnitudeImage[0] >= colOfMagnitudeImage[-1] && colOfMagnitudeImage[0] >= colOfMagnitudeImage[1])
						{
							// 大于高阈值，是边缘，标记为2
							if (colOfMagnitudeImage[0] > highThreshold)
							{
								// 进入队列，并设置标记
								colOfLabelImage[0] = 2;
								queueOfEdgePixel.push(colOfLabelImage);
							}
							else
							{
								// 有可能是边缘，标记为0
								colOfLabelImage[0] = 0;

							}
						}
					}

					// 梯度方向90°
					if (abs_fy>tan675)
					{
						// 极大值，有可能是边缘
						if (colOfMagnitudeImage[0] >= colOfMagnitudeImage[stepOfEnlargedImage] && colOfMagnitudeImage[0] >= colOfMagnitudeImage[-stepOfEnlargedImage])
						{
							// 大于高阈值，是边缘，标记为2
							if (colOfMagnitudeImage[0] > highThreshold)
							{
								// 进入队列，并设置标记
								colOfLabelImage[0] = 2;
								queueOfEdgePixel.push(colOfLabelImage);
							}
							else
							{
								// 有可能是边缘，标记为0
								colOfLabelImage[0] = 0;

							}

						}

					}

					// 梯度方向45°/-45°
					if (abs_fy > tan225&&abs_fy<tan675)
					{
						int s = (fy*fx) < 0 ? -1 : 1;
						// 极大值，有可能是边缘
						if (colOfMagnitudeImage[0] >= colOfMagnitudeImage[-stepOfEnlargedImage - s] && colOfMagnitudeImage[0] >= colOfMagnitudeImage[stepOfEnlargedImage + s])
						{
							// 大于高阈值，是边缘，标记为2
							if (colOfMagnitudeImage[0] > highThreshold)
							{
								// 进入队列，并设置标记
								colOfLabelImage[0] = 2;
								queueOfEdgePixel.push(colOfLabelImage);
							}
							else
							{
								// 有可能是边缘，标记为0
								colOfLabelImage[0] = 0;

							}

						}

					}

				}
			}

		}

		// 连接性分析,这里采用队列实现，也可以采用栈实现
		// 注意，这里采用递归分析
		// 如果A像素为0，与2在8领域连接，那么标记为2，如果此时有像素值为0的像素与A相邻，则被标记为2,
		while (!queueOfEdgePixel.empty())
		{
			uchar *m = queueOfEdgePixel.front();
			queueOfEdgePixel.pop();

			// 在8领域搜索
			if (!m[-1])
			{
				m[-1] = 2;
				queueOfEdgePixel.push(m - 1);
			}

			if (!m[1])
			{
				m[1] = 2;
				queueOfEdgePixel.push(m + 1);
			}
			if (!m[-stepOfEnlargedImage - 1])
			{
				m[-stepOfEnlargedImage - 1] = 2;
				queueOfEdgePixel.push(m - stepOfEnlargedImage - 1);
			}
			if (!m[-stepOfEnlargedImage])
			{
				m[-stepOfEnlargedImage] = 2;
				queueOfEdgePixel.push(m - stepOfEnlargedImage);
			}
			if (!m[-stepOfEnlargedImage + 1])
			{
				m[-stepOfEnlargedImage + 1] = 2;
				queueOfEdgePixel.push(m - stepOfEnlargedImage + 1);
			}
			if (!m[stepOfEnlargedImage - 1])
			{
				m[stepOfEnlargedImage - 1] = 2;
				queueOfEdgePixel.push(m + stepOfEnlargedImage - 1);
			}
			if (!m[stepOfEnlargedImage])
			{
				m[stepOfEnlargedImage] = 2;
				queueOfEdgePixel.push(m + stepOfEnlargedImage);
			}
			if (!m[stepOfEnlargedImage + 1])
			{
				m[stepOfEnlargedImage + 1] = 2;
				queueOfEdgePixel.push(m + stepOfEnlargedImage + 1);
			}
		}

		// 最后生成边缘图
		rowOfLabelImage = labelImage.data + stepOfEnlargedImage + 1;
		uchar *rowOfDst = dstImage.data;
		for (int y = 0; y <= height - 1; ++y, rowOfLabelImage += stepOfEnlargedImage, rowOfDst += stepOffx)
		{
			uchar *colOfLabelImage = rowOfLabelImage;
			uchar *colOfDst = rowOfDst;
			for (int x = 0; x <= width - 1; ++x, ++colOfDst, ++colOfLabelImage)
			{
				if (colOfLabelImage[0] == 2)
					colOfDst[0] = 255;
				else
				{
					colOfDst[0] = 0;
				}
			}
		}

	}

	// 经典的Zhang并行快速细化算法
void thin1(const Mat &src, Mat &dst, const int iterations)
{
	const int height = src.rows - 1;
	const int width = src.cols - 1;

	//拷贝一个数组给另一个数组
	if (src.data != dst.data)
	{
		src.copyTo(dst);
	}

	int n = 0, i = 0, j = 0;
	Mat tmpImg;
	uchar *pU, *pC, *pD;
	bool isFinished = false;

	for (n = 0; n<iterations; n++)
	{
		dst.copyTo(tmpImg);
		isFinished = false;   //一次 先行后列扫描 开始
							  //扫描过程一 开始
		for (i = 1; i<height; i++)
		{
			pU = tmpImg.ptr<uchar>(i - 1);
			pC = tmpImg.ptr<uchar>(i);
			pD = tmpImg.ptr<uchar>(i + 1);
			for (int j = 1; j<width; j++)
			{
				if (pC[j] > 0)
				{
					int ap = 0;
					int p2 = (pU[j] >0);
					int p3 = (pU[j + 1] >0);
					if (p2 == 0 && p3 == 1)
					{
						ap++;
					}
					int p4 = (pC[j + 1] >0);
					if (p3 == 0 && p4 == 1)
					{
						ap++;
					}
					int p5 = (pD[j + 1] >0);
					if (p4 == 0 && p5 == 1)
					{
						ap++;
					}
					int p6 = (pD[j] >0);
					if (p5 == 0 && p6 == 1)
					{
						ap++;
					}
					int p7 = (pD[j - 1] >0);
					if (p6 == 0 && p7 == 1)
					{
						ap++;
					}
					int p8 = (pC[j - 1] >0);
					if (p7 == 0 && p8 == 1)
					{
						ap++;
					}
					int p9 = (pU[j - 1] >0);
					if (p8 == 0 && p9 == 1)
					{
						ap++;
					}
					if (p9 == 0 && p2 == 1)
					{
						ap++;
					}
					if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9)>1 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9)<7)
					{
						if (ap == 1)
						{
							if ((p2*p4*p6 == 0) && (p4*p6*p8 == 0))
							{
								dst.ptr<uchar>(i)[j] = 0;
								isFinished = true;
							}

							//   if((p2*p4*p8==0)&&(p2*p6*p8==0))
							//    {                           
							//         dst.ptr<uchar>(i)[j]=0;
							//         isFinished =TRUE;                            
							//    }

						}
					}
				}

			} //扫描过程一 结束


			dst.copyTo(tmpImg);
			//扫描过程二 开始
			for (i = 1; i<height; i++)  //一次 先行后列扫描 开始
			{
				pU = tmpImg.ptr<uchar>(i - 1);
				pC = tmpImg.ptr<uchar>(i);
				pD = tmpImg.ptr<uchar>(i + 1);
				for (int j = 1; j<width; j++)
				{
					if (pC[j] > 0)
					{
						int ap = 0;
						int p2 = (pU[j] >0);
						int p3 = (pU[j + 1] >0);
						if (p2 == 0 && p3 == 1)
						{
							ap++;
						}
						int p4 = (pC[j + 1] >0);
						if (p3 == 0 && p4 == 1)
						{
							ap++;
						}
						int p5 = (pD[j + 1] >0);
						if (p4 == 0 && p5 == 1)
						{
							ap++;
						}
						int p6 = (pD[j] >0);
						if (p5 == 0 && p6 == 1)
						{
							ap++;
						}
						int p7 = (pD[j - 1] >0);
						if (p6 == 0 && p7 == 1)
						{
							ap++;
						}
						int p8 = (pC[j - 1] >0);
						if (p7 == 0 && p8 == 1)
						{
							ap++;
						}
						int p9 = (pU[j - 1] >0);
						if (p8 == 0 && p9 == 1)
						{
							ap++;
						}
						if (p9 == 0 && p2 == 1)
						{
							ap++;
						}
						if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9)>1 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9)<7)
						{
							if (ap == 1)
							{
								//   if((p2*p4*p6==0)&&(p4*p6*p8==0))
								//   {                           
								//         dst.ptr<uchar>(i)[j]=0;
								//         isFinished =TRUE;                            
								//    }

								if ((p2*p4*p8 == 0) && (p2*p6*p8 == 0))
								{
									dst.ptr<uchar>(i)[j] = 0;
									isFinished = true;
								}

							}
						}
					}

				}

			} //一次 先行后列扫描完成          
			  //如果在扫描过程中没有删除点，则提前退出
			if (isFinished == false)
			{
				break;
			}
		}

	}
}


// Applies a thinning iteration to a binary image
static void thinningIteration(Mat img, int iter)
{
	Mat marker = Mat::zeros(img.size(), CV_8UC1);
	for (int i = 1; i < img.rows - 1; i++)
	{
		for (int j = 1; j < img.cols - 1; j++)
		{
			uchar p2 = img.at<uchar>(i - 1, j);
			uchar p3 = img.at<uchar>(i - 1, j + 1);
			uchar p4 = img.at<uchar>(i, j + 1);
			uchar p5 = img.at<uchar>(i + 1, j + 1);
			uchar p6 = img.at<uchar>(i + 1, j);
			uchar p7 = img.at<uchar>(i + 1, j - 1);
			uchar p8 = img.at<uchar>(i, j - 1);
			uchar p9 = img.at<uchar>(i - 1, j - 1);

			int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
				(p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
				(p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
				(p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
			int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
			int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
			int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

			if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				marker.at<uchar>(i, j) = 1;
		}
	}

	img &= ~marker;
}

// Apply the thinning procedure to a given image
void thin2(Mat &src, Mat &dst)
{
	CV_Assert(src.depth() == CV_8U);

	Mat processed = src.clone();
	// Enforce the range of the input image to be in between 0 - 255
	processed /= 255;

	Mat prev = Mat::zeros(processed.size(), CV_8UC1);
	Mat diff;

	do {
		thinningIteration(processed, 0);
		thinningIteration(processed, 1);
		absdiff(processed, prev, diff);
		processed.copyTo(prev);
	} while (countNonZero(diff) > 0);

	processed *= 255;

	dst=processed;
}



} // end of namespace QQ