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

	// ���� �Ҷ�ͼ������
	std::vector<float> input(const cv::Mat& input_image, const std::vector<std::vector<cv::Point> > &contours);

private:
	CvANN_MLP bp;

	// ���루cv::Mat& input_image����������ͼ���ͼƬ
	// ���������������1������0���㣩
	float detect(cv::Mat& input_image);
};

