﻿/*
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

class MonitorSystem : public QWidget
{
	Q_OBJECT

public:
	MonitorSystem(QWidget *parent = 0);
	~MonitorSystem();

private:

	QTimer*          _t;                     //计时器
	MainWindow*      _main_window;           //主窗口
	VideoProcessing* _video_processing;      //视频处理类
	QThread*         _thread_videoprocessing;//视频处理线程

	ImgProcessSet* _imgp_set;   //图像处理设置类
	SystemSet*     _sys_set;    //系统设置类
	SysDB*         _sys_db;     //系统数据库类


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
