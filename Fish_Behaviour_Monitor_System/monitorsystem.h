/*
MonitorSystem这个类为整个系统应用。 包含了所有的东西。

流程：
【1】打开摄像头/文件
【2】调节图像处理参数
【3】记录检测信息
【4】开始自动监测
【5】报警/到时间换鱼
*/
#pragma execution_character_set("utf-8")//解决中文乱码
#ifndef MONITORSYSTEM_H
#define MONITORSYSTEM_H

#include<QtCore\qobject.h>

#include "stdafx.h"
#include"mainwindow.h"
#include"VideoProcessing.h"

#include"SystemSet.h"
#include"SystemDB.h"
#include"ImgProcessSet.h"

#include "sendsms.h"
#include "sendwatertakingsignal.h"

#include"DetectFishDeath.h"

class MonitorSystem : public QWidget
{
	Q_OBJECT

public:
	MonitorSystem(QWidget *parent = 0);
	~MonitorSystem();
	void init();
	void connect_signal_slot();

private:

	QTimer*          _t;                     //计时器
	MainWindow*      _main_window;           //主窗口
	VideoProcessing* _video_processing;      //视频处理类
	QThread*         _thread_videoprocessing;//视频处理线程

	ImgProcessSet* _imgp_set;   //图像处理设置类
	SystemSet*     _sys_set;    //系统设置类
	SysDB*         _sys_db;     //系统数据库类

	DetectFishDeath _detect_fish_death;//检测死亡

	SendSMS               *_sms_sender;
	SendWaterTakingSignal *_water_taking_siganl_sender;

	SendSMS_view               *_sms_sender_view;
	SendWaterTakingSignal_view *_water_taking_siganl_sender_view;
	
	// need add to mainwindow
	ImgProcessSet_view *img_p_set;
	SystemSetView_dock *set_view;
	//

	int _isRecord = 0;
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
	void record();
	void process_end();
	void exit();

	// 到时间处理
	void time_out_todo();

	// 监测一段时间（24小时）后，保存视频文件与数据，再自动新建一个监测
	void auto_monitor_end();  // 自动结束
	void auto_new_monitor();  // 自动新检测

	void DB_manage();
	void show_DB_table();
	void show_User_table();

	//获取背景图片
	void background_pickup();
	//存储原始数据
	void save_video(const cv::Mat &image);
	//存储特征数据
	void save_data(int index, double val);

	//接收vp处理得到的数据
	void receive_data(int index, double val);

	//检测到鱼死亡后，后续操作
	void fish_died_todo();

	void sendSNS();//发短信
	void collect_water();//收集水

	void send_sms_set();
	void water_taking_set();

	// 日志文件
	std::fstream LOG;
};

#endif // MONITORSYSTEM_H
