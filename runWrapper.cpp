#include "runWrapper.h"

int runWrapper() 
{
	using namespace cv;

	int runTest = 1;
	if(runTest == 1)
	{
		// input params
		char fileName[100] = "Standard_SCU3IU_2014-08-18_0700.002.avi";
		int bufferSize = 30;
		int minObjectSize = 5000;
		int skip = 3;
		int learningTime = 240;
		int nmixtures = 5;
		double backgroundratio = .6;
		bool detectShadows = false;
		bool online = true;

		cv::Point Sp1(150,100);
		cv::Point Sp4(250,100);
		cv::Point Sp2(150,400);
		cv::Point Sp3(250,400);
		
		cv::vector<cv::Point> startRegion;
		startRegion.push_back(Sp1);
		startRegion.push_back(Sp2);
		startRegion.push_back(Sp3);
		startRegion.push_back(Sp4);

		cv::Point Ep1(350,80);
		cv::Point Ep4(450,80);
		cv::Point Ep2(350,180);
		cv::Point Ep3(450,180);

		vector<Point> endRegion;
		endRegion.push_back(Ep1);
		endRegion.push_back(Ep2);
		endRegion.push_back(Ep3);
		endRegion.push_back(Ep4);
		

		// overlays to focus on area of video where the counting should be done. (can be taken as params)
		Point overlay1[1][4];
		overlay1[0][0] = Point( 0, 0 );
		overlay1[0][1] = Point( 0, 220 );
		overlay1[0][2] = Point( 580, 0 );
		overlay1[0][3] = Point( 580, 0 );

		const Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		/** Create some points */
		Point overlay2[1][4];
		overlay2[0][0] = Point( 0, 320 );
		overlay2[0][1] = Point( 0, 480 );
		overlay2[0][2] = Point( 720, 480);
		overlay2[0][3] = Point( 720, 0 );

		const Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(2);
		CC.run(bufferSize,minObjectSize,skip,learningTime,fileName,online,*ppt,*ppt2,npt,npt2,nmixtures,backgroundratio,detectShadows,
			true,true,true,true,true,true,-1,1, startRegion, endRegion);
		return 0;
	}
	
	else{
		// input params
		char fileName[100] = "Standard_SCU3JD_2014-10-21_0845.005.avi";
		int bufferSize = 30;
		int minObjectSize = 2000;
		int skip = 3;
		int learningTime = 240;
		int nmixtures = 5;
		double backgroundratio = .6;
		bool detectShadows = false;
		bool online = true;

		cv::Point Sp1(250,30);
		cv::Point Sp4(350,30);
		cv::Point Sp2(250,150);
		cv::Point Sp3(350,150);
		
		cv::vector<cv::Point> startRegion;
		startRegion.push_back(Sp1);
		startRegion.push_back(Sp2);
		startRegion.push_back(Sp3);
		startRegion.push_back(Sp4);

		cv::Point Ep1(300,325);
		cv::Point Ep4(600,325);
		cv::Point Ep2(300,375);
		cv::Point Ep3(600,375);

		vector<Point> endRegion;
		endRegion.push_back(Ep1);
		endRegion.push_back(Ep2);
		endRegion.push_back(Ep3);
		endRegion.push_back(Ep4);

		// overlays to focus on area of video where the counting should be done. (can be taken as params)
		Point overlay1[1][4];
		overlay1[0][0] = Point( 0, 0 );
		overlay1[0][1] = Point( 0, 480 );
		overlay1[0][2] = Point( 320, 480);
		overlay1[0][3] = Point( 240, 0 );
		

		const Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		/** Create some points */
		Point overlay2[1][4];
		overlay2[0][0] = Point( 230, 0 );
		overlay2[0][1] = Point( 720, 450 );
		overlay2[0][2] = Point( 720, 450);
		overlay2[0][3] = Point( 720, 0 );
		

		const Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(2);
		CC.run(bufferSize,minObjectSize,skip,learningTime,fileName,online,*ppt,*ppt2,npt,npt2,nmixtures,backgroundratio,detectShadows,
			true,true,true,true,true,true,-1,1, startRegion, endRegion);
		return 0;
	}
}