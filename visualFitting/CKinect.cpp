#include "stdafx.h"
#include "CKinect.h"
#include "Head.h"
#include <math.h>
#include <map>
#include <limits>
#include <strsafe.h>
#include "CGALTri.h"
static const DWORD c_AppRunTime = 5 * 60;//程序运行时间(s)，设置5*60表示运行5分钟后程序自动关闭
static const float c_JointThickness = 3.0f;
static const float c_TrackedBoneThickness = 6.0f;
static const float c_InferredBoneThickness = 1.0f;
static const float c_HandSize = 30.0f;
CKinect::CKinect()
{
	cout << "gouzaokinect" << endl;
	m_pKinectSensor = NULL;
	m_pCoordinateMapper = NULL;
	m_pMultiSourceFrameReader = NULL;
	m_pDepthCoordinates = NULL;
	m_pOutputRGBX = NULL;
	m_pBackgroundRGBX = NULL;
	m_pColorRGBX = NULL;

	// create heap storage for composite image pixel data in RGBX format
	m_pOutputRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for background image pixel data in RGBX format
	m_pBackgroundRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for color pixel data in RGBX format
	m_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for the coorinate mapping from color to depth
	m_pDepthCoordinates = new DepthSpacePoint[cColorWidth * cColorHeight];

	m_pDepthRGBX = new RGBQUAD[cDepthWidth * cDepthHeight];// create heap storage for color pixel data in RGBX format

	//初始化OpenCV数组
	m_Depth.create(cDepthHeight, cDepthWidth, CV_16UC1);
	m_Color.create(cColorHeight, cColorWidth, CV_8UC4);
	m_BodyIndex.create(cDepthHeight, cDepthWidth, CV_8UC1);


	fpt = new featurePoint();
	gmt = new Garment();
	//mdl = new Model();
	/*if (!mapJoints.empty())
		mapJoints.clear();*/
	neck_x = 0;
	neck_x = 0;
	shoulderLeft = Point(0, 0);
	shoulderRight = Point(0, 0);
	footRight = Point(0, 0);
	footLeft = Point(0, 0);
	neck = Point(0, 0);
	ankleLeft_x = 0;
	ankleRight_x = 0;
	ankleLeft_y = 0;
	ankleRight_y = 0;
	shoulderLeft_x =0;
	shoulderRight_x = 0;
	shoulderLeft_y = 0;
	shoulderRight_y = 0;
}
CKinect::CKinect(const CKinect& kin){
	/*m_pKinectSensor = NULL;
	m_pCoordinateMapper = NULL;
	m_pMultiSourceFrameReader = NULL;
	m_pDepthCoordinates = NULL;
	m_pOutputRGBX = NULL;
	m_pBackgroundRGBX = NULL;
	m_pColorRGBX = NULL;

	//m_pKinectSensor = new IKinectSensor;
	// create heap storage for composite image pixel data in RGBX format
	m_pOutputRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for background image pixel data in RGBX format
	m_pBackgroundRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for color pixel data in RGBX format
	m_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for the coorinate mapping from color to depth
	m_pDepthCoordinates = new DepthSpacePoint[cColorWidth * cColorHeight];

	m_pDepthRGBX = new RGBQUAD[cDepthWidth * cDepthHeight];// create heap storage for color pixel data in RGBX format
	*/
	/*if (m_pOutputRGBX != NULL) delete m_pOutputRGBX;
	m_pOutputRGBX = new RGBQUAD[cColorWidth * cColorHeight];
	*m_pOutputRGBX = *kin.m_pOutputRGBX;

	if (m_pBackgroundRGBX != NULL) delete m_pBackgroundRGBX;
	m_pBackgroundRGBX = new RGBQUAD[cColorWidth * cColorHeight];
	*m_pBackgroundRGBX = *kin.m_pBackgroundRGBX;

	if (m_pColorRGBX != NULL) delete m_pColorRGBX;
	m_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight];
	*m_pColorRGBX = *kin.m_pColorRGBX;

	if (m_pDepthCoordinates != NULL) delete m_pDepthCoordinates;
	m_pDepthCoordinates = new DepthSpacePoint[cColorWidth * cColorHeight];
	*m_pDepthCoordinates = *kin.m_pDepthCoordinates;

	if (m_pDepthRGBX != NULL) delete m_pDepthRGBX;
	m_pDepthRGBX = new RGBQUAD[cDepthWidth * cDepthHeight];
	*m_pDepthRGBX = *kin.m_pDepthRGBX;*/
	SAFE_RELEASE_VEC(m_pOutputRGBX);
	SAFE_RELEASE_VEC(m_pBackgroundRGBX);
	SAFE_RELEASE_VEC(m_pColorRGBX);
	SAFE_RELEASE_VEC(m_pDepthCoordinates);
	SAFE_RELEASE_VEC(m_pDepthRGBX);
	if (!m_Depth.empty()) m_Depth.release();
	m_Depth = kin.m_Depth.clone();
	if (!m_Color.empty()) m_Color.release();
	m_Color = kin.m_Color.clone();
	if (!m_BodyIndex.empty()) m_BodyIndex.release();
	m_BodyIndex = kin.m_BodyIndex.clone();
	if (!m_body.empty()) m_body.release();
	m_body = kin.m_body.clone();

	if (fpt != NULL) SAFE_RELEASE(fpt);
	fpt = new featurePoint;
	*fpt = *kin.fpt;
	if (gmt != NULL) delete gmt;
	gmt = new Garment;
	*gmt = *kin.gmt;
	/*if (!mapJoints.empty()) mapJoints.clear();
	mapJoints = kin.mapJoints;*/
	neck_x = kin.neck_x;
	neck_y = kin.neck_y;
	shoulderLeft = kin.shoulderLeft;
	shoulderRight = kin.shoulderRight;
	footRight = kin.footRight;
	footLeft = kin.footLeft;
	neck = kin.neck;
	ankleLeft = kin.ankleLeft;
	ankleRight = kin.ankleLeft;
	ankleLeft_x = kin.ankleLeft_x;
	ankleLeft_y = kin.ankleLeft_y;
	ankleRight_y = kin.ankleRight_y;
	ankleRight_x = kin.ankleRight_x;
	shoulderLeft_x = kin.shoulderLeft_x;
	shoulderRight_x = kin.shoulderRight_x;
	shoulderLeft_y = kin.shoulderLeft_y;
	shoulderRight_y = kin.shoulderRight_y;
	//if (!mapJoints.empty()) mapJoints.clear();
	//if (mdl != NULL) delete mdl;
	//mdl = new Model;
	//*mdl = *kin.mdl;

	/*m_Depth.create(cDepthHeight, cDepthWidth, CV_16UC1);
	m_Color.create(cColorHeight, cColorWidth, CV_8UC4);
	m_BodyIndex.create(cDepthHeight, cDepthWidth, CV_8UC1);
	m_body = kin.m_body.clone();
	contourRect = kin.contourRect;
	m_Depth=kin.m_Depth.clone();
	m_Color; kin.m_Color.clone();
	m_BodyIndex=kin.m_BodyIndex.clone();
	fpt=new featurePoint;
	*fpt = *kin.fpt;
	gmt=new Garment;
	*gmt = *kin.gmt;
	mdl=new Model;
	*mdl = *kin.mdl;*/

}

CKinect& CKinect::operator = (const CKinect& kin){

	if (fpt != NULL) delete fpt;
	fpt = new featurePoint;
	*fpt = *kin.fpt;
	if (gmt != NULL) delete gmt;
	gmt = new Garment;
	*gmt = *kin.gmt;
	//if (mdl != NULL) delete mdl;
	//mdl = new Model;
	//*mdl = *kin.mdl;

	m_body = kin.m_body;
	contourRect = kin.contourRect;
	m_Depth = kin.m_Depth;
	m_Color; kin.m_Color;
	m_BodyIndex = kin.m_BodyIndex;

	/*if (!mapJoints.empty()) mapJoints.clear();
	mapJoints = kin.mapJoints;*/

	neck_x = kin.neck_x;
	neck_y = kin.neck_y;
	shoulderLeft = kin.shoulderLeft;
	shoulderRight = kin.shoulderRight;
	footRight = kin.footRight;
	footLeft = kin.footLeft;
	neck = kin.neck;
	ankleLeft = kin.ankleLeft;
	ankleRight = kin.ankleLeft;
	ankleLeft_x = kin.ankleLeft_x;
	ankleLeft_y = kin.ankleLeft_y;
	ankleRight_y = kin.ankleRight_y;
	ankleRight_x = kin.ankleRight_x;
	shoulderLeft_x = kin.shoulderLeft_x;
	shoulderRight_x = kin.shoulderRight_x;
	shoulderLeft_y = kin.shoulderLeft_y;
	shoulderRight_y = kin.shoulderRight_y;
	/*
	//m_pKinectSensor = new IKinectSensor;
	// create heap storage for composite image pixel data in RGBX format
	m_pOutputRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for background image pixel data in RGBX format
	m_pBackgroundRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for color pixel data in RGBX format
	m_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for the coorinate mapping from color to depth
	m_pDepthCoordinates = new DepthSpacePoint[cColorWidth * cColorHeight];

	m_pDepthRGBX = new RGBQUAD[cDepthWidth * cDepthHeight];// create heap storage for color pixel data in RGBX format

	*m_pOutputRGBX = *kin.m_pOutputRGBX;
	*m_pBackgroundRGBX = *kin.m_pBackgroundRGBX;
	*m_pColorRGBX = *kin.m_pColorRGBX;
	*m_pDepthCoordinates = *kin.m_pDepthCoordinates;
	*m_pDepthRGBX = *kin.m_pDepthRGBX;
	*/
	return *this;
}

CKinect::~CKinect()
{
	SAFE_RELEASE_VEC(m_pOutputRGBX);
	SAFE_RELEASE_VEC(m_pBackgroundRGBX);
	SAFE_RELEASE_VEC(m_pColorRGBX);
	SAFE_RELEASE_VEC(m_pDepthCoordinates);
	SAFE_RELEASE_VEC(m_pDepthRGBX);

	// done with frame reader
	SafeRelease(m_pMultiSourceFrameReader);

	// done with coordinate mapper
	SafeRelease(m_pCoordinateMapper);

	// close the Kinect Sensor
	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}

	SafeRelease(m_pKinectSensor);
	delete fpt;
	delete gmt;

	/*if (!mapJoints.empty())
		mapJoints.clear();*/
	//delete mdl;
}


HRESULT	CKinect::InitKinect(Model mod)
{
	HRESULT hr;

	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr))
	{
		cout << "wrong kinect sensor";
		return hr;
	}

	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the frame reader
		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		hr = m_pKinectSensor->Open();

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->OpenMultiSourceFrameReader(
				FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color | FrameSourceTypes::FrameSourceTypes_Body | FrameSourceTypes::FrameSourceTypes_BodyIndex,
				&m_pMultiSourceFrameReader);

			//			hr = m_pKinectSensor->OpenMultiSourceFrameReader(FrameSourceTypes::FrameSourceTypes_Color ,	&m_pMultiSourceFrameReader);
		}
	}

	if (!m_pKinectSensor || FAILED(hr))
	{
		return E_FAIL;
	}

	return hr;
}

void CKinect::Update()
{
	//cout << "kinect update" << endl;
	if (!m_pMultiSourceFrameReader)
	{
		return;
	}

	IMultiSourceFrame* pMultiSourceFrame = NULL;
	IDepthFrame* pDepthFrame = NULL;
	IColorFrame* pColorFrame = NULL;
	IBodyIndexFrame* pBodyIndexFrame = NULL;
	IBodyFrame *pBodyFrame = NULL;

	HRESULT hr = m_pMultiSourceFrameReader->AcquireLatestFrame(&pMultiSourceFrame);

	if (SUCCEEDED(hr))//深度信息
	{
		IDepthFrameReference* pDepthFrameReference = NULL;
		hr = pMultiSourceFrame->get_DepthFrameReference(&pDepthFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameReference->AcquireFrame(&pDepthFrame);
		}
		SafeRelease(pDepthFrameReference);
	}

	if (SUCCEEDED(hr))//彩色信息
	{
		IColorFrameReference* pColorFrameReference = NULL;
		hr = pMultiSourceFrame->get_ColorFrameReference(&pColorFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pColorFrameReference->AcquireFrame(&pColorFrame);
		}
		SafeRelease(pColorFrameReference);
	}

	if (SUCCEEDED(hr))//骨骼信息
	{
		IBodyFrameReference* pBodyFrameReference = NULL;
		hr = pMultiSourceFrame->get_BodyFrameReference(&pBodyFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pBodyFrameReference->AcquireFrame(&pBodyFrame);
		}
		SafeRelease(pBodyFrameReference);
	}

	if (SUCCEEDED(hr))//人体掩膜部分
	{
		IBodyIndexFrameReference* pBodyIndexFrameReference = NULL;
		hr = pMultiSourceFrame->get_BodyIndexFrameReference(&pBodyIndexFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrameReference->AcquireFrame(&pBodyIndexFrame);
		}
		SafeRelease(pBodyIndexFrameReference);
	}

	if (SUCCEEDED(hr))
	{
		INT64 nDepthTime = 0;
		IFrameDescription* pDepthFrameDescription = NULL;
		int nDepthWidth = 0;
		int nDepthHeight = 0;
		UINT nDepthBufferSize = 0;
		UINT16 *pDepthBuffer = NULL;
		USHORT nDepthMinReliableDistance = 0;
		USHORT nDepthMaxDistance = 0;

		IFrameDescription* pColorFrameDescription = NULL;
		int nColorWidth = 0;
		int nColorHeight = 0;
		ColorImageFormat imageFormat = ColorImageFormat_None;
		UINT nColorBufferSize = 0;
		RGBQUAD *pColorBuffer = NULL;

		IBody* ppBodies[BODY_COUNT] = { 0 };

		IFrameDescription* pBodyIndexFrameDescription = NULL;
		int nBodyIndexWidth = 0;
		int nBodyIndexHeight = 0;
		UINT nBodyIndexBufferSize = 0;
		BYTE *pBodyIndexBuffer = NULL;

		// get depth frame data
		hr = pDepthFrame->get_RelativeTime(&nDepthTime);

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->get_FrameDescription(&pDepthFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameDescription->get_Width(&nDepthWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameDescription->get_Height(&nDepthHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
		}

		if (SUCCEEDED(hr))
		{
			nDepthMaxDistance = USHRT_MAX;
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->AccessUnderlyingBuffer(&nDepthBufferSize, &pDepthBuffer);
		}

		m_Depth = Mat(nDepthHeight, nDepthWidth, CV_16UC1, pDepthBuffer).clone();///////////////

		// get color frame data
		if (SUCCEEDED(hr))
		{
			hr = pColorFrame->get_FrameDescription(&pColorFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrameDescription->get_Width(&nColorWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrameDescription->get_Height(&nColorHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
		}

		if (SUCCEEDED(hr))
		{
			if (imageFormat == ColorImageFormat_Bgra)
			{
				hr = pColorFrame->AccessRawUnderlyingBuffer(&nColorBufferSize, reinterpret_cast<BYTE**>(&pColorBuffer));
			}
			else if (m_pColorRGBX)
			{
				pColorBuffer = m_pColorRGBX;
				nColorBufferSize = cColorWidth * cColorHeight * sizeof(RGBQUAD);
				hr = pColorFrame->CopyConvertedFrameDataToArray(nColorBufferSize, reinterpret_cast<BYTE*>(pColorBuffer), ColorImageFormat_Bgra);
			}
			else
			{
				hr = E_FAIL;
			}
		}

		m_Color = Mat(nColorHeight, nColorWidth, CV_8UC4, pColorBuffer);///////////////

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}

		// get body index frame data
		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrame->get_FrameDescription(&pBodyIndexFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrameDescription->get_Width(&nBodyIndexWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrameDescription->get_Height(&nBodyIndexHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrame->AccessUnderlyingBuffer(&nBodyIndexBufferSize, &pBodyIndexBuffer);
		}
		m_BodyIndex = Mat(nBodyIndexHeight, nBodyIndexWidth, CV_8UC1, pBodyIndexBuffer);

		if (SUCCEEDED(hr))
		{
			ProcessFrame(nDepthTime, pDepthBuffer, nDepthWidth, nDepthHeight, nDepthMinReliableDistance, nDepthMaxDistance,
				pColorBuffer, nColorWidth, nColorHeight,
				BODY_COUNT, ppBodies,
				pBodyIndexBuffer, nBodyIndexWidth, nBodyIndexHeight);
		}

		SafeRelease(pDepthFrameDescription);
		SafeRelease(pColorFrameDescription);
		SafeRelease(pBodyIndexFrameDescription);

		for (int i = 0; i < _countof(ppBodies); ++i)
		{
			SafeRelease(ppBodies[i]);
		}
	}

	SafeRelease(pDepthFrame);
	SafeRelease(pColorFrame);
	SafeRelease(pBodyFrame);
	SafeRelease(pBodyIndexFrame);
	SafeRelease(pMultiSourceFrame);
}

void CKinect::ProcessFrame(INT64 nTime,
	const UINT16* pDepthBuffer, int nDepthWidth, int nDepthHeight, USHORT nMinDepth, USHORT nMaxDepth,
	const RGBQUAD* pColorBuffer, int nColorWidth, int nColorHeight,
	int nBodyCount, IBody** ppBodies,
	const BYTE* pBodyIndexBuffer, int nBodyIndexWidth, int nBodyIndexHeight)
{
	LARGE_INTEGER qpcNow = { 0 };
	WCHAR szStatusMessage[64];
	//map<DepthSpacePoint, int> mapDepthPoint;
	//map<DepthSpacePoint, int>::iterator mapit;
	// Make sure we've received valid data
	if (m_pCoordinateMapper && m_pDepthCoordinates && m_pOutputRGBX &&
		pDepthBuffer && (nDepthWidth == cDepthWidth) && (nDepthHeight == cDepthHeight) &&
		pColorBuffer && (nColorWidth == cColorWidth) && (nColorHeight == cColorHeight) &&
		pBodyIndexBuffer && (nBodyIndexWidth == cDepthWidth) && (nBodyIndexHeight == cDepthHeight) &&
		m_pDepthRGBX)
	{
		HRESULT hr = m_pCoordinateMapper->MapColorFrameToDepthSpace(nDepthWidth * nDepthHeight, (UINT16*)pDepthBuffer, nColorWidth * nColorHeight, m_pDepthCoordinates);
		if (FAILED(hr))
		{
			return;
		}

		// loop over output pixels
		for (int colorIndex = 0; colorIndex < (nColorWidth*nColorHeight); ++colorIndex)
		{
			// default setting source to copy from the background pixel
			const RGBQUAD* pSrc = m_pBackgroundRGBX + colorIndex;

			DepthSpacePoint p = m_pDepthCoordinates[colorIndex];

			// Values that are negative infinity means it is an invalid color to depth mapping so we
			// skip processing for this pixel
			if (p.X != -std::numeric_limits<float>::infinity() && p.Y != -std::numeric_limits<float>::infinity())
			{
				int depthX = static_cast<int>(p.X + 0.5f);
				int depthY = static_cast<int>(p.Y + 0.5f);

				if ((depthX >= 0 && depthX < nDepthWidth) && (depthY >= 0 && depthY < nDepthHeight))
				{
					BYTE player = pBodyIndexBuffer[depthX + (depthY * cDepthWidth)];
					// if we're tracking a player for the current pixel, draw from the color camera
					if (player != 0xff)
					{
						// set source for copy to the color pixel
						pSrc = m_pColorRGBX + colorIndex;
					}
				}
			}
			// write output
			m_pOutputRGBX[colorIndex] = *pSrc;
		}
	}//确保参数都准确



	//
	//imshow("color2", m_Color);

	//resize(m_Color, showImage, Size(cColorWidth / 2, cColorHeight / 2));
	resize(m_Color, showImage, Size(cColorWidth, cColorHeight));
	//imshow("Color", showImage);//
	//imshow("Depth", m_Depth);
	imshow("BodyIndex", m_BodyIndex);
	//Mat Icanny;
	Mat bodyIndex2 = m_BodyIndex.clone();
	Mat thpng;//二值图
	Mat m_canny;
	threshold(bodyIndex2, thpng, 170, 255, CV_THRESH_BINARY);
	//imshow("erzhitu", thpng);//二值图显示

	//提取轮廓
	//vector<vector<Point>>bodyContours;
	//findContours(thpng, bodyContours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	//drawContours(result, bodyContours, -1, Scalar(255, 0, 0), 5);
	Canny(thpng, m_canny, 150, 220);
	GaussianBlur(m_canny, m_canny, Size(9, 9), 0, 0);
	//imshow("lunkuo", m_canny);//canny图显示
	Mat m_fillHole;
	fillHole(m_canny, m_fillHole);
	//imshow("fillHole", m_fillHole);//空洞填充图
	Mat m_contours(m_canny.size(), CV_8U, Scalar(255));
	vector<vector<cv::Point>>bodyContours;
	findContours(m_fillHole, bodyContours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	getSizeContours(bodyContours);
	vector<Point>contours = getBodyContoursPoint(bodyContours);
	drawContours(m_contours, bodyContours, -1, CV_RGB(0, 0, 0), 2);
	cout << bodyContours.size() << ": ";
	//cout << showImage.size() << endl;

	for (int i = 0; i < bodyContours.size(); i++){
		cout << bodyContours[i].size() << " -- ";
	}
	cout << endl;
	//imshow("drawcontours", m_contours);//轮廓图显示
	//for (int i = 0; i < nColorHeight*nColorWidth; i++) cout << m_pDepthCoordinates[i].X << "&&" << m_pDepthCoordinates[i].Y<<"  ";
	//cout << endl;
	//寻找彩色图中人体的点
	/*for (int j = 0; j < contours.size(); ++j){
		DepthSpacePoint tPoint;
		tPoint.X = contours[j].x;
		tPoint.Y = contours[j].y;
		cout << tPoint.X << "%%" << tPoint.Y << endl;
		if (tPoint.X >= 0 && tPoint.X < nDepthWidth && tPoint.Y >= 0 && tPoint.Y < nDepthHeight)  //判断是否合法
		{
		int index = (int)tPoint.Y * nDepthWidth + (int)tPoint.X; //取得彩色图上那点对应在BodyIndex里的值(注意要强转)
		if (pBodyIndexBuffer[index] != 0xff)                   //如果判断出彩色图上某点是人体，就用它来替换背景图上对应的点
		{
		ColorSpacePoint colorPoint;
		HRESULT hr = m_pCoordinateMapper->MapDepthPointToColorSpace(tPoint, (UINT16)100, &colorPoint);
		if (SUCCEEDED(hr)){
		int colorX = static_cast<int>(colorPoint.X + 0.5f);
		int colorY = static_cast<int>(colorPoint.Y + 0.5f);
		cout << "colorclor" << colorX << ":" << colorY << endl;
		circle(showImage, Point(colorX, colorY), 8, CV_RGB(255, 0, 0), -1, 8, 0);
		}
		}
		}
		}*/
	//寻找彩色图中人体的点
	//寻找彩色图中人体的点
	cout << nColorWidth << "||" << nColorHeight << endl;
	Mat m_bg(nColorHeight, nColorWidth, CV_8UC1, Scalar(0));
	vector<Point>::iterator it;
	for (int i = 0; i < nColorHeight; ++i){
		for (int j = 0; j < nColorWidth; ++j){
			DepthSpacePoint tPoint = m_pDepthCoordinates[i * nColorWidth + j];
			//Point temp(tPoint.X, tPoint.Y);
			//it = find(contours.begin(),contours.end(),temp);
			//if (it!=contours.end())
			if (tPoint.X >= 0 && tPoint.X < nDepthWidth && tPoint.Y >= 0 && tPoint.Y < nDepthHeight)  //判断是否合法
			{
				int index = (int)tPoint.Y * nDepthWidth + (int)tPoint.X; //取得彩色图上那点对应在BodyIndex里的值(注意要强转)
				if (pBodyIndexBuffer[index] != 0xff)                   //如果判断出彩色图上某点是人体，就用它来替换背景图上对应的点
				{
					//cout << " bodyindex: " << pBodyIndexBuffer[index] << "\t";
					//circle(showImage, Point(j, i), 5, CV_RGB(255, 255, 0), -1, 8, 0);
					m_bg.at<uchar>(i, j) = 255;
					//circle(m_bg, Point(j, i), 1, CV_RGB(255, 255, 255), -1, 8, 0);
					//m_bg.at<Vec3b>(i, j)[0] = 255;
				}
			}
		}
	}
	cout << m_bg.size() << endl;
	//imshow("bg",m_bg);
	/*Mat userThpng;
	threshold(m_bg, userThpng, 200, 255, CV_THRESH_BINARY);
	Mat m_userCanny;
	Canny(thpng, m_userCanny, 150, 220);
	imshow("userCanny",m_userCanny);
	Mat m_userFillHole;
	fillHole(m_userCanny, m_userFillHole);
	imshow("userFillHole", m_userFillHole);*/
	vector<vector<Point>>userContoursPoints;
	userContoursPoints.clear();
	findContours(m_bg, userContoursPoints, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	Mat m_userContours(m_bg.size(), CV_8U, Scalar(255));
	drawContours(m_userContours, userContoursPoints, -1, CV_RGB(0, 0, 0), 2);
	imshow("userContours", m_userContours);
	vector<Point>userContour;
	userContour = getBodyContoursPoint(userContoursPoints);
	for (int i = 0; i < userContour.size(); i++){
		circle(showImage, Point(userContour[i].x, userContour[i].y), 5, CV_RGB(255, 255, 0), -1, 8, 0);
	}


	fpt->featurepointInit();
	IplImage *IPLshowImage;
	IPLshowImage = &IplImage(showImage);
	fpt->getCircle200(IPLshowImage, userContour);
	cout << "contoursPoint2.size()=" << fpt->contoursPoint2.size() << endl;
	vector<Point>tempp = fpt->contoursPoint2;
	cout << "tempp.size()=" << tempp.size() << endl;
	fpt->getSpecialPoint27(IPLshowImage, tempp);
	
	cout << "fpt->featurePt.size()==" << fpt->featurePt.size() << endl;
	//fpt->getBodyRegion(m_bg, IPLshowImage);
	//Mat m_bg(cColorHeight, cColorWidth, CV_8UC4,Scalar(0,0,0));
	//寻找彩色图中人体的点

	//ColorSpacePoint *colorPoint=NULL;
	//HRESULT hr = m_pCoordinateMapper->MapDepthFrameToColorSpace(nColorWidth*nColorHeight, (UINT16*)pDepthBuffer,nDepthWidth * nDepthHeight , colorPoint);
	//cout << "ddd" << SUCCEEDED(hr) << endl;
	//ColorSpacePoint *colorSpacePoint = NULL;
	//HRESULT hr = m_pCoordinateMapper->MapDepthPointsToColorSpace(nDepthWidth*nDepthHeight, m_pDepthCoordinates, cDepthWidth*cDepthHeight, (UINT16*)pDepthBuffer, nColorWidth*nColorHeight, colorSpacePoint);
	//if (SUCCEEDED(hr)){
	/*for (int i = 0; i < bodyContours.size(); i++){
		for (int j = 0; j < bodyContours[i].size(); j++){
		//circle(showImage,Point(i,j),5,CV_RGB(0,255,0),-1,8,0);
		circle(showImage, Point(bodyContours[i][j].x* showImage.cols*1.0 / cDepthWidth, bodyContours[i][j].y * showImage.rows*1.0 / cDepthHeight), 5, CV_RGB(0, 255, 0), -1, 8, 0);
		//cout << "contoursPoint"<<bodyContours[i][j];
		//ColorSpacePoint *colorSpacePoint = NULL;
		//DepthSpacePoint depthSpacePoint = { bodyContours[i][j].x, bodyContours[i][j].y };
		//cout << "深度图的点：" << bodyContours[i][j].x<< bodyContours[i][j].y<<endl;
		//UINT16 depth = pDepthBuffer[bodyContours[i][j].y * cDepthWidth + bodyContours[i][j].x];*/
	/*HRESULT hr = m_pCoordinateMapper->MapDepthPointsToColorSpace(nColorWidth*nColorHeight, m_pDepthCoordinates, cDepthWidth*cDepthHeight, (UINT16*)pDepthBuffer, nDepthWidth*nDepthHeight, colorSpacePoint);
	if (SUCCEEDED(hr)){
	int pos = bodyContours[i][j].y * nColorWidth + bodyContours[i][j].x;
	int x = colorSpacePoint[pos].X;
	int y = colorSpacePoint[pos].Y;
	//circle(showImage, Point(x, y), 3, CV_RGB(0, 255, 0), -1, 8, 0);
	cout << "lunkuodian:" << x << y << endl;
	circle(showImage, Point(x, y), 3, CV_RGB(0, 255, 0), -1, 8, 0);
	}
	int pos = bodyContours[i][j].y * cColorWidth + bodyContours[i][j].x;//像素点在一维序列中的位置。
	int x = colorPoint[pos].X;
	int y = colorPoint[pos].Y;
	circle(showImage, Point(x,y), 3, CV_RGB(0, 255, 0), -1, 8, 0);*/
	/*DepthSpacePoint depthSpacePoint;
	depthSpacePoint.X = bodyContours[i][j].x;
	depthSpacePoint.Y = bodyContours[i][j].y;
	ColorSpacePoint colorSpacePoint;
	UINT16 depth = findDepthBuffer(depthSpacePoint, nDepthWidth*nDepthHeight, pDepthBuffer);
	cout << "depth=" << depth << endl;
	HRESULT hr = m_pCoordinateMapper->MapDepthPointToColorSpace(depthSpacePoint, depth, &colorSpacePoint);
	if (SUCCEEDED(hr)){
	Point temp;
	temp.x = static_cast<int>(colorSpacePoint.X);
	temp.y = static_cast<int>(colorSpacePoint.X);
	circle(showImage, temp, 5, CV_RGB(255, 0, 0), -1, 8, 0);
	}
	}
	}*/
	//}
	//骨骼信息
	for (int count = 0; count < BODY_COUNT; count++){
		BOOLEAN bTracked = false;
		HRESULT hr = ppBodies[count]->get_IsTracked(&bTracked);
		if (SUCCEEDED(hr) && bTracked){
			Joint joint[JointType_Count];
			hr = ppBodies[count]->GetJoints(JointType_Count, joint);
			if (SUCCEEDED(hr)){
				HandState leftHandState = HandState_Unknown;
				hr = ppBodies[count]->get_HandLeftState(&leftHandState);
				if (SUCCEEDED(hr)){
					ColorSpacePoint colorSpacePoint = { 0 };
					hr = m_pCoordinateMapper->MapCameraPointToColorSpace(joint[JointType_HandLeft].Position, &colorSpacePoint);
					if (SUCCEEDED(hr)){
						int x = static_cast<int>(colorSpacePoint.X);
						int y = static_cast<int>(colorSpacePoint.Y);
						if ((x >= 0) && (x < cColorWidth) && (y >= 0) && (y < cColorHeight)){
							if (leftHandState == HandState::HandState_Open){
								cv::circle(showImage, cv::Point(x, y), 75, cv::Scalar(0, 128, 0), 5, CV_AA);
							}
							else if (leftHandState == HandState::HandState_Closed){
								cv::circle(showImage, cv::Point(x, y), 75, cv::Scalar(0, 0, 128), 5, CV_AA);
							}
							else if (leftHandState == HandState::HandState_Lasso){
								cv::circle(showImage, cv::Point(x, y), 75, cv::Scalar(128, 128, 0), 5, CV_AA);
							}
						}
					}
				}
				HandState rightHandState = HandState_Unknown;
				hr = ppBodies[count]->get_HandRightState(&rightHandState);
				if (SUCCEEDED(hr)){
					ColorSpacePoint colorSpacePoint = { 0 };
					hr = m_pCoordinateMapper->MapCameraPointToColorSpace(joint[JointType_HandRight].Position, &colorSpacePoint);
					if (SUCCEEDED(hr)){
						int x = static_cast<int>(colorSpacePoint.X);
						int y = static_cast<int>(colorSpacePoint.Y);;
						if ((x >= 0) && (x < cColorWidth) && (y >= 0) && (y < cColorHeight)){
							if (rightHandState == HandState::HandState_Open){
								cv::circle(showImage, cv::Point(x, y), 75, cv::Scalar(0, 128, 0), 5, CV_AA);
							}
							else if (rightHandState == HandState::HandState_Closed){
								cv::circle(showImage, cv::Point(x, y), 75, cv::Scalar(0, 0, 128), 5, CV_AA);
							}
							else if (rightHandState == HandState::HandState_Lasso){
								cv::circle(showImage, cv::Point(x, y), 75, cv::Scalar(128, 128, 0), 5, CV_AA);
							}
						}
					}
				}
				for (int type = 0; type < JointType_Count; type++){
					ColorSpacePoint colorSpacePoint = { 0 };
					hr = m_pCoordinateMapper->MapCameraPointToColorSpace(joint[type].Position, &colorSpacePoint);
					if (SUCCEEDED(hr)){
						int x = static_cast<int>(colorSpacePoint.X);
						int y = static_cast<int>(colorSpacePoint.Y);
						if ((x >= 0) && (x < cColorWidth) && (y >= 0) && (y < cColorHeight)){
							//mapJoints.insert(pair<int, cv::Point>(type, Point(x, y)));
							//if (type == JointType_Neck)
								//cout << "mapJoints_Neck:" << mapJoints[type].x << ",," << mapJoints[type].y << endl;
							//cv::circle(showImage, cv::Point(x, y), 5, Scalar(255, 255, 0), -1, CV_AA);

							if (type == JointType_Neck){
								//cv::circle(showImage, cv::Point(x, y), 8, Scalar(0, 255, 255), -1, CV_AA);
								neck = Point(x, y);
								neck_x = x;
								neck_y = y;
								cout << "jingdian -x-y" << x << "," << y << endl;
							}
							else if (type == JointType_FootLeft){
								footLeft = Point(x, y);
							}
							else if (type == JointType_FootRight){
								footRight = Point(x, y);
							}
							else if (type == JointType_ShoulderLeft){
								shoulderLeft_x = x;
								shoulderLeft_y = y;
								shoulderLeft = Point(x, y);
							}
							else if (type == JointType_ShoulderRight){
								shoulderRight_x = x;
								shoulderRight_y = y;
								shoulderRight = Point(x, y);
							}
							else if (type == JointType_AnkleLeft){
								//cv::circle(showImage, cv::Point(x, y), 8, Scalar(0, 255, 255), -1, CV_AA);
								cout << "find ankleLeft" << endl; 
								ankleLeft = Point(x, y);
								ankleLeft_x = x;
								ankleLeft_y = y;
							}
							else if (type == JointType_AnkleRight){
								//cv::circle(showImage, cv::Point(x, y), 8, Scalar(0, 255, 128), -1, CV_AA);
								cout << "find ankleRight" << endl;
								ankleRight = Point(x, y);
								ankleRight_x = x;
								ankleRight_y = y;
							}
						}
					}
				}
			}
		}
		//resize(m_Color, showImage, cv::Size(),0.5,0.5);
	}

	cout << endl;
	//imshow("Color", showImage);
	if (userContour.size() && fpt->featurePt.size()!=0){
		contourRect = boundingRect(userContour);
		/*vector<Point>testP;
		testP.clear();
		testP.push_back(Point(contourRect.x, contourRect.y));
		testP.push_back(Point(contourRect.x+contourRect.height, contourRect.y));
		testP.push_back(Point(contourRect.x, contourRect.y+contourRect.width));
		testP.push_back(Point(contourRect.x + contourRect.height, contourRect.y + contourRect.width));*/
		rect_user = contourRect;
		rectangle(showImage, contourRect, Scalar(255, 128, 64));
		cout << "contourRect.size=" << contourRect.size() << endl;
		m_body = showImage(contourRect);
		
		//int ress2 = CGALTri::getmain(m_body, fpt->contoursPoint2, "userTriImage");
		
		//waitKey(0);
		transComputing(showImage);
		Mat m_body2;
		//resize(m_body, m_body2, cv::Size(), 0.5, 0.5);
		resize(m_body, m_body2, cv::Size(), 1, 1);
		//imshow("bodybody", m_body2);
		/*IplImage *bodyImage, *cpBodyImage;
		bodyImage = &IplImage(showImage);
		cpBodyImage = cvCreateImage(contourRect.size(), 8, 3);
		cvSetImageROI(bodyImage, contourRect);
		cvCopy(bodyImage, cpBodyImage);
		Mat m_body2(cpBodyImage);
		m_body = m_body2.clone();
		imshow("bodybody", m_body);
		cvResetImageROI(bodyImage);
		free(bodyImage);
		bodyImage = NULL;
		free(cpBodyImage);
		cpBodyImage = NULL;*/
		//m_user = m_body;
		//Mat ss(contourRect.size(), showImage.type());

		//delaunayTest(m_body2, contourRect, fpt->featurePt);
	}
	
	Mat showImageResize;
	resize(showImage, showImageResize, cv::Size(), 1, 1);
	imshow("Color", showImageResize);
	RGBQUAD* pRGBX = m_pDepthRGBX;
	// end pixel is start + width*height - 1
	const UINT16* pBufferEnd = pDepthBuffer + (nDepthWidth * nDepthHeight);
	while (pDepthBuffer < pBufferEnd)
	{
		USHORT depth = *pDepthBuffer;
		BYTE intensity = static_cast<BYTE>((depth >= nMinDepth) && (depth <= nMaxDepth) ? (depth % 256) : 0);
		pRGBX->rgbRed = intensity;
		pRGBX->rgbGreen = intensity;
		pRGBX->rgbBlue = intensity;
		++pRGBX;
		++pDepthBuffer;
	}

	// Draw the data nDepthHeight OpenCV
	Mat DepthImage(nDepthHeight, nDepthWidth, CV_8UC4, m_pDepthRGBX);
	Mat show = DepthImage.clone();
	imshow("DepthImage", show);
	/*
	Mat ColorImage2(cColorWidth, cColorHeight, CV_8UC4, m_pOutputRGBX);
	Mat show2 = ColorImage2.clone();
	imshow("colorbodyImage",show2);*/

	waitKey(1);
}

void CKinect::fillHole(const Mat srcBw, Mat &dstBw)//孔洞填充
{
	Size m_Size = srcBw.size();
	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//延展图像
	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	cv::floodFill(Temp, Point(0, 0), Scalar(255));

	Mat cutImg;//裁剪延展的图像
	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

	dstBw = srcBw | (~cutImg);

}

void CKinect::getSizeContours(vector<vector<Point>> &contours)//去除小轮廓和极大轮廓   <500  >5000
{
	int cmin = 500;   // 最小轮廓长度  
	int cmax = 5000;   // 最大轮廓长度  
	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if ((itc->size()) < cmin || (itc->size()) > cmax)
		{
			itc = contours.erase(itc);
		}
		else ++itc;
	}
}

UINT16 CKinect::findDepthBuffer(DepthSpacePoint DSP, int DSP_SIZE, const UINT16 *pDepthBuffer){
	for (int index = 0; index < DSP_SIZE; ++index)
	{
		DepthSpacePoint p = m_pDepthCoordinates[index];
		if (DSP.X == p.X && DSP.Y == p.Y){
			return pDepthBuffer[index];
		}
	}
	return UINT16(0);
}

vector<Point>  CKinect::getBodyContoursPoint(vector<vector<Point>> &contours)
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

void CKinect::deformation(){
	//cout << mdl->fpt->featurePt.size() << "|model|" << mdl->fpt->auxiliaryPoints.size() << endl;
	cout << fpt->featurePt.size() << "|kinect|" << fpt->auxiliaryPoints.size() << endl;
	//imshow("defor_color", m_Color);
}

void CKinect::transferInit(Model mod, Garment gar){
	this->mod = mod;
	this->gar = gar;
}
void CKinect::transComputing(Mat& m_user){

	getmModel();
	imshow("m_model", mod.modelSrc);
	getrectModel();
	getmGarment();
	cout << "|rect_user_x,y|=>" << rect_user.x << "," << rect_user.y << endl;
	Mat m_model2(m_model.size(), m_model.type());
	if (!m_user.empty()){
		resize(m_model, m_model2, cv::Size(), rect_user.width*1.0 / m_model.cols, rect_user.height*1.0 / m_model.rows);
	}
	cout << "|m_model2_size|=>" << m_model2.size() << endl;
	imshow("m_model2", m_model2);
	mod.getFpt();
	Mat imgL = mod.modelSrc.clone();
	int ress = CGALTri::getmain(imgL, mod.fpt->featurePt,"ModTriImage");

	
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
		imshow("newm_garment", newm_garment);
		Mat newGarmentMask;
		threshold(newm_garment, newGarmentMask, 200, 255, CV_THRESH_BINARY);

		//int garmentWidth = getGarmentWidth(newGarmentMask);
		int garmentWidth = garmentRect.width;
		cout << "garmentWidth height" << garmentWidth << "," << garmentHight << endl;
		Mat newm_garment2;
		int addwidth = 0;
		resize(newm_garment, newm_garment2, cv::Size(), (addwidth + userWidth*1.0) / (garmentWidth*1.0), (userHeight*1.0) / (garmentHight*1.0));
		imshow("newg2", newm_garment2);
		cout << "newgarment.size()" << newm_garment2.size() << endl;
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
		//Mat user2;
		/*user2.create(m_user.size(), m_user.type());
		//m_user.copyTo(user2);
		user2 = m_user.clone();*/

		cout << "mask_newm_garment.size()" << mask_newm_garment.size() << endl;
		cout << "outside function" << m_user.size() << newm_garment2.size() << mask_newm_garment.size() << endl;

		//combineGarmentAndBody(m_user, newm_garment2, user_neck, newGarmentNeckPoint1, mask_newm_garment);
		combineGarmentAndBody(m_user, newm_garment2, user_neck, newGarmentNeckPoint1, mask_newm_garment);

		cout << "after function" << m_user.size() << newm_garment2.size() << mask_newm_garment.size() << endl;
		//imshow("user garment", m_user);
		//imshow("mask_newm_garment", mask_newm_garment);

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
void CKinect::getModel(Model model){
	mod = model;
}
void CKinect::getGarment(Garment garment){
	gar = garment;
}

void CKinect::getmModel(){
	mod.modelInitial();
	m_model = mod.modelSrc;
	imshow("modsrc", mod.modelSrc);
	cout << "|model_m_body_size|=>" << m_model.size() << endl;
}

void CKinect::getrectModel(){
	rect_model = mod.modelSrcRect;
	cout << "|rect_model_size|=>" << rect_model.size() << endl;
}
void CKinect::getmGarment(){
	m_garment = gar.m_garment;
	cout << "|m_garment_size|=>" << m_garment.size() << endl;
}
void CKinect::connectGarUser(Mat m_gar, Mat m_user){

}
Point CKinect::getPointUser_x(){
	Point user_x(0, 0);
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
	int cnt = 1;
	if (neck_x != 0 || neck_y != 0){
		user_x = Point(neck_x, neck_y);
	}
	else if (shoulderLeft.x != 0 && shoulderLeft.y != 0 && shoulderRight.x != 0 && shoulderRight.x != 0 && shoulderRight.y != 0){
		user_x += Point(shoulderLeft.x, shoulderLeft.y);
		user_x += Point(shoulderRight.x , shoulderRight.y);
		cnt = 3;
	}
	return Point(user_x.x / cnt, user_x.y / cnt);
}
Point CKinect::getPointModel_y(){
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
Point CKinect::getNeckPFromMat(Mat mask){
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
Mat  CKinect::combineMat(Mat x, Mat y, Mat mask){
	Mat dst = x.clone();
	int rownum = x.rows<y.rows ? x.rows : y.rows;
	int colnum = x.cols<y.cols ? x.cols : y.cols;
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
Mat  CKinect::positionCorrect(Point user_x, Point model_y, Mat src){
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

Rect CKinect::getGarmentRect(Mat garment){
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
int	CKinect::getUserWidth(){
	cout << "shoulderLeft Point" << shoulderLeft_x << "," << shoulderLeft_y << "|" << shoulderLeft.x << "," << shoulderLeft.y << endl;
	cout << "shoulderRight Point" << shoulderRight_x << "," << shoulderRight_y << "|" << shoulderRight.x << "," << shoulderRight.y << endl;
	if (shoulderRight.x != 0 && shoulderLeft.x != 0)
		return abs(shoulderRight.x - shoulderLeft.x)>0 ? abs(shoulderRight.x - shoulderLeft.x) : contourRect.width;
	else
		return contourRect.width;
}
int	CKinect::getUserHeight(){
	cout << "ankleRight Point" << ankleRight_x << "," << ankleRight_y << "|" << ankleRight.x << "," << ankleRight.y << endl;
	cout << "ankleLeft Point" << ankleLeft_x << "," << ankleLeft_y << "|" << ankleLeft.x << "," << ankleLeft.y << endl;
	if (ankleLeft.y != 0 && ankleRight.y != 0 && neck_y != 0 && neck_y - ankleLeft.y != 0 && neck_y - ankleLeft.y != 0){
		return abs(neck_y - ankleLeft.y) > abs(neck_y - ankleRight.y) ?
			abs(neck_y - ankleLeft.y) : abs(neck_y - ankleRight.y);
	}
	else{
		return contourRect.height;
	}
}
int	CKinect::getGarmentWidth(Mat mask){
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
	for (int j = colnum - 1; j >= 0; j--){
		if (!(mask.at<Vec3b>(rownum / 2, j)[0] && mask.at<Vec3b>(rownum / 2, j)[1] && mask.at<Vec3b>(rownum / 2, j)[2])){
			right = j;
			break;
		}
	}
	return right - left;
}
int CKinect::getGarmentHeight(Mat mask){
	return 0;
}
void CKinect::combineGarmentAndBody(Mat& user, Mat& garment, Point bodyNeck, Point garmentNeck, Mat mask_garment){
	cout << user.type() <<"type"<< garment.type()<<"type" << mask_garment.type() << endl;
	cvtColor(garment, garment, CV_RGB2BGRA);
	cout << user.type() << "type" << garment.type() << "type" << mask_garment.type() << endl;
	int garrow = garment.rows;
	int garcol = garment.cols;

	int userrow = user.rows;
	int usercol = user.cols;
	Point temp = Point((bodyNeck.x - garmentNeck.x), bodyNeck.y - garmentNeck.y);
	//Point temp(400,400);
	int move = 0;
	temp.x += move;
	//temp.y -= move;
	cout << "diff of neck garment neck" << temp.x << "," << temp.y << endl;
	cout << "function_user.size()" << user.size() << endl;
	cout << "functon _garment.size()" << garment.size() << endl;
	cout << "functon _mask_garment size()" << mask_garment.size() << endl;
	Vec3b t = Vec3b(255, 255, 255);
	int minleft = usercol;
	int maxright = 0;
	int minbottom = 0;
	int maxtop = userrow;
	int cnt = 0;
	for (int i = 0; i < garcol; i++){
		for (int j = 0; j < garrow; j++){
			//if (temp.x + j>=0 && temp.y + i>=0 && temp.x + j<=usercol && temp.y + i<=userrow
			//	&& !(mask_garment.at<Vec3b>(i, j)[0] || mask_garment.at<Vec3b>(i, j)[1] || mask_garment.at<Vec3b>(i, j)[2])){
			if (temp.x + i >= 0 && temp.y + j >= 0 && temp.x + i < usercol && temp.y + j < userrow){
				if (mask_garment.at<Vec3b>(j, i) != t){
					//user.at<Vec3b>(j + temp.y, i + temp.x)= garment.at<Vec3b>(j, i);
					Vec4b t = garment.at<Vec4b>(j, i);
					user.at<Vec4b>(j + temp.y, i + temp.x) = t;

					/*maxright = j + temp.x >= maxright ? j + temp.x : maxright;
					minleft = j + temp.x <= minleft ? j + temp.x : minleft;
					minbottom = i + temp.y > minbottom ? i + temp.y : minbottom;
					maxtop = i + temp.y < maxtop ? i + temp.y : maxtop;*/

					cnt++;
				}

			}
		}
	}
	cout << "cnt: " << cnt << endl;
	cout << "minleft:" << minleft << "|maxright:" << maxright << "|dis=" << maxright - minleft << endl;
	cout << "minbottom:" << minbottom << "|maxtop:" << maxright << "|dis=" << minbottom - maxtop << endl;
}

//delaunay
void CKinect::delaunayTest(Mat src,Rect rect,vector<Point>point){
	cout << "doing triangulation..." << endl;
	Mat imgL = src.clone();
	vector<Vec3i> tri;

	vector<Point2f> vec_points;
	int lens = point.size();
	for (int i = 0; i < lens; i++)
	{
		Point p = point[i];
		Point2f fp = cvPoint2D32f((float)(p.x-rect.x), (float)(p.y-rect.y));//使点约束在距离边框10像素之内。    
		vec_points.push_back(fp);
	}

	TriSubDiv(vec_points, imgL, tri);
}
/*
pts，要剖分的散点集,in
img,剖分的画布,in
tri,存储三个表示顶点变换的正数,out
*/
void CKinect::TriSubDiv(vector<Point2f> &pts, Mat &img, vector<Vec3i> &tri){
	CvSubdiv2D* subdiv;
	CvMemStorage* storage = cvCreateMemStorage(0); //创建存储器  
	Rect rc = Rect(0, 0, img.cols, img.rows);//矩形是图像的大小  

	subdiv = cvCreateSubdiv2D(CV_SEQ_KIND_SUBDIV2D, sizeof(*subdiv),
		sizeof(CvSubdiv2DPoint),
		sizeof(CvQuadEdge2D),
		storage);//为剖分数据分配空间  
	cvInitSubdivDelaunay2D(subdiv, rc);
	for (size_t i = 0; i < pts.size(); i++)
	{
		if (pts[i].x < img.cols &&pts[i].x>0 && pts[i].y < img.rows&&pts[i].y>0){
			CvSubdiv2DPoint *pt = cvSubdivDelaunay2DInsert(subdiv, pts[i]);//利用插入法进行剖分  
			pt->id = i;//为每一个顶点分配一个id  
		}

	}

	CvSeqReader reader;//利用CvSeqReader遍历  
	int total = subdiv->edges->total;//边的总数  
	int elem_size = subdiv->edges->elem_size;//边的大小  

	cvStartReadSeq((CvSeq*)(subdiv->edges), &reader, 0);
	Point buf[3];
	const Point *pBuf = buf;
	Vec3i verticesIdx;
	Mat imgShow = img.clone();

	srand((unsigned)time(NULL));
	for (int i = 0; i < total; i++)
	{
		CvQuadEdge2D* edge = (CvQuadEdge2D*)(reader.ptr);

		if (CV_IS_SET_ELEM(edge))
		{
			CvSubdiv2DEdge t = (CvSubdiv2DEdge)edge;
			int iPointNum = 3;
			//Scalar color = CV_RGB(rand() & 255, rand() & 255, rand() & 255);
			Scalar color = CV_RGB(255, 0, 0);
			//bool isNeg = false;  
			int j;
			for (j = 0; j < iPointNum; j++)
			{
				CvSubdiv2DPoint* pt = cvSubdiv2DEdgeOrg(t);//获取t边的源点  
				if (!pt) break;
				buf[j] = pt->pt;//将点存储起来  
				//if (pt->id == -1) isNeg = true;  
				verticesIdx[j] = pt->id;//获取顶点的Id号，将三个点的id存储到verticesIdx中  
				t = cvSubdiv2DGetEdge(t, CV_NEXT_AROUND_LEFT);//获取下一条边  
			}
			if (j != iPointNum) continue;
			if (isGoodTri(verticesIdx, tri))
			{
				//tri.push_back(verticesIdx);  
				polylines(imgShow, &pBuf, &iPointNum,
					1, true, color,
					1, CV_AA, 0);//画出三条边  
				//printf("(%d, %d)-(%d, %d)-(%d, %d)\n", buf[0].x, buf[0].y, buf[1].x, buf[1].y, buf[2].x, buf[2].y);  
				//printf("%d\t%d\t%d\n", verticesIdx[0], verticesIdx[1], verticesIdx[2]);  
				//imshow("Delaunay", imgShow);  
				//waitKey();  
			}

			t = (CvSubdiv2DEdge)edge + 2;//相反边缘 reversed e  

			for (j = 0; j < iPointNum; j++)
			{
				CvSubdiv2DPoint* pt = cvSubdiv2DEdgeOrg(t);
				if (!pt) break;
				buf[j] = pt->pt;
				verticesIdx[j] = pt->id;
				t = cvSubdiv2DGetEdge(t, CV_NEXT_AROUND_LEFT);
			}
			if (j != iPointNum) continue;
			if (isGoodTri(verticesIdx, tri))
			{
				//tri.push_back(verticesIdx);  
				polylines(imgShow, &pBuf, &iPointNum,
					1, true, color,
					1, CV_AA, 0);
				//printf("(%d, %d)-(%d, %d)-(%d, %d)\n", buf[0].x, buf[0].y, buf[1].x, buf[1].y, buf[2].x, buf[2].y);  
				//printf("%d\t%d\t%d\n", verticesIdx[0], verticesIdx[1], verticesIdx[2]);  
				//imshow("Delaunay", imgShow);  
				//waitKey();  
			}

		}

		CV_NEXT_SEQ_ELEM(elem_size, reader);

	}

	//RemoveDuplicate(tri);  
	char title[100];
	sprintf_s(title, 100, "Delaunay: %d Triangles", tri.size());//tri存储的为3个顶点为一个vec3i，故tri.size()表示三角形的个数。
	cout << "tri.size()=" << tri.size() << endl;
	imshow("delaunay", imgShow);
	//waitKey();
}
// used for doing delaunay trianglation with opencv function  
//该函数用来防止多次重画并消去虚拟三角形的 顶点  
bool CKinect::isGoodTri(Vec3i &v, vector<Vec3i> & tri){
	int a = v[0], b = v[1], c = v[2];
	v[0] = min(a, min(b, c));//v[0]找到点插入的先后顺序（0....N-1，N为点的个数）的最小值  
	v[2] = max(a, max(b, c));//v[2]存储最大值.  
	v[1] = a + b + c - v[0] - v[2];//v[1]为中间值  
	if (v[0] == -1) return false;

	vector<Vec3i>::iterator iter = tri.begin();//开始时为空  
	for (; iter != tri.end(); iter++)
	{
		Vec3i &check = *iter;//如果当前待压入的和存储的重复了，则停止返回false。  
		if (check[0] == v[0] &&
			check[1] == v[1] &&
			check[2] == v[2])
		{
			break;
		}
	}
	if (iter == tri.end())
	{
		tri.push_back(v);
		return true;
	}
	return false;
}