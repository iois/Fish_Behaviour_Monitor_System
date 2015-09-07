#pragma once
#include"stdafx.h"
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/contrib/contrib.hpp>


#include<opencv2\ml\ml.hpp>

const int NUM_INPUT=60;

class DetectFishDeath
{
public:
	DetectFishDeath();
	~DetectFishDeath();

	// 输入 灰度图像，轮廓
	std::vector<float> input(const cv::Mat& input_image, const std::vector<std::vector<cv::Point> > &contours);

private:
	CvANN_MLP bp;

	// 输入（cv::Mat& input_image）：包含鱼图像的图片
	// 输出，网络的输出（1死亡，0活鱼）
	float detect(cv::Mat& input_image);
};

