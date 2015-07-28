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
//#include<QSpinBox>// class QSpinBox???

#include<QFileDialog>
#include<QCheckBox>
#include <qdir.h>

class QSpinBox;
class UnitInfoPage;
class VideoSetPage;
class MonitorSetPage;

// ����Ӳ������
int get_remain_storage(QString path);


class SystemSet : public QObject
{
	Q_OBJECT
public:
	explicit SystemSet(QObject *parent = 0);
	~SystemSet();
public:
	// ��Ƶ����
	QString   _file_save_path;       //��Ƶ�ļ�����·��
	void      set_file_save_path(QString path){ _file_save_path = path; };
	QString   get_file_save_path(){ return _file_save_path; };
	int     _video_length_of_time; //��Ƶ�ļ����� min
	int get_video_length_of_time(){ return _video_length_of_time; }
	void set_video_length_of_time(int length){ _video_length_of_time = length; }
	int     _cameraHeight;
	int     _realLength;
	int get_realLength(){ return _realLength; }
	void set_realLength(int l){ _realLength = l; }

	// �ڵ�����
	QString _unit_id;
	QString get_unit_id(){ return _unit_id; };
	void    set_unit_id(QString id){
		_unit_id = id;
		/*
		QSettings settings("config.ini", QSettings::IniFormat); // ��ǰĿ¼��INI�ļ�
		settings.beginGroup("UnitInfo");
		settings.setValue("unitID", _unit_id);
		settings.endGroup();
		*/
	};
	QString _unit_name;
	QString _unit_position;

	// �������
	int _dritionTime;    //�쳣����ʱ��min
	int _speedthreshold;
	int _WPthreshold;

public:
	int _isrecordVideo;
	int	_isrecordData;

	void write_all_data(){
		QSettings settings("Resources/config.ini", QSettings::IniFormat); // ��ǰĿ¼��INI�ļ�
		settings.beginGroup("UnitInfo");
		settings.setValue("unitID", _unit_id);
		settings.endGroup();

		settings.beginGroup("SystemSet");
		settings.setValue("videoSavePath", _file_save_path);
		settings.setValue("videoLength", _video_length_of_time);
		settings.setValue("cameraHeight", _cameraHeight);
		settings.setValue("realLength", _realLength);
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
class SystemSetView :public QWidget
{
	Q_OBJECT
public:
	static SystemSetView *_instance;//Ψһʵ�� ָ��
	static SystemSetView *instance(QWidget *parent = 0,SystemSet *s = 0)
	{
		if (!_instance)
			_instance = new SystemSetView(0, s);
		return _instance;
	}

private:
	SystemSetView(QWidget *parent = 0, SystemSet *s = 0);
	~SystemSetView(){};

	public slots :
	void changePage(QListWidgetItem*, QListWidgetItem*);
	void button_ok();

private:
	void createIcons();
	void Init_Gui();
	void setup_data();

	QPushButton* closeButton;
	QPushButton* sure_Button;

	SystemSet* _sys_set;

	QListWidget* contentsWidget;
	QStackedWidget* pagesWidget;

	UnitInfoPage* info_page;
	VideoSetPage* vidset_page;
	MonitorSetPage* monitor_page;
};


class UnitInfoPage : public QWidget
{
public:
	UnitInfoPage(QWidget *parent = 0);
	~UnitInfoPage();
	QLineEdit *ui_unit_ID_lineedit;
	QLineEdit *ui_unit_name_lineedit;
	QLineEdit *ui_unit_position_lineedit;
};

class VideoSetPage : public QWidget
{
public:
	VideoSetPage(QWidget *parent = 0);

	QSpinBox  *ui_video_length;    //��Ƶ����
	QLineEdit *ui_video_save_path;
	QPushButton *ui_set_path;
	QLabel *ui_remaining_space;

	QSpinBox *ui_videoLength_edit;
	QSpinBox *ui_imageScale_edit;//����ͷ�߶�

	QSpinBox *ui_realLength_edit;

	QString set_path();//slot
};

class MonitorSetPage : public QWidget
{
public:
	MonitorSetPage(QWidget *parent = 0);
	QSpinBox  *ui_dritionTime;
	QSpinBox  *ui_speedthreshold;
	QSpinBox  *ui_WPthreshold;
	QCheckBox *ui_recordviedo_CheckBox;
	QCheckBox *ui_recorddata_CheckBox;

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

	QListWidget* contentsWidget;
	QStackedWidget* pagesWidget;

	// 1
	QLineEdit *ui_unit_ID_lineedit;
	QLineEdit *ui_unit_name_lineedit;
	QLineEdit *ui_unit_position_lineedit;

	//2
	//QSpinBox  *ui_video_length;    //��Ƶ����
	QLineEdit *ui_video_save_path;
	QPushButton *ui_set_path;
	QLabel *ui_remaining_space;

	//QSpinBox *ui_videoLength_edit;
	QSpinBox *ui_imageScale_edit;//����ͷ�߶�

	QSpinBox *ui_realLength_edit;

	QString set_path();//slot
	// 3
	QSpinBox  *ui_dritionTime;
	QSpinBox  *ui_speedthreshold;
	QSpinBox  *ui_WPthreshold;
	QCheckBox *ui_recordviedo_CheckBox;
	QCheckBox *ui_recorddata_CheckBox;
};
