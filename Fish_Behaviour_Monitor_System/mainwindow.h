/* 主界面类
*/
#pragma execution_character_set("utf-8")//解决中文乱码
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

	QAction *opencamera;  // 1.1 打开摄像头
	QAction *openfile;    // 1.2 打开文件

	//QAction *background_pickup_Act;    // 1-2 提取背景

	QAction *startAct;    // 2.1 开始处理
	QAction *recodeAct;   // 2.2 开始记录
	QAction *endAct;      // 2.3 结束处理

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

	DataShowWidget *ui_data_view_1;//速度
	DataShowWidget *ui_data_view_2;//尾频
	DataShowWidget *ui_data_view_3;//群聚半径
	DataShowWidget *ui_data_view_4;//指定半径外鱼条数
	DataShowWidget *ui_data_view_5;//群簇数量
	
public:
	//其他特征
	DataShowWidget *ui_data_view_8;//半死亡数
	DataShowWidget *ui_data_view_9;//死亡条数

public:
	QDockWidget *dock_set;            // need to add widget on this;
	QDockWidget *dock_img_process_set;// need to add widget on this;

public:	//给 video_processing 调用
	void updata_img(IplImage *src);//观察vp的变化->显示图像
	void updata_img(const cv::Mat &mat); //观察vp的变化->显示图像
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
