#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "featurePoint.h"
using namespace std;
using namespace cv;

class Model{
public:
	Model();
	Model(const Model& m);
	~Model();
	void modelInitial();
	featurePoint* fpt;
	Rect contourRect;
	Mat m_body;
	Model& operator = (const Model&);  
	void getFpt(Mat src);
	void getFpt();
	void changeRect(Rect & r, int change);

	Mat modelSrc;
	Rect modelSrcRect;
	Mat cannyImage;
	Mat fillholeImage;
private:
	Mat m_model;
};

#endif