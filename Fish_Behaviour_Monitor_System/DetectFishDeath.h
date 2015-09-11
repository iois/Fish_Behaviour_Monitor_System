#pragma once
#include"stdafx.h"
#include <algorithm>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/contrib/contrib.hpp>


#include<opencv2\ml\ml.hpp>

const int NUM_INPUT=60;

using namespace std;

class DetectFishDeath
{
public:
	DetectFishDeath();
	~DetectFishDeath();

	// ÊäÈë »Ò¶ÈÍ¼Ïñ£¬ÂÖÀª
	std::vector<float> input(const cv::Mat& input_image, const std::vector<std::vector<cv::Point> > &contours,int seconds);
	std::vector<float> get_p(){ return p; }
private:
	CvANN_MLP bp;

	// ÊäÈë£¨cv::Mat& input_image£©£º°üº¬ÓãÍ¼ÏñµÄÍ¼Æ¬
	// Êä³ö£¬ÍøÂçµÄÊä³ö£¨1ËÀÍö£¬0»îÓã£©
	float detect(cv::Mat& input_image);
	std::vector<float> p;
	vector<int> _num_detected_death;
};

class DetectFishDeath_by_speed
{
public:
	DetectFishDeath_by_speed(){}
	~DetectFishDeath_by_speed(){}

	void set_num_fishs(int n, int seconds, double speed_threshold){
		this->speed_threshold = speed_threshold;
		_speeds = vector<vector<double>>(n, vector<double>(NUM_FRAMES * seconds/0.7, 0));
	}

	void update(const std::vector<double> &speeds){
		for (size_t i = 0; i < speeds.size(); ++i)
		{
			_speeds[i].push_back(speeds[i]);
			_speeds[i].pop_back();
		}
	}

	int isdied(){
		// todo
		int num_speed_less_than = 0;
		for each (vector<double> var in _speeds)
		{
			for each (double speed_i in var)
			{
				if (speed_i < speed_threshold){
					++num_speed_less_than;
				}
			}
		}
		if (num_speed_less_than > NUM_FRAMES * _speeds.size())
			return 1;
		else
			return 0;
	}

private:
	vector<vector<double>> _speeds;
	double speed_threshold = 3;//cm
};