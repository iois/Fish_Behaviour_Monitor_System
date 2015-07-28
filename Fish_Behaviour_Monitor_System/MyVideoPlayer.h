#pragma execution_character_set("utf-8")
#pragma once
#include<QtWidgets\qapplication.h>
#include <QtWidgets\qwidget.h>
#include <QtWidgets\qslider.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qcombobox.h>
#include <QtWidgets\qlayout.h>
#include <QtCore\qtimer.h>
#include "ImgShowWidget.h"

// �Լ�д��������ʾ��Ƶ����
// ������ִ��Open_File(QString filename);
// ������Ƶ�ļ�������ʼ���ţ���������������Open_File
// ����ͨ���ⲿ��timerָ�봫�������ƣ�Ҳ�����Լ��ڲ���timer����
class MyVideoPlayer : public QWidget
{
public:
	//����timer�������ⲿ���룬��û�У����Լ�����
	MyVideoPlayer(QWidget* parent, QTimer* timer);
	~MyVideoPlayer();

	void set_size(QSize size); //�ı��С

public:
	bool Open_File(QString filename);

	public slots:
	void nextFrame();
	void CallBackTrackBarSlide(int i);

	void playStart();
	void playStop();
	void play_speed_change(const QString &);

public:
	QSlider *_grogressbar_horizontalSlider;
	QComboBox* _videspeed_comboBox;
protected:
	ImgShowWidget* videoplayer;

	QPushButton *_start_pushButton;
	QPushButton *_pause_pushButton;

	QLabel *_time_label;
	QLabel *_time_label_2;

	void setupUI(){
		// [0]
		QHBoxLayout *hLayout_main = new QHBoxLayout(this);
		// [1]
		QVBoxLayout *vLayout_img_Widgets = new QVBoxLayout();

		// [1][1]
		videoplayer = new ImgShowWidget(this);
		// [1][2]
		QHBoxLayout* hLayout_control_bar = new QHBoxLayout();
		// [1][2][1]
		_start_pushButton = new QPushButton(this); _start_pushButton->setText(tr("��ʼ"));
		_start_pushButton->setMaximumWidth(50);
		// [1][2][2]
		_pause_pushButton = new QPushButton(this); _pause_pushButton->setText(tr("��ͣ"));
		_pause_pushButton->setMaximumWidth(50);
		// [1][2][3]
		_time_label = new QLabel(this);
		// [1][2][4]
		_time_label_2 = new QLabel(this);
		// [1][2][5]
		QSpacerItem * horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Maximum);
		// [1][2][6]
		QLabel *videspeed_label = new QLabel(this);
		// [1][2][7]
		_videspeed_comboBox = new QComboBox(this);
		_videspeed_comboBox->insertItems(0, QStringList()
			<< QApplication::translate("VideoPlayer", "1", 0)
			<< QApplication::translate("VideoPlayer", "2", 0)
			<< QApplication::translate("VideoPlayer", "3", 0)
			<< QApplication::translate("VideoPlayer", "4", 0)
			);
		// [1][2][add]
		hLayout_control_bar->addWidget(_start_pushButton);
		hLayout_control_bar->addWidget(_pause_pushButton);
		hLayout_control_bar->addWidget(_time_label);
		hLayout_control_bar->addWidget(_time_label_2);
		hLayout_control_bar->addItem(horizontalSpacer);
		hLayout_control_bar->addWidget(videspeed_label);
		hLayout_control_bar->addWidget(new QLabel(tr("�����ٶȣ�")));
		hLayout_control_bar->addWidget(_videspeed_comboBox);
		// [1][3]
		_grogressbar_horizontalSlider = new QSlider(this);
		_grogressbar_horizontalSlider->setOrientation(Qt::Horizontal);
		_grogressbar_horizontalSlider->setInvertedControls(false);
		_grogressbar_horizontalSlider->setTickPosition(QSlider::NoTicks);
		// [1][add]
		vLayout_img_Widgets->addWidget((QWidget*)videoplayer);
		vLayout_img_Widgets->addLayout(hLayout_control_bar);
		vLayout_img_Widgets->addWidget(_grogressbar_horizontalSlider);

		// [0][add]
		hLayout_main->addLayout(vLayout_img_Widgets);
	}

protected:
	QString _file_name;
	QTimer *_timer;

	CvCapture *_capture;
	IplImage  *_iplImg;
	IplImage  *_frame;

	QImage *_qImg;


	int _Position;        // ��Ƶ֡λ��
	int _totalFrames;     // ��Ƶ����֡��
public:
	bool _play_flag;      // ��־��Ƶ���Ż���ͣ
};
