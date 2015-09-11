#include "DataShowWidget.h"


DataShowWidget::DataShowWidget(QWidget *parent) :QWidget(parent)
{
	setupUi();
}


DataShowWidget::~DataShowWidget()
{
}


void DataShowWidget::setupUi(){
	ui_qcustomplot = new QCustomPlot(this);

	ui_qcustomplot->addGraph(); // blue line
	ui_qcustomplot->graph(0)->setPen(QPen(Qt::red));
	ui_qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

	ui_qcustomplot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
	ui_qcustomplot->xAxis->setDateTimeFormat("hh:mm:ss");
	ui_qcustomplot->xAxis->setAutoTickStep(true);

	ui_qcustomplot->axisRect()->setupFullAxesBox();
	ui_qcustomplot->yAxis->setRange(0, 6);

	connect(ui_qcustomplot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui_qcustomplot->xAxis2, SLOT(setRange(QCPRange)));
	connect(ui_qcustomplot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui_qcustomplot->yAxis2, SLOT(setRange(QCPRange)));


	QVBoxLayout * ui_layout_main = new QVBoxLayout(this);
	ui_layout_main->setContentsMargins(5, 0, 5, 0);
	QHBoxLayout * ui_layout_top = new QHBoxLayout(this);

	ui_layout_top->addWidget(&ui_show_title);
	ui_layout_top->addWidget(&ui_show_data);
	ui_layout_top->addWidget(&ui_show_unit);
	

	QSpacerItem* tanhuang = new QSpacerItem(1600,20,QSizePolicy::Preferred);
	ui_layout_top->addSpacerItem(tanhuang);

	ui_layout_main->addLayout(ui_layout_top);
	ui_layout_main->addWidget(ui_qcustomplot);

	this->setLayout(ui_layout_main);

	ui_show_title.setText(tr("<b>最小速度:<\b>"));
	ui_show_data.setText(tr("0.0"));
	ui_show_unit.setText(tr(""));
	ui_show_title.setMaximumHeight(20);
	ui_show_data.setMaximumHeight(20);
	ui_show_unit.setMaximumHeight(20);

	this->setAutoFillBackground(true);

	QPalette palette;
	palette.setColor(QPalette::Background, QColor(255, 255, 255));
	this->setPalette(palette);
}

void DataShowWidget::add_Graphs(int num_graphs){
	for (size_t i = 0; i < num_graphs; i++)
	{
		ui_qcustomplot->addGraph(); // blue line
		int r = std::rand() / RAND_MAX * 255, g = std::rand() / RAND_MAX * 255, b = std::rand() / RAND_MAX * 255;
		ui_qcustomplot->graph(1+i)->setPen(QPen(QColor(r,g,b)));
	}
}

void DataShowWidget::set_title(QString t){

	ui_show_title.setText(tr("<b>%1<\b>").arg(t));

}

void DataShowWidget::set_unit(QString u){
	ui_show_unit.setText(u);
}

void DataShowWidget::updata_data(double data){

	double key = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
	static double lastPointKey = 0;
	{
		ui_qcustomplot->graph(0)->addData(key, data);

		// remove data of lines that's outside visible range:
		ui_qcustomplot->graph(0)->removeDataBefore(key - 500);

		// rescale value (vertical) axis to fit the current data:
		ui_qcustomplot->graph(0)->rescaleValueAxis(true);

		lastPointKey = key;
	}
	//qcustomplot_speed->xAxis->setRange(key + 0.25, 8, Qt::AlignRight);
	ui_qcustomplot->xAxis->setRange(key + 0.5, 500, Qt::AlignRight);//??
	ui_qcustomplot->replot();

	ui_show_data.setText(tr("%1").arg(data));
}

void DataShowWidget::updata_data(std::vector<double> data){
	double key = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
	static double lastPointKey = 0;
	{
		for (size_t i = 0; i < data.size(); i++)
		{
			ui_qcustomplot->graph(i)->addData(key, data[i]);

			// remove data of lines that's outside visible range:
			ui_qcustomplot->graph(i)->removeDataBefore(key - 500);

			// rescale value (vertical) axis to fit the current data:
			ui_qcustomplot->graph(i)->rescaleValueAxis(true);
		}


		lastPointKey = key;
	}
	//qcustomplot_speed->xAxis->setRange(key + 0.25, 8, Qt::AlignRight);
	ui_qcustomplot->xAxis->setRange(key + 0.5, 500, Qt::AlignRight);//??
	ui_qcustomplot->replot();

	double min_speed = 10000000;
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i] < min_speed){ min_speed = data[i]; }
	}
	ui_show_data.setText(tr("%1  条数：%2").arg(min_speed).arg(data.size()-1));
}
