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
public:
	SendSMS               *_sms_sender;
	SendWaterTakingSignal *_water_taking_siganl_sender;


private:
	void open_camera();
	void open_file();
	void process_start();
	void process_end();
	void record();

	void DB_manage();
	void show_DB_table();

	void background_pickup();
};

#endif // MONITORSYSTEM_H
