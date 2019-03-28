#include "stdafx.h"

int GrayScale(Mat ROI_img)
{
	double sum = 0;
	int ave = 0;
	for (int i = 0; i < ROI_img.rows; ++i)
	{
		Vec3b * p = ROI_img.ptr<Vec3b>(i);
		for (int j = 0; j < ROI_img.cols; ++j)
		{
			sum = sum + p[j][0] + p[j][1] + p[j][2];
		}
	}
	ave = sum / 3 / (ROI_img.rows*ROI_img.cols);
	//cout << "ROI����ҶȾ�ֵ" << ":" << ave << endl;
	return ave;
}

int BrightScale(Mat ROI_img)
{
	Mat ROI_img_hsv;
	cvtColor(ROI_img, ROI_img_hsv, CV_BGR2HSV);//BGRͨ��תHSVͨ��
	double sum_V = 0;
	int ave = 0;
	for (int i = 0; i < ROI_img_hsv.rows; ++i)
	{
		Vec3b * p = ROI_img_hsv.ptr<Vec3b>(i);
		for (int j = 0; j < ROI_img_hsv.cols; ++j)
		{
			sum_V = sum_V + p[j][2];
		}
	}
	ave = sum_V / (ROI_img_hsv.rows*ROI_img_hsv.cols);
	//cout << "ROI�������Ⱦ�ֵ" << ":" << ave << endl;
	return ave;
}
int HueScale(Mat ROI_img)
{
	Mat ROI_img_hsv;
	cvtColor(ROI_img, ROI_img_hsv, CV_BGR2HSV);//BGRͨ��תHSVͨ��
	double sum_H = 0;
	int ave = 0;
	for (int i = 0; i < ROI_img_hsv.rows; ++i)
	{
		Vec3b * p = ROI_img_hsv.ptr<Vec3b>(i);
		for (int j = 0; j < ROI_img_hsv.cols; ++j)
		{
			sum_H = sum_H + p[j][0];
		}
	}
	ave = sum_H / (ROI_img_hsv.rows*ROI_img_hsv.cols);
	//cout << "ROI����ɫ�Ⱦ�ֵ" << ":" << ave << endl;
	return ave;
}

int SaturationScale(Mat ROI_img)
{
	Mat ROI_img_hsv;
	cvtColor(ROI_img, ROI_img_hsv, CV_BGR2HSV);//BGRͨ��תHSVͨ��
	double sum_S = 0;
	int ave = 0;
	for (int i = 0; i < ROI_img_hsv.rows; ++i)
	{
		Vec3b * p = ROI_img_hsv.ptr<Vec3b>(i);
		for (int j = 0; j < ROI_img_hsv.cols; ++j)
		{
			sum_S = sum_S + p[j][1];
		}
	}
	ave = sum_S / (ROI_img_hsv.rows*ROI_img_hsv.cols);
	//cout << "ROI���򱥺ͶȾ�ֵ" << ":" << ave << endl;
	return ave;
}

int GrayVariance(Mat ROI_img, int ave)
{
	double sum = 0, temp;
	for (int i = 0; i < ROI_img.rows; ++i)
	{
		Vec3b * p = ROI_img.ptr<Vec3b>(i);
		for (int j = 0; j < ROI_img.cols; ++j)
		{
			temp = ((p[j][0] + p[j][1] + p[j][2]) / 3 - ave)*((p[j][0] + p[j][1] + p[j][2]) / 3 - ave);
			sum = sum + temp;
		}
	}
	sum = sum / (ROI_img.rows*ROI_img.cols);
	//cout << "ROI����Ҷȷ���" << ":" << sum << endl;
	return sum;
}

//int Otsu2D(Mat srcimage)
//{
//	double Histogram[256][256];        //������ά�Ҷ�ֱ��ͼ
//	double TrMax = 0.0;                //���ڴ洢����ļ�������Խ���֮�ͣ�
//	int height = srcimage.rows;        //���������
//	int width = srcimage.cols;         //���������
//	int N = height*width;              //���ص�����
//	int T;                             //������ֵ
//	uchar *data = srcimage.data;
//	for (int i = 0; i < 256; i++)
//	{
//		for (int j = 0; j < 256; j++)
//		{
//			Histogram[i][j] = 0;      //��ʼ������
//		}
//	}
//	for (int i = 0; i < height; i++)
//	{
//		for (int j = 0; j < width; j++)
//		{
//			int Data1 = data[i*srcimage.step + j];         //��ȡ��ǰ�Ҷ�ֵ
//			int Data2 = 0;                           //���ڴ�ŻҶȵ�ƽ��ֵ
//			for (int m = i - 1; m <= i + 1; m++)
//			{
//				for (int n = j - 1; n <= j + 1; n++)
//				{
//					if ((m >= 0) && (m < height) && (n >= 0) && (n < width))
//						Data2 += data[m*srcimage.step + n];//����Ҷ�ֵ�ܺ�
//				}
//			}
//			Data2 = Data2 / 9;
//			Histogram[Data1][Data2]++;                  //��¼��i,j��������
//		}
//	}
//	for (int i = 0; i < 256; i++)
//	{
//		for (int j = 0; j < 256; j++)
//		{
//			Histogram[i][j] /= N;     //��һ����ÿһ����Ԫ��ĸ��ʷֲ�
//		}
//	}
//
//	double S[256];                     //ͳ��ǰi�и��ʵ�����
//	double N1[256];                    //ͳ�Ʊ���������ǰ����i������ֵ
//	double N2[256];                    //ͳ�Ʊ���������ǰ����j������ֵ
//	S[0] = 0;
//	N1[0] = 0;
//	N2[0] = 0;
//	for (int i = 1; i < 256; i++)
//	{
//		double x = 0, n1 = 0, n2 = 0;
//		for (int j = 0; j < 256; j++)
//		{
//			x += Histogram[i - 1][j];
//			n1 += ((i - 1)*Histogram[i - 1][j]); //����������ǰ����i������ֵ
//			n2 += (j*Histogram[i - 1][j]); //����������ǰ����j������ֵ
//		}
//		S[i] = x + S[i - 1];
//		N1[i] = n1 + N1[i - 1];
//		N2[i] = n2 + N2[i - 1];
//	}
//
//	double Pai = 0.0;    //ȫ�־�ֵ����i���� panorama(ȫ��)
//	double Paj = 0.0;    //ȫ�־�ֵ����j����
//	int Threshold_s = 0; //��ֵs
//	int Threshold_t = 0; //��ֵt
//	int M = 0;           //�м����
//	double temp = 0.0;   //�洢���󼣵����ֵ
//	double num3 = 0.0;   //���������б�����i������ֵ
//	double num4 = 0.0;   //���������б�����j������ֵ
//	double Fgi = 0.0;    //ǰ�������ֵ����i����
//	double Fgj = 0.0;    //ǰ�������ֵ����j����
//	double Bgi = 0.0;    //���������ֵ����i����
//	double Bgj = 0.0;    //���������ֵ����j����
//	for (int i = 0; i<256; i++)
//	{
//		for (int j = 0; j < 256; j++)
//		{
//			Pai += i*Histogram[i][j];   //ȫ�־�ֵ����i��������
//			Paj += j*Histogram[i][j];   //ȫ�־�ֵ����j��������
//		}
//	}
//	for (int i = 0; i < 256; i++)
//	{
//		double w0 = 0.0;     //ǰ���������ϸ����ܶ�
//		double w1 = 0.0;     //�����������ϸ����ܶ�
//		double num1 = 0.0;   //����������ǰ����i������ֵ         ��w0һ������ش���
//		double num2 = 0.0;   //����������ǰ����j������ֵ
//
//		if (i >= 1)
//		{
//			w0 += S[i - 1];
//			num1 += N1[i - 1];
//			num2 += N2[i - 1];
//		}
//		for (int j = 0; j < 256; j++)
//		{
//			w0 += Histogram[i][j];        //ǰ���ĸ���
//			num1 += i*Histogram[i][j];    //����������ǰ����i������ֵ
//			num2 += j*Histogram[i][j];    //����������ǰ����j������ֵ
//			w1 = 1 - w0;                  //�����ĸ���
//			num3 = Pai - num1;            //���������б�����i������ֵ
//			num4 = Paj - num2;            //���������б�����j������ֵ
//		}
//		Fgi = num1 / w0;
//		Fgj = num2 / w1;
//		Bgi = num3 / w0;
//		Bgj = num4 / w1;
//		TrMax = ((w0*Pai - num1)*(w0*Pai - num1) + (w0*Paj - num2)*(w0*Paj - num2)) / (w0*w1);
//		if (TrMax > temp)
//		{
//			temp = TrMax;
//			Threshold_s = i;
//		}
//	}
//	T = Threshold_s;
//	return T;
//}