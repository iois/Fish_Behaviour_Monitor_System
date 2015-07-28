#include <QtWidgets/QApplication>
#include"App.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	App app;
	SendSMS_view v(0, app._sms_sender);
	v.show();
	SendWaterTakingSignal_view vv(0, app._water_taking_siganl_sender);
	vv.show();
	return a.exec();
}
