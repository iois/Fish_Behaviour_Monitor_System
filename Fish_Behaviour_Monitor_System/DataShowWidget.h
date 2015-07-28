#pragma execution_character_set("utf-8")
#pragma once

#include<fstream>

#include <qstring.h>
#include <QtCore\qdebug.h>
#include <QtCore\qvector.h>
#include <QtWidgets\QWidget>
#include <QtWidgets\qpushbutton.h>
#include "qcustomplot.h"

// ��������mainwindow�У�������Ƶ֡��Ӧ�����ݵ���ʾ
class DataShowWidget : public QWidget
{
public:
	DataShowWidget(QWidget *parent = 0);
	~DataShowWidget();

public:
	void set_title(QString t);
	void set_unit(QString u);
	void updata_data(double data);
private:
	void setupUi();

private:
	QCustomPlot* ui_qcustomplot;
	QString title;
	float current_data;

	QLabel ui_show_title;
	QLabel ui_show_data;
	QLabel ui_show_unit;
};

