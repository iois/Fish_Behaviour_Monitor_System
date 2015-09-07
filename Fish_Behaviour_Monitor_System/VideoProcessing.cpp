#include<vector>
#include "VideoProcessing.h"
#include "mainwindow.h"
#include"SystemSet.h"

int element_shape = cv::MORPH_RECT;

// ��������
static void OpenClose(cv::Mat &src, cv::Mat &dst, int n=-1)
{
	int an = n > 0 ? n : -n;
	cv::Mat element = cv::getStructuringElement(element_shape, cv::Size(an * 2 + 1, an * 2 + 1), cv::Point(an, an));
	if (n < 0)
		morphologyEx(src, dst, CV_MOP_OPEN, element);
	else
		morphologyEx(src, dst, CV_MOP_CLOSE, element);
}
// ��ʴ����
static void ErodeDilate(cv::Mat &src, cv::Mat &dst, int n = 9 - 10)
{
	int an = n > 0 ? n : -n;
	cv::Mat element = cv::getStructuringElement(element_shape, cv::Size(an * 2 + 1, an * 2 + 1), cv::Point(an, an));
	if (n < 0)
		erode(src, dst, element);
	else
		dilate(src, dst, element);
}

//��ֵ����get mat(rgb) from �Ѿ�����������ͼƬ by color
cv::Mat get_contours_colors(const cv::Mat &src,int color_threshold){
	cv::Mat dst;
	src.copyTo(dst);

	int height1 = src.size().height;
	int width1 = src.size().width;
	for (int k = 0; k < height1; k++)
	{
		for (int l = 0; l < width1; l++)
		{
			const cv::Vec3b& bgr = src.at<cv::Vec3b>(k,l);
			int b1 = bgr[0];//B
			int g1 = bgr[1];//G
			int r1 = bgr[2];//R

			// ������ɫͨ����Ϣ�ж�Ŀ��
			// ����Ƚ�С���Ǳ���
			//if ((40 <= r1) && (r1<250) && (0 <= g1) && (g1<color_threshold) && (0 <= b1) && (b1<color_threshold))
			if ((0 <= r1) && (r1<40+color_threshold) && (0 <= g1) && (g1<color_threshold) && (0 <= b1) && (b1<color_threshold+10))
			{
				dst.at<cv::Vec3b>(k, l)[0] = 255;
				dst.at<cv::Vec3b>(k, l)[1] = 255;
				dst.at<cv::Vec3b>(k, l)[2] = 255;
			}
			else
			{
				dst.at<cv::Vec3b>(k, l)[0] = 0;
				dst.at<cv::Vec3b>(k, l)[1] = 0;
				dst.at<cv::Vec3b>(k, l)[2] = 0;  //[0,0,0]��
			}
		}
	} //������ɫ��ֵ����ϡ� */
	return dst;
}


void Segment_HSV(cv::Mat &src, cv::Mat &dst, int S_threshold)
{
	cv::Mat res, img;

	if (src.empty()){
		return;
	}

	cv::cvtColor(src, img, CV_RGB2HSV);
	std::vector<cv::Mat> HSV;
	split(img, HSV);

	cv::Mat HImg = HSV[0];
	cv::Mat SImg = HSV[1];
	cv::Mat VImg = HSV[2];


	for (int i = 0; i < SImg.rows; ++i)
	{
		for (int j = 0; j < SImg.cols; ++j)
		{
			//int value = SImg.at<char>(i, j);
			if (SImg.at<char>(i, j) >= S_threshold || SImg.at<char>(i, j) < 0)
			{
				dst.at<cv::Vec3b>(i, j)[0] = 255;
				dst.at<cv::Vec3b>(i, j)[1] = 255;
				dst.at<cv::Vec3b>(i, j)[2] = 255;
			}
			else
			{
				dst.at<cv::Vec3b>(i, j)[0] = 0;
				dst.at<cv::Vec3b>(i, j)[1] = 0;
				dst.at<cv::Vec3b>(i, j)[2] = 0;
			}
		}
	}
}

//==========================================================================


VideoProcessing::VideoProcessing(QObject *parent, SystemSet *set, ImgProcessSet  *img_p_set)
	:QObject(parent), _sys_set(set), _img_process_set(img_p_set)
{
	_mode_processing = new Speedmode_processing(img_p_set);
	//todo:2015-07-22-11:21
	_mode_processing_wp = new WPmode_processing(img_p_set);
	_mode_processing_Cluster = new Clustermode_processing(img_p_set);
}

void VideoProcessing::attach(MainWindow *Object)
{
	_main_window = Object;
}

VideoProcessing::~VideoProcessing()
{

}

IplImage* VideoProcessing::ImgProcessing(IplImage *src, IplImage *dst, IplImage *img_draw)
{
	// ͼƬԤ����ת���ɻҶ�ͼ��
	cvCvtColor(src, dst, CV_BGR2GRAY);  // ��ɫͼ��ת���ɻҶ�ͼ��
	cvSmooth(dst, dst, CV_GAUSSIAN, 3, 0, 0); //��˹�˲�

	// ͼƬ ��ֵ�ָ�
	cvThreshold(dst, dst, _img_process_set->get_segment_threshold(), 255, CV_THRESH_BINARY); //ȡ��ֵ��ͼ��תΪ��ֵͼ��

	// ������
	cvMorphologyEx(dst, dst, NULL, 0, CV_MOP_OPEN, 1);
	// �����㣨�������ٸ�ʴ��:Ч���Ϻã�����ʹĿ������������������Ծ���һЩ������
	cvMorphologyEx(dst, dst, NULL, 0, CV_MOP_CLOSE, 1);

	// ��ʴ
	cvErode(dst, dst, 0, 1);
	// ����
	cvDilate(dst, dst, 0, 1);
	//��ɫ��ת
	cvNot(dst, dst);

	return dst;
}

cv::Mat VideoProcessing::ImgProcessing(const cv::Mat &src, cv::Mat &dst)
{
	// ��1��
	cvtColor(src, this->_gray_img, CV_BGR2GRAY);  // ��ɫͼ��ת���ɻҶ�ͼ��
	
	// ��2��
	cv::Mat temp_rgb;//����cv::Mat temp_rgb��src��, ǳ����
	src.copyTo(temp_rgb);

	GaussianBlur(temp_rgb, temp_rgb, cv::Size(7, 7), 0, 0); //��˹
	
	// Ŀ��ָ�: 

	// ����1�����ñ�����֣�RGB��ɫ��Ϣ�ָ�
	//if (!_background.empty())
	//{
	//    temp_rgb = abs(src - _background);
	//}
	//cv::imshow("abs_img", temp_rgb);
	//cv::Mat temp_dst_rgb = get_contours_colors(temp_rgb, _img_process_set->get_segment_threshold());

	// ����2������HSV��ɫ�ռ�ָ�
	cv::Mat temp_dst_rgb;
	temp_rgb.copyTo(temp_dst_rgb);
	Segment_HSV(temp_rgb, temp_dst_rgb,_img_process_set->get_segment_threshold());
		

	cvtColor(temp_dst_rgb, dst, CV_BGR2GRAY);  // ��ɫͼ��ת���ɻҶ�ͼ��
	
	medianBlur(dst, dst, 3);
	GaussianBlur(dst, dst, cv::Size(5,5), 0, 0); //��˹�˲�

	//bitwise_xor(cv::Scalar(255, 0, 0, 0), dst, dst);//xor,��ɫȡ��
	//cv::imshow("Display Image2", dst);

	return dst;
}

bool VideoProcessing::open_camera()
{
	//cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
	//cv::namedWindow("Display Image2", cv::WINDOW_AUTOSIZE);
	//cv::namedWindow("abs_img", cv::WINDOW_AUTOSIZE);
	_cap.open(0);
	if (!_cap.isOpened()){
		return false;
	} else {
		_cap >> _frame;
		_img_size = { _frame.size().width, _frame.size().height };
		return true;
	}
}

bool VideoProcessing::close_camera()
{
	if (_cap.isOpened()){
		_cap.release();
	}
	return true;
}

bool VideoProcessing::open_file(const char* file_name)
{
	_cap.open(file_name);

	//qDebug() << file_name;

	if (!_cap.isOpened()){
		return false;
	} else {
		_cap >> _frame;
		CvSize img_size = _frame.size();

		this->notify();
		return true;
	}
}

bool VideoProcessing::open_file(const std::string &file_name){
	_cap.open(file_name);
	if (!_cap.isOpened()){
		return false;
	}
	else {
		_cap >> _frame;
		_img_size = _frame.size();
		this->notify();
		return true;
	}
}

void VideoProcessing::time_out_todo_1()
{

	if (!_background.empty()){

		cv::imshow("Display Image", _background);
	}
	
	//��CvCapture�л��һ֡
	_cap >> _frame;

	_frame.copyTo(_img_for_show);

	// �����ʼ����
	if (_isPrecess)
	{
		ImgProcessing(_frame, _img_temp);  // ����ͼ��

		_contours.clear();
		_contours = this->compute_Contour(_img_temp);

		if (_img_process_set->get_num_fish() == 1){

			double speed = _mode_processing->execute(_img_temp, _img_for_show, _contours);
			double wp = _mode_processing_wp->execute(_img_temp, _img_for_show, _contours);

			emit send_data_signal(1, speed);
			emit send_data_signal(2, wp);

		}else if (_img_process_set->get_num_fish() > 1){
			double r = _mode_processing_Cluster->execute(_img_temp, _img_for_show, _contours);

			emit send_data_signal(3, r);
		}
	}

	//bitwise_xor(cv::Scalar(255, 0, 0, 0), _frame, _frame);//xor,��ɫȡ��
	++_num_of_frames;
	this->notify();
}

void VideoProcessing::notify()// ͼ�� �� ���� �ı��ˣ���۲���mainwindow ����֪ͨ
{
	if (_main_window)
	{
		_main_window->updata_img(_img_for_show);
	}
}

void VideoProcessing::process_end()
{
	if (_isPrecess){
		close_camera();
		_isPrecess = 0;
	}
}

void VideoProcessing::process_begin()
{
	if (_cap.isOpened())
	{
		_isPrecess = 1;
	}
}

cv::Mat VideoProcessing::background_pickup(){

	cv::Mat background;

	int times = 30;    //��30֡��
	int num_frames = 5;//ȡ5֡��ƽ��������
	int step = times / num_frames;
	
	if (_cap.isOpened()){
		_cap >> _frame;
		_frame.copyTo(background);
		for (size_t i = 1; i < times+1; ++i)
		{
			_cap >> _frame;
			int kk = i / step;
			if (i % step == 0)
			{
				int height1 = _frame.size().height;
				int width1 = _frame.size().width;
				for (int k = 0; k < height1; k++)
				{
					for (int l = 0; l < width1; l++)
					{
						const cv::Vec3b& bgr = _frame.at<cv::Vec3b>(k, l);
						int b1 = bgr[0];//B
						int g1 = bgr[1];//G
						int r1 = bgr[2];//R
						cv::Vec3b& bgr2 = background.at<cv::Vec3b>(k, l);
						int b2 = bgr2[0];//B 
						int g2 = bgr2[1];//G
						int r2 = bgr2[2];//R

						//ͼƬ�뱳���Ĳ�ֵ��������ƽ��
						if (abs(b1 - b2) > 10 && abs(g1 - g2) > 10 && abs(g1 - g2) > 10){
							bgr2[0] = (b1 + bgr2[0] * (kk - 1)) / kk;
							bgr2[1] = (g1 + bgr2[1] * (kk - 1)) / kk;
							bgr2[2] = (r1 + bgr2[2] * (kk - 1)) / kk;
						}
					}
				}
			}
		}

		GaussianBlur(background, background, cv::Size(5, 5), 0, 0); //��˹�˲�

		background.copyTo(this->_background);
		cv::imwrite("background.bmp", background);
		emit background_pickup_done();
	}
	else{}

	return background;
}




// ���� ������ ����Ϊ�Ҷ�ͼ��ʵ��ֻ�� �ڰ� ������ɫ
vector<vector<cv::Point>> VideoProcessing::compute_Contour(cv::Mat &src){

	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;

	cv::findContours(src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	int num_cont = contours.size();      // ������
	CvPoint   fish_centerpoint;

	// ����õ�ÿһ��������ģ� ������ _fishCenter ��
	for (int i = 0; i < num_cont; ++i)
	{
		double s = contourArea(contours[i]);// �����������������
		if (fabs(s) < _img_process_set->get_min_area())
		{
			continue;
		}
		else if (fabs(s) > _img_process_set->get_max_area())
		{
			continue;
		}
	}
	return contours;
}
