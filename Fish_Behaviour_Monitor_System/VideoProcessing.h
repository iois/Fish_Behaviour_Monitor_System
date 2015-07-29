#pragma once

#include <string>
#include <vector>
#include <math.h>
#include <fstream>

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

	void attach(MainWindow *Object);//��MainWindow �� VideoProcessing ���� //�۲��ߵ���

private:
	void notify();// ͼ�� �� ���� �ı��ˣ���۲���mainwindow ����֪ͨ 
	void send_data(size_t modeIndex, double data);

	// ͼƬԤ���� ����
	IplImage *ImgProcessing(IplImage *src, IplImage *dst, IplImage *img_draw);
	cv::Mat   ImgProcessing(cv::Mat &src, cv::Mat &dst, cv::Mat &img_draw);

public:
	bool open_file(const char* file_name);//0
	bool open_file(const std::string &file_name);//0
	bool open_camera();                   //0 ������ͷ��һֱ��ȡԭʼͼ��ֱ����ʼ����

	
	//��ȡ���� ,���ر���RGB
	cv::Mat background_pickup();  


signals:
	void background_pickup_done();

	void send_data_signal(int index, double value);

public:
	void time_out_todo_1();   //���ö�ʱ����ʱ��һ����ִ����Ӧ����

	void process_end();
	void process_begin();

	cv::Mat get_original_img(){ return this->_frame; }

	//flags
	int _isPrecess = 0;    // �Ƿ����ڴ���1�ǣ�0��

private:
	MainWindow    *_main_window;
	//SysDB         *_sys_db;
	SystemSet     *_sys_set;
	ImgProcessSet *_img_process_set;

	//imgs
	CvCapture    *_capture;   //1��Ƶ����׽��
	cv::VideoCapture  _cap;   //
	//IplImage     *_frame;     //2��׽����һ֡ͼ�� | frame = cvQueryFrame(capture);if (frame)
	cv::Mat      _frame;
	cv::Mat      _p_temp;
	IplImage     *_target_Img;  //4���洦����ͼ��
	//IplImage     *_p_temp;    //3�м�洢ͼƬ

	//����ͼƬ���Ҷ�
	cv::Mat _background;

	int _num_of_frames = 0;

private:
	// mode_processings
	mode_processing* _mode_processing;
	mode_processing* _mode_processing_wp;
	mode_processing* _mode_processing_Cluster;

	//QString _video_id;
};

