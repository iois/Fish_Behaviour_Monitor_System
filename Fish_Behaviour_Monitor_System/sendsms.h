#ifndef SENDSMS_H
#define SENDSMS_H

#include <vector>
#include <string>
#include <qwidget.h>
#include "masterthread.h"

class QLabel;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QComboBox;

class SendSMS_view;

//发送短信应该在一个独立的进程中。
class SendSMS : public QObject
{
    Q_OBJECT
public:
    SendSMS();

public:
    //发送短信
    bool send(const QString &number,const QString &message);

	//设置
	void set_serial_port(const QString &serial_port);
	void set_baud_rate(const QString &baudrate);
    void set_waitTimeout(int val);

    std::vector<QString> get_serial_ports(){
        return _serial_ports;
    }

private:
    MasterThread _thread;

    std::vector<QString> _serial_ports;

    QString _portName;
    int _baud_rate;
    int _waitTimeout;

    QString _request;
  
signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);
public:
    void emit_response(const QString &s){ emit response(s); }
    void emit_error(const QString &s){ emit error(s); }
    void emit_timeout(const QString &s){ emit timeout(s); }
};

//--------------------------------------------------------------

// 设置和测试短息模块
class SendSMS_view:public QWidget
{
	Q_OBJECT
public:
    SendSMS_view(QWidget *parent = 0,SendSMS *sendsms=0);

private slots:
    void transaction();
    void showResponse(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);

private:
    void setControlsEnabled(bool enable);

private:
    int transactionCount;

    QLabel *serialPortLabel;
    QComboBox *serialPortComboBox;

    QLabel *waitResponseLabel;
    QSpinBox *waitResponseSpinBox;

	QLabel    *numberLabel;
	QLineEdit *numberLineEdit;

    QLabel    *messageLabel;
	QLineEdit *messageLineEdit;

    QLabel *trafficLabel;
    QLabel *statusLabel;
    QPushButton *runButton;

    SendSMS *send_sms;
};

#endif // SENDSMS_H
