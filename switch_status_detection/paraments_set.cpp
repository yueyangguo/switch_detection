#include "stdafx.h"
#include <algorithm>

string pHashValue(Mat &src)
{
	Mat img, dst;
	string rst(64, '\0');
	double dIdex[64];
	double mean = 0.0;
	int k = 0;
	if (src.channels() == 3)
	{
		cvtColor(src, src, CV_BGR2GRAY);
		img = Mat_<double>(src);
	}
	else
	{
		img = Mat_<double>(src);
	}
	/* 第一步，缩放尺寸*/
	resize(img, img, Size(8, 8));
	/* 第二步，离散余弦变换，DCT系数求取*/
	dct(img, dst);
	/* 第三步，求取DCT系数均值（左上角8*8区块的DCT系数）*/
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j)
		{
			dIdex[k] = dst.at<double>(i, j);
			mean += dst.at<double>(i, j) / 64;
			++k;
		}
	}
	/* 第四步，计算哈希值。*/
	for (int i = 0; i<64; ++i)
	{
		if (dIdex[i] >= mean)
		{
			rst[i] = '1';
		}
		else
		{
			rst[i] = '0';
		}
	}
	return rst;
}

int aHash(Mat matSrc1,Mat matSrc2)
{
	string str1, str2;
	str1 = pHashValue(matSrc1);
	str2 = pHashValue(matSrc2);
	if ((str1.size() != 64) || (str2.size() != 64))
		return -1;
	int difference = 0;
	for (int i = 0; i<64; i++)
	{
		if (str1[i] != str2[i])
			difference++;
	}
	return difference;
}

void picture_corr(Mat matSrc, int&circle_r_max, int&circle_r_min)
{
	Mat tem_1 = imread("template_1.jpg");
	Mat tem_2 = imread("template_2.jpg");
	Mat tem_3 = imread("template_3.jpg");
	Mat tem_4 = imread("template_4.jpg");
	int tem_1_corr = aHash(matSrc, tem_1);
	int tem_2_corr = aHash(matSrc, tem_2);
	int tem_3_corr = aHash(matSrc, tem_3);
	int tem_4_corr = aHash(matSrc, tem_4);
	int tem_corr_max = min(min(tem_1_corr, tem_2_corr), min(tem_3_corr, tem_4_corr));
	if (tem_1_corr == tem_corr_max)
	{
		circle_r_max = 200;
		circle_r_min = 150;
	}
	if (tem_2_corr == tem_corr_max)
	{
		circle_r_max = 75;
		circle_r_min = 50;
	}
	if (tem_3_corr == tem_corr_max)
	{
		circle_r_max = 130;
		circle_r_min = 100;
	}
	if (tem_4_corr == tem_corr_max)
	{
		circle_r_max = 75;
		circle_r_min = 50;
	}
}