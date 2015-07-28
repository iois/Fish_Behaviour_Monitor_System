#include "SystemSet.h"

#include<qDebug>
#include<QSpinBox>


// ��ȡconfig.ini�ļ������ݣ���ֵ����Ա����
SystemSet::SystemSet(QObject *parent) : QObject(parent)
{
	QSettings settings("Resources/config.ini", QSettings::IniFormat); // ��ǰĿ¼��INI�ļ�

	// �ڵ�����
	_unit_id = settings.value("UnitInfo/unitID").toString();
	_unit_name = settings.value("UnitInfo/unitName").toString();
	_unit_position = settings.value("UnitInfo/unitPosition").toString();
	// ��Ƶ����
	_file_save_path = settings.value("SystemSet/videoSavePath").toString();
	_video_length_of_time = settings.value("SystemSet/videoLength").toInt();
	_cameraHeight = settings.value("SystemSet/cameraHeight").toInt();
	_realLength = settings.value("SystemSet/realLength").toInt();
	// Ԥ������
	_dritionTime = settings.value("MonitorSet/dritionTime").toInt();
	_speedthreshold = settings.value("MonitorSet/speedthreshold").toInt();
	_WPthreshold = settings.value("MonitorSet/WPthreshold").toInt();

	_isrecordVideo = settings.value("MonitorSet/isrecordVideo").toInt();
	_isrecordData = settings.value("MonitorSet/isrecordData").toInt();;
}

SystemSet::~SystemSet()
{

}

//------------------------------------------------------------

SystemSetView* SystemSetView::_instance = nullptr;

SystemSetView::SystemSetView(QWidget *parent, SystemSet *s)
	:QWidget(parent)
{
	this->_sys_set = s;
	Init_Gui();
	setup_data();

	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(sure_Button, SIGNAL(clicked()), this, SLOT(button_ok()));

}

void SystemSetView::Init_Gui(){


	// contents Widget
	contentsWidget = new QListWidget;
	contentsWidget->setViewMode(QListView::IconMode);
	//contentsWidget->setIconSize(QSize(96, 84));
	contentsWidget->setMovement(QListView::Static);
	contentsWidget->setMaximumWidth(128);
	contentsWidget->setSpacing(12);
	//contentsWidget->setCurrentRow(0);
	createIcons();

	// pages widget
	pagesWidget = new QStackedWidget(this);

	info_page = new UnitInfoPage(this);
	vidset_page = new VideoSetPage(this);
	monitor_page = new MonitorSetPage(this);

	pagesWidget->addWidget(info_page);
	pagesWidget->addWidget(vidset_page);
	pagesWidget->addWidget(monitor_page);


	// button
	this->closeButton = new QPushButton(tr("ȡ��"));
	this->sure_Button = new QPushButton(tr("ȷ��"));

	QHBoxLayout *buttons_Layout = new QHBoxLayout;
	buttons_Layout->addSpacing(100);
	buttons_Layout->addWidget(sure_Button);
	buttons_Layout->addWidget(closeButton);

	// page layout
	QVBoxLayout *page_layout = new QVBoxLayout;
	page_layout->addWidget(pagesWidget);
	page_layout->addLayout(buttons_Layout);


	// main layout
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(contentsWidget);
	mainLayout->addLayout(page_layout);

	setLayout(mainLayout);
	setWindowTitle(tr("ϵͳ����"));
	this->setWindowOpacity(1);
	//this->setWindowFlags(Qt::FramelessWindowHint);

	this->setMaximumHeight(180);
}

void SystemSetView::createIcons()
{
	QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
	configButton->setIcon(QIcon(":/images/config.png"));
	configButton->setText(tr("�ڵ�����"));
	configButton->setTextAlignment(Qt::AlignHCenter);
	configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QListWidgetItem *updateButton = new QListWidgetItem(contentsWidget);
	updateButton->setIcon(QIcon(":/images/update.png"));
	updateButton->setText(tr("��Ƶ����"));
	updateButton->setTextAlignment(Qt::AlignHCenter);
	updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QListWidgetItem *queryButton = new QListWidgetItem(contentsWidget);
	queryButton->setIcon(QIcon(":/images/query.png"));
	queryButton->setText(tr("Ԥ������"));
	queryButton->setTextAlignment(Qt::AlignHCenter);
	queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	connect(contentsWidget,
		SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
		this, SLOT(changePage(QListWidgetItem*, QListWidgetItem*)));
}

void SystemSetView::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
	if (!current)
		current = previous;
	pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

void SystemSetView::setup_data(){

	this->info_page->ui_unit_ID_lineedit->setText(this->_sys_set->_unit_id);
	this->info_page->ui_unit_name_lineedit->setText(this->_sys_set->_unit_name);
	this->info_page->ui_unit_position_lineedit->setText(this->_sys_set->_unit_position);

	this->vidset_page->ui_video_save_path->setText(this->_sys_set->_file_save_path);

	if (this->monitor_page)
	{
		this->monitor_page->ui_dritionTime->setValue(this->_sys_set->_dritionTime);
		this->monitor_page->ui_speedthreshold->setValue(this->_sys_set->_speedthreshold);
		this->monitor_page->ui_WPthreshold->setValue(this->_sys_set->_WPthreshold);

		if (this->_sys_set->_isrecordData){
			this->monitor_page->ui_recorddata_CheckBox->setCheckState(Qt::Checked);
		}
		else{
			this->monitor_page->ui_recorddata_CheckBox->setCheckState(Qt::Unchecked);
		}

		if (this->_sys_set->_isrecordVideo){
			this->monitor_page->ui_recordviedo_CheckBox->setCheckState(Qt::Checked);
		}
		else{
			this->monitor_page->ui_recordviedo_CheckBox->setCheckState(Qt::Unchecked);
		}
	}

	if (this->vidset_page)
	{
		this->vidset_page->ui_realLength_edit->setValue(this->_sys_set->_realLength);
	}
}

void SystemSetView::button_ok()
{
	_sys_set->set_unit_id(this->info_page->ui_unit_ID_lineedit->text());

	if (this->vidset_page){
		_sys_set->set_file_save_path(this->vidset_page->ui_video_save_path->text());
		_sys_set->set_realLength(this->vidset_page->ui_realLength_edit->text().toInt());
	}
	_sys_set->write_all_data();
	this->close();
}

UnitInfoPage::UnitInfoPage(QWidget *parent) :QWidget(parent)
{
	/*�ڵ�����*/
	QLabel *ui_unit_ID_label = new QLabel(this);
	ui_unit_ID_label->setText(tr("<b>���ڵ�ID��</b>"));

	this->ui_unit_ID_lineedit = new QLineEdit(this);

	ui_unit_name_lineedit = new QLineEdit(this);
	QLabel *ui_unit_name_label = new QLabel(this);
	ui_unit_name_label->setText(tr("<b>���ڵ����ƣ�</b>"));

	ui_unit_position_lineedit = new QLineEdit(this);
	QLabel *ui_unit_position_label = new QLabel(this);
	ui_unit_position_label->setText(tr("<b>���ڵ�λ�ã�</b>"));

	/* layout*/
	QGridLayout   *ui_grid_layout_main = new QGridLayout();
	ui_grid_layout_main->addWidget(ui_unit_ID_label, 0, 0);
	ui_grid_layout_main->addWidget(ui_unit_ID_lineedit, 0, 1);

	ui_grid_layout_main->addWidget(ui_unit_name_label, 1, 0);
	ui_grid_layout_main->addWidget(ui_unit_name_lineedit, 1, 1);

	ui_grid_layout_main->addWidget(ui_unit_position_label, 2, 0);
	ui_grid_layout_main->addWidget(ui_unit_position_lineedit, 2, 1);

	this->setLayout(ui_grid_layout_main);
}

UnitInfoPage::~UnitInfoPage(){}

VideoSetPage::VideoSetPage(QWidget *parent)
{
	QLabel *ui_path_label = new QLabel(this);
	ui_path_label->setText(tr("<b>��Ƶ�洢·����</b>"));

	ui_video_save_path = new QLineEdit(this);
	ui_set_path = new QPushButton(this); ui_set_path->setText(tr("ѡ��·��"));

	connect(ui_set_path, &QPushButton::clicked, this, &VideoSetPage::set_path);

	ui_remaining_space = new QLabel(this);

	QLabel *ui_videoLength_label = new QLabel(this);
	ui_videoLength_label->setText(tr("<b>��Ƶ���ȣ�</b>"));
	ui_videoLength_edit = new QSpinBox(this);

	QLabel *ui_imageScale_labe = new QLabel(this);
	ui_imageScale_labe->setText(tr("<b>����ͷ�߶ȣ�</b>"));
	ui_imageScale_edit = new QSpinBox(this);

	QLabel *ui_realLength_labe = new QLabel(this);
	ui_realLength_labe->setText(tr("<b>��Ƶʵ�ʳ��ȣ�cm����</b>"));
	ui_realLength_edit = new QSpinBox(this);

	/* layout*/
	QGridLayout* ui_grid_layout_main = new QGridLayout(this);
	ui_grid_layout_main->addWidget(ui_path_label, 0, 0);
	ui_grid_layout_main->addWidget(ui_video_save_path, 0, 1);
	ui_grid_layout_main->addWidget(ui_set_path, 0, 2);
	ui_grid_layout_main->addWidget(ui_remaining_space, 1, 1);
	ui_grid_layout_main->addWidget(ui_videoLength_label, 2, 0);
	ui_grid_layout_main->addWidget(ui_videoLength_edit, 2, 1);
	ui_grid_layout_main->addWidget(ui_imageScale_labe, 3, 0);
	ui_grid_layout_main->addWidget(ui_imageScale_edit, 3, 1);

	ui_grid_layout_main->addWidget(ui_realLength_labe, 4, 0);
	ui_grid_layout_main->addWidget(ui_realLength_edit, 4, 1);

	this->setLayout(ui_grid_layout_main);
}

QString VideoSetPage::set_path(){

	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	//��ʾ·��
	ui_video_save_path->setText(dir);

	//��ʾʣ������
	unsigned long long freeBytesToCaller = 0, totalBytes = 0, freeBytes = 0;
	bool b;
	b = GetDiskFreeSpaceEx(QString(dir[0] + ":/").toStdWString().c_str(), (PULARGE_INTEGER)&freeBytesToCaller,
		(PULARGE_INTEGER)&totalBytes, (PULARGE_INTEGER)&freeBytes);

	if (b){
		ui_remaining_space->setText(tr("ʣ��ռ䣺<font color='#006600'>%0GB").arg(freeBytesToCaller / 1024.0 / 1024.0 / 1024.0));
	}

	return dir;
}

MonitorSetPage::MonitorSetPage(QWidget *parent)
{
	ui_dritionTime = new QSpinBox(this);
	QLabel* ui_dritionTime_label = new QLabel(this);
	ui_dritionTime_label->setText(tr("<b>�쳣����ʱ�䣺</b>"));

	ui_speedthreshold = new QSpinBox(this);
	QLabel* ui_speedthreshold_label = new QLabel(this);
	ui_speedthreshold_label->setText(tr("<b>�ٶ���ֵ��</b>"));

	ui_WPthreshold = new QSpinBox(this);
	QLabel* ui_WPthreshold_label = new QLabel(this);
	ui_WPthreshold_label->setText(tr("<b>βƵ��ֵ��</b>"));

	ui_recordviedo_CheckBox = new QCheckBox(tr("��¼��Ƶ"));
	ui_recorddata_CheckBox = new QCheckBox(tr("��¼����"));

	QGridLayout* ui_grid_layout_main = new QGridLayout(this);
	ui_grid_layout_main->addWidget(ui_dritionTime_label, 0, 0);
	ui_grid_layout_main->addWidget(ui_dritionTime, 0, 1);
	ui_grid_layout_main->addWidget(ui_speedthreshold_label, 1, 0);
	ui_grid_layout_main->addWidget(ui_speedthreshold, 1, 1);
	ui_grid_layout_main->addWidget(ui_WPthreshold_label, 2, 0);
	ui_grid_layout_main->addWidget(ui_WPthreshold, 2, 1);
	ui_grid_layout_main->addWidget(ui_recordviedo_CheckBox, 3, 0);
	ui_grid_layout_main->addWidget(ui_recorddata_CheckBox, 4, 0);

	this->setLayout(ui_grid_layout_main);
}

//--------------------------------------------------------------------------

SystemSetView_dock* SystemSetView_dock::_instance = nullptr;

SystemSetView_dock::SystemSetView_dock(QWidget *parent, SystemSet *s)
	:QWidget(parent)
{
	this->_sys_set = s;
	this->sure_Button = new QPushButton(tr("ȷ��"));
	sure_Button->setMaximumWidth(80);

	set_up_UI();
	setup_data();

	connect(sure_Button, SIGNAL(clicked()), this, SLOT(button_ok()));

	this->setMaximumWidth(300);
}

void SystemSetView_dock::set_up_UI()
{


	/*�ڵ�����*/
	QLabel *ui_unit_ID_label = new QLabel(this);
	ui_unit_ID_label->setText(tr("�ڵ�ID��"));

	this->ui_unit_ID_lineedit = new QLineEdit(this);

	ui_unit_name_lineedit = new QLineEdit(this);
	QLabel *ui_unit_name_label = new QLabel(this);
	ui_unit_name_label->setText(tr("�ڵ����ƣ�"));

	ui_unit_position_lineedit = new QLineEdit(this);
	QLabel *ui_unit_position_label = new QLabel(this);
	ui_unit_position_label->setText(tr("�ڵ�λ�ã�"));

	// 2
	QLabel *ui_path_label = new QLabel(this);
	ui_path_label->setText(tr("�洢·����"));

	ui_video_save_path = new QLineEdit(this);
	ui_set_path = new QPushButton(this); 
	ui_set_path->setText(tr("ѡ��·��"));
	ui_set_path->setMaximumWidth(80);

	connect(ui_set_path, &QPushButton::clicked, this, &SystemSetView_dock::set_path);

	ui_remaining_space = new QLabel(this);

	QLabel *ui_imageScale_labe = new QLabel(this);
	ui_imageScale_labe->setText(tr("����ͷ�߶ȣ�"));
	ui_imageScale_edit = new QSpinBox(this);
	ui_imageScale_edit->setRange(0, 200);       //���ñ仯��Χ  
	ui_imageScale_edit->setSuffix("cm");       //���������ʾǰ׺ 

	// 3
	ui_dritionTime = new QSpinBox(this);
	ui_dritionTime->setRange(0, 600);       //���ñ仯��Χ  
	ui_dritionTime->setSuffix("��");       //���������ʾǰ׺ 
	QLabel* ui_dritionTime_label = new QLabel(this);
	ui_dritionTime_label->setText(tr("�쳣ʱ�䣺"));//�쳣����ʱ��

	ui_speedthreshold = new QSpinBox(this);
	QLabel* ui_speedthreshold_label = new QLabel(this);
	ui_speedthreshold_label->setText(tr("�ٶ���ֵ��"));

	ui_WPthreshold = new QSpinBox(this);
	QLabel* ui_WPthreshold_label = new QLabel(this);
	ui_WPthreshold_label->setText(tr("βƵ��ֵ��"));

	ui_recordviedo_CheckBox = new QCheckBox(tr("��¼��Ƶ"));
	ui_recorddata_CheckBox = new QCheckBox(tr("��¼����"));
	ui_recordviedo_CheckBox->setEnabled(false);
	ui_recorddata_CheckBox->setEnabled(false);


	// 
	QGridLayout* ui_grid_layout_main = new QGridLayout(this);

	// 1
	ui_grid_layout_main->addWidget(new QLabel(tr("<b>�ڵ�����<\b>")), 0, 0);
	ui_grid_layout_main->addWidget(ui_unit_ID_label, 1, 0);
	ui_grid_layout_main->addWidget(ui_unit_ID_lineedit, 1, 1);

	ui_grid_layout_main->addWidget(ui_unit_name_label, 2, 0);
	ui_grid_layout_main->addWidget(ui_unit_name_lineedit, 2, 1);

	ui_grid_layout_main->addWidget(ui_unit_position_label, 3, 0);
	ui_grid_layout_main->addWidget(ui_unit_position_lineedit, 3, 1);

	// 2
	ui_grid_layout_main->addWidget(new QLabel(tr("<b>��Ƶ����<\b>")), 5, 0);

	ui_grid_layout_main->addWidget(ui_path_label, 6, 0);
	ui_grid_layout_main->addWidget(ui_video_save_path, 6, 1);
	ui_grid_layout_main->addWidget(ui_set_path, 7, 1);
	ui_grid_layout_main->addWidget(new QLabel(tr("ʣ��ռ䣺")), 8, 0);
	ui_grid_layout_main->addWidget(ui_remaining_space, 8, 1);

	ui_grid_layout_main->addWidget(ui_imageScale_labe, 10, 0);
	ui_grid_layout_main->addWidget(ui_imageScale_edit,	10, 1);

	// 3
	ui_grid_layout_main->addWidget(new QLabel(tr("<b>�������<\b>")), 11, 0);

	ui_grid_layout_main->addWidget(ui_dritionTime_label, 12, 0);
	ui_grid_layout_main->addWidget(ui_dritionTime, 12, 1);
	ui_grid_layout_main->addWidget(ui_speedthreshold_label, 13, 0);
	ui_grid_layout_main->addWidget(ui_speedthreshold, 13, 1);
	ui_grid_layout_main->addWidget(ui_WPthreshold_label, 14, 0);
	ui_grid_layout_main->addWidget(ui_WPthreshold, 14, 1);
	ui_grid_layout_main->addWidget(ui_recordviedo_CheckBox, 15, 0);
	ui_grid_layout_main->addWidget(ui_recorddata_CheckBox, 16, 0);

	ui_grid_layout_main->addWidget(sure_Button,18, 1);

	this->setMaximumHeight(450);
}

void SystemSetView_dock::setup_data(){

	this->ui_unit_ID_lineedit->setText(this->_sys_set->_unit_id);
	this->ui_unit_name_lineedit->setText(this->_sys_set->_unit_name);
	this->ui_unit_position_lineedit->setText(this->_sys_set->_unit_position);

	this->ui_video_save_path->setText(this->_sys_set->_file_save_path);

	this->ui_dritionTime->setValue(this->_sys_set->_dritionTime);
	this->ui_speedthreshold->setValue(this->_sys_set->_speedthreshold);
	this->ui_WPthreshold->setValue(this->_sys_set->_WPthreshold);

	if (this->_sys_set->_isrecordData){
		this->ui_recorddata_CheckBox->setCheckState(Qt::Checked);
	}
	else{
		this->ui_recorddata_CheckBox->setCheckState(Qt::Unchecked);
	}

	if (this->_sys_set->_isrecordVideo){
		this->ui_recordviedo_CheckBox->setCheckState(Qt::Checked);
	}
	else{
		this->ui_recordviedo_CheckBox->setCheckState(Qt::Unchecked);
	}

	/*
	{
	this->ui_realLength_edit->setValue(this->_sys_set->_realLength);
	}
	*/
}



QString SystemSetView_dock::set_path(){

	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	//��ʾ·��
	ui_video_save_path->setText(dir);

	//��ʾʣ������
	int storage = get_remain_storage(dir);
	ui_remaining_space->setText(tr("<font color='#006600'>%0GB").arg(storage));

	return dir;
}
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


void SystemSetView_dock::button_ok()
{
	_sys_set->set_unit_id(this->ui_unit_ID_lineedit->text());

	_sys_set->set_file_save_path(this->ui_video_save_path->text());

	_sys_set->write_all_data();

	sure_Button->setBackgroundRole(QPalette::ColorRole::Dark);
}