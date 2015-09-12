#include "SystemSet.h"
#include "Button_in_LineEdit.h"
#include<qDebug>

#include<QtWidgets\qspinbox.h>


int get_remain_storage(QString path){
	int storage = 0;
	if (!path.isEmpty()){
		unsigned long long freeBytesToCaller = 0, totalBytes = 0, freeBytes = 0;
		bool b;
		b = GetDiskFreeSpaceEx(QString(path[0] + ":/").toStdWString().c_str(), (PULARGE_INTEGER)&freeBytesToCaller,
			(PULARGE_INTEGER)&totalBytes, (PULARGE_INTEGER)&freeBytes);
		if (b){
			storage = freeBytesToCaller / 1024.0 / 1024.0 / 1024.0;
		}
	}
	return storage;
}


// 读取config.ini文件中数据，赋值给成员变量
SystemSet::SystemSet(QObject *parent) : QObject(parent)
{
	QSettings settings("Resources/config.ini", QSettings::IniFormat); // 当前目录的INI文件

	// 节点设置
	_unit_id = settings.value("UnitInfo/unitID").toString();
	_unit_name = settings.value("UnitInfo/unitName").toString();
	_unit_position = settings.value("UnitInfo/unitPosition").toString();

	// 视频设置
	_file_save_path = settings.value("VideoSet/videoSavePath").toString();
	_video_length = settings.value("VideoSet/videoLength").toInt();
	_real_width = settings.value("VideoSet/RealWidth").toInt();
	
	// 预警设置
	_dritionTime = settings.value("MonitorSet/dritionTime").toInt();
	_speedthreshold = settings.value("MonitorSet/speedthreshold").toInt();
	_WPthreshold = settings.value("MonitorSet/WPthreshold").toInt();

}

SystemSet::~SystemSet()
{

}

//--------------------------------------------------------------------------

SystemSetView_dock* SystemSetView_dock::_instance = nullptr;

SystemSetView_dock::SystemSetView_dock(QWidget *parent, SystemSet *s)
	:QWidget(parent)
{
	this->_sys_set = s;
	this->sure_Button = new QPushButton(tr("确定"));
	sure_Button->setMaximumWidth(80);
	sure_Button->setDefault(true);

	set_up_UI();
	setup_data();

	connect(sure_Button, SIGNAL(clicked()), this, SLOT(button_ok()));

	this->setMaximumWidth(300);
}

void SystemSetView_dock::set_up_UI()
{

	// 1 节点设置
	QLabel *ui_unit_ID_label = new QLabel(this);
	ui_unit_ID_label->setText(tr("节点编号："));
	this->ui_unit_ID_lineedit = new QLineEdit(this);
	
	QLabel *ui_unit_name_label = new QLabel(this);
	ui_unit_name_label->setText(tr("节点名称："));
	ui_unit_name_lineedit = new QLineEdit(this);

	ui_unit_position_lineedit = new QLineEdit(this);
	QLabel *ui_unit_position_label = new QLabel(this);
	ui_unit_position_label->setText(tr("节点位置："));

	// 2 视频设置
	QLabel *ui_path_label = new QLabel(this);
	ui_path_label->setText(tr("视频存储路径："));

	ui_video_save_path = new QLineEdit(this);
	ui_set_path = new Button_in_LineEdit("选择", ui_video_save_path);

	connect(ui_set_path, &QPushButton::clicked, this, &SystemSetView_dock::set_path);

	ui_remaining_space = new QLabel(this);

	QLabel *ui_real_width_labe = new QLabel(this);
	ui_real_width_labe->setText(tr("视频实际宽度："));
	ui_real_width_edit = new QSpinBox(this);
	ui_real_width_edit->setRange(0, 100);       //设置变化范围  
	ui_real_width_edit->setSuffix("cm");        //设置输出显示前缀 

	// 3
	ui_dritionTime = new QSpinBox(this);
	ui_dritionTime->setRange(0, 600);       //设置变化范围  
	ui_dritionTime->setSuffix("秒");        //设置输出显示前缀 
	QLabel* ui_dritionTime_label = new QLabel(this);
	ui_dritionTime_label->setText(tr("异常持续时间："));//异常持续时间

	ui_speedthreshold = new QSpinBox(this);
	QLabel* ui_speedthreshold_label = new QLabel(this);
	ui_speedthreshold_label->setText(tr("速度阈值："));

	ui_WPthreshold = new QSpinBox(this);
	QLabel* ui_WPthreshold_label = new QLabel(this);
	ui_WPthreshold_label->setText(tr("尾频阈值："));

	ui_send_sms_CheckBox = new QCheckBox(tr("检测到异常发送短信提示"));
	ui_send_sms_CheckBox->setChecked(true);
	ui_take_water_CheckBox = new QCheckBox(tr("检测到异常收集水样"));;
	ui_take_water_CheckBox->setChecked(true);

	// 布局
	QGridLayout* ui_grid_layout_main = new QGridLayout(this);
	size_t line_number = 0;

	// 1
	ui_grid_layout_main->addWidget(new QLabel(tr("<b>节点设置<\b>")), line_number++, 0);

	ui_grid_layout_main->addWidget(ui_unit_ID_label,    line_number  , 0);
	ui_grid_layout_main->addWidget(ui_unit_ID_lineedit, line_number++, 1);

	ui_grid_layout_main->addWidget(ui_unit_name_label,    line_number  , 0);
	ui_grid_layout_main->addWidget(ui_unit_name_lineedit, line_number++, 1);

	ui_grid_layout_main->addWidget(ui_unit_position_label,    line_number  , 0);
	ui_grid_layout_main->addWidget(ui_unit_position_lineedit, line_number++, 1);

	// 2
	ui_grid_layout_main->addWidget(new QLabel(tr("<b>视频设置<\b>")), line_number++, 0);

	ui_grid_layout_main->addWidget(ui_path_label,      line_number  , 0);
	ui_grid_layout_main->addWidget(ui_video_save_path, line_number++, 1);

	ui_grid_layout_main->addWidget(new QLabel(tr("剩余空间：")), line_number, 0);

	ui_grid_layout_main->addWidget(ui_remaining_space, line_number++, 1);

	ui_grid_layout_main->addWidget(ui_real_width_labe, line_number  , 0);
	ui_grid_layout_main->addWidget(ui_real_width_edit, line_number++, 1);

	// 3
	ui_grid_layout_main->addWidget(new QLabel(tr("<b>监测设置<\b>")), line_number++, 0);

	ui_grid_layout_main->addWidget(ui_dritionTime_label, line_number, 0);
	ui_grid_layout_main->addWidget(ui_dritionTime, line_number++, 1);

	ui_grid_layout_main->addWidget(ui_speedthreshold_label, line_number, 0);
	ui_grid_layout_main->addWidget(ui_speedthreshold, line_number++, 1);

	ui_grid_layout_main->addWidget(ui_WPthreshold_label, line_number, 0);
	ui_grid_layout_main->addWidget(ui_WPthreshold, line_number++, 1);

	ui_grid_layout_main->addWidget(ui_send_sms_CheckBox, line_number++,0,1,2);
	ui_grid_layout_main->addWidget(ui_take_water_CheckBox, line_number++, 0, 1, 2);

	ui_grid_layout_main->addWidget(sure_Button, line_number++, 0, 1, 2);
	ui_grid_layout_main->addWidget(new QLabel(tr("设置修改后请点击“确定”按钮，否则修改无效")), line_number, 0,1,2);
	
	//this->setMaximumHeight(450);
}

void SystemSetView_dock::setup_data(){

	this->ui_unit_ID_lineedit->setText(this->_sys_set->get_unit_id());
	this->ui_unit_name_lineedit->setText(this->_sys_set->get_unit_name());
	this->ui_unit_position_lineedit->setText(this->_sys_set->get_unit_position());

	this->ui_video_save_path->setText(this->_sys_set->get_file_save_path());
	int storage = get_remain_storage(this->_sys_set->get_file_save_path());
	ui_remaining_space->setText(tr("<font color='#006600'>%0GB").arg(storage));
	this->ui_real_width_edit->setValue(this->_sys_set->get_real_width());

	this->ui_dritionTime->setValue(this->_sys_set->get_dritionTime());
	this->ui_speedthreshold->setValue(this->_sys_set->get_speedthreshold());
	this->ui_WPthreshold->setValue(this->_sys_set->get_WPthreshold());

}

QString SystemSetView_dock::set_path(){

	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	//显示路径
	ui_video_save_path->setText(dir);

	//显示剩余容量
	int storage = get_remain_storage(dir);
	ui_remaining_space->setText(tr("<font color='#006600'>%0GB").arg(storage));

	return dir;
}

void SystemSetView_dock::button_ok()
{
	_sys_set->set_unit_id(this->ui_unit_ID_lineedit->text());
	_sys_set->set_unit_name(this->ui_unit_name_lineedit->text());
	_sys_set->set_unit_position(this->ui_unit_position_lineedit->text());

	//_sys_set->set_video_length(this->ui_realLength_edit->value());
	_sys_set->set_real_width(this->ui_real_width_edit->value());
	_sys_set->set_file_save_path(this->ui_video_save_path->text());

	_sys_set->set_dritionTime(this->ui_dritionTime->value());
	_sys_set->set_speedthreshold(this->ui_speedthreshold->value());
	_sys_set->set_WPthreshold(this->ui_WPthreshold->value());

	sure_Button->setBackgroundRole(QPalette::ColorRole::Dark);
}