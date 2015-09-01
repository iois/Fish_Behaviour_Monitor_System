/*
App�����Ϊ����ϵͳӦ�á�
���������еĶ�����

��������࣬��Ϊʹ�� MonitorSystem �����
*/
#pragma once

#include<QtCore\qobject.h>
#include<QtCore\qthread.h>
#include<QtCore\qtimer.h>

#include"mainwindow.h"
#include"VideoProcessing.h"
#include"SystemSet.h"
#include"SystemDB.h"
#include"ImgProcessSet.h"

#include "sendsms.h"
#include "sendwatertakingsignal.h"
using namespace std;

class App :	public QObject
{
public:
	App();
	~App();

	// ����ϵͳ�������²��֣�
private:
	QTimer*          _t;                     //��ʱ��
	MainWindow*      _main_window;           //������
	VideoProcessing* _video_processing;      //��Ƶ������
	QThread*         _thread_videoprocessing;//��Ƶ�����߳�

	ImgProcessSet* _imgp_set;  //ͼ����������
	SystemSet*     _sys_set;   //ϵͳ������
	SysDB*         _sys_db;    //ϵͳ���ݿ���

public:
	SendSMS *_sms_sender;
	SendWaterTakingSignal* _water_taking_siganl_sender;

};


/*
�����ֵ䣺

���� - ����  modeIndex
����
1  - �ٶ�
2  - βƵ��
����
3  - Ⱥ�۰뾶
4  - ָ���뾶��������
5  - Ⱥ������
ͳһ
8  - ��������
9  - ��������

�洢�ļ��ļ������ڵ���+��ʼʱ��              (01_201506061100)
��Ƶ�ļ���     �ڵ���+��ʼʱ��.avi          (01_201506061100.avi)
�����ļ���     �ڵ���+��ʼʱ��+����index.txt (01_201506061100_01.txt)
*/

/*
�������̣�
�����->
������ͷ->�����ʼ����->
�����ָ������ʹ�÷ָ�Ч����ã�->
��ʼ��¼(��¼�Ĺ����в��ܸ���ϵͳ����)->
������¼��ͬʱֹͣͼ����->
�ر�����ͷ��ֹͣtimer����ѡ��->
����
*/