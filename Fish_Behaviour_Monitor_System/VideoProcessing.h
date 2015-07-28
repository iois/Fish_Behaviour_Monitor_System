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

//class QMessageBox;
class MainWindow;
class SystemSet;
class SysDB;

class VideoProcessing : public QObject
{
	Q_OBJECT

public:
	VideoProcessing(QObject *parent = 0, SystemSet *set = 0, SysDB* sys_db = 0,ImgProcessSet  *img_p_set=0);
	~VideoProcessing();

	void attach(MainWindow *Object);//��MainWindow �� VideoProcessing ���� //�۲��ߵ���

private:
	void notify();// ͼ�� �� ���� �ı��ˣ���۲���mainwindow ����֪ͨ 
	void send_data(size_t modeIndex, double data);

private:
	// ͼƬԤ���� ����
	IplImage *ImgProcessing(IplImage *src, IplImage *dst, IplImage *img_draw);
	cv::Mat   ImgProcessing(cv::Mat &src, cv::Mat &dst, cv::Mat &img_draw);
	bool      save_video();

public:
	bool open_file(const char* file_name);//0
	bool open_camera();                   //0 ������ͷ��һֱ��ȡԭʼͼ��ֱ����ʼ����
	cv::Mat background_pickup();  //��ȡ����
signals:
	void background_pickup_done();

public:
	void time_out_todo_1();   //���ö�ʱ����ʱ��һ����ִ����Ӧ����

public:

	void process_end();
	void process_begin();
	bool record();

	//flags
	int _isPrecess = 0;    // �Ƿ����ڴ���1�ǣ�0��
	int _isRecord = 0;     // �Ƿ���м�¼

private:
	MainWindow    *_main_window;
	SysDB         *_sys_db;
	SystemSet     *_sys_set;
	ImgProcessSet *_img_process_set;

	//imgs
	CvCapture    *_capture;   //1��Ƶ����׽��
	cv::VideoCapture  _cap;   //
	//IplImage     *_frame;     //2��׽����һ֡ͼ�� | frame = cvQueryFrame(capture);if (frame)
	cv::Mat      _frame;
	cv::Mat      _p_temp;
	IplImage     *_target_Img;//4���洦����ͼ��
	//IplImage     *_p_temp;    //3�м�洢ͼƬ

	//����ͼƬ���Ҷ�
	cv::Mat _background;

	int _num_of_frames = 0;
	unsigned int _num_of_frames_recoded = 0;

	// mode_processings
	mode_processing* _mode_processing;
	mode_processing* _mode_processing_wp;
	mode_processing* _mode_processing_Cluster;

	//std::ofstream _data_writer;    // �洢���ݵ�����
	std::ofstream _data_writer_1;
	std::ofstream _data_writer_2;
	std::ofstream _data_writer_3;
	// CvVideoWriter*_video_Writer; 
	//CvVideoWriter*_video_Writer=nullptr;   // �洢��Ƶ������
	cv::VideoWriter _video_Writer;   // �洢��Ƶ������
	QString   _video_name;         //   ��Ƶ�洢�����ļ�������ʶ��Ƶ���ƣ��Լ���Ӧ�����������硰001_201505200951��
	const int _fps;                //   ��Ƶ�洢����֡��
	const int _codec;

	QString _video_id;
};

