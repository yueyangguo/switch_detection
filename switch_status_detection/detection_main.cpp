#include "stdafx.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

typedef struct DETECTION_RESULT
{
	int type;
	int index;
	int value;
};

int main()
{
	DETECTION_RESULT pResults[5];

	Mat src, src_gray, src_color, src_gs, src_canny, template_error;
	Mat src_rgb[3];
	int row_resize, thresholdValue,circle_r_max, circle_r_min;
	src = imread("./data/tianhe1.jpg");
	//template_error = imread("template_error.jpg");
	//transpose(src, src);
	//flip(src, src, 1);

	//预处理
	row_resize = (src.rows * 1280) / src.cols;
	resize(src, src, Size(1280,row_resize), 0, 0, INTER_LINEAR);
	GaussianBlur(src, src_gs,Size(3,3),0,0);
	cvtColor(src, src_gray, CV_BGR2GRAY);
	//thresholdValue = Otsu2D(src_gray);   
	//threshold(src_gray, src_th, thresholdValue, 255, THRESH_BINARY);
	Canny(src_gray, src_canny, 40, 120);

	//指示灯识别
	////picture_corr(src,circle_r_max,circle_r_min);
	//天河：60,20
	vector<int> switch_light = light_judge(src_canny, src, 60, 20);

	int j = 1;
	for (int i = 0; i<switch_light.size(); ++i)
	{
		pResults[i].type = j;
		pResults[i].index = i;
		pResults[i].value = switch_light[i];
		j++;
	}

	//检测识别开关图像
	split(src_gs, src_rgb);
	src_color = src_rgb[1];
	Mat src_color_th(src_color.rows, src_color.cols, CV_8UC1, Scalar::all(0));
	//天河：8,1,60,50
	find_line(src_color, src_color_th, 8, 1, 60, 50);
	vector<int> switch_line = switch_line_res(src_color_th, src, src_gray, 100);

	for (int i = 0; i<switch_line.size(); ++i)
	{
		pResults[i + j - 1].type = 5;
		pResults[i + j - 1].index = i + j - 1;
		pResults[i + j - 1].value = switch_line[i];
	}
	int nResultNum = switch_light.size() + switch_line.size();
	waitKey(0);
	return 0;
}