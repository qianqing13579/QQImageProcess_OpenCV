//////////////////////////////////////////////////////////////////////////
// ������ ImageProcessTest.h

//////////////////////////////////////////////////////////////////////////


#ifndef __IMAGEPROCESS_TEST_H__
#define __IMAGEPROCESS_TEST_H__

class ImageProcessTest
{
public:
	// �򵥵Ĳ����㷨��ƽ��ʱ��
	void TestMeanTime();

	void TestAccessImage();//���Է���ͼ��ĸ��ַ���
	void TestConnectImage();//����ƴ��ͼ��
	void TestProject();//����ͶӰ
	void TestMat_IplImage4ALigned();//����Mat��IplImage�Ƿ�4�ֽڶ���
	void TestDrawHist();//���Ի�ֱ��ͼ
	void TestImageMedianMinMax();//����ͼ���������ֵ�����ֵ����Сֵ
	void TestSmooth();//�����Լ����˲���
	void TestConvolution();// ���Ծ��
	void TestEdge();//���Ա�Ե���
	void TestCanny();
	void TestGeometryTransformation();//���Լ��α任
	void TestRotate();
	void TestCvtColor(); // ���ԻҶȱ任
	void TestEqualiz();
	void TestThin();
	void TestGaussianBlur();
	void TestGradientHist();

	// LBP�Ĳ���
	void LBP_Uniform_SVM(); // LBP�ȼ�ģʽ
	void LBP_Rotation_Uniform_SVM(); // LBP��ת���䣬�ȼ�ģʽ
	void TestLBP(); // ����LBP����

};

#endif



