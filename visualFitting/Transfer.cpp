#include "stdafx.h"
#include"Transfer.h"

Transfer::Transfer()
{
}

Transfer::~Transfer()
{
}
void Transfer::transferInit(Model mod, Garment gar, CKinect kin){
	this->mod = mod;
	this->gar = gar;
	this->kin = kin;
	//this->rect_model = mod.contourRect;
	this->rect_model = mod.modelSrcRect;
	this->rect_user = kin.contourRect;
	//this->m_model = mod.m_body;
	this->m_model = mod.modelSrc;
	this->m_user = kin.m_body;
	this->m_garment = gar.m_garment;
}
void Transfer::getModel(Model model){
	mod = model;
	//rect_model = model.contourRect;
	//m_model = model.m_body;
	//imshow("trans_m_body",m_model); 
}
void Transfer::getGarment(Garment garment){
	gar = garment;
}
void Transfer::getKinect(CKinect &kinect){
	kin = kinect;
	//rect_user = kinect.contourRect;
	//m_user = kinect.m_body;
}
void Transfer::getmUser(){
	m_user = kin.m_body;
	cout << "|kin_m_body_size|=>" << m_user.size() << endl;
}
void Transfer::getmModel(){
	//m_model = mod.m_body;
	mod.modelInitial();
	m_model = mod.modelSrc;
	imshow("modsrc", mod.modelSrc);
	cout << "|model_m_body_size|=>" << m_model.size() << endl;
}

void Transfer::getrectModel(){
	//rect_model = mod.contourRect;
	rect_model = mod.modelSrcRect;
	cout << "|rect_model_size|=>" << rect_model.size() << endl;
}
void Transfer::getRectUser(){
	rect_user = kin.contourRect;
	cout << "|rect_user_size|=>" << rect_user.size() << endl;
}

void Transfer::getmGarment(){
	m_garment = gar.m_garment;
	cout << "|m_garment_size|=>" << m_garment.size() << endl;
}

int Transfer::getUserWidth(){
	cout << "shoulderLeft Point" << kin.shoulderLeft_x << "," << kin.shoulderLeft_y << "|" << kin.shoulderLeft.x << "," << kin.shoulderLeft.y << endl;
	cout << "shoulderRight Point" << kin.shoulderRight_x << "," << kin.shoulderRight_y << "|" << kin.shoulderRight.x << "," << kin.shoulderRight.y << endl;
	if (kin.shoulderRight.x != 0 && kin.shoulderLeft.x != 0)
		return abs(kin.shoulderRight.x - kin.shoulderLeft.x)>0 ? abs(kin.shoulderRight.x - kin.shoulderLeft.x) : kin.contourRect.width;
	else
		return kin.contourRect.width;
}

int Transfer::getUserHeight(){
	cout << "ankleRight Point" << kin.ankleRight_x << "," << kin.ankleRight_y << "|" << kin.ankleRight.x << "," << kin.ankleRight.y << endl;
	cout << "ankleLeft Point" << kin.ankleLeft_x << "," << kin.ankleLeft_y << "|" << kin.ankleLeft.x << "," << kin.ankleLeft.y << endl;
	if (kin.ankleLeft.y != 0 && kin.ankleRight.y != 0 && kin.neck_y != 0 && kin.neck_y - kin.ankleLeft.y != 0 && kin.neck_y - kin.ankleLeft.y!=0){
		return abs(kin.neck_y - kin.ankleLeft.y) > abs(kin.neck_y - kin.ankleRight.y) ?
			abs(kin.neck_y - kin.ankleLeft.y) : abs(kin.neck_y - kin.ankleRight.y);
	}
	else{
		return kin.contourRect.height;
	}
}
int Transfer::getGarmentHeight(Mat mask){
	return 0;
}
int Transfer::getGarmentWidth(Mat mask){
	int rownum = mask.rows;
	int colnum = mask.cols;
	int flag = false;
	int left = 0;
	int right = colnum;
	for (int j = 0; j < colnum; j++){
		if (!(mask.at<Vec3b>(rownum / 2, j)[0] && mask.at<Vec3b>(rownum / 2, j)[1] && mask.at<Vec3b>(rownum / 2, j)[2])){
			left = j;
			break;
		}
	}
	for (int j = colnum-1; j >=0; j--){
		if (!(mask.at<Vec3b>(rownum / 2, j)[0] && mask.at<Vec3b>(rownum / 2, j)[1] && mask.at<Vec3b>(rownum / 2, j)[2])){
			right = j;
			break;
		}
	}
	return right-left;
}
Mat Transfer::combineMat(Mat x, Mat y, Mat mask){
	Mat dst = x.clone();
	int rownum = x.rows<y.rows?x.rows:y.rows;
	int colnum = x.cols<y.cols?x.cols:y.cols;
	for (int i = 0; i < rownum; i++){
		for (int j = 0; j < colnum; j++){
			//cout << "mask" << mask.at<Vec3b>(i, j)[0] << mask.at<Vec3b>(i, j)[1] << mask.at<Vec3b>(i, j)[2] << endl;
			/*if (mask.at<Vec3b>(i, j)[0] == 255 && mask.at<Vec3b>(i, j)[1] == 255 && mask.at<Vec3b>(i, j)[2] == 255){
				dst.at<Vec3b>(i, j)[0] = x.at<Vec3b>(i, j)[0];
				dst.at<Vec3b>(i, j)[1] = x.at<Vec3b>(i, j)[1];
				dst.at<Vec3b>(i, j)[2] = x.at<Vec3b>(i, j)[2];
			}
			else{
				dst.at<Vec3b>(i, j)[0] = y.at<Vec3b>(i, j)[0];
				dst.at<Vec3b>(i, j)[1] = y.at<Vec3b>(i, j)[1];
				dst.at<Vec3b>(i, j)[2] = y.at<Vec3b>(i, j)[2];
			}*/
			if (!(mask.at<Vec3b>(i, j)[0] && mask.at<Vec3b>(i, j)[1] && mask.at<Vec3b>(i, j)[2])){
				dst.at<Vec3b>(i, j)[0] = y.at<Vec3b>(i, j)[0];
				dst.at<Vec3b>(i, j)[1] = y.at<Vec3b>(i, j)[1];
				dst.at<Vec3b>(i, j)[2] = y.at<Vec3b>(i, j)[2];
			}
		}
	}
	return dst;
}

Point Transfer::getPointUser_x(){
	
	Point user_x(0,0);
	/*if (!kin.mapJoints.empty()){
		//cout << "jingdian 2:" << kin.mapJoints[JointType_Neck].x << "," << kin.mapJoints[JointType_Neck].y << endl;
		cout << "jingdian 3:" << kin.neck_x << "," << kin.neck_y << endl;
		//user_x.x = kin.mapJoints[JointType_Neck].x - kin.contourRect.x;
		//user_x.y = kin.mapJoints[JointType_Neck].y - kin.contourRect.y;
		user_x.x = kin.neck_x - kin.contourRect.x;
		user_x.y = kin.neck_y - kin.contourRect.y;
		circle(kin.m_body, user_x, 8, CV_RGB(255, 0, 0) ,- 1, 8, 0);
		//circle(kin.m_Color, kin.mapJoints[JointType_Neck], 8, CV_RGB(255, 0, 0), -1, 8, 0);
		//user_x.x = kin.mapJoints[JointType_Neck].x;
		//user_x.y = kin.mapJoints[JointType_Neck].y;
	}*/
	int cnt=1;
	if (kin.neck_x != 0 || kin.neck_y != 0){
		user_x = Point(kin.neck_x - kin.contourRect.x, kin.neck_y - kin.contourRect.y);
	}
	if (kin.shoulderLeft.x != 0 && kin.shoulderLeft.y != 0 && kin.shoulderRight.x!=0 &&kin.shoulderRight.x!=0 && kin.shoulderRight.y!=0){
		user_x += Point(kin.shoulderLeft.x - kin.contourRect.x, kin.shoulderLeft.y - kin.contourRect.y);
		user_x += Point(kin.shoulderRight.x - kin.contourRect.x, kin.shoulderRight.y - kin.contourRect.y);
		cnt = 3;
	}
	return Point(user_x.x/cnt,user_x.y/cnt);
}
Point Transfer::getPointModel_y(){
	Point model_y(0, 0);
	if (!mod.fpt->featurePt.empty()){
		model_y.x = (mod.fpt->featurePt[0].x + mod.fpt->featurePt[26].x) / 2;
			model_y.y = (mod.fpt->featurePt[0].y + mod.fpt->featurePt[26].y) / 2;
		//model_y.x = (mod.fpt->featurePt[0].x + mod.fpt->featurePt[26].x) / 2;
		//model_y.y = (mod.fpt->featurePt[0].y + mod.fpt->featurePt[26].y) / 2;
		circle(mod.modelSrc, model_y, 8, CV_RGB(0, 255, 255), -1, 8, 0);
		imshow("modelSrc2", mod.modelSrc);
	}
	return model_y;
}

Point Transfer::getNeckPFromMat(Mat mask){
	int rownum = mask.rows;
	int colnum = mask.cols;
	Point first(0, 0);
	Point second(0, 0);
	bool flagfirst = false;
	bool flagsec = false;
	for (int i = 0; i < rownum; i++){
		for (int j = 0; j < colnum; j++){
			if (!(mask.at<Vec3b>(i, j)[0] && mask.at<Vec3b>(i, j)[1] && mask.at<Vec3b>(i, j)[2])){
				if (!flagfirst){
					flagfirst = true;
					first.x = j;
					first.y = i;
				}
				if (!flagsec && flagfirst){
					if (abs(first.x - j)>5 && abs(first.y - i) < 5){
						flagsec = true;
						second.x = j;
						second.y = i;
					}
				}
			}
			if (flagfirst && flagsec) return Point((first.x + second.x) / 2, (first.y + second.y) / 2);
		}
	}
	return Point(0, 0);
}

Mat Transfer::positionCorrect(Point user_x, Point model_y, Mat src){
	Mat dst;
	const int rows = src.rows;
	const int cols = src.cols;
	int dx = user_x.x - model_y.x;
	int dy = user_x.y - model_y.y;
	int dstrows = src.rows + abs(dx);
	int dstcols = src.cols + abs(dy);
	dst.create(dstrows, dstcols, src.type());

	Vec3b *p;
	for (int i = 0; i < rows; i++)
	{
		p = dst.ptr<Vec3b>(i);
		for (int j = 0; j < cols; j++)
		{
			//平移后坐标映射到原图像
			int x = j - dx;
			int y = i - dy;

			//保证映射后的坐标在原图像范围内
			if (x >= 0 && y >= 0 && x < cols && y < rows)
				p[j] = src.ptr<Vec3b>(y)[x];
		}
	}
	return dst;
}
void Transfer::fillHole(const Mat srcBw, Mat &dstBw){
	Size m_Size = srcBw.size();
	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//延展图像
	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	cv::floodFill(Temp, Point(0, 0), Scalar(255));

	Mat cutImg;//裁剪延展的图像
	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

	dstBw = srcBw | (~cutImg);
}
Rect Transfer::getGarmentRect(Mat garment){
	Mat mgray;
	Mat mcanny;
	Mat mfillhole;
	cvtColor(garment, mgray, CV_BGR2GRAY);
	Canny(mgray, mcanny, 20, 200, 3);
	fillHole(mcanny, mfillhole);

	vector<vector<Point>> contours;
	//if (!contours.empty()) contours.clear();
	findContours(mfillhole, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	Rect res;
	res = boundingRect(contours[0]);
	return res;
	/*vector<vector<Point> > contours;
	threshold(garment, temp, 200, 255, CV_THRESH_BINARY);
	bitwise_xor(temp, Scalar(255,255,255), temp);
	imshow("hanshutemp",temp);
	findContours(temp, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);*/

}

/*
	对于任意的x属于user,存在x'属于garment，使得x-人体颈点p1=x'-服装颈点p2
*/
void Transfer::combineGarmentAndBody(Mat& user, Mat& garment, Point bodyNeck, Point garmentNeck, Mat mask_garment){
	int garrow = garment.rows;
	int garcol = garment.cols;

	int userrow = user.rows;
	int usercol = user.cols;
	Point temp = bodyNeck - garmentNeck;
	int move = 0;
	temp.x += move;
	temp.y -= move;
	cout << "diff of neck garment neck" << temp.x << "," << temp.y << endl;
	cout << "function_user.size()" << user.size() << endl;
	cout << "functon _garment.size()" << garment.size() << endl;
	cout << "functon _mask_garment size()" << mask_garment.size() << endl;
	int minleft=usercol;
	int maxright = 0;
	int minbottom = 0;
	int maxtop = userrow;
	int cnt = 0;
	for (int i = 0; i < garrow; i++){
		for (int j = 0; j < garcol; j++){
			//if (temp.x + j>=0 && temp.y + i>=0 && temp.x + j<=usercol && temp.y + i<=userrow
			//	&& !(mask_garment.at<Vec3b>(i, j)[0] || mask_garment.at<Vec3b>(i, j)[1] || mask_garment.at<Vec3b>(i, j)[2])){
			if (temp.x + j >= 0 && temp.y + i >= 0 && temp.x + j < usercol && temp.y + i < userrow){
				if (mask_garment.at<Vec3b>(i, j)[0] != 255 && mask_garment.at<Vec3b>(i, j)[1] != 255 && mask_garment.at<Vec3b>(i, j)[2] != 255){
					user.at<Vec3b>(i + temp.y, j + temp.x)[0] = garment.at<Vec3b>(i, j)[0];
					user.at<Vec3b>(i + temp.y, j + temp.x)[1] = garment.at<Vec3b>(i, j)[1];
					user.at<Vec3b>(i + temp.y, j + temp.x)[2] = garment.at<Vec3b>(i, j)[2];

					maxright = j + temp.x >= maxright ? j + temp.x : maxright;
					minleft = j + temp.x <= minleft ? j + temp.x : minleft;
					minbottom = i + temp.y > minbottom ? i + temp.y : minbottom;
					maxtop = i + temp.y < maxtop ? i + temp.y : maxtop;

					cnt++;
				}
				/*else{
					user.at<Vec3b>(i + temp.y, j + temp.x)[0] = 255;
					user.at<Vec3b>(i + temp.y, j + temp.x)[1] = 255;
					user.at<Vec3b>(i + temp.y, j + temp.x)[2] = 0;
				}*/
			}
		}
	}
	cout << "cnt: " <<cnt<< endl;
	cout << "minleft:" << minleft << "|maxright:" << maxright << "|dis=" << maxright - minleft << endl;
	cout << "minbottom:" << minbottom << "|maxtop:" << maxright << "|dis=" << minbottom - maxtop << endl;
}

void Transfer::transComputing(){
	//cout << "|kin_m_body_size|=>" << kin.m_Color.size() << "  |mod_m_body_size|=>" << mod.m_body.size() << endl;
	//imshow("trans_m_body",mod.m_body);
	getmUser();
	getmModel();
	imshow("m_model",mod.modelSrc);
	getRectUser();
	getrectModel();
	getmGarment();
	Mat m_model2(m_model.size(), m_model.type());
	if (!m_user.empty()){
		resize(m_model, m_model2, cv::Size(), m_user.cols*1.0 / m_model.cols, m_user.rows*1.0 / m_model.rows);
	}
	cout << "|m_model2_size|=>" << m_model2.size() << endl;
	imshow("m_model2", m_model2);
	mod.getFpt();
	//Mat m_garment2(m_garment.size(),m_garment.type(),Scalar(255));
	Mat m_garment2;
	m_garment2 = m_garment(rect_model);
	cout << "|m_garment2_size|=>" << m_garment2.size() << endl;
	Mat m_garment3(m_garment.size(), m_garment.type(), Scalar(255));
	if (!m_garment2.empty() && !m_user.empty()) {
		//resize(m_garment2, m_garment3, cv::Size(), m_user.cols*1.0 / m_model.cols, m_user.rows*1.0 / m_model.rows);
		m_garment3 = m_garment2.clone();
		//imshow("m_garment3", m_garment3);
		int userHeight = getUserHeight();
		int userWidth = getUserWidth();
		cout << "user width height" << userWidth << "," << userHeight << endl;
		Rect garmentRect = getGarmentRect(m_garment3);
		int garmentHight = garmentRect.height;

		Mat newm_garment = m_garment3(garmentRect);
		imshow("newm_garment",newm_garment);
		Mat newGarmentMask;
		threshold(newm_garment, newGarmentMask, 200, 255, CV_THRESH_BINARY);

		//int garmentWidth = getGarmentWidth(newGarmentMask);
		int garmentWidth = garmentRect.width;
		cout << "garmentWidth height" << garmentWidth << "," << garmentHight << endl;
		Mat newm_garment2;
		int addwidth = 0;
		resize(newm_garment, newm_garment2, cv::Size(), (addwidth + userWidth*1.0) / (garmentWidth*1.0), (userHeight*1.0) / (garmentHight*1.0));
		imshow("newg2", newm_garment2);
		cout << "newgarment.size()"<<newm_garment2.size() <<endl;
		Point garmentNeckPoint = getNeckPFromMat(newGarmentMask);
		Point newGarmentNeckPoint1 = Point(garmentNeckPoint.x*userWidth*1.0 / garmentWidth, garmentNeckPoint.y*userHeight*1.0 / garmentHight);
		//circle(newm_garment, garmentNeckPoint, 8, CV_RGB(255, 0, 0),- 1, 8, 0);
		//circle(newm_garment2, Point(newGarmentNeckPoint.x, newGarmentNeckPoint.y+10), 8, CV_RGB(255, 0, 0), -1, 8, 0);
		cout << "garmentNeckPoint=" << garmentNeckPoint.x << "," << garmentNeckPoint.y << endl;
		cout << "newGarmentNeckPoint1=" << newGarmentNeckPoint1.x << "," << newGarmentNeckPoint1.y << endl;

		cout << "|m_garment3_size|=>" << m_garment3.size() << endl;
		Point user_neck = getPointUser_x();
		cout << "|user_neck-x,y|=>" << user_neck.x << "," << user_neck.y << endl;
		//Point model_y = getPointModel_y();
		//cout << "|model_y-x,y|=>" << model_y.x << "," << model_y.y << endl;

		Mat mask_newm_garment;
		threshold(newm_garment2, mask_newm_garment, 200, 255, CV_THRESH_BINARY);
		Point newGarmentNeckPoint2 = getNeckPFromMat(mask_newm_garment);
		cout << "newGarmentNeckPoint2=" << newGarmentNeckPoint2.x << "," << newGarmentNeckPoint2.y << endl;
		Mat user2;
		user2.create(m_user.size(),m_user.type());
		//m_user.copyTo(user2);
		user2 = m_user.clone();

		cout << "mask_newm_garment.size()"<<mask_newm_garment.size() << endl;
		cout << "outside function" << user2.size() << newm_garment2.size() << mask_newm_garment.size() << endl;

		combineGarmentAndBody(user2, newm_garment2, user_neck, newGarmentNeckPoint1, mask_newm_garment);


		cout << "after function" << user2.size() << newm_garment2.size() << mask_newm_garment.size() << endl;
		imshow("user garment", user2);
		imshow("mask_newm_garment", mask_newm_garment);

		/*Mat m_garment4(m_garment3.size(), m_garment3.type());
		m_garment4 = positionCorrect(user_neck, newGarmentNeckPoint1, newm_garment2);

		imshow("garment4", m_garment4);*/
		/*
		//model_y.x *= m_user.cols*1.0 / m_model.cols;
		//model_y.y *= m_user.rows*1.0 / m_model.rows;
		Mat m_garment4(m_garment3.size(), m_garment3.type());
		m_garment4 = positionCorrect(user_x,model_y,m_garment3);
		circle(m_garment4, model_y,8, CV_RGB(0, 255, 255), -1, 8, 0);
		imshow("m_garment4", m_garment4);
		cout << "|m_garment4_size|=>" << m_garment4.size() << endl;
		Mat temp;
		//threshold(m_garment3, temp, 200, 255, CV_THRESH_BINARY);
		threshold(m_garment4, temp, 200, 255, CV_THRESH_BINARY);
		imshow("temp", temp);
		//Mat mbodymask2;
		Mat mbodymask(m_garment3.size(), CV_8UC1);
		//bitwise_xor(temp, Scalar(255,255,255), mbodymask2);
		//imshow("mbodymask2", mbodymask2);
		cvtColor(temp, mbodymask, CV_BGR2GRAY);
		imshow("mbodymask", mbodymask);
		//Mat m_test(m_garment3.size(), m_garment3.type());
		//m_user.copyTo(m_test, mbodymask);
		//imshow("m_test", m_test);
		imshow("m_user2", m_user);
		imshow("m_garment3", m_garment3);
		Mat m_test2(m_garment3.size(), m_garment3.type());
		m_test2 = combineMat(m_user, m_garment4, temp);
		imshow("m_test2", m_test2);

		*/

		/*Mat mbodymask2(m_garment3.size(), CV_8UC1);
		bitwise_xor(mbodymask, Scalar(255), mbodymask2);
		imshow("mbodymask2", mbodymask2);
		m_test.copyTo(m_test2, mbodymask2);
		imshow("m_test2", m_test2);
		//m_test.copyTo(m_garment3, mbodymask);
		//imshow("m_user2", m_garment3);
		//Mat mtest2 = m_garment3(mbodymask);
		//add(m_user,m_garment3, m_test, mbodymask);
		//bitwise_xor(m_garment3,m_test,m_test2);
		//imshow("m_test",m_test2);
		//cout << "|m_color_size|=>" << kin.m_Color.size() << endl;
		//imshow("m_color",kin.m_Color);*/

		

	}



}
