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
			if ((0 <= r1) && (r1<color_threshold) && (0 <= g1) && (g1<color_threshold) && (0 <= b1) && (b1<color_threshold))
			{
				dst.at<cv::Vec3b>(k, l)[0] = 0;
				dst.at<cv::Vec3b>(k, l)[1] = 0;
				dst.at<cv::Vec3b>(k, l)[2] = 0;
			}
			else
			{
				dst.at<cv::Vec3b>(k, l)[0] = 255;
				dst.at<cv::Vec3b>(k, l)[1] = 255;
				dst.at<cv::Vec3b>(k, l)[2] = 255;
			}
		}
	} //������ɫ��ֵ����ϡ� */
	return dst;
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

cv::Mat VideoProcessing::ImgProcessing(cv::Mat &src, cv::Mat &dst, cv::Mat &img_draw)
{
	/*
	// ͼƬԤ����ת���ɻҶ�ͼ��
	if (src.channels() > 1){
		cvtColor(src, dst, CV_BGR2GRAY);  // ��ɫͼ��ת���ɻҶ�ͼ��
	}
	*/
	if (!_background.empty())
	{
		src = abs(src - _background);
	}
	
	cv::imshow("Display Image3", src);

	cv::Mat temp = get_contours_colors(src, _img_process_set->get_segment_threshold());

	cvtColor(temp, dst, CV_BGR2GRAY);  // ��ɫͼ��ת���ɻҶ�ͼ��

	GaussianBlur(dst, dst, cv::Size(3,3), 0, 0); //��˹�˲�

	//bitwise_xor(cv::Scalar(255, 0, 0, 0), dst, dst);//xor,��ɫȡ��


	// ͼƬ ��ֵ�ָ�
	//dst = dst - _background;
	//threshold(dst, dst, _img_process_set->get_segment_threshold(), 255, 0);//��ֵ�ָ�
	//ErodeDilate(dst, dst);
	//OpenClose(dst, dst);
	

	bitwise_xor(cv::Scalar(255, 0, 0, 0), dst, dst);//xor,��ɫȡ��

	cv::imshow("Display Image2", dst);

	return dst;
}

bool VideoProcessing::open_camera()
{
	cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Display Image2", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Display Image3", cv::WINDOW_AUTOSIZE);
	_cap.open(0);
	if (!_cap.isOpened()){
		return false;
	} else {
		_cap >> _frame;
		CvSize img_size = _frame.size();
		return true;
	}
}

bool VideoProcessing::open_file(const char* file_name)
{
	//_video_file_name = file_name;
	_cap.open(file_name);
	qDebug() << file_name;
	if (!_cap.isOpened()){
		return false;
	} else {
		_cap >> _frame;
		CvSize img_size = _frame.size();
		//cvtColor(_frame, _p_temp, CV_BGR2GRAY);

		//ImgProcessing(_frame, _p_temp, _frame);
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
		CvSize img_size = _frame.size();
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
	{

		++_num_of_frames;
		this->notify();

		// �����ʼ����
		if (_isPrecess)
		{
			ImgProcessing(_frame, _p_temp, _frame);
			if (_img_process_set->get_num_fish() == 1){
				
				double speed = _mode_processing->execute(_p_temp, _frame, _img_process_set->get_min_area());
				double wp = _mode_processing_wp->execute(_p_temp, _frame, _img_process_set->get_min_area());
				send_data(1, speed / (640 / this->_sys_set->get_realLength()));
				send_data(2, wp);

				emit send_data_signal(1, speed);
				emit send_data_signal(2, wp);
				
			}
			else if (_img_process_set->get_num_fish() > 1)
			{
				double r = _mode_processing_Cluster->execute(_p_temp, _frame, _img_process_set->get_min_area());
				send_data(3, r);
			}
		}
	}
}

void VideoProcessing::notify()// ͼ�� �� ���� �ı��ˣ���۲���mainwindow ����֪ͨ
{
	if (_main_window)
	{
		_main_window->updata_img(_frame);
	}
}

void VideoProcessing::send_data(size_t modeIndex, double data){
	static double speed = 0;
	static int duration = 4;//�쳣����ʱ��
	static int wp_count = 0;
	static int old_data = 0;  //ǰһ������

	static int r = 0;
	static int r_count = 0;
	static int is_warning = 0;

	static int speedcount = 0;
	static int wpcount = 0;

	QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

	/*
	switch (modeIndex)
	{
	case 1: // �ٶ�
	{
		if (_num_of_frames % 15 == 0){
			// ������ʾ������
			_main_window->updata_data(1, data);

			if (_isRecord){
				_data_writer_1 << (data) << " ";	// ����д���Ӧ�ļ���

				// Ԥ�����
				if (data > 2){
					++speedcount;
					if (speedcount > duration && is_warning == 0){
						_main_window->ui_warning_view->add_warning_item(1, 1, "�ٶ��쳣 " + current_date);
						_sys_db->Insert_warning(1, 0, modeIndex);
						is_warning = 1;
					}
				}
				else {
					speedcount = 0;
					is_warning = 0;
				}
			}
		}
		break;
	}
	
	case 2: //
	{
		if (_num_of_frames % 15 == 0){
			_main_window->updata_data(2, wp_count);

			if (_isRecord){
				_data_writer_2 << (wp_count) << std::endl;	// ����д���Ӧ�ļ���

				if (data > 1.5){
					++wpcount;
					if (wpcount > duration && is_warning == 0){
						_main_window->ui_warning_view->add_warning_item(1, 2, "βƵ�쳣 " + current_date);
						_sys_db->Insert_warning(2, 0, modeIndex);
						is_warning = 1;
					}
				}
				else{
					wpcount = 0;
					is_warning = 0;
				}
			}
			wp_count = 0;
			old_data = data;
		}
		else{
			if ((old_data == 1 && data == 2) || (old_data == 2 && data == 1)){
				++wp_count;
				old_data = data;
			}
		}
		break;
	}
	case 3: //Ⱥ��������
	{
		if (_num_of_frames % 15 == 0){

			_main_window->updata_data(3, r / 15);
			if (_isRecord){
				_data_writer_3 << (r / 15) << std::endl;	// ����д���Ӧ�ļ���

				if (data > 50){
					++r_count;
					if (r_count > duration && is_warning == 0){
						_main_window->ui_warning_view->add_warning_item(1, 3, "�뾶�쳣 " + current_date);
						_sys_db->Insert_warning(3, 0, modeIndex);
						is_warning = 1;
					}
				}
				else{
					r_count = 0;
					is_warning = 0;
				}
			}
			r = 0;
		}
		else {
			r += data;
		}
		break;
	}
	default:
		break;
	}
	*/
}


void VideoProcessing::process_end()
{
	if (_isPrecess){
		if (this->_capture){
			_isPrecess = 0;
		}
	}
}

void VideoProcessing::process_begin()
{
	if (this->_capture)
	{
		_isPrecess = 1;
	}
}

cv::Mat VideoProcessing::background_pickup(){

	cv::Mat background;
	
	if (_cap.isOpened()){
		_cap >> _frame;
		//cvtColor(_frame, temp, CV_BGR2GRAY);  // ��ɫͼ��ת���ɻҶ�ͼ��
		_frame.copyTo(background);
		for (size_t i = 0; i < 15; ++i)
		{
			_cap >> _frame;
			if (i % 5 == 0)
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

						if (abs(b1 - b2) > 10 && abs(g1 - g2) > 10 && abs(g1 - g2) > 10){
							bgr2[0] = (b1 + bgr2[0]) / 2;
							bgr2[1] = (g1 + bgr2[1]);
							bgr2[2] = (r1 + bgr2[2]);
						}
					}
				}
				/*
				cvtColor(_frame, temp, CV_BGR2GRAY);  // ��ɫͼ��ת���ɻҶ�ͼ��
				{
				background = background*0.95 + _frame*0.05;
				}
				*/
			}
		}
		background.copyTo(this->_background);
		cv::imwrite("background.bmp", background);
		emit background_pickup_done();
	}
	else{}

	return background;
}