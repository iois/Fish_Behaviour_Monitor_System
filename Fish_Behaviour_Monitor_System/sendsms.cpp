#include "sendsms.h"

#include<qdebug.h>
#include<QTime>

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>

#include <QtSerialPort/QSerialPortInfo>

SendSMS::SendSMS() :_waitTimeout(100), _baud_rate(9600)
{
    //检测串口
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
         _serial_ports.push_back(info.portName());
    }
    //若有串口
    if(!_serial_ports.empty()){
        this->_portName=_serial_ports[0];
    }
    else{//没有串口；
        qDebug()<<"no chuankou";
    }
    connect(&_thread, &MasterThread::response, this, &SendSMS::emit_response);
    connect(&_thread, &MasterThread::error, this, &SendSMS::emit_error);
    connect(&_thread, &MasterThread::timeout, this, &SendSMS::emit_timeout);
}

bool SendSMS::send(const QString &number,const QString &message){

    if(number.isEmpty()){
        return false;
    }

    QString request=QString(number)+QString(":0:")+message;
    _thread.transaction(this->_portName,
                        this->_waitTimeout,
                        request);

    //等待10s发送给第二个人
    //QTime t;
    //t.start();
    //while(t.elapsed()<1000*10){}

    return true;
}

void SendSMS::set_waitTimeout(int val){
    this->_waitTimeout=val;
}

void SendSMS::set_serial_port(const QString &serial_port){
	this->_portName = serial_port;
}

void SendSMS::set_baud_rate(const QString &baudrate){
	this->_baud_rate = baudrate.toInt();
}

//----------------------------------------------------------------

SendSMS_view::SendSMS_view(QWidget *parent, SendSMS *sendsms)
    : QWidget(parent)
    , transactionCount(0)
    , serialPortLabel(new QLabel(tr("Serial port:")))
    , serialPortComboBox(new QComboBox())
    , waitResponseLabel(new QLabel(tr("Wait response, msec:")))
    , waitResponseSpinBox(new QSpinBox())
	, numberLabel(new QLabel("号码"))
	, numberLineEdit(new QLineEdit(tr("15068855805")))
    , messageLabel(new QLabel(tr("message:")))
    , messageLineEdit(new QLineEdit(tr("Who are you?")))
    , trafficLabel(new QLabel(tr("No traffic.")))
    , statusLabel(new QLabel(tr("Status: Not running.")))
    , runButton(new QPushButton(tr("Start")))
    ,send_sms(sendsms)
{
    if(send_sms){
        foreach (const QString &info, send_sms->get_serial_ports())
            serialPortComboBox->addItem(info);


        QGridLayout *mainLayout = new QGridLayout;
        mainLayout->addWidget(serialPortLabel, 0, 0);
        mainLayout->addWidget(serialPortComboBox, 0, 1);
        mainLayout->addWidget(waitResponseLabel, 1, 0);
        mainLayout->addWidget(waitResponseSpinBox, 1, 1);
        mainLayout->addWidget(runButton, 0, 2, 2, 1);

		mainLayout->addWidget(numberLabel, 2, 0);
		mainLayout->addWidget(numberLineEdit, 2, 1, 1, 3);

		mainLayout->addWidget(messageLabel, 3, 0);
		mainLayout->addWidget(messageLineEdit, 3, 1, 1, 3);

        mainLayout->addWidget(trafficLabel, 4, 0, 1, 4);
        mainLayout->addWidget(statusLabel, 5, 0, 1, 5);
        setLayout(mainLayout);

        setWindowTitle(tr("短信串口设置"));
        serialPortComboBox->setFocus();

        waitResponseSpinBox->setRange(0, 1000*180);
        waitResponseSpinBox->setValue(1000*120);

        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间

        QString str = time.toString("yyyy-MM-dd hh:mm"); //设置显示格式
        QString message="[监测信息,测试短信，内容虚构]\n"+str+" ，站点I\n发生异常，检测出死亡";
		messageLineEdit->setText(message);

        connect(runButton, &QPushButton::clicked,this, &SendSMS_view::transaction);

		connect(send_sms, &SendSMS::response, this, &SendSMS_view::showResponse);

		connect(send_sms, &SendSMS::error, this, &SendSMS_view::processError);

		connect(send_sms, &SendSMS::timeout, this, &SendSMS_view::processTimeout);
    }
}


void SendSMS_view::transaction()
{
    setControlsEnabled(false);
	if (send_sms->send(numberLineEdit->text(), messageLineEdit->text())){
        statusLabel->setText(tr("Status: Running, connected to port %1.")
                             .arg(serialPortComboBox->currentText()));
    }
    else{
        statusLabel->setText(tr("Status: Running, connected to port %1. no number")
                             .arg(serialPortComboBox->currentText()));
        setControlsEnabled(true);
    }
}

void SendSMS_view::showResponse(const QString &s)
{
    setControlsEnabled(true);
    trafficLabel->setText(tr("Traffic, transaction #%1:"
                             "\n\r-request: %2"
                             "\n\r-response: %3")
                          .arg(++transactionCount)
						  .arg(messageLineEdit->text())
                          .arg(s));
}

void SendSMS_view::processError(const QString &s)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Not running, %1.").arg(s));
    trafficLabel->setText(tr("No traffic."));
}

void SendSMS_view::processTimeout(const QString &s)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Running, %1.").arg(s));
    trafficLabel->setText(tr("No traffic."));
}

void SendSMS_view::setControlsEnabled(bool enable)
{
    runButton->setEnabled(enable);
    serialPortComboBox->setEnabled(enable);
    waitResponseSpinBox->setEnabled(enable);
	messageLineEdit->setEnabled(enable);
}
