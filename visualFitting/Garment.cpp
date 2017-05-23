#include "stdafx.h"
#include "Garment.h"
#include "CGALTri.h"
#include <vector>

Garment::Garment(){

}
Garment::Garment(const Garment& gar){
	if (!m_garment.empty()) m_garment.release();
	m_garment = gar.m_garment.clone();
	featureVector = gar.featureVector;
}
Garment&Garment::operator = (const Garment& gar){
	if (!m_garment.empty()) m_garment.release();
	m_garment = gar.m_garment.clone();
	featureVector = gar.featureVector;
	return *this;
}
Garment::~Garment(){

}
void Garment::garmentInitial(){
	IplImage *srcIplImage = cvLoadImage("garment\\allgarment.png");
	if (!srcIplImage->imageData)
	{
		cout << "Fail to load garment pic" << endl;
		return ;
	}
	Mat srcGarment(srcIplImage);
	m_garment = srcGarment.clone();
	imshow("srcGarment",m_garment);

	getContourPoint();
}

void Garment::getContourPoint(){
	Mat grayGarment;
	cvtColor(m_garment, grayGarment, CV_RGB2GRAY);
	Mat thdGarment;
	threshold(grayGarment, thdGarment,200,255,CV_THRESH_BINARY);
	bitwise_not(thdGarment, thdGarment);
	vector<vector<Point>> garmentContours;
	Mat garment_temp = m_garment.clone();
	findContours(thdGarment, garmentContours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	drawContours(garment_temp, garmentContours, -1, CV_RGB(0, 0, 0), 2);
	cout << "garmentContours[0].size()=="<<garmentContours[0].size() << endl;
	vector<Point>temp = getGarmentContoursPoint(garmentContours);
	
	getCircle200(garment_temp,temp);
	imshow("garment_temp", garment_temp);

	int res = CGALTri::getmain(40,garment_temp, garmentPoints, "garmentTriImage");
}

vector<Point>  Garment::getGarmentContoursPoint(vector<vector<Point>> &contours)
{
	int cmax = 0;   // 最大轮廓长度  
	vector<Point>temp;
	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if ((itc->size()) > cmax)
		{
			cmax = itc->size();
			temp.clear();
			temp = *itc;
		}
		itc++;
	}
	vector<Point>contoursPoint = temp;
	//cout << "contoursPoint1_size()=" << contoursPoint1.size() << endl;
	return contoursPoint;
}

void Garment::getCircle200(Mat srcBw, vector<Point> &contours){
	for (int j = 0; j < contours.size(); j += contours.size() / 200 + 1){
		circle(srcBw, contours[j], 3, CV_RGB(255, 0, 0), -1, 8, 0);
		garmentPoints.push_back(contours[j]);
	}
	//cvCircle(srcBw, contoursPoint2[2], 5, CV_RGB(0, 255, 0), -1, 8, 0);
	//cout << "contoursPoint2_size()=" << contoursPoint2.size() << endl;
	//cout << "contoursPoint2[0]_y()=" << contoursPoint2[0].y << endl;
}