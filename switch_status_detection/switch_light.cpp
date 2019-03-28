#include "stdafx.h"

vector<int> light_judge(Mat &img_gray, Mat &img_light_bg, int circle_r_max, int circle_r_min)
{
	Mat imageROI[100];
	int ave = 0, var = 0, ave_bright = 0,ave_s=0;
	vector<Vec3f> circles;
	vector<Vec3f> circles_res;
	vector<int> circles_mark,circles_out;
	HoughCircles(img_gray, circles, CV_HOUGH_GRADIENT, 1, circle_r_max, 100, 30, circle_r_min, circle_r_max);
	for (int i = 0; i < circles.size(); i++)
	{
		Vec3f cc = circles[i];
		if (cc[1] + cc[2] / 2>720 || cc[1] - cc[2] / 2 < 0)
			continue;
		if (cc[0] + cc[2] / 2 > 1280 || cc[0] - cc[2] / 2 < 0)
			continue;
		Rect rect(cc[0] - cc[2] / 2, cc[1] - cc[2] / 2, cc[2], cc[2]);//矩形区域
		imageROI[i] = img_light_bg(Rect(cc[0] - cc[2] / 2, cc[1] - cc[2] / 2, cc[2], cc[2]));//生成矩形ROI区域
		ave = GrayScale(imageROI[i]);
		var = GrayVariance(imageROI[i], ave);
		ave_bright = BrightScale(imageROI[i]);
		ave_s = SaturationScale(imageROI[i]);

		if (ave > 127 && ave_bright > 240)//判定为亮,标记
		{
			circles_res.push_back(cc);
			circles_mark.push_back(1);
			//circle(img_light_bg, Point(cc[0], cc[1]), cc[2], Scalar(0, 0, 255), 3, 8, 0);
			//circle(img_light_bg, Point(cc[0], cc[1]), 1, Scalar(155, 50, 255), -1, 8, 0);
		}
		else if (var < 190 && ave_bright < 155 && ave_s > 50 && ave_bright>50)//方差小视为未发光的指示灯，但圆形纯色按键也会读取进来
		{
			circles_res.push_back(cc);
			circles_mark.push_back(0);
			//circle(img_light_bg, Point(cc[0], cc[1]), cc[2], Scalar(0, 0, 255), 3, 8, 0);
			//circle(img_light_bg, Point(cc[0], cc[1]), 1, Scalar(155, 50, 255), -1, 8, 0);
		}
	}
	if (circles_res.size()==0)
		return circles_out;
	for (int i = 0; i < circles_res.size()-1; ++i)
	{
		for (int j = i + 1; j < circles_res.size(); ++j)
		{
			if (abs(circles_res[j][1] - circles_res[i][1]) < 10)
			{
				if (circles_res[j][0] < circles_res[i][0])
				{
					Vec3f circles_res_mid = circles_res[i];
					int circles_mark_mid = circles_mark[i];
					circles_res[i] = circles_res[j];
					circles_mark[i] = circles_mark[j];
					circles_res[j] = circles_res_mid;
					circles_mark[j] = circles_mark_mid;
				}
				else
					continue;
			}
			else
			{
				if (circles_res[j][1] < circles_res[i][1])
				{
					Vec3f circles_res_mid = circles_res[i];
					int circles_mark_mid = circles_mark[i];
					circles_res[i] = circles_res[j];
					circles_mark[i] = circles_mark[j];
					circles_res[j] = circles_res_mid;
					circles_mark[j] = circles_mark_mid;
				}
				else
					continue;
			}
		}
	}

	for (int i = 0; i < circles_mark.size(); ++i)
	{
		circles_out.push_back(circles_mark[i]);
		if (circles_mark[i] == 1)
		{
			circle(img_light_bg, Point(circles_res[i][0], circles_res[i][1]), circles_res[i][2], Scalar(255, 0, 0), 3, 8, 0);
			putText(img_light_bg, "on", Point(circles_res[i][0], circles_res[i][1]), FONT_HERSHEY_TRIPLEX, 2.0, Scalar(255, 0, 0));
		}
		else{
			circle(img_light_bg, Point(circles_res[i][0], circles_res[i][1]), circles_res[i][2], Scalar(0, 255, 0), 3, 8, 0);
			putText(img_light_bg, "off", Point(circles_res[i][0], circles_res[i][1]), FONT_HERSHEY_TRIPLEX, 2.0, Scalar(0, 255, 0));
		}
	}

	//vector<int>::iterator diss_min = min_element(begin(points_diss), end(points_diss));
	//int y_else_mark = distance(std::begin(points_diss), diss_min);

	return circles_out;
}