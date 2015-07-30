#pragma execution_character_set("utf-8")//解决中文乱码
#ifndef MONITORSYSTEM_H
#define MONITORSYSTEM_H

#include<QtCore\qobject.h>

#include"mainwindow.h"
#include"VideoProcessing.h"

#include"SystemSet.h"
#include"SystemDB.h"
#include"ImgProcessSet.h"

#include "sendsms.h"
#include "sendwatertakingsignal.h"


const int _fps = 15;//   视频存储——帧率
const int _codec = CV_FOURCC('D', 'I', 'V', 'X');


class MonitorSystem : public QWidget
{
	Q_OBJECT

public:
	MonitorSystem(QWidget *parent = 0);
	~MonitorSystem();

private:

	QTimer*          _t;

	MainWindow*      _main_window;
	VideoProcessing* _video_processing;

	QThread*         _thread_videoprocessing;//ÊÓÆµ´¦ÀíÏß³Ì

	ImgProcessSet* _imgp_set;
	SystemSet*     _sys_set;

	SysDB*         _sys_db;


	SendSMS               *_sms_sender;
	SendWaterTakingSignal *_water_taking_siganl_sender;

	SendSMS_view               *_sms_sender_view;
	SendWaterTakingSignal_view *_water_taking_siganl_sender_view;
	
	// need add to mainwindow
	ImgProcessSet_view *img_p_set;
	SystemSetView_dock *set_view;
	//

	int _isRecord = 1;
	unsigned int _num_of_frames_recoded = 0;
	unsigned int _num_of_frames = 0;

	cv::VideoWriter _video_Writer;   // 存储视频到本地
	std::ofstream _data_writer_1;
	std::ofstream _data_writer_2;
	std::ofstream _data_writer_3;

	QString _video_id;



private:
	void open_camera();
	void open_file();
	void process_start();
	void process_end();
	void record();

	void exit();

	void DB_manage();
	void show_DB_table();

	//获取背景图片
	void background_pickup();

	//
	void time_out_todo();

	//存储原始数据
	void save_video(const cv::Mat &image);
	//存储特征数据
	void save_data(int index, double val);

	//接收vp处理得到的数据
	void receive_data(int index, double val);
};

#endif // MONITORSYSTEM_H
