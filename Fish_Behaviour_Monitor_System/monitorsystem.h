#ifndef MONITORSYSTEM_H
#define MONITORSYSTEM_H

#include <QtWidgets/QWidget>
#include "ui_monitorsystem.h"

class MonitorSystem : public QWidget
{
	Q_OBJECT

public:
	MonitorSystem(QWidget *parent = 0);
	~MonitorSystem();

private:
	Ui::MonitorSystemClass ui;
};

#endif // MONITORSYSTEM_H
