#pragma once
#include<QtWidgets\qwidget.h>
#include<QtWidgets\qlabel.h>

#include<QtCore\qfile.h>
#include<QtCore\qtextstream.h>

#include <cv.h>     //iplimage
#include <highgui.h>//CvCapture

//Opencv Mat -> Qt QImage
static QImage Mat2QImage(cv::Mat &mat);

//  ������
//  �����������ʾһ��ͼƬQImage *src �� QLabel   *_img_label ��
//  ������Ϊ�����ؼ����ӿؼ���
//  void show_img(const QImage *src); ��ʾͼƬ
// *void set_size(const QSize size);  �ı��С
class ImgShowWidget : public QWidget
{
public:
	ImgShowWidget(QWidget *parent);
	~ImgShowWidget();
	void show_img(const QImage *src);//��ʾͼƬ
	void set_size(const QSize& size); //�ı��С

protected:
	QLabel* _img_label;
	QSize   _size;
	void setupUi();
};

//  �̳���img_show_widget
//  �����������ʾһ��ͼƬQImage *src �� QLabel   *_img_label ��
//  ������Ϊ�����ؼ����ӿؼ���
//  void show_img(const QImage *src); ��ʾͼƬ
//  void set_size(const QSize size);  �ı��С
// *void updata_img(IplImage *src);   ��ʾopencv��iplimageͼ��
class ImgShowWidget_opencv :public ImgShowWidget
{
public:
	ImgShowWidget_opencv(QWidget *parent);
	~ImgShowWidget_opencv();

	void update_img(const IplImage *src);
private:
	IplImage *_iplImg;
	QImage   *_qimg;
};

//  �̳���img_show_widget
//  �����������ʾһ��ͼƬQImage *src �� QLabel   *_img_label ��
//  ������Ϊ�����ؼ����ӿؼ���
//  void show_img(const QImage *src); ��ʾͼƬ
//  void set_size(const QSize size);  �ı��С
// *void updata_img(IplImage *src);   ��ʾopencv��iplimageͼ��
class ImgShowWidget_Mat:public ImgShowWidget
{
public:
	ImgShowWidget_Mat(QWidget *parent);
	~ImgShowWidget_Mat();

	void update_img(cv::Mat &mat);
private:
	IplImage *_iplImg;
	QImage   *_qimg;
};

//Mat->QImage
static QImage Mat2QImage(cv::Mat &mat)
{
	QImage img;
	int nChannel = mat.channels();
	if (nChannel == 3)
	{
		cv::cvtColor(mat, mat, CV_BGR2RGB);
		img = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, QImage::Format_RGB888);
	}
	else if (nChannel == 4 || nChannel == 1)
	{
		img = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, QImage::Format_ARGB32);
	}
	return img;
}