#pragma once

#include <string>
#include <vector>
#include <math.h>

#include <QtCore\qobject.h>
#include <QMessageBox>

#include <cv.h>     //iplimage
#include <highgui.h>//CvCapture
#include <opencv2\opencv.hpp> //cv::VideoCapture
#include <opencv2\core\core.hpp>

#include"mode_processing.h"
#include"ImgProcessSet.h"

class MainWindow;
class SystemSet;
class SysDB;


class VideoProcessing : public QObject
{
	Q_OBJECT

public:
	VideoProcessing(QObject *parent = 0, SystemSet *set = 0,ImgProcessSet  *img_p_set=0);
	~VideoProcessing();
	
	//将MainWindow 与 VideoProcessing 关联 //观察者调用
	void attach(MainWindow *Object);

	bool open_file(const char* file_name);
	bool open_file(const std::string &file_name);

	bool open_camera();
	bool close_camera();

	//设置定时器，时间一到就执行相应函数
	void time_out_todo_1();   
	void process_end();
	void process_begin();

	cv::Size get_img_size(){ return _img_size; }

	//提取背景 ,返回背景RGB
	cv::Mat background_pickup(); 


	cv::Mat get_original_img(){ return this->_frame; } // 返回原始的图片
	cv::Mat get_gray_img(){ return this->_gray_img; }  // 返回原始的图片，灰度

	std::vector<std::vector<cv::Point>> get_fish_contours(){ return _contours; }   //返回当前帧的鱼的轮廓

private:

	// 图像改变了，向观察者mainwindow 发出通知 
	void notify();

	// 图片预处理 函数,得到包含目标的灰度图像（dst：非黑即白的灰度图像）
	IplImage *ImgProcessing(IplImage *src, IplImage *dst, IplImage *img_draw);
	cv::Mat   ImgProcessing(const cv::Mat &src, cv::Mat &dst);

	// 计算 轮廓
	vector<vector<cv::Point>> VideoProcessing::compute_Contour(cv::Mat &src);

signals:
	void background_pickup_done();
	void send_data_signal(int index, double value);

public:
	//flags,是否正在处理：1是，0否
	int _isPrecess = 0;

private:
	MainWindow    *_main_window;
	SystemSet     *_sys_set;
	ImgProcessSet *_img_process_set;

	//imgs

	// 视频流捕捉器
	cv::VideoCapture  _cap;  
	// 原始图片
	cv::Mat      _frame;       //RGB : 原始图片
	cv::Mat      _gray_img;    //gray: 原始图像的gray版
	cv::Mat      _img_temp;    //gray: 实际是只有黑白的灰度图像
	cv::Mat      _img_for_show;//RGB : 用于显示的视频

	std::vector<std::vector<cv::Point> > _contours;

	//背景图片
	cv::Mat _background;   // 当前版本无用

	int _num_of_frames = 0;
	cv::Size _img_size;

	// mode_processings
	mode_processing* _mode_processing;
	mode_processing* _mode_processing_wp;
	mode_processing* _mode_processing_Cluster;
};

