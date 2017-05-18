#ifndef FEATUREPOINT_H
#define FEATUREPOINT_H
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#define ABS_FLOAT_0 0.000001 
using namespace std;
using namespace cv;
template<class T>
class Compare
{
public:
	int operator()(const T& A, const T& B)const
	{
		if (A.x > B.x)
			return 0;
		if (A.x == B.x && A.y > B.y) return 0;
		return 1;
	}
};

static  const Scalar color[] = {
	CV_RGB(255, 0, 0), CV_RGB(0, 255, 0), CV_RGB(0, 0, 255),
	CV_RGB(255, 255, 0), CV_RGB(255, 0, 255), CV_RGB(0, 255, 255),
	CV_RGB(128, 0, 0), CV_RGB(0, 128, 0), CV_RGB(0, 0, 128),
	CV_RGB(128, 128, 0), CV_RGB(128, 0, 128), CV_RGB(0, 128, 128),
	CV_RGB(64, 0, 0), CV_RGB(0, 0, 0)
};


class featurePoint{
public:


	featurePoint();
	~featurePoint();

	featurePoint(const featurePoint& fpt);
	featurePoint& operator = (const featurePoint& fpt);
	void featurepointInit();
	//void getSpecialPoint27(vector<Point>contoursPoints);
	double getDistance(Point p1, Point p2);
	double getCosAngle2(Point pt1, Point pt2, Point pt3, Point pt4);
	double getCosAngle(Point pt1, Point pt2, Point pt0);
	float GetTriangleSquar(Point pt0, Point pt1, Point pt2);
	bool isInTriangle(Point A, Point B, Point C, Point D);
	bool PtInAnyRect(Point pCur, Point pLT, Point pLB, Point pRB, Point pRT);
	Point getPointP0(vector<Point>contoursPoint, Point v0);
	Point getPointP26(vector<Point>contoursPoint, Point v0);
	Point getPointP25(vector<Point>contoursPoint, Point p26);
	Point getPointP1(vector<Point>contoursPoint, Point p0);
	Point getPointV0(vector<Point>contoursPoint);
	Point getPointV1(vector<Point>contoursPoint, Point p1);
	Point getPointV2(vector<Point>contoursPoint, Point p25);
	Point getPointP2(vector<Point>contoursPoint, Point p1, Point v1);
	Point getPointP24(vector<Point>contoursPoint, Point v2, Point p25);
	Point getPointP3(vector<Point>contoursPoint, Point p1, Point p2, Point v1);
	Point getPointP23(vector<Point>contoursPoint, Point p24, Point p25, Point v2);
	Point getPointP4(vector<Point>contoursPoint, Point p2, Point p3, Point v1, Point p6);
	Point getPointP5(vector<Point>contoursPoint, Point p1, Point p2, Point v1, Point p6);
	Point getPointP21(vector<Point>contoursPoint, Point p25, Point p24, Point p20, Point v2);
	Point getPointP7(vector<Point>contoursPoint, Point p5, Point p6, Point v3);
	Point getPointP19(vector<Point>contoursPoint, Point p21, Point p20, Point v4);
	Point getPointP8(vector<Point>contoursPoint, Point p6, Point p7, Point v3);
	Point getPointP9(vector<Point>contoursPoint, Point p6, Point p8, Point v3);
	Point getPointP10(vector<Point>contoursPoint, Point p8, Point p9, Point v3);
	Point getPointP11(vector<Point>contoursPoint, Point p9, Point p10, Point v3, Point p13);
	Point getPointP12(vector<Point>contoursPoint, Point p8, Point p9, Point v3, Point p13);
	Point getPointP18(vector<Point>contoursPoint, Point p20, Point p19, Point v4);
	Point getPointP17(vector<Point>contoursPoint, Point p20, Point p18, Point v4);
	Point getPointP16(vector<Point>contoursPoint, Point p18, Point p17, Point v4);
	Point getPointP15(vector<Point>contoursPoint, Point p17, Point p16, Point p13, Point v4);
	Point getPointP14(vector<Point>contoursPoint, Point p18, Point p17, Point p13, Point v4);
	Point getPointP22(vector<Point>contoursPoint, Point p24, Point p23, Point p20, Point v2);
	Point getPointP6(vector<Point>contoursPoint, Point v1);
	Point getPointP20(vector<Point>contoursPoint, Point v2);
	Point getPointV3(vector<Point>contoursPoint, Point p6);
	Point getPointV4(vector<Point>contoursPoint, Point p20);
	Point getPointP13(vector<Point>contoursPoint, Point v3);
	void on_mouse(int event, int x, int y, int flags, void* img);
	void fillHole(const Mat srcBw, Mat &dstBw);
	void getSizeContours(vector<vector<Point>> &contours);
	vector<vector<Point>> getMaxSizeContours(vector<vector<Point>> &contours);
	void getBodyContoursPoint(vector<vector<Point>> &contours);
	//vector<Point> getBodyContoursPoint(vector<vector<Point>> &contours);
	void getCircle200(Mat srcBw, vector<Point> &contours);
	void getSpecialPoint27(Mat srcBw, vector<Point>&contoursPoint);
	int getRegion(Point p);
	void getBodyRegion(Mat& pic, IplImage *srcBw);
	void getUpGarmentRegion(Mat& upgarment, Mat srcBw);

	vector<Point>contours;
	vector<Point>featurePt;

	vector<Point>contoursPoint1;
	vector<Point>contoursPoint2;
	//vector<Point>featurePoints;
	vector<Point>auxiliaryPoints;

	map<Point, int, Compare<Point> >mapBodyRegion;
	map<Point, int, Compare<Point> >mapUpGarmentRegion;

	//Scalar color[14];

	int threshval;
	int pointinterval;
	int M;

private:

};



#endif