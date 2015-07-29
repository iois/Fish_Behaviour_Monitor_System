#include <QtWidgets/QApplication>
#include"App.h"
#include"monitorsystem.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//App app;
	MonitorSystem app(0);
	SendSMS_view v(0, app._sms_sender);
	v.show();
	SendWaterTakingSignal_view vv(0, app._water_taking_siganl_sender);
	vv.show();
	return a.exec();
}
