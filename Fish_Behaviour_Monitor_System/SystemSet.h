#pragma execution_character_set("utf-8")//�����������
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

// ����Ӳ������
int get_remain_storage(QString path);

class SystemSet : public QObject
{
	Q_OBJECT
public:
	explicit SystemSet(QObject *parent = 0);
	~SystemSet();

	// ��Ƶ����
	void     set_file_save_path(QString path){ _file_save_path = path; write_all_data(); };
	QString  get_file_save_path(){ return _file_save_path; };
	int      get_video_length(){ return _video_length; }
	void     set_video_length(int length){ _video_length = length; write_all_data(); }
	int      get_real_width(){ return _real_width; }
	void     set_real_width(int l){ _real_width = l; write_all_data(); }

	// �ڵ�����
	QString   get_unit_id(){ return _unit_id; };
	void      set_unit_id(QString id){ _unit_id = id; write_all_data(); };
	QString   get_unit_name(){ return _unit_name; };
	void      set_unit_name(QString id){ _unit_name = id; write_all_data(); };
	QString   get_unit_position(){ return _unit_position; };
	void      set_unit_position(QString id){ _unit_position = id; write_all_data(); };

	// �������
	int get_dritionTime(){ return _dritionTime; };
	void set_dritionTime(int x){ _dritionTime = x; write_all_data(); };
	int get_speedthreshold(){ return _speedthreshold; };
	void set_speedthreshold(int x){ _speedthreshold = x; write_all_data(); };
	int get_WPthreshold(){ return _WPthreshold; };
	void set_WPthreshold(int x){ _WPthreshold = x; write_all_data(); };

private:

	// ��Ƶ����
	QString   _file_save_path;       //��Ƶ�ļ�����·��
	int       _video_length;         //��Ƶ�ļ����� h
	int       _real_width;           //ʵ�ʿ��cm

	// �ڵ�����
	QString   _unit_id;
	QString   _unit_name;
	QString   _unit_position;

	// �������
	int       _dritionTime;    //�쳣����ʱ��min
	int       _speedthreshold;
	int       _WPthreshold;

	void write_all_data(){
		QSettings settings("Resources/config.ini", QSettings::IniFormat); // ��ǰĿ¼��INI�ļ�
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
// ϵͳ���� ����ͼ������ģʽ
// ��static SystemSetView *instance(SystemSet *s)�õ�������Ψһʵ��
class SystemSetView_dock :public QWidget
{
	Q_OBJECT
public:
	static SystemSetView_dock *_instance;//Ψһʵ�� ָ��
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

	QSpinBox *ui_real_width_edit;//ʵ�ʿ��cm
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
