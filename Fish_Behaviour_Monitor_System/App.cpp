#include "App.h"

App::App()
{
	_imgp_set = new ImgProcessSet();

	_sys_set  = new SystemSet(this);

	_sys_db = new SysDB();
	_sys_db->AddConnection("QSQLITE", "data.db", "", "", "", 0);//连接到本地数据库文件data.db

	_t = new QTimer(this);

	_video_processing = new VideoProcessing(this, _sys_set, _imgp_set);
	_main_window = new MainWindow(0, _video_processing);

	_thread_videoprocessing = new QThread(this);
	_video_processing->moveToThread(_thread_videoprocessing); //将视频处理类移动到新线程_thread_videoprocessing
	_thread_videoprocessing->start();

	_water_taking_siganl_sender = new SendWaterTakingSignal();

	_sms_sender = new SendSMS();

	//_main_window->showFullScreen();// 最大化显示主窗口（无边框，无最小化最大化窗口）
	_main_window->showMaximized();  // 最大化显示主窗口

	connect(_t, &QTimer::timeout, _video_processing, &VideoProcessing::time_out_todo_1);
}


App::~App()
{

}
