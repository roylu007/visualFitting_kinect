#ifndef GARMENT_H
#define GARMENT_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class Garment{
public:
	Garment();
	Garment(const Garment& gar);
	Garment& operator = (const Garment& gar);
	~Garment();
	Mat m_garment;
	Point featureVector;
	void garmentInitial();
private:

};

#endif