#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>

using namespace std;
using namespace cv;

//int Otsu2D(Mat srcimage);
int SaturationScale(Mat ROI_img);
int GrayScale(Mat ROI_img);
int BrightScale(Mat ROI_img);
int GrayVariance(Mat ROI_img, int ave);
int aHash(Mat matSrc1, Mat matSrc2);
void picture_corr(Mat matSrc, int&circle_r_max, int&circle_r_min);
vector<int> light_judge(Mat &img_gray, Mat &img_light_bg, int circle_r_max, int circle_r_min);
Mat pixel_same_cut(Mat img_line_r, Mat img_line_b);
void find_line(Mat &img_split, Mat &img_line, int convolution_side, int covolution_mid, int th_line_v, int th_line_mid);
vector<int> switch_line_res(Mat &img_line, Mat &img_bg, Mat &img_th, double minarea);