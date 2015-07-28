#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, VideoProcessing *vp, QTimer *_t, SystemSet* sys_set, SysDB* sys_db, ImgProcessSet* imgset)
	: QMainWindow(parent), _video_processing(vp), _timer(_t), _sys_set(sys_set), _sys_db(sys_db), _img_process_set(imgset)
{
	if (!_timer){
		_timer = new QTimer(this);
	}

	//ui_img_view = new ImgShowWidget_opencv(this);
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
	// [3]������ʼ��
	createActions();
	// [4]
	createMenus();
	// [5]
	setupToolBar();

	this->setWindowTitle(tr("ˮ�ʼ��ϵͳ V1.2"));
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
	QHBoxLayout *hLayout_main = new QHBoxLayout(this);
	hLayout_main->setSpacing(6);
	hLayout_main->setContentsMargins(0, 5, 0, 0);

	QVBoxLayout *vLayout_1 = new QVBoxLayout(this);
	vLayout_1->setContentsMargins(5, 5, 0, 0);
	vLayout_1->addWidget(this->ui_data_view_1);

	ui_data_view_2->set_title("βƵ:");
	ui_data_view_2->set_unit("n/s");
	vLayout_1->addWidget(this->ui_data_view_2);

	QVBoxLayout *vLayout_2 = new QVBoxLayout(this);
	vLayout_2->setContentsMargins(5, 5, 0, 0);

	ui_data_view_3->set_title("Ⱥ�۰뾶:");
	ui_data_view_3->set_unit("cm");
	vLayout_2->addWidget(this->ui_data_view_3);
	ui_data_view_4->set_title("��Ⱥ����:");
	ui_data_view_4->set_unit("��");
	vLayout_2->addWidget(this->ui_data_view_4);

	QVBoxLayout *vLayout_other = new QVBoxLayout(this);
	vLayout_other->setContentsMargins(5, 5, 0, 0);

	ui_data_view_8->set_title("����:");
	ui_data_view_8->set_unit("��");
	vLayout_other->addWidget(ui_data_view_8);
	ui_data_view_9->set_title("������:");
	ui_data_view_9->set_unit("��");
	vLayout_other->addWidget(ui_data_view_9);

	data_show_1 = new QWidget(this);
	data_show_2 = new QWidget(this);
	data_show_3 = new QWidget(this);

	data_show_1->setLayout(vLayout_1);
	data_show_2->setLayout(vLayout_2);
	data_show_3->setLayout(vLayout_other);

	tabWidget = new QTabWidget(this);
	tabWidget->addTab(data_show_1, tr("��β������"));
	tabWidget->addTab(data_show_2, tr("Ⱥ��������"));
	tabWidget->addTab(data_show_3, tr("��������"));

	QVBoxLayout *vLayout_22 = new QVBoxLayout(this);
	vLayout_22->setSpacing(6);
	vLayout_22->setContentsMargins(5, 5, 0, 0);
	vLayout_22->setObjectName(QStringLiteral("verticalLayout_2"));
	vLayout_22->setSizeConstraint(QLayout::SetMinAndMaxSize);

	vLayout_22->addWidget(new QLabel(tr(" �����Ƶ")));
	vLayout_22->addWidget((QWidget*)this->ui_img_view);
	vLayout_22->addWidget(new QLabel(tr(" Ԥ����Ϣ")));
	vLayout_22->addWidget(this->ui_warning_view);

	hLayout_main->addLayout(vLayout_22);
	hLayout_main->addWidget(tabWidget);
	
	QWidget *centralWidget = new QWidget(this);
	centralWidget->setLayout(hLayout_main);
	this->setCentralWidget(centralWidget);

	//dock 
	dock_img_process_set=new QDockWidget(tr("��Ƶ��������"), this);
	dock_img_process_set->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	ImgProcessSet_view *img_p_set = new ImgProcessSet_view(_img_process_set);
	dock_img_process_set->setWidget(img_p_set);
	this->addDockWidget(Qt::LeftDockWidgetArea, dock_img_process_set);

	//dock window system set
	dock_set = new QDockWidget(tr("ϵͳ����"), this);
	dock_set->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	SystemSetView_dock *set_view = SystemSetView_dock::instance(dock_set, _sys_set);

	dock_set->setWidget(set_view);
	this->addDockWidget(Qt::LeftDockWidgetArea, dock_set);
	dock_set->setMinimumWidth(200);
}

// [2]
void MainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("����"));
}

// [3]������ʼ��
void MainWindow::createActions()
{
	opencamera = new QAction(QIcon("images/open_camera.ico"), tr("&������ͷ"), this);
	opencamera->setStatusTip(tr("������ͷ"));
	connect(opencamera, SIGNAL(triggered()), this, SLOT(open_camera()));

	openfile = new QAction(QIcon("images/open_file.ico"), tr("&����Ƶ�ļ�..."), this);
	openfile->setStatusTip(tr("��������Ƶ�ļ�"));
	connect(openfile, SIGNAL(triggered()), this, SLOT(open_file()));

	aboutAct = new QAction(QIcon("images/about.ico"), tr("&����"), this);
	aboutAct->setStatusTip(tr("��ʾ��Ӧ�á����ڡ�����"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	exitAct = new QAction(QIcon("images/Exit.ico"), tr("&�˳�"), this);
	exitAct->setStatusTip(tr("�˳�"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	background_pickup_Act = new QAction(QIcon("images/background_pickup.png"), tr("&������ȡ"), this);
	background_pickup_Act->setStatusTip(tr("������ȡ"));
	connect(background_pickup_Act, SIGNAL(triggered()), this, SLOT(background_pickup()));

	startAct = new QAction(QIcon("images/start.ico"), tr("&��ʼ����"), this);
	startAct->setStatusTip(tr("��ʼ����"));
	connect(startAct, SIGNAL(triggered()), this, SLOT(process_start()));
	startAct->setEnabled(false);

	setviewdefaultAct = new QAction(tr("&����Ĭ�ϴ���"), this);
	connect(setviewdefaultAct, SIGNAL(triggered()), this, SLOT(set_view_default()));

	//newMonitor = new QAction(QIcon("images/new.ico"), tr("&�½����"), this);
	//newMonitor->setStatusTip(tr("create a new monitor"));
	//connect(newMonitor, SIGNAL(triggered()), this, SLOT(new_monitor()));

	setAct = new QAction(QIcon("images/set.ico"), tr("&ϵͳ����"), this);
	setAct->setStatusTip(tr("ϵͳ����"));
	connect(setAct, SIGNAL(triggered()), this, SLOT(system_set()));

	DB_manage_Act = new QAction(QIcon("images/DB.ico"), tr("&���ݿ����"), this);
	DB_manage_Act->setStatusTip(tr("���ݿ����"));
	connect(DB_manage_Act, SIGNAL(triggered()), this, SLOT(DB_manage()));


	endAct = new QAction(QIcon("images/end.ico"), tr("&��������"), this);
	endAct->setStatusTip(tr("��������"));
	connect(endAct, SIGNAL(triggered()), this, SLOT(process_end()));
	//endAct->setEnabled(false);

	recodeAct = new QAction(QIcon("images/record.ico"), tr("&��ʼ��¼"), this);
	recodeAct->setStatusTip(tr("��¼"));
	connect(recodeAct, SIGNAL(triggered()), this, SLOT(record()));
}

// [4]
void MainWindow::createMenus(){
	fileMenu = menuBar()->addMenu(tr("&�ļ�"));
	fileMenu->addAction(opencamera);
	fileMenu->addAction(openfile);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	editMenu = menuBar()->addMenu(tr("&�༭"));
	editMenu->addAction(startAct);
	editMenu->addAction(recodeAct);
	editMenu->addAction(endAct);

	DBMenu = menuBar()->addMenu(tr("&���ݿ�"));
	DBMenu->addAction(DB_manage_Act);

	SetMenu = menuBar()->addMenu(tr("&����"));
	SetMenu->addAction(setAct);

	ViewSetMenu = menuBar()->addMenu(tr("&��ͼ"));
	ViewSetMenu->addAction(setviewdefaultAct);
	ViewSetMenu->addAction(dock_set->toggleViewAction());
	ViewSetMenu->addAction(dock_img_process_set->toggleViewAction());
	
	helpMenu = menuBar()->addMenu(tr("&����"));
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
	editToolBar->addAction(background_pickup_Act);
	editToolBar->addAction(recodeAct);
	editToolBar->addAction(endAct);

	setToolBar = addToolBar(tr("Set"));
	setToolBar->addAction(setAct);
}

///
void MainWindow::updata_img(IplImage *src){
	//this->ui_img_view->update_img(src);
}

void MainWindow::updata_img(cv::Mat &mat){
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
///

void MainWindow::about()
{
	QString MESSAGE =
		tr("����ʽˮ�ʼ��ϵͳV1.0 \n�㽭��ҵ��ѧ\n�����ѧԺ \nˮ�ʼ����Ŀ��\n\n����opencv��Qt \n\n�����������ڷ�                              ");
	QMessageBox msgBox(QMessageBox::Icon::Information, tr("����"), MESSAGE, 0, this);
	msgBox.setWindowIcon(QIcon("images/about.ico"));
	msgBox.exec();
}

// slots
void MainWindow::open_camera(){
	if (!this->_video_processing->open_camera()){
		QMessageBox::information(this, tr("Information"), tr("Can notopen the camera !"));
	}
	_timer->start(66);
	this->opencamera->setEnabled(false);
	startAct->setEnabled(true);
}

void MainWindow::open_file()
{
	QString path = QFileDialog::getOpenFileName(this, tr("����Ƶ�ļ�"), ".", tr("(*.avi)"));

	if (path.isEmpty()){
		QMessageBox::information(this, tr("Information"), tr("Can Not Open The File !"));
	}
	else
	{
		char*  ch;
		QByteArray ba = path.toLatin1();
		ch = ba.data();
		this->_video_processing->open_file(ch);
		_timer->start(66);

		this->openfile->setEnabled(false);
		this->opencamera->setEnabled(false);
		startAct->setEnabled(true);

		_timer->stop();
	}
}

void MainWindow::process_start(){

	if (_video_processing){
		_video_processing->process_begin();
		_timer->start(66);

		if (_img_process_set->get_num_fish() > 1)
		{
			tabWidget->setCurrentWidget(data_show_2);
		}
	}
	startAct->setEnabled(false);
	recodeAct->setEnabled(true);
	endAct->setEnabled(true);
};

void MainWindow::process_end(){
	if (_video_processing){
		_video_processing->process_end();
	}
	startAct->setEnabled(true);
	endAct->setEnabled(false);
	recodeAct->setIcon(QIcon("images/record.ico"));

	startAct->setEnabled(true);
	recodeAct->setEnabled(true);

	dock_set->setEnabled(true);
	dock_img_process_set->setEnabled(true);
}

void MainWindow::system_set(){
	if (dock_set)
	{
		dock_set->show();
	}
	if (dock_img_process_set)
	{
		dock_img_process_set->show();
	}
};

void MainWindow::DB_manage()
{
	SysDB_view* db_view = SysDB_view::instance(0, _sys_db);
	db_view->show();
}

void MainWindow::record()
{
	if (this->_video_processing->_isPrecess){

		/*todo��
		// ��Ӳ�̿ռ䲻����ʱ��ɾ��һЩ��ǰ����Ƶ����
		QString dir = this->_sys_set->...
		unsigned long long freeBytesToCaller = 0, totalBytes = 0, freeBytes = 0;
		bool b;
		b = GetDiskFreeSpaceEx(QString(dir[0] + ":/").toStdWString().c_str(), (PULARGE_INTEGER)&freeBytesToCaller,
			(PULARGE_INTEGER)&totalBytes, (PULARGE_INTEGER)&freeBytes);

		if (b){
			ui_remaining_space->setText(tr("ʣ��ռ䣺<font color='#006600'>%0GB").arg(freeBytesToCaller / 1024.0 / 1024.0 / 1024.0));
		}
		*/
		NewMonitor_dlg *new_monitor_dlg = new NewMonitor_dlg();
		int res = new_monitor_dlg->exec();

		if (res == QDialog::Accepted){//ȷ����ť
			if (_video_processing->record()){

				recodeAct->setIcon(QIcon("images/recording.ico"));
				statusBar()->showMessage(tr("���ڼ�¼��Ƶ������"));

				dock_set->setEnabled(false);
				dock_img_process_set->setEnabled(false);
				_img_process_set->save_all_data();

				startAct->setEnabled(false);
				recodeAct->setEnabled(false);
			}
		}
	}
}

void MainWindow::set_view_default()
{
	if (dock_set)
	{
		dock_set->show();
	}
	if (dock_img_process_set)
	{
		dock_img_process_set->show();
	}
}

// ��ȡ���� -> return cv::Mat@bakground
// ǰ�᣺����ͷ�� open_camera()
void MainWindow::background_pickup(){
	LoadingDialog *loading_dialog = new LoadingDialog(0, tr("������ȡ��..."));
	QThread* thread = new QThread;
	loading_dialog->moveToThread(thread);

	connect(this->_video_processing, &VideoProcessing::background_pickup_done, loading_dialog, &LoadingDialog::close);

	thread->start();
	loading_dialog->exec();

	this->_video_processing->background_pickup();

	//this->opencamera->setEnabled(false);
	//startAct->setEnabled(true);
}