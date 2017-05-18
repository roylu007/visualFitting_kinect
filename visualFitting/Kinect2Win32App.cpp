#include "stdafx.h"
#include "Tools.h"
#include <windows.h>
#include <Mmsystem.h>//需要 Winmm.lib库的支持 ----timeGetTime()
//#include "Kinect.h"//主函数
#include <cstdlib>
#include "Transfer.h"

int main()
{
	printf_s("start analysis Kinect for Windows V2 body data....");
	Garment gar;
	gar.garmentInitial();
	Model mod;
	mod.modelInitial();
	CKinect kinect;
	kinect.InitKinect(mod);
	kinect.getGarment(gar);
	kinect.getModel(mod);
	//Transfer tra;
	//tra.getModel(mod);
	//cout << "mod.contourRect.size="<<mod.contourRect.size() << endl;
	//system("pause");
	while(1)
	{
		kinect.Update();
		//tra.getKinect(kinect);
		//tra.getGarment(gar);
		//tra.transComputing();
	}

	return 0;
}


