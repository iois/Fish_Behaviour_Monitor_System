#include "VideoProcessing.h"
#include "mainwindow.h"
#include"SystemSet.h"

int element_shape = cv::MORPH_RECT;

// 开闭运算
static void OpenClose(cv::Mat &src, cv::Mat &dst, int n=-1)
{
	int an = n > 0 ? n : -n;
	cv::Mat element = cv::getStructuringElement(element_shape, cv::Size(an * 2 + 1, an * 2 + 1), cv::Point(an, an));
	if (n < 0)
		morphologyEx(src, dst, CV_MOP_OPEN, element);
	else
		morphologyEx(src, dst, CV_MOP_CLOSE, element);
}
// 腐蚀膨胀
static void ErodeDilate(cv::Mat &src, cv::Mat &dst, int n = 9 - 10)
{
	int an = n > 0 ? n : -n;
	cv::Mat element = cv::getStructuringElement(element_shape, cv::Size(an * 2 + 1, an * 2 + 1), cv::Point(an, an));
	if (n < 0)
		erode(src, dst, element);
	else
		dilate(src, dst, element);
}

//二值化，get mat(rgb) from 已经剪过背景的图片 by color
cv::Mat get_contours_colors(const cv::Mat &src,int color_threshold){
	cv::Mat dst;
	src.copyTo(dst);

	int height1 = src.size().height;
	int width1 = src.size().width;
	for (int k = 0; k < height1; k++)
	{
		for (int l = 0; l < width1; l++)
		{
			const cv::Vec3b& bgr = src.at<cv::Vec3b>(k,l);
			int b1 = bgr[0];//B
			int g1 = bgr[1];//G
			int r1 = bgr[2];//R

			// 根据颜色通道信息判断目标
			// 减后比较小的是背景
			//if ((40 <= r1) && (r1<250) && (0 <= g1) && (g1<color_threshold) && (0 <= b1) && (b1<color_threshold))
			if ((0 <= r1) && (r1<40+color_threshold) && (0 <= g1) && (g1<color_threshold) && (0 <= b1) && (b1<color_threshold+10))
			{
				dst.at<cv::Vec3b>(k, l)[0] = 255;
				dst.at<cv::Vec3b>(k, l)[1] = 255;
				dst.at<cv::Vec3b>(k, l)[2] = 255;
			}
			else
			{
				dst.at<cv::Vec3b>(k, l)[0] = 0;
				dst.at<cv::Vec3b>(k, l)[1] = 0;
				dst.at<cv::Vec3b>(k, l)[2] = 0;  //[0,0,0]黑
			}
		}
	} //根据颜色二值化完毕。 */
	return dst;
}

//==========================================================================


VideoProcessing::VideoProcessing(QObject *parent, SystemSet *set, ImgProcessSet  *img_p_set)
	:QObject(parent), _sys_set(set), _img_process_set(img_p_set)
{
	_mode_processing = new Speedmode_processing(img_p_set);
	//todo:2015-07-22-11:21
	_mode_processing_wp = new WPmode_processing(img_p_set);
	_mode_processing_Cluster = new Clustermode_processing(img_p_set);
}

void VideoProcessing::attach(MainWindow *Object)
{
	_main_window = Object;
}

VideoProcessing::~VideoProcessing()
{

}

IplImage* VideoProcessing::ImgProcessing(IplImage *src, IplImage *dst, IplImage *img_draw)
{
	// 图片预处理，转化成灰度图像
	cvCvtColor(src, dst, CV_BGR2GRAY);  // 彩色图像转化成灰度图像
	cvSmooth(dst, dst, CV_GAUSSIAN, 3, 0, 0); //高斯滤波

	// 图片 阈值分割
	cvThreshold(dst, dst, _img_process_set->get_segment_threshold(), 255, CV_THRESH_BINARY); //取阀值把图像转为二值图像

	// 开运算
	cvMorphologyEx(dst, dst, NULL, 0, CV_MOP_OPEN, 1);
	// 闭运算（先膨胀再腐蚀）:效果较好，可以使目标更加完整清晰，但仍就有一些噪声。
	cvMorphologyEx(dst, dst, NULL, 0, CV_MOP_CLOSE, 1);

	// 腐蚀
	cvErode(dst, dst, 0, 1);
	// 膨胀
	cvDilate(dst, dst, 0, 1);
	//颜色反转
	cvNot(dst, dst);

	return dst;
}

cv::Mat VideoProcessing::ImgProcessing(const cv::Mat &src, cv::Mat &dst, cv::Mat &img_draw)
{

	cv::Mat temp_rgb;//不能cv::Mat temp_rgb（src）, 浅拷贝
	src.copyTo(temp_rgb);

	GaussianBlur(dst, dst, cv::Size(7,7), 0, 0); //高斯
	
	
	if (!_background.empty())
	{
		temp_rgb = abs(src - _background);
	}
	
	cv::imshow("abs_img", temp_rgb);

	cv::Mat temp_dst_rgb = get_contours_colors(temp_rgb, _img_process_set->get_segment_threshold());

		
	cvtColor(temp_dst_rgb, dst, CV_BGR2GRAY);  // 彩色图像转化成灰度图像
	

	medianBlur(dst, dst, 3);
	GaussianBlur(dst, dst, cv::Size(7,7), 0, 0); //高斯滤波

	


	//bitwise_xor(cv::Scalar(255, 0, 0, 0), dst, dst);//xor,颜色取反

	// todo
	// 图片 阈值分割

	//threshold(dst, dst, _img_process_set->get_segment_threshold(), 255, 0);//阈值分割
	//ErodeDilate(dst, dst);
	//OpenClose(dst, dst);
	
	//bitwise_xor(cv::Scalar(255, 0, 0, 0), dst, dst);//xor,颜色取反

	cv::imshow("Display Image2", dst);

	return dst;
}

bool VideoProcessing::open_camera()
{
	cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Display Image2", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("abs_img", cv::WINDOW_AUTOSIZE);
	_cap.open(0);
	if (!_cap.isOpened()){
		return false;
	} else {
		_cap >> _frame;
		_img_size = { _frame.size().height, _frame.size().width };
		return true;
	}
}

bool VideoProcessing::open_file(const char* file_name)
{
	//_video_file_name = file_name;
	_cap.open(file_name);
	qDebug() << file_name;
	if (!_cap.isOpened()){
		return false;
	} else {
		_cap >> _frame;
		CvSize img_size = _frame.size();
		//cvtColor(_frame, _p_temp, CV_BGR2GRAY);

		//ImgProcessing(_frame, _p_temp, _frame);
		this->notify();
		return true;
	}
}

bool VideoProcessing::open_file(const std::string &file_name){
	_cap.open(file_name);
	if (!_cap.isOpened()){
		return false;
	}
	else {
		_cap >> _frame;
		_img_size = _frame.size();
		this->notify();
		return true;
	}
}

void VideoProcessing::time_out_todo_1()
{

	if (!_background.empty()){

		cv::imshow("Display Image", _background);
	}
	
	//从CvCapture中获得一帧
	_cap >> _frame;
	{
		// 如果开始处理
		if (_isPrecess)
		{
			ImgProcessing(_frame, _img_temp, _frame);

			if (_img_process_set->get_num_fish() == 1){
				
				double speed = _mode_processing->execute(_img_temp, _frame, _img_process_set->get_min_area());
				double wp = _mode_processing_wp->execute(_img_temp, _frame, _img_process_set->get_min_area());

				emit send_data_signal(1, speed);
				emit send_data_signal(2, wp);
				
			}
			else if (_img_process_set->get_num_fish() > 1)
			{
				double r = _mode_processing_Cluster->execute(_img_temp, _frame, _img_process_set->get_min_area());

				emit send_data_signal(3, r);
			}
		}
	}
	//bitwise_xor(cv::Scalar(255, 0, 0, 0), _frame, _frame);//xor,颜色取反
	++_num_of_frames;
	this->notify();
}

void VideoProcessing::notify()// 图像 或 数据 改变了，向观察者mainwindow 发出通知
{
	if (_main_window)
	{
		_main_window->updata_img(_frame);
	}
}

void VideoProcessing::process_end()
{
	if (_isPrecess){
		if (_cap.isOpened()){
			_cap.release();
			_isPrecess = 0;
		}
	}
}

void VideoProcessing::process_begin()
{
	if (_cap.isOpened())
	{
		_isPrecess = 1;
	}
}

cv::Mat VideoProcessing::background_pickup(){

	cv::Mat background;

	int times = 30;    //从30帧中
	int num_frames = 5;//取5帧做平均做背景
	int step = times / num_frames;
	
	if (_cap.isOpened()){
		_cap >> _frame;
		_frame.copyTo(background);
		for (size_t i = 1; i < times+1; ++i)
		{
			_cap >> _frame;
			int kk = i / step;
			if (i % step == 0)
			{
				int height1 = _frame.size().height;
				int width1 = _frame.size().width;
				for (int k = 0; k < height1; k++)
				{
					for (int l = 0; l < width1; l++)
					{
						const cv::Vec3b& bgr = _frame.at<cv::Vec3b>(k, l);
						int b1 = bgr[0];//B
						int g1 = bgr[1];//G
						int r1 = bgr[2];//R
						cv::Vec3b& bgr2 = background.at<cv::Vec3b>(k, l);
						int b2 = bgr2[0];//B 
						int g2 = bgr2[1];//G
						int r2 = bgr2[2];//R

						//图片与背景的差值过大，则做平均
						if (abs(b1 - b2) > 10 && abs(g1 - g2) > 10 && abs(g1 - g2) > 10){
							bgr2[0] = (b1 + bgr2[0] * (kk - 1)) / kk;
							bgr2[1] = (g1 + bgr2[1] * (kk - 1)) / kk;
							bgr2[2] = (r1 + bgr2[2] * (kk - 1)) / kk;
						}
					}
				}
			}
		}

		GaussianBlur(background, background, cv::Size(5, 5), 0, 0); //高斯滤波

		background.copyTo(this->_background);
		cv::imwrite("background.bmp", background);
		emit background_pickup_done();
	}
	else{}

	return background;
}