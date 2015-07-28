#include "loadingdialog.h"

LoadingDialog::LoadingDialog(QDialog *parent, QString text) :QDialog(parent)
{
	QVBoxLayout  *_layout_v_main = new QVBoxLayout(this);

	label = new QLabel();
	label_2 = new QLabel();
	_layout_v_main->addWidget(label);
	_layout_v_main->addWidget(label_2);

	this->setFixedSize(200, 200);
	this->setWindowOpacity(0.7);  //����͸����
	this->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);//ȡ��������

	this->setContentsMargins(0, 0, 0, 0);

	_layout_v_main->setContentsMargins(0, 0, 0, 0);

	label->setContentsMargins(0, 0, 0, 0);

	movie = new QMovie("loading.gif");
	label->setMovie(movie);
	movie->start();
	label_2->setText(text);
}

LoadingDialog::~LoadingDialog()
{
	delete movie;
	delete label;
	delete label_2;

}
