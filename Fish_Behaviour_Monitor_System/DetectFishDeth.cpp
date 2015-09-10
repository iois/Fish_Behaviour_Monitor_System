#include"DetectFishDeth.h"

int    DetectFishDeth::SIZE_FRAME=15;
double DetectFishDeth::MAX_SPEED=60;
double DetectFishDeth::MAX_AREA=1400;
double DetectFishDeth::MAX_POZ=640;

double DetectFishDeth::SPEED_THRESH=0.8;
double DetectFishDeth::NEAREST_POINT_THRESH=6;

double DetectFishDeth::theta1=0.8;//relative position
double DetectFishDeth::theta2=0.7;
double DetectFishDeth::theta3=1.5;//speed

double DetectFishDeth::alpha1=2;//
double DetectFishDeth::alpha2=1;
double DetectFishDeth::alpha3=2;

DetectFishDeth::DetectFishDeth(const vector<vector<Point> > &contours, int bp, int _avg_area, int _num_fish)
{
	num_fish = _num_fish;

	border_position = bp;
	avg_area = _avg_area;

	for (int i = 0; i < num_fish; ++i)
	{
		vector <Point> tra(SIZE_FRAME, { 0, 0 });

		trajectory.push_back(tra);

		Moments m = moments(contours[i]);

		center_point.push_back(Point(m.m10 / m.m00, m.m01 / m.m00));//计算鱼的中心点

		contour_area.push_back(contourArea(contours[i]));//调用外部函数求轮廓面积

		fish_Assigned.push_back(0);

		nearest_point.push_back(0);
	}
}

void DetectFishDeth::update_data(vector<vector<Point>> contours)
{
	for (int i = 0; i < num_fish; ++i){
		fish_Assigned[i] = 0;
	}
	// 没有重叠的情况，目标数量==鱼条数
	if (contours.size() == num_fish){
		for (int i = 0; i < num_fish; ++i)
		{
			Moments m = moments(contours[i]);
			Point center = Point(m.m10 / m.m00, m.m01 / m.m00);

			//todo:
			//目标身份分配
			int index = FindIndex(center_point, center);

			trajectory[index].insert(trajectory[index].cbegin(), center);
			trajectory[index].pop_back();
			center_point[index] = center;

			nearest_point[index] = get_nearest_ponit(center, contours[i]);

			contour_area[index] = contourArea(contours[i]);
			//cout << index<<": "<<contour_area[index] << endl;
		}
	}
	//重叠的情况
	else if (contours.size() < num_fish){

		sort(contours.begin(), contours.end(), [](vector<Point> a, vector<Point> b){return contourArea(a) > contourArea(b); });

		for (int j = 0; j < contours.size(); ++j){
			double n = round(contourArea(contours[j]) / avg_area);//重叠区域有几个鱼

			Moments m = moments(contours[j]);
			Point center = Point(m.m10 / m.m00, m.m01 / m.m00);

			for (int i = 0; i < n; ++i){
				int index = FindIndex(center_point, center);
				trajectory[index].insert(trajectory[index].cbegin(), center);
				trajectory[index].pop_back();
				center_point[index] = center;
				contour_area[index] = contourArea(contours[j]) / n;

				if (n > 1){
					nearest_point[index] = -1;
				}
				else{
					nearest_point[index] = get_nearest_ponit(center, contours[i]);
				}
			}
		}
	}
}

double DetectFishDeth::get_nearest_ponit(Point center, const vector<Point> &contours){
	vector<double> dis;
	for (int j = 0; j < contours.size(); ++j){
		dis.push_back(sqrt(pow(center.x - contours[j].x, 2) + pow(center.y - contours[j].y, 2)));
	}
	return *min_element(dis.begin(), dis.end());
}

vector<double> DetectFishDeth::get_prob_death(const vector<vector<Point> > &contours)
{
	this->update_data(contours);

	vector<double> prob;
	vector<double> x = get_relativepoz();   //位置	
	vector<double> diffarea = get_diff_area();   //面积
	vector<double> _speed = get_speed();   //速度

	vector<double> y1;
	vector<double> y2;
	vector<double> y3;
	//double  = 1, theta2 = 1, theta3 = 1;
	for (int i = 0; i < num_fish; ++i)
	{
		y3.push_back(-log(_speed[i]*alpha3));
		y2.push_back(-log((1 - diffarea[i])*alpha2));
		y1.push_back(-log(x[i]*alpha1));
		//cout << "y3: " << y3[i] << endl;
		//cout << "y1:" << y1[i] << " y2:" << y2[i] << " y3:" << y3[i] << endl;
	}
	
	for (int i = 0; i < num_fish; ++i)
	{
		if (_speed[i]>SPEED_THRESH){
			prob.push_back(0);
		}
		else if (nearest_point[i]>0 && nearest_point[i] < NEAREST_POINT_THRESH){
			prob.push_back(0);
		}
		else{
			prob.push_back(sigmoid(theta1*y1[i] + theta2*y2[i] + theta3*y3[i]));
		}
		cout << "nearest_point : "<<nearest_point[i] << endl;
	}
	
	return prob;
}

vector<double> DetectFishDeth::get_speed()
{
	vector<double> speed;
	for (int i = 0; i < num_fish; ++i)
	{
		double x_sum = 0;
		double y_sum = 0;
		double s=0;
		for (int j = 1; j < trajectory[i].size(); ++j)
		{
			x_sum = pow((trajectory[i][j].x - trajectory[i][j - 1].x),2);
			y_sum = pow((trajectory[i][j].y - trajectory[i][j - 1].y),2);
			s += sqrt(x_sum + y_sum);
		}
		//cout << "s: " << s << endl;
		if (s > MAX_SPEED){
			speed.push_back(1);
		}
		else{
			speed.push_back(s/ MAX_SPEED);
		}
	}
	return speed;
}

vector<double> DetectFishDeth::get_diff_area()
{
	//todo: 考虑重叠的情况
	vector<double> diff_area;
	for (int i = 0; i < num_fish; ++i)
	{
		//if (contour_area[i] > avg_area){
			diff_area.push_back((contour_area[i] - avg_area) / MAX_AREA);
		//}
		//else{
		//	diff_area.push_back(0.05);
		//}
	}
	return diff_area;
}

DetectFishDeth::~DetectFishDeth()
{

}

int DetectFishDeth::FindIndex(vector<Point> center_point, Point center)
{
	int index = 0;
	double distence2 = 0;
	double max_distence = INT_MAX;
	for (int j = 0; j < num_fish; j++)
	{
		distence2 = (pow(center_point[j].x - center.x, 2) + pow(center_point[j].y - center.y, 2));

		if (distence2<max_distence && fish_Assigned[j] == 0)
		{
			index = j;
			max_distence = distence2;
		}
	}
	fish_Assigned[index] = 1;
	return index;
}

double sigmoid(double x){
	return 1 / (1 + exp(-x));
}