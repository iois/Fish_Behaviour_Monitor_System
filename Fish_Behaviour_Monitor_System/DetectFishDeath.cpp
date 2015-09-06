#include "DetectFishDeath.h"


DetectFishDeath::DetectFishDeath()
{
	bp.load("bp_network.xml");
}


DetectFishDeath::~DetectFishDeath()
{
}

// ÊäÈë£¨cv::Mat& input_image£©£º°üº¬ÓãÍ¼ÏñµÄÍ¼Æ¬
// Êä³ö£¬ÍøÂçµÄÊä³ö£¨1ËÀÍö£¬0»îÓã£©
float DetectFishDeath::detect(cv::Mat& input_image){
	cv::Mat dst, hist;

	olbp(input_image, dst);
	My_calc_Hist(dst, hist);

	cv::Mat outputMat;
	bp.predict(hist, outputMat);

	return outputMat.at<float>(0, 0);
}

std::vector<float> DetectFishDeath::input(const cv::Mat& input_image, const std::vector<std::vector<cv::Point> > &contours){
	cv::Mat gray;

	std::vector<float> p;

	for (size_t i = 0; i < contours.size(); ++i)
	{
		if (contourArea(contours[i]) > 116 && contourArea(contours[i]) < 600 )
		{
			cv::Rect r = cv::boundingRect(cv::Mat(contours[i]));
			cv::Mat fish_rect_gray = gray(r);
			p.push_back(this->detect(fish_rect_gray));
		}
	}
	return p;
}