#include <QtWidgets/QApplication>
#include "stdafx.h"
#include "monitorsystem.h"
using namespace std;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MonitorSystem app(0);

	// 以下测试取水和发短信
	//SendSMS_view v(0, app._sms_sender);
	//v.show();
	//SendWaterTakingSignal_view vv(0, app._water_taking_siganl_sender);
	//vv.show();

	return a.exec();
}
