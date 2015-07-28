#include "MyVideoPlayer.h"
#include<qdebug.h>
#include<QMessageBox>

MyVideoPlayer::MyVideoPlayer(QWidget* parent, QTimer* timer)
	:QWidget(parent), _timer(timer)
{
	setupUI();
	this->setFixedSize(340, 320);
	_qImg = nullptr;
	_capture = nullptr;
	_frame = nullptr;
	//this->setFixedSize(640, 560);

	_Position = 0;
	_play_flag = false;

	//�������Ĳ���timerΪ�գ����Լ�����һ����
	if (!_timer)
	{
		_timer = new QTimer(this);
	}
	connect(_timer, &QTimer::timeout, this, &MyVideoPlayer::nextFrame);
	connect(this->_start_pushButton, &QPushButton::clicked, this, &MyVideoPlayer::playStart);
	connect(this->_pause_pushButton, &QPushButton::clicked, this, &MyVideoPlayer::playStop);
	connect(this->_videspeed_comboBox, &QComboBox::currentTextChanged, this, &MyVideoPlayer::play_speed_change);

	//Open_File("C:/Users/liyi/Videos/test_6fish.AVI");
}


MyVideoPlayer::~MyVideoPlayer()
{
}

void MyVideoPlayer::set_size(QSize size)
{
	this->videoplayer->set_size(size);
}


bool MyVideoPlayer::Open_File(QString filename){
	_file_name = filename;
	char*  ch;
	QByteArray ba = _file_name.toLatin1();
	ch = ba.data();

	this->setWindowTitle(_file_name);

	_capture = cvCaptureFromFile(ch);

	if (_capture){

		_totalFrames = (int)cvGetCaptureProperty(_capture, CV_CAP_PROP_FRAME_COUNT);
		//qDebug() << _totalFrames;
		this->_grogressbar_horizontalSlider->setRange(0, _totalFrames);

		int sec = _totalFrames / 15;
		int min = sec / 60; sec = sec % 60;
		int h = min / 60; min = min % 60;
		if (sec < 10 && min < 10){
			this->_time_label_2->setText(tr("/ %0:0%1:0%2").arg(h).arg(min).arg(sec));
		}
		else if (sec < 10){
			this->_time_label_2->setText(tr("/ %0:%1:0%2").arg(h).arg(min).arg(sec));
		}
		else if (min < 10){
			this->_time_label_2->setText(tr("/ %0:0%1:%2").arg(h).arg(min).arg(sec));
		}
		else{
			this->_time_label_2->setText(tr("/ %0:%1:%2").arg(h).arg(min).arg(sec));
		}

		_frame = cvQueryFrame(_capture);

		_qImg = new QImage(QSize(_frame->width, _frame->height), QImage::Format_RGB888);
		_iplImg = cvCreateImageHeader(cvSize(_frame->width, _frame->height), 8, 3);
		_iplImg->imageData = (char*)_qImg->bits();

		cvCopy(_frame, _iplImg, 0);
		cvCvtColor(_iplImg, _iplImg, CV_BGR2RGB);
		this->videoplayer->show_img(_qImg);

		connect(this->_grogressbar_horizontalSlider, &QSlider::valueChanged, this, &MyVideoPlayer::CallBackTrackBarSlide);

		_timer->start();
		_play_flag = true;

		this->update();

		return true;
	}
	else{
		QMessageBox::information(this, tr("��ʾ"), tr("�޷���%0��").arg(filename));
		return false;
	}
}



void MyVideoPlayer::nextFrame(){
	if (_capture)
	{
		_frame = cvQueryFrame(_capture);
	}
	if (_frame){
		cvCopy(_frame, _iplImg, 0);
		cvCvtColor(_iplImg, _iplImg, CV_BGR2RGB);

		this->videoplayer->show_img(_qImg);

		_Position++;
		if (_Position % 15 == 0)
		{
			this->_grogressbar_horizontalSlider->setValue(_Position);
			//data_cruve_widget_1->timeouttodo();
		}
	}
	else { //video end
		_timer->stop();
		QMessageBox::information(this, tr("��ʾ"), tr("��Ƶ������"));
		//this->close();//�رյ�ǰ����
	}
}

void MyVideoPlayer::CallBackTrackBarSlide(int i){

	//data_cruve_widget_1->goto_x(i / 15);

	cvSetCaptureProperty(_capture, CV_CAP_PROP_POS_FRAMES, i);
	_Position = i;

	int sec = _Position / 15;
	int min = sec / 60; sec = sec % 60;
	int h = min / 60; min = min % 60;
	if (sec < 10 && min < 10){
		this->_time_label->setText(tr("%0:0%1:0%2").arg(h).arg(min).arg(sec));
	}
	else if (sec < 10){
		this->_time_label->setText(tr("%0:%1:0%2").arg(h).arg(min).arg(sec));
	}
	else if (min < 10){
		this->_time_label->setText(tr("%0:0%1:%2").arg(h).arg(min).arg(sec));
	}
	else{
		this->_time_label->setText(tr("%0:%1:%2").arg(h).arg(min).arg(sec));
	}

	_frame = cvQueryFrame(_capture);
	cvCopy(_frame, _iplImg, 0);
	cvCvtColor(_iplImg, _iplImg, CV_BGR2RGB);
	this->videoplayer->show_img(_qImg);
}

void MyVideoPlayer::playStart(){
	_timer->start();
	_play_flag = true;
}

void MyVideoPlayer::playStop(){
	_timer->stop();
	_play_flag = false;
}

// ���������ٶ�
// ͨ����С�ȴ�ʱ�������������ٶ�Ϊ4��ʱ�򣬳����ر��cpu������д��Ľ���
void MyVideoPlayer::play_speed_change(const QString &){
	int play_speed = this->_videspeed_comboBox->currentText().toInt();
	if (_play_flag){
		_timer->stop();
		_timer->start(66 / play_speed);
	}
	else{
		_timer->start(66 / play_speed);
		_timer->stop();
	}
}