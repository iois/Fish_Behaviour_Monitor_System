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
	
	//��MainWindow �� VideoProcessing ���� //�۲��ߵ���
	void attach(MainWindow *Object);

	bool open_file(const char* file_name);
	bool open_file(const std::string &file_name);

	bool open_camera();
	bool close_camera();

	//���ö�ʱ����ʱ��һ����ִ����Ӧ����
	void time_out_todo_1();   
	void process_end();
	void process_begin();

	cv::Size get_img_size(){ return _img_size; }

	//��ȡ���� ,���ر���RGB
	cv::Mat background_pickup(); 


	cv::Mat get_original_img(){ return this->_frame; } // ����ԭʼ��ͼƬ
	cv::Mat get_gray_img(){ return this->_img_temp; }  // ����ԭʼ��ͼƬ���Ҷ�

	std::vector<std::vector<cv::Point>> get_fish_contours(){ return _contours; }   //���ص�ǰ֡���������

private:

	// ͼ��ı��ˣ���۲���mainwindow ����֪ͨ 
	void notify();

	// ͼƬԤ���� ����,�õ�����Ŀ��ĻҶ�ͼ��dst��
	IplImage *ImgProcessing(IplImage *src, IplImage *dst, IplImage *img_draw);
	cv::Mat   ImgProcessing(const cv::Mat &src, cv::Mat &dst, cv::Mat &img_draw);

signals:
	void background_pickup_done();
	void send_data_signal(int index, double value);

public:
	//flags,�Ƿ����ڴ���1�ǣ�0��
	int _isPrecess = 0;

private:
	MainWindow    *_main_window;
	SystemSet     *_sys_set;
	ImgProcessSet *_img_process_set;

	//imgs

	// ��Ƶ����׽��
	cv::VideoCapture  _cap;  
	// ԭʼͼƬ
	cv::Mat      _frame;    //RGB
	cv::Mat      _img_temp; //gray

	// ������ʾ����Ƶ
	cv::Mat      _img_for_show;

	std::vector<std::vector<cv::Point> > _contours;

	//����ͼƬ
	cv::Mat _background;

	int _num_of_frames = 0;
	cv::Size _img_size;

	// mode_processings
	mode_processing* _mode_processing;
	mode_processing* _mode_processing_wp;
	mode_processing* _mode_processing_Cluster;
};

