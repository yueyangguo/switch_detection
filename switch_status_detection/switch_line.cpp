#include "stdafx.h"
#include <numeric>

void find_line(Mat &img_split, Mat &img_line ,int convolution_side, int covolution_mid, int th_line_v, int th_line_mid)
{
	Mat img_split_small(img_split.rows, img_split.cols, CV_16SC1);
	for (int i = 0; i < img_split_small.rows; i++){
		unsigned char* img_split_src = img_split.ptr<unsigned char>(i);
		short int* img_split_dst = img_split_small.ptr<short int>(i);
		img_split_dst[0] = 0;
		for (int j = 1; j < img_split_small.cols; j++){
			img_split_dst[j] = abs(img_split_src[j - 1] - img_split_src[j]);
		}
	}
	for (int i = 1; i < img_split_small.rows; ++i)
	{
		short int* img_line_src = img_split_small.ptr<short int>(i);
		unsigned char* img_line_dst = img_line.ptr<unsigned char>(i);
		for (int j = convolution_side + 0.1*img_split.cols; j < 0.9*img_split.cols - convolution_side - 1 ; ++j)
		{
			int sum_left = accumulate(&img_line_src[j - convolution_side], &img_line_src[j - covolution_mid], 0);
			int sum_mid = accumulate(&img_line_src[j - covolution_mid], &img_line_src[j + covolution_mid+1], 0);
			int sum_right = accumulate(&img_line_src[j + covolution_mid+1], &img_line_src[j + convolution_side + 1], 0);
			if (sum_left > th_line_v&&sum_right > th_line_v&&sum_mid < th_line_mid)
			{
				img_line_dst[j] = 255;
			}
		}
	}
}

Mat pixel_same_cut(Mat img_line_r, Mat img_line_b)
{
	for (int i = 0; i < img_line_r.cols; ++i)
	{
		for (int j = 0; j < img_line_r.rows; ++j)
		{
			if (img_line_b.at<uchar>(j, i) == img_line_r.at<uchar>(j, i))
			{
				img_line_r.at<uchar>(j, i) = 0;
			}
		}
	}
	return img_line_r;
}

void state_change(int &state_1, int &state_2)
{
	int state_mid;
	state_mid = state_1;
	state_1 = state_2;
	state_2 = state_mid;
}

vector<int> switch_line_res(Mat &img_line, Mat &img_bg, Mat &img_th, double minarea)
{
	vector<int> switch_line_mark, x_finds,y_finds,width_finds,height_finds;
	vector<vector<Point>> contours;
	vector<Rect> rects;
	findContours(img_line, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Rect targetRect = boundingRect(contours[i]);
		double area = (targetRect.width-1)*(targetRect.height-1);
		//rectangle(img_bg, targetRect, cv::Scalar(255));
		//if (targetRect.x == 875)
		//{
		//	double h = double(targetRect.height) / double(targetRect.width);
		//	double area = targetRect.width*targetRect.height;
		//}
		if (area > minarea&&double(targetRect.height) / double(targetRect.width) >= 2.6)
		{
			rects.push_back(targetRect);
			//rectangle(img_bg, targetRect, cv::Scalar(255));
		}
	}
	for (int j = 0; j < rects.size(); ++j)
	{
		for (int k = j + 1; k < rects.size(); ++k)
		{	
			int rect_x_dis = abs(rects[j].x + rects[j].width / 2 - rects[k].x - rects[k].width / 2);
			int rect_y_dis = abs(rects[j].y + rects[j].height / 2 - rects[k].y - rects[k].height / 2);
			if (rect_x_dis <= rects[j].width / 2 + rects[k].width / 2)
			{
				if (rect_y_dis > rects[j].height / 2 + rects[k].height / 2+28 && rect_y_dis < rects[j].height/2 + rects[k].height/2+70)
				{
					x_finds.push_back(min(rects[j].x, rects[k].x) - 10);
					width_finds.push_back(max(rects[j].x + rects[j].width, rects[k].x + rects[k].width) - min(rects[j].x, rects[k].x) + 20);
					if (rects[j].y > rects[k].y)
					{ 
						y_finds.push_back(rects[k].y + rects[k].height);
						height_finds.push_back(rects[j].y - rects[k].y - rects[k].height);
					}
					else
					{
						y_finds.push_back(rects[j].y + rects[j].height);
						height_finds.push_back(rects[k].y - rects[j].y - rects[j].height);
					}
					break;
				}
			}
		}
	}
	for (int i = 0; i < x_finds.size(); ++i)
	{
		for (int j = i + 1; j < x_finds.size(); ++j)
		{
			if (x_finds[j] <= x_finds[i])
			{
				state_change(x_finds[i], x_finds[j]);
				state_change(y_finds[i], y_finds[j]);
				state_change(width_finds[i], width_finds[j]);
				state_change(height_finds[i], height_finds[j]);
			}
			else
				continue;
		}
	}
	for (int i = 0; i < x_finds.size(); ++i)
	{	
		//Mat img_bg_find = img_bg(Rect(x_finds[i], y_finds[i], width_finds[i], height_finds[i]));
		//int testt = aHash(img_bg_find, img_tem_error);
		//imwrite("template_error.jpg", img_bg_find);
		//if (aHash(img_bg_find, img_tem_error)<15)
		//{
		//	continue;
		//}
		Mat img_th_find = img_th(Rect(x_finds[i], y_finds[i], width_finds[i], height_finds[i]));
		int switch_line_value = 0;
		for (int j = 0; j < img_th_find.cols; ++j)
		{
			for (int k = 0; k < img_th_find.rows; ++k)
			{
				switch_line_value += img_th_find.at<uchar>(k, j);
			}
		}
		rectangle(img_bg, Rect(x_finds[i], y_finds[i], width_finds[i], height_finds[i]), Scalar(255));
		if (switch_line_value < 100 * img_th_find.rows*img_th_find.cols)
		{
			switch_line_mark.push_back(1);
			putText(img_bg, "on", Point(x_finds[i], y_finds[i]-10), FONT_HERSHEY_TRIPLEX, 0.8, Scalar(255,0,0));
		}
		else
		{
			switch_line_mark.push_back(0);
			putText(img_bg, "off", Point(x_finds[i], y_finds[i] - 10), FONT_HERSHEY_TRIPLEX, 0.8, Scalar(0,255,0));
		}
	}
	return switch_line_mark;
}