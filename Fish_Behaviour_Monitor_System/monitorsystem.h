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

	QTimer*          _t;

	MainWindow*      _main_window;
	VideoProcessing* _video_processing;

	QThread*         _thread_videoprocessing;//ÊÓÆµ´¦ÀíÏß³Ì

	ImgProcessSet* _imgp_set;
	SystemSet*     _sys_set;

	SysDB*         _sys_db;

	ImgProcessSet_view *img_p_set;
	SystemSetView_dock *set_view;

private:
	int _isRecord = 1;
	unsigned int _num_of_frames_recoded = 0;
	cv::VideoWriter _video_Writer;   // 存储视频到本地
	std::ofstream _data_writer_1;
	std::ofstream _data_writer_2;
	std::ofstream _data_writer_3;

	QString _video_id;

public:
	SendSMS               *_sms_sender;
	SendWaterTakingSignal *_water_taking_siganl_sender;


private:
	void open_camera();
	void open_file();
	void process_start();
	void process_end();
	void record();

	void exit();

	void DB_manage();
	void show_DB_table();

	void background_pickup();

	void time_out_todo();

	void save_video();
};

#endif // MONITORSYSTEM_H
