#pragma execution_character_set("utf-8")//解决中文乱码
#pragma once

#include "Windows.h"

#include <qobject.h>
#include <QWidget>
#include <QSettings>

#include<QListWidget>
#include<QListWidgetItem>
#include<QStackedWidget>

#include<QLayout>
#include<QLabel>

#include<QLineEdit>
#include<QPushButton>

#include<QFileDialog>
#include<QCheckBox>
#include <qdir.h>

class QSpinBox;
class UnitInfoPage;
class VideoSetPage;
class MonitorSetPage;
class Button_in_LineEdit;

// 返回硬盘容量
int get_remain_storage(QString path);

class SystemSet : public QObject
{
	Q_OBJECT
public:
	explicit SystemSet(QObject *parent = 0);
	~SystemSet();

	// 视频设置
	void     set_file_save_path(QString path){ _file_save_path = path; write_all_data(); };
	QString  get_file_save_path(){ return _file_save_path; };
	int      get_video_length(){ return _video_length; }
	void     set_video_length(int length){ _video_length = length; write_all_data(); }
	int      get_real_width(){ return _real_width; }
	void     set_real_width(int l){ _real_width = l; write_all_data(); }

	// 节点设置
	QString   get_unit_id(){ return _unit_id; };
	void      set_unit_id(QString id){ _unit_id = id; write_all_data(); };
	QString   get_unit_name(){ return _unit_name; };
	void      set_unit_name(QString id){ _unit_name = id; write_all_data(); };
	QString   get_unit_position(){ return _unit_position; };
	void      set_unit_position(QString id){ _unit_position = id; write_all_data(); };

	// 监测设置
	int get_dritionTime(){ return _dritionTime; };
	void set_dritionTime(int x){ _dritionTime = x; write_all_data(); };
	int get_speedthreshold(){ return _speedthreshold; };
	void set_speedthreshold(int x){ _speedthreshold = x; write_all_data(); };
	int get_WPthreshold(){ return _WPthreshold; };
	void set_WPthreshold(int x){ _WPthreshold = x; write_all_data(); };

private:

	// 视频设置
	QString   _file_save_path;       //视频文件储存路径
	int       _video_length;         //视频文件长度 h
	int       _real_width;           //实际宽度cm

	// 节点设置
	QString   _unit_id;
	QString   _unit_name;
	QString   _unit_position;

	// 监测设置
	int       _dritionTime;    //异常持续时间min
	int       _speedthreshold;
	int       _WPthreshold;

	void write_all_data(){
		QSettings settings("Resources/config.ini", QSettings::IniFormat); // 当前目录的INI文件
		settings.beginGroup("UnitInfo");
		settings.setValue("unitID", _unit_id);
		settings.setValue("unitName", _unit_name);
		settings.setValue("unitPosition", _unit_position);
		settings.endGroup();

		settings.beginGroup("VideoSet");
		settings.setValue("videoSavePath", _file_save_path);
		settings.setValue("videoLength", _video_length);
		settings.setValue("RealWidth", _real_width);
		settings.endGroup();

		settings.beginGroup("MonitorSet");
		settings.setValue("dritionTime", _dritionTime);
		settings.setValue("speedthreshold", _speedthreshold);
		settings.setValue("WPthreshold", _WPthreshold);
		settings.endGroup();
	}
};


// define SystemSet view
// 系统设置 （视图）单例模式
// 由static SystemSetView *instance(SystemSet *s)得到函数的唯一实例
class SystemSetView_dock :public QWidget
{
	Q_OBJECT
public:
	static SystemSetView_dock *_instance;//唯一实例 指针
	static SystemSetView_dock *instance(QWidget *parent = 0, SystemSet *s = 0)
	{
		if (!_instance)
			_instance = new SystemSetView_dock(0, s);
		return _instance;
	}

private:
	SystemSetView_dock(QWidget *parent = 0, SystemSet *s = 0);
	~SystemSetView_dock(){};

public slots :
	void button_ok();

private:
	void createIcons();
	void set_up_UI();
	void setup_data();

	QPushButton* sure_Button;

	SystemSet* _sys_set;

	//QListWidget* contentsWidget;
	//QStackedWidget* pagesWidget;

// 1
	QLineEdit *ui_unit_ID_lineedit;
	QLineEdit *ui_unit_name_lineedit;
	QLineEdit *ui_unit_position_lineedit;

//2
	QLineEdit *ui_video_save_path;
	Button_in_LineEdit *ui_set_path;

	QLabel *ui_remaining_space;

	QSpinBox *ui_real_width_edit;//实际宽度cm
	QSpinBox *ui_realLength_edit;

	QString set_path();//slot

// 3
	QSpinBox  *ui_dritionTime;
	QSpinBox  *ui_speedthreshold;
	QSpinBox  *ui_WPthreshold;

public:
	QCheckBox *ui_send_sms_CheckBox;
	QCheckBox *ui_take_water_CheckBox;
};
