#include "mode_processing.h"
#include "My_Delaunay.h"

int Distance(CvPoint a, CvPoint b)
{
	int x = a.x > b.x ? (a.x - b.x) : (b.x - a.x);
	int y = a.y > b.y ? (a.y - b.y) : (b.y - a.y);
	return  (x * x + y * y);
}

double cvCos(CvPoint a, CvPoint b)
{
	CvPoint p = { 0, 0 };
	return (Distance(a, p) + Distance(b, p) - Distance(a, b)) /
		(2 * sqrt(double(Distance(a, p)*Distance(b, p))));
}

mode_processing::mode_processing(ImgProcessSet  *img_p_set) :_img_process_set(img_p_set)
{
}

mode_processing::~mode_processing()
{
}

/*******************************************************************************************/

double Speedmode_processing::execute(IplImage *src, IplImage *img_draw, int minContour){

	CvPoint center = this->compute_Contour(src, img_draw, _img_process_set->get_min_area());

	return this->compute_speed(center);
}
double Speedmode_processing::execute(cv::Mat &src, cv::Mat &img_draw, int minContour){

	CvPoint center = this->compute_Contour(src, img_draw, _img_process_set->get_min_area());

	return this->compute_speed(center);
}

CvPoint Speedmode_processing::compute_Contour(IplImage *src, IplImage *img_draw, int minContour){
	CvSeq        *_cont;      //���ڴ洢�����Ľṹ
	CvMemStorage *_stor;      //
	_stor = cvCreateMemStorage(0); // �����ڴ�ռ䣬֮������ͷ�
	_cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), _stor);

	int Number_All_Object = cvFindContours(src, _stor, &_cont, sizeof(CvContour),
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
	//ֻ��ȡ����������,�����е���������ʽ����Ϊ��������ʽ,ƫ��Ϊ0

	//////////////////////////////////////////////////////////////////////////////////

	// ����
	int num_cont = 0;      // ������
	CvPoint   fish_centerpoint;
	// ����õ�ÿһ��������ģ� ������ _fishCenter ��
	while (_cont)
	{
		double s = cvContourArea(_cont);// ����������������� 
		if (fabs(s) < _img_process_set->get_min_area())
		{
			_cont = _cont->h_next;
			continue;
		}
		else if (fabs(s) > _img_process_set->get_max_area())
		{
			_cont = _cont->h_next;
			continue;
		}
		cvDrawContours(img_draw, _cont, CV_RGB(0, 255, 0), CV_RGB(255, 0, 0), 0);

		// ���³���飺���㡢����ÿ���������㣩������ (����cont��num_cont����� _fishCenter)
		{
			CvMoments moments;   //??
			cvMoments(_cont, &moments, 0);
			// ������������  ���������ָ����һ��������ģ����������ģ�
			float cx, cy;         // ������������
			cx = cvRound(moments.m10 / moments.m00);
			cy = cvRound(moments.m01 / moments.m00);  //����ء����������ꣿ
			// ������������������ �� xy ��	

			fish_centerpoint.x = int(cx);
			fish_centerpoint.y = int(cy);
			//fishCenter.push_back(fish_centerpoint);
		}

		// ָ����һ������
		_cont = _cont->h_next;
		++num_cont;
	}
	cvReleaseMemStorage(&_stor);// �ڴ�й©����������⡣֮ǰ����ռ���Init�����У�û��cvReleaseMemStorage(&_stor);���ڴ�й©	
	return fish_centerpoint;
}
CvPoint Speedmode_processing::compute_Contour(cv::Mat &src, cv::Mat &img_draw, int minContour){

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	cv::findContours(src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	int num_cont = contours.size();      // ������
	CvPoint   fish_centerpoint;

	// ����õ�ÿһ��������ģ� ������ _fishCenter ��
	for (int i = 0; i < num_cont;++i)
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

		drawContours(img_draw, contours, i, CV_RGB(0, 255, 0), 1, 8);

		// ���³���飺���㡢����ÿ���������㣩������ (����cont��num_cont����� _fishCenter)
		Moments m = moments(contours[i]);
		// ������������  ���������ָ����һ��������ģ����������ģ�
		fish_centerpoint.x = m.m10 / m.m00;
		fish_centerpoint.y = m.m01 / m.m00;
	}

	return fish_centerpoint;
}

double Speedmode_processing::compute_speed(CvPoint fishCenter){
	if (_old_point.x == 0){
		_old_point = fishCenter;
	}
	{

		if (_distence.size() > num_frm_Sec){
			double d2 = std::sqrt(abs((_old_point.x - fishCenter.x) ^ 2 + (_old_point.y - fishCenter.y) ^ 2));
			double d1 = _distence.front();
			_distence.pop_front();
			_distence.push_back(d2);
			speed -= d1;
			speed += d2;
		}

		else{
			double d2 = std::sqrt(abs((_old_point.x - fishCenter.x) ^ 2 + (_old_point.y - fishCenter.y) ^ 2));
			_distence.push_back(d2);
			speed += d2;
		}
		_old_point = fishCenter;
	}
	return speed;//���ص��ǵ�ǰ֡ǰһ���λ�ƣ���λ������/s��
}

/*******************************************************************************************/

Fish WPmode_processing::compute_Contour(IplImage *src, IplImage *img_draw, int minContour)
{
	CvSeq        *_cont;      //���ڴ洢�����Ľṹ
	CvMemStorage *_stor;      //
	_stor = cvCreateMemStorage(0); // �����ڴ�ռ䣬֮������ͷ�
	_cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), _stor);

	int Number_All_Object = cvFindContours(src, _stor, &_cont, sizeof(CvContour),
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

	int num_cont = 0;      // ������
	Fish fish;
	CvPoint   fish_centerpoint;
	while (_cont)
	{
		double s = cvContourArea(_cont);// ����������������� 
		if (fabs(s) < _img_process_set->get_min_area())
		{
			_cont = _cont->h_next;
			continue;
		}
		else if (fabs(s) > _img_process_set->get_max_area())
		{
			_cont = _cont->h_next;
			continue;
		}
		fish = Fish(_cont);
		_cont = _cont->h_next;
		++num_cont;
	}
	cvReleaseMemStorage(&_stor);// �ڴ�й©����������⡣֮ǰ����ռ���Init�����У�û��cvReleaseMemStorage(&_stor);���ڴ�й©	
	return fish;
}
Fish WPmode_processing::compute_Contour(cv::Mat &src, cv::Mat &img_draw, int minContour){
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	cv::findContours(src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	Fish fish;

	int num_cont = contours.size();      // ������
	CvPoint   fish_centerpoint;

	// ����õ�ÿһ��������ģ� ������ _fishCenter ��
	for (int i = 1; i < num_cont; ++i)
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

		drawContours(img_draw, contours, i, CV_RGB(0, 255, 0), 1, 8);

		fish = Fish(contours[i]);
	}
	return fish;
}
int  WPmode_processing::compute_WP(Fish fish)
{
	if (_old_fish.center.x == 0 && _old_fish.center.y == 0)
	{
		_old_fish = fish;
		return 0;
	}
	else
	{
		CvPoint tmp1 = { fish.center.x - fish.head.x, fish.center.y - fish.head.y };	//ͷ-����
		CvPoint tmp2 = { fish.tail.x - fish.center.x, fish.tail.y - fish.center.y };	//����-β
		double cos = cvCos(tmp1, tmp2);

		if (cos < 0.99)
		{
			if ((fish.center.y - fish.head.y)*(fish.tail.x - fish.center.x) + (fish.head.x - fish.center.x)*(fish.tail.y - fish.center.y) > 0){

				return 1;
			}
			else{
				return 2;
			}
		}
		else{
			return 0;
		}
	}
}

double WPmode_processing::execute(IplImage *src, IplImage *img_draw, int minContour){

	Fish center = this->compute_Contour(src, img_draw, minContour);

	return this->compute_WP(center);
}
double WPmode_processing::execute(cv::Mat &src, cv::Mat &img_draw, int minContour){
	Fish center = compute_Contour(src, img_draw, minContour);
	return this->compute_WP(center);
}

//-----------------------------------------------------------------------------------------------

double Clustermode_processing::execute(IplImage *src, IplImage *img_draw, int minContour)
{
	_fishCenter.clear();
	compute_Contour(src, img_draw, minContour, _fishCenter);
	return compute_R(_fishCenter, img_draw);
}
double Clustermode_processing::execute(cv::Mat &src, cv::Mat &img_draw, int minContour){
	_fishCenter.clear();
	compute_Contour(src, img_draw, minContour, _fishCenter);
	return this->compute_R(_fishCenter, img_draw);
}
void Clustermode_processing::compute_Contour(IplImage *src, IplImage *img_draw, int minContour,
	std::vector<CvPoint>& fishCenter)
{
	CvSeq        *_cont;      //���ڴ洢�����Ľṹ
	CvMemStorage *_stor;      //
	_stor = cvCreateMemStorage(0); // �����ڴ�ռ䣬֮������ͷ�
	_cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), _stor);

	int Number_All_Object = cvFindContours(src, _stor, &_cont, sizeof(CvContour),
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

	int num_cont = 0;      // ������
	Fish fish;
	CvPoint   fish_centerpoint;
	while (_cont)
	{
		double s = cvContourArea(_cont);// ����������������� 
		if (fabs(s) < _img_process_set->get_min_area())
		{
			_cont = _cont->h_next;
			continue;
		}
		else if (fabs(s) > _img_process_set->get_max_area())
		{
			_cont = _cont->h_next;
			continue;
		}
		fish = Fish(_cont);
		fishCenter.push_back(fish.center);
		_cont = _cont->h_next;
		++num_cont;
	}
	cvReleaseMemStorage(&_stor);// �ڴ�й©����������⡣֮ǰ����ռ���Init�����У�û��cvReleaseMemStorage(&_stor);���ڴ�й©	
}
void Clustermode_processing::compute_Contour(Mat &src, Mat &img_draw, int minContour, std::vector<CvPoint>& fishCenter){

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	cv::findContours(src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	int num_cont = contours.size();      // ������
	CvPoint   fish_centerpoint;

	// ����õ�ÿһ��������ģ� ������ _fishCenter ��
	for (int i = 1; i < num_cont; ++i)
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

		drawContours(img_draw, contours, i, CV_RGB(0, 255, 0), 1, 8);

		Fish fish = Fish(contours[i]);
		fishCenter.push_back(fish.center);
	}
}

int Clustermode_processing::compute_R(std::vector<CvPoint>& fishCenter, IplImage *img_draw)
{
	int _r = 0;
	int num_cont = fishCenter.size();
	CvPoint _all_triangles_center = _Delaunay(fishCenter, img_draw);

	// ���������ĵ� Triangles_center �ľ��룬 �����򣬵õ��ڶ�Զ�ľ�����Ϊ�뾶 R��
	std::vector<int> fishDiff;

	//����ÿ��������Ⱥ���������λ�ã��ŵ�fishDiff[i]��.
	for (int i = 0; i < num_cont; ++i)
	{
		fishDiff.push_back(sqrt((_all_triangles_center.x - _fishCenter[i].x)*(_all_triangles_center.x - _fishCenter[i].x)
			+ (_all_triangles_center.y - _fishCenter[i].y)*(_all_triangles_center.y - _fishCenter[i].y)));
	}

	sort(fishDiff.begin(), fishDiff.end());
	if (num_cont >= 2){
		_r = fishDiff[num_cont - 2]; //ȡ�ڶ�Զ��Ϊ�뾶�� �Ե�
	}
	else{
		_r = 30;
	}

	// �����ĵ�
	cvCircle(img_draw, _all_triangles_center, 2, cvScalar(0, 255, 255, 0), 2, 8, 0);
	// ��ԲȦ
	cvCircle(img_draw, _all_triangles_center, _r, cvScalar(0, 255, 255, 0), 1, 8, 0);
	//_all_triangles_center;
	return _r;
}
int Clustermode_processing::compute_R(std::vector<CvPoint>& fishCenter, cv::Mat &img_draw){

	int _r = 0;
	int num_cont = fishCenter.size();
	
	CvPoint _all_triangles_center = _Delaunay(fishCenter, img_draw);

	// ���������ĵ� Triangles_center �ľ��룬 �����򣬵õ��ڶ�Զ�ľ�����Ϊ�뾶 R��
	std::vector<int> fishDiff;

	//����ÿ��������Ⱥ���������λ�ã��ŵ�fishDiff[i]��.
	for (int i = 0; i < num_cont; ++i)
	{
		fishDiff.push_back(sqrt((_all_triangles_center.x - _fishCenter[i].x)*(_all_triangles_center.x - _fishCenter[i].x)
			+ (_all_triangles_center.y - _fishCenter[i].y)*(_all_triangles_center.y - _fishCenter[i].y)));
	}

	sort(fishDiff.begin(), fishDiff.end());
	if (num_cont >= 2){
		_r = fishDiff[num_cont - 2]; //ȡ�ڶ�Զ��Ϊ�뾶�� �Ե�
	}
	else{
		_r = 30;
	}

	// �����ĵ�
	cv::circle(img_draw, _all_triangles_center, 2, cvScalar(0, 255, 255, 0), 2, 8, 0);
	// ��ԲȦ
	cv::circle(img_draw, _all_triangles_center, _r, cvScalar(0, 255, 255, 0), 1, 8, 0);
	//_all_triangles_center;
	return _r;
}
CvPoint Clustermode_processing::_Delaunay(std::vector<CvPoint> points, IplImage *dst){
	if (points.size() < 1){
		return{ 0, 0 };
	}
	if (points.size() == 1){
		return points[0];
	}
	if (points.size() == 2){

		cvLine(dst, points[0], points[1], cvScalar(0, 0, 0, 0), 1, 8, 0);
		return{ (points[0].x + points[1].x) / 2, (points[0].y + points[1].y) / 2 };
	}
	if (points.size() == 3){

		cvLine(dst, points[0], points[1], cvScalar(0, 0, 0, 0), 1, 8, 0);
		cvLine(dst, points[1], points[2], cvScalar(0, 0, 0, 0), 1, 8, 0);
		cvLine(dst, points[0], points[2], cvScalar(0, 0, 0, 0), 1, 8, 0);
		return{ (points[0].x + points[1].x + points[2].x) / 3, (points[0].y + points[1].y + points[2].y) / 3 };
	}
	//points.size() > 3

	My_Delaunay D({ dst->width, dst->height });
	D.set_points(points);
	D.draw_subdiv(dst, { 0, 255, 0 }, 1);

	return D.Get_Center();
}

CvPoint Clustermode_processing::_Delaunay(std::vector<CvPoint> points, cv::Mat &dst){
	if (points.size() < 1){
		return{ 0, 0 };
	}
	if (points.size() == 1){
		return points[0];
	}
	if (points.size() == 2){

		line(dst, points[0], points[1], cvScalar(0, 0, 0, 0), 1, 8, 0);
		return{ (points[0].x + points[1].x) / 2, (points[0].y + points[1].y) / 2 };
	}
	if (points.size() == 3){

		line(dst, points[0], points[1], cvScalar(0, 0, 0, 0), 1, 8, 0);
		line(dst, points[1], points[2], cvScalar(0, 0, 0, 0), 1, 8, 0);
		line(dst, points[0], points[2], cvScalar(0, 0, 0, 0), 1, 8, 0);
		return{ (points[0].x + points[1].x + points[2].x) / 3, (points[0].y + points[1].y + points[2].y) / 3 };
	}
	//points.size() > 3

	My_Delaunay D(dst.size());
	D.set_points(points);
	D.draw_subdiv(dst, { 0, 255, 0 }, 1);

	return D.Get_Center();
}