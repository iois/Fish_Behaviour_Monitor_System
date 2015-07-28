#pragma once
#include "qwidget.h"

#include<QtWidgets\qwidget.h>
#include<QtWidgets\qlayout.h>
#include<QtWidgets\QLabel>
#include<QtWidgets\QListWidget>
#include<QtWidgets\QListWidgetItem>

/* ������ʾԤ����Ϣ�Ŀؼ���*/
class WarningViewWidget : public QWidget
{
public:
	WarningViewWidget(QWidget *parent = 0);
	~WarningViewWidget();

	//*���һ��Ԥ����¼��degree�ȼ���mode������ţ�text������Ϣ����ʱ�䣩
	void add_warning_item(size_t degree, int modeindex, QString text);

	void on_btn_Add_clicked();
	void on_btn_Del_clicked();
private:

	QListWidget *warningList;
};

