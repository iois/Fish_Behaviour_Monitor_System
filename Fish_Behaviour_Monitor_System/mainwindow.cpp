#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, VideoProcessing *vp): QMainWindow(parent), _video_processing(vp)
{
	ui_img_view = new ImgShowWidget_Mat(this);
	ui_img_view->set_size(QSize(320 * 1.5,240 * 1.5));

	ui_warning_view = new WarningViewWidget(this);

	ui_data_view_1 = new DataShowWidget(this);
	ui_data_view_2 = new DataShowWidget(this);
	ui_data_view_3 = new DataShowWidget(this);
	ui_data_view_4 = new DataShowWidget(this);

	ui_data_view_8 = new DataShowWidget(this);
	ui_data_view_9 = new DataShowWidget(this);

	// [1] 
	setupUi();
	// [2]
	createStatusBar();
	// [3]动作初始化
	createActions();
	// [4]
	createMenus();
	// [5]
	setupToolBar();

	this->setWindowTitle(tr("水质检测系统 V1.3"));
	this->setWindowIcon(QIcon("images/system.ico"));

	_video_processing->attach(this);

	recodeAct->setEnabled(false);
	   endAct->setEnabled(false);

}

MainWindow::~MainWindow()
{

}

// [1] 
void MainWindow::setupUi()
{
	// 主布局管理器（水平）
	QHBoxLayout *hLayout_main = new QHBoxLayout(this);
	hLayout_main->setSpacing(6);
	hLayout_main->setContentsMargins(0, 5, 0, 0);

	QVBoxLayout *vLayout_1 = new QVBoxLayout(this);
	vLayout_1->setContentsMargins(5, 5, 0, 0);
	vLayout_1->addWidget(this->ui_data_view_1);

	ui_data_view_2->set_title("尾频:");
	ui_data_view_2->set_unit("n/s");
	vLayout_1->addWidget(this->ui_data_view_2);

	QVBoxLayout *vLayout_2 = new QVBoxLayout(this);
	vLayout_2->setContentsMargins(5, 5, 0, 0);

	ui_data_view_3->set_title("群聚半径:");
	ui_data_view_3->set_unit("cm");
	vLayout_2->addWidget(this->ui_data_view_3);
	ui_data_view_4->set_title("簇群数量:");
	ui_data_view_4->set_unit("个");
	vLayout_2->addWidget(this->ui_data_view_4);

	QVBoxLayout *vLayout_other = new QVBoxLayout(this);
	vLayout_other->setContentsMargins(5, 5, 0, 0);

	ui_data_view_8->set_title("死亡:");
	ui_data_view_8->set_unit("");
	ui_data_view_8->setRange(-1,2);
	vLayout_other->addWidget(ui_data_view_8);
	ui_data_view_9->set_title("半死亡:");
	ui_data_view_9->set_unit("");
	vLayout_other->addWidget(ui_data_view_9);

	data_show_1 = new QWidget(this);
	data_show_2 = new QWidget(this);
	data_show_3 = new QWidget(this);

	data_show_1->setLayout(vLayout_1);
	data_show_2->setLayout(vLayout_2);
	data_show_3->setLayout(vLayout_other);

	tabWidget = new QTabWidget(this);
	tabWidget->addTab(data_show_1, tr("速度与尾频"));
	tabWidget->addTab(data_show_2, tr("群聚半径"));
	tabWidget->addTab(data_show_3, tr("死亡检测"));

	QVBoxLayout *vLayout_22 = new QVBoxLayout(this);
	vLayout_22->setSpacing(6);
	vLayout_22->setContentsMargins(5, 5, 0, 0);
	vLayout_22->setObjectName(QStringLiteral("verticalLayout_2"));
	vLayout_22->setSizeConstraint(QLayout::SetMinAndMaxSize);

	vLayout_22->addWidget(new QLabel(tr(" 监测视频")));
	vLayout_22->addWidget((QWidget*)this->ui_img_view);
	vLayout_22->addWidget(new QLabel(tr(" 预警信息")));
	vLayout_22->addWidget(this->ui_warning_view);

	hLayout_main->addLayout(vLayout_22);
	hLayout_main->addWidget(tabWidget);
	
	QWidget *centralWidget = new QWidget(this);
	centralWidget->setLayout(hLayout_main);
	this->setCentralWidget(centralWidget);

	//dock 
	dock_img_process_set=new QDockWidget(tr("视频处理设置"), this);
	dock_img_process_set->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	this->addDockWidget(Qt::LeftDockWidgetArea, dock_img_process_set);

	//dock window system set
	dock_set = new QDockWidget(tr("系统设置"), this);
	dock_set->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	this->addDockWidget(Qt::LeftDockWidgetArea, dock_set);
	dock_set->setMinimumWidth(200);
}

// [2]
void MainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("就绪"));
}

// [3]动作初始化
void MainWindow::createActions()
{
	opencamera = new QAction(QIcon("images/open_camera.ico"), tr("&打开摄像头"), this);
	opencamera->setStatusTip(tr("打开摄像头"));

	openfile = new QAction(QIcon("images/open_file.ico"), tr("&打开视频文件..."), this);
	openfile->setStatusTip(tr("打开已有视频文件"));

	aboutAct = new QAction(QIcon("images/about.ico"), tr("&关于"), this);
	aboutAct->setStatusTip(tr("显示此应用“关于”窗口"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	exitAct = new QAction(QIcon("images/Exit.ico"), tr("&退出"), this);
	exitAct->setStatusTip(tr("退出"));

	//background_pickup_Act = new QAction(QIcon("images/background_pickup.png"), tr("&背景提取"), this);
	//background_pickup_Act->setStatusTip(tr("背景提取"));

	startAct = new QAction(QIcon("images/start.ico"), tr("&开始处理"), this);
	startAct->setStatusTip(tr("开始处理"));

	startAct->setEnabled(false);

	setviewdefaultAct = new QAction(tr("&设置默认窗口"), this);
	connect(setviewdefaultAct, SIGNAL(triggered()), this, SLOT(set_view_default()));

	//newMonitor = new QAction(QIcon("images/new.ico"), tr("&新建监测"), this);
	//newMonitor->setStatusTip(tr("create a new monitor"));
	//connect(newMonitor, SIGNAL(triggered()), this, SLOT(new_monitor()));

	setAct = new QAction(QIcon("images/set.ico"), tr("&系统设置"), this);
	setAct->setStatusTip(tr("系统设置"));
	connect(setAct, SIGNAL(triggered()), this, SLOT(system_set()));

	DB_manage_Act = new QAction(QIcon("images/DB.ico"), tr("&数据库管理"), this);
	DB_manage_Act->setStatusTip(tr("数据库管理"));

	DB_user_manage_Act = new QAction(QIcon("images/DB.ico"), tr("&用户管理"), this);
	DB_user_manage_Act->setStatusTip(tr("用户管理"));

	endAct = new QAction(QIcon("images/end.ico"), tr("&结束处理"), this);
	endAct->setStatusTip(tr("结束处理"));
	//connect(endAct, SIGNAL(triggered()), this, SLOT(process_end()));
	//endAct->setEnabled(false);

	recodeAct = new QAction(QIcon("images/record.ico"), tr("&开始记录"), this);
	recodeAct->setStatusTip(tr("记录"));
	//connect(recodeAct, SIGNAL(triggered()), this, SLOT(record()));


	_water_taking_sender_set_Act = new QAction(QIcon("images/set.ico"), tr("&取水串口设置"), this);
	_sms_sender_Act = new QAction(QIcon("images/set.ico"), tr("&短信串口设置"), this);
}

// [4]
void MainWindow::createMenus(){
	fileMenu = menuBar()->addMenu(tr("&文件"));
	fileMenu->addAction(opencamera);
	fileMenu->addAction(openfile);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	editMenu = menuBar()->addMenu(tr("&编辑"));
	editMenu->addAction(startAct);
	editMenu->addAction(recodeAct);
	editMenu->addAction(endAct);

	DBMenu = menuBar()->addMenu(tr("&数据库"));
	DBMenu->addAction(DB_manage_Act);
	DBMenu->addAction(DB_user_manage_Act);

	SetMenu = menuBar()->addMenu(tr("&设置"));
	SetMenu->addAction(setAct);
	SetMenu->addAction(_water_taking_sender_set_Act);
	SetMenu->addAction(_sms_sender_Act);

	ViewSetMenu = menuBar()->addMenu(tr("&视图"));
	ViewSetMenu->addAction(setviewdefaultAct);
	ViewSetMenu->addAction(dock_set->toggleViewAction());
	ViewSetMenu->addAction(dock_img_process_set->toggleViewAction());
	
	helpMenu = menuBar()->addMenu(tr("&帮助"));
	helpMenu->addAction(aboutAct);
}

// [5]
void MainWindow::setupToolBar()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(opencamera);
	fileToolBar->addAction(openfile);

	editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(startAct);
	//editToolBar->addAction(background_pickup_Act);
	editToolBar->addAction(recodeAct);
	editToolBar->addAction(endAct);

	setToolBar = addToolBar(tr("Set"));
	setToolBar->addAction(setAct);
}

///
void MainWindow::updata_img(IplImage *src){
	//this->ui_img_view->update_img(src);
}

void MainWindow::updata_img(const cv::Mat &mat){
	this->ui_img_view->update_img(mat);
}

void MainWindow::updata_data(size_t modeIndex, double data){
	switch (modeIndex)
	{
	case 1:
		this->ui_data_view_1->updata_data(data);
		break;
	case 2:
		this->ui_data_view_2->updata_data(data);
		break;
	case 3:
		this->ui_data_view_3->updata_data(data);
		break;
	default:
		break;
	}
}

void MainWindow::updata_data(size_t modeIndex, vector<double> data){
	switch (modeIndex)
	{
	case 1:
		this->ui_data_view_1->updata_data(data);
		break;
	case 2:
		this->ui_data_view_2->updata_data(data);
		break;
	case 3:
		this->ui_data_view_3->updata_data(data);
		break;
	default:
		break;
	}
}
///

void MainWindow::about()
{
	QString MESSAGE =
		tr("生物式水质监测系统V1.0 \n浙江工业大学\n计算机学院 \n水质监测项目组\n\n基于opencv，Qt \n\n                            ");
	QMessageBox msgBox(QMessageBox::Icon::Information, tr("关于"), MESSAGE, 0, this);
	msgBox.setWindowIcon(QIcon("images/about.ico"));
	msgBox.exec();
}

void MainWindow::system_set(){
	if (dock_set){
		dock_set->show();
	}
	if (dock_img_process_set){
		dock_img_process_set->show();
	}
};

void MainWindow::set_view_default()
{
	if (dock_set){
		dock_set->show();
	}
	if (dock_img_process_set){
		dock_img_process_set->show();
	}
}
