#include "stdafx.h"
#include "Garment.h"
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
}