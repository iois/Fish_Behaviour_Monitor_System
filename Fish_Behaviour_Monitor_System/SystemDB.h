# pragma execution_character_set("utf-8")//�����������
#pragma once
#include "qobject.h"


#include <QWidget>

#include<QtWidgets\qtableview.h>

#include<QtWidgets\qlayout.h>
#include<QtWidgets\qtableview.h>
#include<QtWidgets\QDateTimeEdit>
#include<QtWidgets\QPushButton>
#include<QtWidgets/qlabel.h>
#include<qtimer.h>

#include<QtSql\QSqlTableModel>
#include<QtSql\qsql.h>
#include<QtSql\qsqldriver.h>
#include<QtSql\qsqldatabase.h> //db
#include<QtSql\qsqlquery.h>
#include<QtSql\qsqlrelationaltablemodel.h>
#include<QtSql\qsqlerror.h>

#include<QMessageBox>

#include"VideoDataDisplayer.h"

class SysDB :
	public QObject
{
	Q_OBJECT
public:
	SysDB();
	~SysDB();
	// ���һ���µ����ݿ�����
	void AddConnection(const QString &driver, const QString &dbName, const QString &host,
		const QString &user, const QString &passwd, int port);

	//���ص�ǰ���ӵ����ݿ�
	QSqlDatabase CurrentDatabase() const;
	//���� �µļ����Ϣ
	bool InsertNewMonitor(QString id, QString name, int num_fish, QString remark, QString date);
	//���� �µ���Ƶ�ļ���Ϣ
	bool InsertNewVideo(int video_id, int monitor_id, QString path, QString name, QString timestamp);
	//ɾ�� ���

	//���� warning
	bool Insert_warning(int monitor_id, QString video_id, int mode);

	// 2015-06-12 19:48
	// [1]�����µļ�¼
	bool InsertNewRecord(QString video_id, const QString &path, int num_fish, QString time_begin, const QString &remark);
	// [2]���½���ʱ��
	bool InsertNewRecord_endtime(QString time_end, QString video_id);

	QString get_del_file_name();

signals:
	void DBChanged(); // ���ݿ����ݷ����仯  //ֻ���ڲ�����

private:
	QString _driverName;
	QString _databaseName;
	QString _hostName;
	QString _userName;
	QString _password;
	QString _port;

	QSqlDatabase *db;
};


class SysDB_view : public QWidget
{
	Q_OBJECT
public:
	static SysDB_view *_instance;//Ψһʵ�� ָ��
	static SysDB_view *instance(QWidget *parent = 0, SysDB *s = 0)
	{
		if (!_instance)
			_instance = new SysDB_view(parent, s);
		return _instance;
	}

	SysDB_view(QWidget *parent = 0, SysDB *sys_db = 0);
	~SysDB_view();

private:
	SysDB *_sys_db;

	// 2015-06-14
	QTableView *_record_tab;
	QSqlTableModel *_record_model;
	//

	QTableView *_warning_tab;
	QSqlTableModel *_warning_model;

	QDateTimeEdit *_begin_time_Edit;
	QDateTimeEdit *_end_time_Edit;
	QPushButton   *_button;

	QVBoxLayout *_vboxLayout_left;
	QVBoxLayout *_vboxLayout_right;
	QHBoxLayout *_HboxLayout_main;

	QLayout *_hboxLayout_timeedit;

public:

	void updata_DB_view();
	void find_by_time();  //video_tab
	void on_tableView_doubleClicked(const QModelIndex &index);
private:
	void setupUI();
	void setupModel();

	QModelIndex current_index;//
};

