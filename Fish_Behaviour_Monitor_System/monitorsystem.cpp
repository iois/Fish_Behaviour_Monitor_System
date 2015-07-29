#include "monitorsystem.h"


#include<QtCore\qdebug.h>

MonitorSystem::MonitorSystem(QWidget *parent)
	: QWidget(parent)
{
	//init
	_imgp_set = new ImgProcessSet();
	_sys_set = new SystemSet(this);

	_sys_db = new SysDB();
	_sys_db->AddConnection("QSQLITE", "data.db", "", "", "", 0);

	_t = new QTimer(this);

	_video_processing = new VideoProcessing(this, _sys_set, _sys_db, _imgp_set);

	_main_window = new MainWindow(0, _video_processing, _t, _sys_set, _sys_db, _imgp_set);

	_thread_videoprocessing = new QThread(this);
	_video_processing->moveToThread(_thread_videoprocessing);
	_thread_videoprocessing->start();

	_water_taking_siganl_sender = new SendWaterTakingSignal();

	_sms_sender = new SendSMS();


	//connect
	connect( _main_window->opencamera, &QAction::triggered, this, &MonitorSystem::open_camera );
	connect( _main_window->openfile,   &QAction::triggered, this, &MonitorSystem::open_file );
	connect( _main_window->startAct,   &QAction::triggered, this, &MonitorSystem::process_start );
	connect( _main_window->endAct,     &QAction::triggered, this, &MonitorSystem::process_end );
	connect( _main_window->recodeAct,  &QAction::triggered, this, &MonitorSystem::record);

	connect(_main_window->DB_manage_Act, &QAction::triggered, this, &MonitorSystem::show_DB_table);

	connect(_main_window->background_pickup_Act, &QAction::triggered, this, &MonitorSystem::background_pickup);

	connect(_t, &QTimer::timeout, _video_processing, &VideoProcessing::time_out_todo_1);

	//main_window->showFullScreen();//ÎÞ±ß¿ò£¬×î´ó»¯£¬ÎÞ×îÐ¡»¯×î´ó»¯´°¿Ú
	_main_window->showMaximized();


}

MonitorSystem::~MonitorSystem()
{

}

void MonitorSystem::open_camera(){
	qDebug() << "monitorSystem open camera";
	if (!this->_video_processing->open_camera()){
		QMessageBox::information(this->_main_window, tr("Information"), tr("Can notopen the camera !"));
	}
	_t->start(66);
	_main_window->opencamera->setEnabled(false);
	_main_window->startAct->setEnabled(true);
}

void MonitorSystem::open_file()
{
	QString path_filename = QFileDialog::getOpenFileName(this, tr("打开视频文件"), ".", tr("(*.avi)"));

	if (path_filename.isEmpty()){
		QMessageBox::information(this, tr("Information"), tr("Can Not Open The File !"));
	}
	else
	{
		/* QWtring  -> char*  //现在不用了，直接用string
		char*  char_path_filename;
		{
			QByteArray ba = path_filename.toLatin1();
			char_path_filename = ba.data();
		}
		*/
		this->_video_processing->open_file(path_filename.toStdString());
		//_t->start(66);

		this->_main_window->openfile->setEnabled(false);
		this->_main_window->opencamera->setEnabled(false);
		this->_main_window->startAct->setEnabled(true);

		_t->stop();
	}
}

void MonitorSystem::process_start(){

	if (_video_processing){
		_video_processing->process_begin();
		_t->start(66);

		if (this->_main_window->_img_process_set->get_num_fish() > 1)
		{
			this->_main_window->tabWidget->setCurrentWidget(this->_main_window->data_show_2);
		}
	}
	this->_main_window->startAct->setEnabled(false);
	this->_main_window->recodeAct->setEnabled(true);
	this->_main_window->endAct->setEnabled(true);
};

void MonitorSystem::process_end(){
	if (_video_processing){
		_video_processing->process_end();
	}
	this->_main_window->startAct->setEnabled(true);
	this->_main_window->endAct->setEnabled(false);
	this->_main_window->recodeAct->setIcon(QIcon("images/record.ico"));

	this->_main_window->startAct->setEnabled(true);
	this->_main_window->recodeAct->setEnabled(true);

	this->_main_window->dock_set->setEnabled(true);
	this->_main_window->dock_img_process_set->setEnabled(true);

	// todo:
	// 显示数据 那块 清空
}

void MonitorSystem::record()
{
	if (this->_video_processing->_isPrecess){

		/*todo：
		// 当硬盘空间不够的时候，删除一些以前的视频数据
		QString dir = this->_sys_set->...
		unsigned long long freeBytesToCaller = 0, totalBytes = 0, freeBytes = 0;
		bool b;
		b = GetDiskFreeSpaceEx(QString(dir[0] + ":/").toStdWString().c_str(), (PULARGE_INTEGER)&freeBytesToCaller,
		(PULARGE_INTEGER)&totalBytes, (PULARGE_INTEGER)&freeBytes);

		if (b){
		ui_remaining_space->setText(tr("剩余空间：<font color='#006600'>%0GB").arg(freeBytesToCaller / 1024.0 / 1024.0 / 1024.0));
		}
		*/
		NewMonitor_dlg *new_monitor_dlg = new NewMonitor_dlg();
		int res = new_monitor_dlg->exec();

		if (res == QDialog::Accepted){//确定按钮
			if (_video_processing->record()){

				this->_main_window->recodeAct->setIcon(QIcon("images/recording.ico"));
				this->_main_window->statusBar()->showMessage(tr("正在记录视频与数据"));

				this->_main_window->dock_set->setEnabled(false);
				this->_main_window->dock_img_process_set->setEnabled(false);
				this->_main_window->_img_process_set->save_all_data();

				this->_main_window->startAct->setEnabled(false);
				this->_main_window->recodeAct->setEnabled(false);
			}
		}
	}
}


void MonitorSystem::show_DB_table()
{
	SysDB_view* db_view = SysDB_view::instance(this, _sys_db);
	db_view->show();
}

void MonitorSystem::background_pickup(){

	LoadingDialog *loading_dialog = new LoadingDialog(0, tr("背景提取中..."));
	QThread* thread = new QThread;
	loading_dialog->moveToThread(thread);

	connect(this->_video_processing, &VideoProcessing::background_pickup_done, loading_dialog, &LoadingDialog::close);

	thread->start();
	loading_dialog->exec();

	this->_video_processing->background_pickup();

	//this->opencamera->setEnabled(false);
	//startAct->setEnabled(true);
}