/* ��������
*/
#pragma execution_character_set("utf-8")//�����������
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets\QMainWindow>
#include <QtWidgets\qaction.h>
#include <QtWidgets\qdockwidget.h>
#include <QtWidgets\qstatusbar.h>
#include <QtWidgets\qmenu.h>
#include <QtWidgets\qtoolbar.h>

class QMessageBox;

#include"VideoProcessing.h"

#include"ImgShowWidget.h"
#include"DataShowWidget.h"
#include"WarningViewWidget.h"

#include"SystemSet.h"
#include"SystemDB.h"

#include"NewMonitor_dlg.h"

#include"ImgProcessSet.h"

#include "LoadingDialog.h"

using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, VideoProcessing *vp = 0);
	~MainWindow();

public:
	// Action
	void createActions();

	QAction *opencamera;  // 1.1 ������ͷ
	QAction *openfile;    // 1.2 ���ļ�

	//QAction *background_pickup_Act;    // 1-2 ��ȡ����

	QAction *startAct;    // 2.1 ��ʼ����
	QAction *recodeAct;   // 2.2 ��ʼ��¼
	QAction *endAct;      // 2.3 ��������

	QAction *setviewdefaultAct;
	QAction *setAct;
	QAction *DB_manage_Act;
	QAction *DB_user_manage_Act;
	QAction *aboutAct;
	QAction *exitAct;

	QAction *_water_taking_sender_set_Act;
	QAction *_sms_sender_Act;

private:
	// Menu
	void createMenus();
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *DBMenu;
	QMenu *helpMenu;
	QMenu *ViewSetMenu;
	QMenu *SetMenu;

	// ToolBar
	void setupToolBar();
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QToolBar *setToolBar;

	// StatusBar
	void createStatusBar();

private:
	void setupUi();

	ImgShowWidget_Mat *ui_img_view;

public:
	WarningViewWidget *ui_warning_view;

	QTabWidget *tabWidget;//data show 

	QWidget *data_show_1;
	QWidget *data_show_2;
	QWidget *data_show_3;

	DataShowWidget *ui_data_view_1;//�ٶ�
	DataShowWidget *ui_data_view_2;//βƵ
	DataShowWidget *ui_data_view_3;//Ⱥ�۰뾶
	DataShowWidget *ui_data_view_4;//ָ���뾶��������
	DataShowWidget *ui_data_view_5;//Ⱥ������
	
public:
	//��������
	DataShowWidget *ui_data_view_8;//��������
	DataShowWidget *ui_data_view_9;//��������

public:
	QDockWidget *dock_set;            // need to add widget on this;
	QDockWidget *dock_img_process_set;// need to add widget on this;

public:	//�� video_processing ����
	void updata_img(IplImage *src);//�۲�vp�ı仯->��ʾͼ��
	void updata_img(const cv::Mat &mat); //�۲�vp�ı仯->��ʾͼ��
	void updata_data(size_t modeIndex, double data); // 
	void updata_data(size_t modeIndex, vector<double> data); // 

public:
	void clean_window(){ 
		ui_img_view->clean_show_img(); 
	}
public slots:
	void system_set();
	void set_view_default();
	void about();
	
private:
	VideoProcessing* const _video_processing;
};

#endif // MAINWINDOW_H
