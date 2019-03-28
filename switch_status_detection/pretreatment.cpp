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
	//cout << "ROI区域灰度均值" << ":" << ave << endl;
	return ave;
}

int BrightScale(Mat ROI_img)
{
	Mat ROI_img_hsv;
	cvtColor(ROI_img, ROI_img_hsv, CV_BGR2HSV);//BGR通道转HSV通道
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
	//cout << "ROI区域亮度均值" << ":" << ave << endl;
	return ave;
}
int HueScale(Mat ROI_img)
{
	Mat ROI_img_hsv;
	cvtColor(ROI_img, ROI_img_hsv, CV_BGR2HSV);//BGR通道转HSV通道
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
	//cout << "ROI区域色度均值" << ":" << ave << endl;
	return ave;
}

int SaturationScale(Mat ROI_img)
{
	Mat ROI_img_hsv;
	cvtColor(ROI_img, ROI_img_hsv, CV_BGR2HSV);//BGR通道转HSV通道
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
	//cout << "ROI区域饱和度均值" << ":" << ave << endl;
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
	//cout << "ROI区域灰度方差" << ":" << sum << endl;
	return sum;
}

//int Otsu2D(Mat srcimage)
//{
//	double Histogram[256][256];        //建立二维灰度直方图
//	double TrMax = 0.0;                //用于存储矩阵的迹（矩阵对角线之和）
//	int height = srcimage.rows;        //矩阵的行数
//	int width = srcimage.cols;         //矩阵的列数
//	int N = height*width;              //像素的总数
//	int T;                             //最终阈值
//	uchar *data = srcimage.data;
//	for (int i = 0; i < 256; i++)
//	{
//		for (int j = 0; j < 256; j++)
//		{
//			Histogram[i][j] = 0;      //初始化变量
//		}
//	}
//	for (int i = 0; i < height; i++)
//	{
//		for (int j = 0; j < width; j++)
//		{
//			int Data1 = data[i*srcimage.step + j];         //获取当前灰度值
//			int Data2 = 0;                           //用于存放灰度的平均值
//			for (int m = i - 1; m <= i + 1; m++)
//			{
//				for (int n = j - 1; n <= j + 1; n++)
//				{
//					if ((m >= 0) && (m < height) && (n >= 0) && (n < width))
//						Data2 += data[m*srcimage.step + n];//邻域灰度值总和
//				}
//			}
//			Data2 = Data2 / 9;
//			Histogram[Data1][Data2]++;                  //记录（i,j）的数量
//		}
//	}
//	for (int i = 0; i < 256; i++)
//	{
//		for (int j = 0; j < 256; j++)
//		{
//			Histogram[i][j] /= N;     //归一化的每一个二元组的概率分布
//		}
//	}
//
//	double S[256];                     //统计前i行概率的数组
//	double N1[256];                    //统计遍历过程中前景区i分量的值
//	double N2[256];                    //统计遍历过程中前景区j分量的值
//	S[0] = 0;
//	N1[0] = 0;
//	N2[0] = 0;
//	for (int i = 1; i < 256; i++)
//	{
//		double x = 0, n1 = 0, n2 = 0;
//		for (int j = 0; j < 256; j++)
//		{
//			x += Histogram[i - 1][j];
//			n1 += ((i - 1)*Histogram[i - 1][j]); //遍历过程中前景区i分量的值
//			n2 += (j*Histogram[i - 1][j]); //遍历过程中前景区j分量的值
//		}
//		S[i] = x + S[i - 1];
//		N1[i] = n1 + N1[i - 1];
//		N2[i] = n2 + N2[i - 1];
//	}
//
//	double Pai = 0.0;    //全局均值向量i分量 panorama(全景)
//	double Paj = 0.0;    //全局均值向量j分量
//	int Threshold_s = 0; //阈值s
//	int Threshold_t = 0; //阈值t
//	int M = 0;           //中间变量
//	double temp = 0.0;   //存储矩阵迹的最大值
//	double num3 = 0.0;   //遍历过程中背景区i分量的值
//	double num4 = 0.0;   //遍历过程中背景区j分量的值
//	double Fgi = 0.0;    //前景区域均值向量i分量
//	double Fgj = 0.0;    //前景区域均值向量j分量
//	double Bgi = 0.0;    //背景区域均值向量i分量
//	double Bgj = 0.0;    //背景区域均值向量j分量
//	for (int i = 0; i<256; i++)
//	{
//		for (int j = 0; j < 256; j++)
//		{
//			Pai += i*Histogram[i][j];   //全局均值向量i分量计算
//			Paj += j*Histogram[i][j];   //全局均值向量j分量计算
//		}
//	}
//	for (int i = 0; i < 256; i++)
//	{
//		double w0 = 0.0;     //前景区域联合概率密度
//		double w1 = 0.0;     //背景区域联合概率密度
//		double num1 = 0.0;   //遍历过程中前景区i分量的值         与w0一样做相关处理
//		double num2 = 0.0;   //遍历过程中前景区j分量的值
//
//		if (i >= 1)
//		{
//			w0 += S[i - 1];
//			num1 += N1[i - 1];
//			num2 += N2[i - 1];
//		}
//		for (int j = 0; j < 256; j++)
//		{
//			w0 += Histogram[i][j];        //前景的概率
//			num1 += i*Histogram[i][j];    //遍历过程中前景区i分量的值
//			num2 += j*Histogram[i][j];    //遍历过程中前景区j分量的值
//			w1 = 1 - w0;                  //背景的概率
//			num3 = Pai - num1;            //遍历过程中背景区i分量的值
//			num4 = Paj - num2;            //遍历过程中背景区j分量的值
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