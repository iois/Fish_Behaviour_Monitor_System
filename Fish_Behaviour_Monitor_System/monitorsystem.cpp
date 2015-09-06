#include "monitorsystem.h"
#include<QtCore\qdebug.h>

MonitorSystem::MonitorSystem(QWidget *parent): QWidget(parent)
{
	init();

	connect_signal_slot();

	// add to mainwindow
	_main_window->dock_img_process_set->setWidget(img_p_set);

	set_view = SystemSetView_dock::instance(_main_window->dock_set, _sys_set);

	// add to mainwindow
	_main_window->dock_set->setWidget(set_view);

	// show main window
	_main_window->showMaximized();

	// 追加方式打开日志文件
	LOG.open(LOG_FILE_NAME, ios::app);
	QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
	LOG << "--------------------------------------" << endl;
	LOG << current_date.toStdString() << " : open system." << endl;
}

MonitorSystem::~MonitorSystem()
{
	if (_video_Writer.isOpened()){
		_video_Writer.release();
	}

	_data_writer_1.close();
	_data_writer_2.close();
	_data_writer_3.close();
}

void MonitorSystem::init(){
	//init
	_imgp_set = new ImgProcessSet();
	_sys_set = new SystemSet(this);

	_sys_db = new SysDB();
	_sys_db->AddConnection("QSQLITE", "data.db", "", "", "", 0);//连接到本地数据库文件data.db

	_t = new QTimer(this);

	_video_processing = new VideoProcessing(this, _sys_set, _imgp_set);

	_main_window = new MainWindow(0, _video_processing);

	_thread_videoprocessing = new QThread(this);
	_video_processing->moveToThread(_thread_videoprocessing);

	_thread_videoprocessing->start();

	_water_taking_siganl_sender = new SendWaterTakingSignal();

	_sms_sender = new SendSMS();

	img_p_set = new ImgProcessSet_view(_imgp_set);

	//init over-----------------------------------------------------//
}

void MonitorSystem::connect_signal_slot(){
	//connect
	connect(_main_window->opencamera, &QAction::triggered, this, &MonitorSystem::open_camera);
	connect(_main_window->openfile, &QAction::triggered, this, &MonitorSystem::open_file);
	connect(_main_window->startAct, &QAction::triggered, this, &MonitorSystem::process_start);
	connect(_main_window->endAct, &QAction::triggered, this, &MonitorSystem::process_end);
	connect(_main_window->recodeAct, &QAction::triggered, this, &MonitorSystem::record);
	connect(_main_window->exitAct, &QAction::triggered, this, &MonitorSystem::exit);

	connect(_main_window->DB_manage_Act, &QAction::triggered, this, &MonitorSystem::show_DB_table);

	//connect(_main_window->background_pickup_Act, &QAction::triggered, this, &MonitorSystem::background_pickup);

	connect(_t, &QTimer::timeout, this, &MonitorSystem::time_out_todo);

	connect(_video_processing, &VideoProcessing::send_data_signal, this, &MonitorSystem::receive_data);
}

void MonitorSystem::open_camera(){
	//qDebug() << "monitorSystem open camera";
	if (!this->_video_processing->open_camera()){
		QMessageBox::information(this->_main_window, tr("警告"), tr("无法打开摄像头!"));
		return ;
	}
	_t->start(TIME_INTERVAL);

	_main_window->opencamera->setEnabled(false);
	_main_window->startAct->setEnabled(true);
}

void MonitorSystem::open_file()
{
	QString path_filename = QFileDialog::getOpenFileName(this, tr("打开视频文件"), ".", tr("(*.avi)"));

	if (path_filename.isEmpty()){
		QMessageBox::information(this, tr("警告"), tr("无法打开文件!"));
	}
	else
	{
		this->_video_processing->open_file(path_filename.toStdString());

		this->_main_window->openfile->setEnabled(false);
		this->_main_window->opencamera->setEnabled(false);
		this->_main_window->startAct->setEnabled(true);

		_t->stop();
	}
}

void MonitorSystem::process_start(){
	if ( _video_processing ){
		_video_processing->process_begin();
		_t->start(NUM_FRAMES);

		if (this->_imgp_set->get_num_fish() > 1){
			this->_main_window->tabWidget->setCurrentWidget(this->_main_window->data_show_2);
		}
	}
	this->_main_window->startAct->setEnabled(false);
	this->_main_window->recodeAct->setEnabled(true);
	this->_main_window->endAct->setEnabled(true);
};

void MonitorSystem::record()
{
	if (this->_video_processing->_isPrecess){
		_isRecord = 1;
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

		// 删除最早的文件
		{
			int storage = get_remain_storage(this->_sys_set->get_file_save_path());

			if (storage < REMAIN_STORAGE){  //5:硬盘剩余空间小于5G
				// 删除最早的文件
				QString file_del = this->_sys_db->get_del_file_name();
				if (!file_del.isEmpty()){
					QFile fileTemp(file_del);
					fileTemp.remove();
				}
			}
		}

		QString current_date = QDateTime::currentDateTime().toString("yyyyMMddhhmm");

		this->_video_id = current_date;
		// 保存数据的文件名
		QString new_datafile_name = _sys_set->get_file_save_path() + '/' + current_date;
		// 保存视频的文件名
		QString new_video_name = new_datafile_name + ".avi";
		// 打开视频保存流
		_video_Writer.open(new_video_name.toStdString(), CODEC, FPS, _video_processing->get_img_size(), 1);
		
		if (_video_Writer.isOpened())
		{
			if (_imgp_set->get_num_fish() == 1){
				_data_writer_1.open((new_datafile_name + "_1.txt").toStdString());
				_data_writer_2.open((new_datafile_name + "_2.txt").toStdString());
			}else if (_imgp_set->get_num_fish() > 1){
				_data_writer_3.open((new_datafile_name + "_3.txt").toStdString());
			}

			// 数据库中写入新的纪录
			_sys_db->InsertNewRecord(
				_video_id,
				_sys_set->get_file_save_path(),
				_imgp_set->get_num_fish(),
				QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"),
				""/*remark*/);

			this->_main_window->recodeAct->setIcon(QIcon("images/recording.ico"));
			this->_main_window->statusBar()->showMessage(tr("正在记录视频与数据"));

			this->_main_window->dock_set->setEnabled(false);
			this->_main_window->dock_img_process_set->setEnabled(false);

			this->_imgp_set->save_all_data();

			this->_main_window->opencamera->setEnabled(false);
			this->_main_window->openfile->setEnabled(false);
			this->_main_window->startAct->setEnabled(false);
			this->_main_window->recodeAct->setEnabled(false);
		}
		else{
			QMessageBox::information(nullptr, tr("警告"), tr("无法保存视频!"));
		}
	}
}

void MonitorSystem::process_end(){
	if (_video_processing && _video_processing->_isPrecess){
		_video_processing->process_end();

		if (_video_Writer.isOpened()){
			_video_Writer.release();
		}
		if (_data_writer_1.is_open()) _data_writer_1.close();
		if (_data_writer_2.is_open()) _data_writer_2.close();
		if (_data_writer_3.is_open()) _data_writer_3.close();
		//todo
		_sys_db->InsertNewRecord_endtime(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"), _video_id);
	}
	this->_main_window->startAct->setEnabled(true);
	this->_main_window->endAct->setEnabled(false);
	this->_main_window->recodeAct->setIcon(QIcon("images/record.ico"));

	this->_main_window->startAct->setEnabled(true);
	this->_main_window->recodeAct->setEnabled(true);

	this->_main_window->dock_set->setEnabled(true);
	this->_main_window->dock_img_process_set->setEnabled(true);

	this->_main_window->opencamera->setEnabled(true);
	this->_main_window->openfile->setEnabled(true);
	// todo:
	// 显示数据 那块 清空
}

void MonitorSystem::exit(){

	if (_video_processing){ process_end(); }
	SysDB_view* db_view = SysDB_view::instance(0, _sys_db);
	if (db_view){ db_view->close(); }
	if (_main_window){ _main_window->close(); }

	LOG.close();
	this->close();
}

//-----------------------------------------------------------

void MonitorSystem::show_DB_table()
{
	qDebug() << "db_view";
	SysDB_view* db_view = SysDB_view::instance(0, _sys_db);
	db_view->show();
}

void MonitorSystem::background_pickup(){

	LoadingDialog *loading_dialog = new LoadingDialog(0, tr("背景提取中..."));
	QThread* thread = new QThread;
	loading_dialog->moveToThread(thread);

	connect(this->_video_processing, &VideoProcessing::background_pickup_done, loading_dialog, &LoadingDialog::close);

	thread->start();
	loading_dialog->show();

	this->_video_processing->background_pickup();

	//this->opencamera->setEnabled(false);
	//startAct->setEnabled(true);
}

//------------------------------------------------------
// 监测一段时间（24小时）后，保存视频文件与数据，再自动新建一个监测
// 自动结束
void MonitorSystem::auto_monitor_end(){  // 主要功能：close video_writer
	LOG << "auto_monitor_end : " << _video_id.toStdString() << endl;
	_t->stop();
	if (_video_Writer.isOpened()){_video_Writer.release();}
	if (_data_writer_1.is_open()) _data_writer_1.close();
	if (_data_writer_2.is_open()) _data_writer_2.close();
	if (_data_writer_3.is_open()) _data_writer_3.close();
	//todo
	_sys_db->InsertNewRecord_endtime(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"), _video_id);
	_num_of_frames_recoded = 0;
}
// 自动新检测
void MonitorSystem::auto_new_monitor(){// 主要功能：open a new video_writer

	QString current_date = QDateTime::currentDateTime().toString("yyyyMMddhhmm");

	this->_video_id = current_date;
	// 保存数据的文件名
	QString new_datafile_name = _sys_set->get_file_save_path() + '/' + current_date;
	// 保存视频的文件名
	QString new_video_name = new_datafile_name + ".avi";
	// 打开视频保存流
	_video_Writer.open(new_video_name.toStdString(), CODEC, FPS, _video_processing->get_img_size(), 1);

	if (_video_Writer.isOpened())
	{
		if (_imgp_set->get_num_fish() == 1){
			_data_writer_1.open((new_datafile_name + "_1.txt").toStdString());
			_data_writer_2.open((new_datafile_name + "_2.txt").toStdString());
		}
		else if (_imgp_set->get_num_fish() > 1){
			_data_writer_3.open((new_datafile_name + "_3.txt").toStdString());
		}

		// 数据库中写入新的纪录
		_sys_db->InsertNewRecord(
			_video_id,
			_sys_set->get_file_save_path(),
			_imgp_set->get_num_fish(),
			QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"),
			""/*remark*/);

		//_video_processing->process_begin();
		_t->start();
		LOG << "auto_new_monitor : " << _video_id.toStdString() << endl;
	}
	else{
		QMessageBox::information(nullptr, tr("警告"), tr("无法保存视频!"));
	}
}
//------------------------------------------------------
void MonitorSystem::time_out_todo(){

	if (!_video_processing)
	{
		//qDebug() << "can not save video";
		return;
	}

	// [1] 处理视频中获取的图片
	_video_processing->time_out_todo_1();

	_detect_fish_death.input();

	// [2] 如果是在纪录状态（_isRecord） && 视频保存流打开,则 存储视频与数据
	if (_isRecord && _video_Writer.isOpened()){

		//视频存储达到指定的时长，则关闭，重新纪录
		if (_num_of_frames_recoded > VIDEO_LENGTH){
			auto_monitor_end();
			auto_new_monitor();
		}

		// 保存视频文件
		save_video(_video_processing->get_original_img());//保存视频

		//_video_processing 发送处理后的数据，
		// this接收，保存 ： receive_data（）中处理了，信号槽自动调用
		++_num_of_frames;
	}
	else{}
	
}

void MonitorSystem::save_video(const cv::Mat &image){
	if (!image.empty()){
		_video_Writer << image;//_video_processing->get_original_img();
		++_num_of_frames_recoded;
	}else{
		this->_main_window->statusBar()->showMessage(tr("无法保存视频"));
		//todo : 退出
	}
}

void MonitorSystem::receive_data(int index, double val){

	static double speed = 0;
	static int    wp_count = 0;
	static int    wp_old_dtat = 1;
	static int    r = 0;

	switch (index){
	case 1: {// 速度
				if (_num_of_frames % 15 == 0){
					// 数据显示，保存
					_main_window->updata_data(1, speed);
					this->save_data(1, speed);
					speed = 0;
				}
				else{
					speed += val;
				}
				break;
	}
	case 2:{
			   if (_num_of_frames % 15 == 0){
				   _main_window->updata_data(2, wp_count);
				   this->save_data(2, wp_count);
				   wp_count = 0;
			   }
			   else{
				   if ((wp_old_dtat == 1 && val == 2) || (wp_old_dtat == 2 && val == 1)){
					   ++wp_count;
					   wp_old_dtat = val;
				   }
			   }
			   break;
	}
	case 3:{
			   if (_num_of_frames % 15 == 0){
				   _main_window->updata_data(3, r/15);
				   this->save_data(3, r / 15);
				   r = 0;
			   }
			   else{
				   r += val;
			   }
			   break;
	}
	}
}

void MonitorSystem::save_data(int index, double val){
	if (!_isRecord){
		return;
	}
	//else
	switch (index){
		case 1: // 速度
		{
					_data_writer_1 << (val) << " ";	// 数据写入对应文件中
					break;
		}
		case 2: //
		{
					_data_writer_2 << (val) << " ";	// 数据写入对应文件
					break;
		}
		case 3: //群聚特征，
		{
					_data_writer_3 << (val) << " ";	// 数据写入对应文件中
					break;
		}
		default:
			break;
	}
}

/*
void VideoProcessing::send_data(size_t modeIndex, double data){
	static double speed = 0;
	static int duration = 4;//异常持续时间
	static int wp_count = 0;
	static int old_data = 0;  //前一个数据

	static int r = 0;
	static int r_count = 0;
	static int is_warning = 0;

	static int speedcount = 0;
	static int wpcount = 0;

	QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

	switch (modeIndex)
	{
	case 1: // 速度
	{
	if (_num_of_frames % 15 == 0){
	// 数据显示，保存
	_main_window->updata_data(1, data);

	if (_isRecord){
	_data_writer_1 << (data) << " ";	// 数据写入对应文件中

	// 预警检测
	if (data > 2){
	++speedcount;
	if (speedcount > duration && is_warning == 0){
	_main_window->ui_warning_view->add_warning_item(1, 1, "速度异常 " + current_date);
	_sys_db->Insert_warning(1, 0, modeIndex);
	is_warning = 1;
	}
	}
	else {
	speedcount = 0;
	is_warning = 0;
	}
	}
	}
	break;
	}

	case 2: //
	{
	if (_num_of_frames % 15 == 0){
	_main_window->updata_data(2, wp_count);

	if (_isRecord){
	_data_writer_2 << (wp_count) << std::endl;	// 数据写入对应文件中

	if (data > 1.5){
	++wpcount;
	if (wpcount > duration && is_warning == 0){
	_main_window->ui_warning_view->add_warning_item(1, 2, "尾频异常 " + current_date);
	_sys_db->Insert_warning(2, 0, modeIndex);
	is_warning = 1;
	}
	}
	else{
	wpcount = 0;
	is_warning = 0;
	}
	}
	wp_count = 0;
	old_data = data;
	}
	else{
	if ((old_data == 1 && data == 2) || (old_data == 2 && data == 1)){
	++wp_count;
	old_data = data;
	}
	}
	break;
	}
	case 3: //群聚特征，
	{
	if (_num_of_frames % 15 == 0){

	_main_window->updata_data(3, r / 15);
	if (_isRecord){
	_data_writer_3 << (r / 15) << std::endl;	// 数据写入对应文件中

	if (data > 50){
	++r_count;
	if (r_count > duration && is_warning == 0){
	_main_window->ui_warning_view->add_warning_item(1, 3, "半径异常 " + current_date);
	_sys_db->Insert_warning(3, 0, modeIndex);
	is_warning = 1;
	}
	}
	else{
	r_count = 0;
	is_warning = 0;
	}
	}
	r = 0;
	}
	else {
	r += data;
	}
	break;
	}
	default:
	break;
	}
	
}
*/