#include "stdafx.h"
#include "Model.h"

Model::Model(){
	fpt = new featurePoint();
}

Model::Model(const Model& m){
	fpt = new featurePoint;
	*fpt=*(m.fpt);
	contourRect = m.contourRect;
	m_body = m.m_body;
	modelSrc = m.modelSrc;
	modelSrcRect = m.modelSrcRect;
	//m_model = m.m_model;
}
Model::~Model(){
	delete fpt;
}
Model& Model:: operator = (const Model& m){

	if (fpt != NULL) delete fpt;
	fpt = new featurePoint;
	*fpt = *m.fpt;
	contourRect = m.contourRect;
	m_body = m.m_body;
	m_model = m.m_model.clone();
	modelSrc = m.modelSrc.clone();
	modelSrcRect = m.modelSrcRect;
	return *this;
}

void Model::changeRect(Rect & r, int change){
	//r = r + Size(change, change);
}
void Model::modelInitial(){
	fpt->featurepointInit();
	IplImage *plmgsrc = cvLoadImage("cccc.png");
	modelSrc = Mat(plmgsrc);
	modelSrcRect = Rect(0, 0, modelSrc.cols, modelSrc.rows);
	

	if (!plmgsrc->imageData)
	{
		cout << "Fail to load image" << endl;
		return ;
	}
	IplImage *plmg8u = NULL;
	IplImage *plmgCanny = NULL;
	plmg8u = cvCreateImage(cvGetSize(plmgsrc), IPL_DEPTH_8U, 1);
	plmgCanny = cvCreateImage(cvGetSize(plmgsrc), IPL_DEPTH_8U, 1);

	cvCvtColor(plmgsrc, plmg8u, CV_BGR2GRAY);
	cvCanny(plmg8u, plmgCanny, 20, 200, 3);
	cannyImage=Mat(plmgCanny);
	fpt->fillHole(cannyImage, fillholeImage);

	/*vector<vector<Point>> contours;
	//CV_CHAIN_APPROX_NONE  获取每个轮廓每个像素点  
	findContours(fillholeImage, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	//getSizeContours(contours);
	//contours2=getMaxSizeContours(contours);
	cout << "contours_size=" << contours.size() << endl;
	fpt->getBodyContoursPoint(contours);
	//cout << contours[contours.size() - 1].size() << endl;
	Mat result(cannyImage.size(), CV_8U, Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 2);   // -1 表示所有轮廓  
	//namedWindow("result", 1);
	//cvSetMouseCallback("result", on_mouse, (IplImage *) result);
	//imshow("result", result);
	Mat src(plmgsrc);
	/*fpt->getCircle200(plmgsrc, fpt->contoursPoint1);
	cout << "dd"<<fpt->contoursPoint2.size()<<endl;
	fpt->getSpecialPoint27(plmgsrc, fpt->contoursPoint2);
	//fpt->getBodyRegion(resulttemp, plmgsrc);
	drawContours(src, contours, -1, Scalar(0, 0, 255, 0), 1);   // -1 表示所有轮廓
	contourRect = boundingRect(fpt->contoursPoint1);
	//changeRect(contourRect, 20);
	rectangle(src,contourRect,Scalar(255,128,64),3);
	imshow("src", src);

	m_body = src(contourRect);
	imshow("body", m_body);*/
	/*delete plmg8u;
	plmg8u = NULL;
	delete plmgsrc;
	plmgsrc = NULL;
	delete plmgCanny;
	plmgCanny = NULL;*/

}
void getFpt(Mat src){

}
void Model::getFpt(){
	/*Mat src;
	src = src2.clone();
	fpt = new featurePoint();
	fpt->featurepointInit();
	IplImage *plmgsrc = &IplImage(src);
	if (!plmgsrc->imageData)
	{
		cout << "Fail to load image" << endl;
		return;
	}
	IplImage *plmg8u = NULL;
	IplImage *plmgCanny = NULL;
	plmg8u = cvCreateImage(cvGetSize(plmgsrc), IPL_DEPTH_8U, 1);
	plmgCanny = cvCreateImage(cvGetSize(plmgsrc), IPL_DEPTH_8U, 1);

	cvCvtColor(plmgsrc, plmg8u, CV_BGR2GRAY);
	cvCanny(plmg8u, plmgCanny, 20, 200, 3);

	Mat cannyImage(plmgCanny);
	Mat resulttemp;
	//int cnt = 10;
	fpt->fillHole(cannyImage, resulttemp);
	//while (cnt--) fpt->fillHole(resulttemp, resulttemp);
	imshow("resulttemp",resulttemp);
	*/

	vector<vector<Point>> contours;
	//if (!contours.empty()) contours.clear();
	findContours(fillholeImage, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	fpt->getBodyContoursPoint(contours);
	//cout << contours[contours.size() - 1].size() << endl;
	Mat result(cannyImage.size(), CV_8U, Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 2);   // -1 表示所有轮廓
	//Mat src(plmgsrc);
	fpt->getCircle200(modelSrc, fpt->contoursPoint1);
	cout << "dd" << fpt->contoursPoint2.size() << endl;
	fpt->getSpecialPoint27(modelSrc, fpt->contoursPoint2);
	//fpt->getBodyRegion(resulttemp, plmgsrc);
	drawContours(modelSrc, contours, -1, Scalar(0, 0, 255, 0), 1);   // -1 表示所有轮廓
	cout << "fpt->contourspoint"<<fpt->contoursPoint1.size() <<endl;
	contourRect = boundingRect(fpt->contoursPoint1);
	changeRect(contourRect, 10);
	rectangle(modelSrc, contourRect, Scalar(255, 128, 64), 3);
	//imshow("src", src);

	m_body = modelSrc(contourRect);
	imshow("m_body", m_body);

	/*delete plmg8u;
	plmg8u = NULL;
	delete plmgsrc;
	plmgsrc = NULL;
	delete plmgCanny;
	plmgCanny = NULL;*/
}

