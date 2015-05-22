// This demo will show you how the Vista-core-API can be used. It was test code I left for demonstration purposes
// so it hasn't been properely managed in any way. You'll see a lot of unecessary redundancy. 
// The absolutely addresses to videos on my local machines directory should be replaced with your own. 
// If you are using your own videos then make sure the input params to run are
// properly choosen. Details on what the input params should be are available in the carCounter.h file.

#include "main.h"

// Wrapper methods for creating instances of car counter and calling the run function on them.
// These can be threaded to make multiple instances of carCounters run at the same time.
// Ideally you should make a single wrapper capable of handing multiple params instead of creating 
// multiple ones and defining the inputs to run() locally.

void runACounter1(int id){
	char fileName[100] = "Standard_SCU3IU_2014-08-18_0700.002.avi";
	char saveImgTo[200] = "C:/Users/Vanya/Desktop/Car/";
	char dataBase[200] = "C:/Users/Vanya/Desktop/car_count_db";
	int bufferSize = 2;
	int minObjectSizeDay = 5000;
	int minObjectSizeNight = 10;
	int skip = 3;
	int learningTime = 240;
	int fps = 30;
	int expectedDist = 20;
	int horizontalBandwidth = 50;
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

	cv::Point Ep1(300,80);
	cv::Point Ep4(450,80);
	cv::Point Ep2(300,180);
	cv::Point Ep3(450,180);

	cv::vector<cv::Point> endRegion;
	endRegion.push_back(Ep1);
	endRegion.push_back(Ep2);
	endRegion.push_back(Ep3);
	endRegion.push_back(Ep4);
		
	cv::Point overlay1[1][4];
	overlay1[0][0] = cv::Point( 0, 0 );
	overlay1[0][1] = cv::Point( 0, 180 );
	overlay1[0][2] = cv::Point( 580, 0 );
	overlay1[0][3] = cv::Point( 580, 0 );

	const cv::Point* ppt[1] = { overlay1[0] };
	int npt[] = { 4 };

	cv::Point overlay2[1][4];
	overlay2[0][0] = cv::Point( 0, 320 );
	overlay2[0][1] = cv::Point( 0, 480 );
	overlay2[0][2] = cv::Point( 720, 480);
	overlay2[0][3] = cv::Point( 720, 0 );

	const cv::Point* ppt2[1] = { overlay2[0] };
	int npt2[] = { 4 };

	carCounter CC(id,false);
	CC.run(bufferSize,minObjectSizeDay,minObjectSizeNight,skip,learningTime,fileName,saveImgTo,dataBase,fps,expectedDist,horizontalBandwidth,RUN_ONLINE,
		*ppt,*ppt2,npt,npt2,
		USE_MOG2_ON,nmixtures,backgroundratio,
		DETECT_SHADOWS_ON,SHOW_LAST_CAR_ON,SHOW_BOUNDBOX_ON,SHOW_PREDICTION_ON,SHOW_LATEST_PATHS_ON,SHOW_TRANSIT_LEDGER_ON,SHOW_REGION_MONITORS_ON,
		SHOW_PATH_OF_CAR_X_OFF,USE_RESIZED_WITH_OVERLAYS_FRAME, startRegion, endRegion);
}

void runACounter2(int id){
		char fileName[100] = "Standard_SCU3IP_2014-10-21_0830.023.mp4";
		char saveImgTo[200] = "C:/Users/Vanya/Desktop/Car/";
		char dataBase[200] = "C:/Users/Vanya/Desktop/car_count_db";
		int bufferSize = 30;
		int minObjectSizeDay = 5000;
		int minObjectSizeNight = 10;
		int skip = 3;
		int learningTime = 240;
		int fps = 30;
		int expectedDist = 20;
		int horizontalBandwidth = 100;
		int nmixtures = 5;
		double backgroundratio = .6;
		bool detectShadows = false;
		bool online = true;

		cv::Point Sp1(200,250);
		cv::Point Sp4(600,250);
		cv::Point Sp2(200,300);
		cv::Point Sp3(600,300);
		
		cv::vector<cv::Point> startRegion;
		startRegion.push_back(Sp1);
		startRegion.push_back(Sp2);
		startRegion.push_back(Sp3);
		startRegion.push_back(Sp4);

		cv::Point Ep1(300,375);
		cv::Point Ep4(700,375);
		cv::Point Ep2(300,425);
		cv::Point Ep3(700,425);

		cv::vector<cv::Point> endRegion;
		endRegion.push_back(Ep1);
		endRegion.push_back(Ep2);
		endRegion.push_back(Ep3);
		endRegion.push_back(Ep4);

		cv::Point overlay1[1][4];
		overlay1[0][0] = cv::Point( 0, 0 );
		overlay1[0][1] = cv::Point( 0, 480 );
		overlay1[0][2] = cv::Point( 320, 480);
		overlay1[0][3] = cv::Point( 30, 0 );

		const cv::Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		cv::Point overlay2[1][4];
		overlay2[0][0] = cv::Point( 40, 0 );
		overlay2[0][1] = cv::Point( 720, 380 );
		overlay2[0][2] = cv::Point( 720, 380 );
		overlay2[0][3] = cv::Point( 720, 0 );
		

		const cv::Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(7,true);
		CC.run(bufferSize,minObjectSizeDay,minObjectSizeNight,skip,learningTime,fileName,saveImgTo,dataBase,fps,expectedDist,horizontalBandwidth,RUN_ONLINE,
			*ppt,*ppt2,npt,npt2,
			USE_MOG2_ON,nmixtures,backgroundratio,
			DETECT_SHADOWS_ON,SHOW_LAST_CAR_ON,SHOW_BOUNDBOX_ON,SHOW_PREDICTION_ON,SHOW_LATEST_PATHS_ON,SHOW_TRANSIT_LEDGER_ON,SHOW_REGION_MONITORS_ON,
			SHOW_PATH_OF_CAR_X_OFF,USE_RESIZED_WITH_OVERLAYS_FRAME, startRegion, endRegion);
}

void runACounter3(int id){
		char fileName[100] = "Standard_SCU3IP_2014-10-21_0830.010.mp4";
		char saveImgTo[200] = "C:/Users/Vanya/Desktop/Car/";
		char dataBase[200] = "C:/Users/Vanya/Desktop/car_count_db";
		int bufferSize = 30;
		int minObjectSizeDay = 5000;
		int minObjectSizeNight = 10;
		int skip = 3;
		int learningTime = 240;
		int fps = 30;
		int expectedDist = 100;
		int horizontalBandwidth = 50;
		int nmixtures = 5;
		double backgroundratio = .6;
		bool detectShadows = false;
		bool online = true;

		cv::Point Sp1(200,250);
		cv::Point Sp4(600,250);
		cv::Point Sp2(200,300);
		cv::Point Sp3(600,300);
		
		cv::vector<cv::Point> startRegion;
		startRegion.push_back(Sp1);
		startRegion.push_back(Sp2);
		startRegion.push_back(Sp3);
		startRegion.push_back(Sp4);

		cv::Point Ep1(200,325);
		cv::Point Ep4(600,325);
		cv::Point Ep2(200,375);
		cv::Point Ep3(600,375);

		cv::vector<cv::Point> endRegion;
		endRegion.push_back(Ep1);
		endRegion.push_back(Ep2);
		endRegion.push_back(Ep3);
		endRegion.push_back(Ep4);

		cv::Point overlay1[1][4];
		overlay1[0][0] = cv::Point( 0, 0 );
		overlay1[0][1] = cv::Point( 0, 480 );
		overlay1[0][2] = cv::Point( 320, 480);
		overlay1[0][3] = cv::Point( 30, 0 );
		

		const cv::Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		cv::Point overlay2[1][4];
		overlay2[0][0] = cv::Point( 40, 0 );
		overlay2[0][1] = cv::Point( 720, 400 );
		overlay2[0][2] = cv::Point( 720, 400 );
		overlay2[0][3] = cv::Point( 720, 0 );
		

		const cv::Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(6,false);
		CC.run(bufferSize,minObjectSizeDay,minObjectSizeNight,skip,learningTime,fileName,saveImgTo,dataBase,fps,expectedDist,horizontalBandwidth,RUN_ONLINE,
			*ppt,*ppt2,npt,npt2,
			USE_MOG2_ON,nmixtures,backgroundratio,
			DETECT_SHADOWS_ON,SHOW_LAST_CAR_ON,SHOW_BOUNDBOX_ON,SHOW_PREDICTION_ON,SHOW_LATEST_PATHS_ON,SHOW_TRANSIT_LEDGER_ON,SHOW_REGION_MONITORS_ON,
			SHOW_PATH_OF_CAR_X_OFF,USE_RESIZED_WITH_OVERLAYS_FRAME, startRegion, endRegion);
}

void runACounter4(int id){
		char fileName[100] = "Standard_SCU3IS_2014-10-21_0930.012.mp4";
		char saveImgTo[200] = "C:/Users/Vanya/Desktop/Car/";
		char dataBase[200] = "C:/Users/Vanya/Desktop/car_count_db";
		int bufferSize = 30;
		int minObjectSizeDay = 5000;
		int minObjectSizeNight = 10;
		int skip = 3;
		int learningTime = 240;
		int fps = 30;
		int expectedDist = 20;
		int horizontalBandwidth = 50;
		int nmixtures = 5;
		double backgroundratio = .6;
		bool detectShadows = false;
		bool online = true;

		cv::Point Sp1(100,325);
		cv::Point Sp4(400,325);
		cv::Point Sp2(100,375);
		cv::Point Sp3(400,375);
		
		cv::vector<cv::Point> startRegion;
		startRegion.push_back(Sp1);
		startRegion.push_back(Sp2);
		startRegion.push_back(Sp3);
		startRegion.push_back(Sp4);

		cv::Point Ep1(100,250);
		cv::Point Ep4(400,250);
		cv::Point Ep2(100,300);
		cv::Point Ep3(400,300);

		cv::vector<cv::Point> endRegion;
		endRegion.push_back(Ep1);
		endRegion.push_back(Ep2);
		endRegion.push_back(Ep3);
		endRegion.push_back(Ep4);

		cv::Point overlay1[1][4];
		overlay1[0][0] = cv::Point( 0, 0 );
		overlay1[0][1] = cv::Point( 0, 480 );
		overlay1[0][2] = cv::Point( 0, 480);
		overlay1[0][3] = cv::Point( 370, 0 );
		

		const cv::Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		cv::Point overlay2[1][4];
		overlay2[0][0] = cv::Point( 410, 0 );
		overlay2[0][1] = cv::Point( 480, 480 );
		overlay2[0][2] = cv::Point( 720, 480 );
		overlay2[0][3] = cv::Point( 720, 0 );
		

		const cv::Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(8,true);
		CC.run(bufferSize,minObjectSizeDay,minObjectSizeNight,skip,learningTime,fileName,saveImgTo,dataBase,fps,expectedDist,horizontalBandwidth,RUN_ONLINE,
			*ppt,*ppt2,npt,npt2,
			USE_MOG2_ON,nmixtures,backgroundratio,
			DETECT_SHADOWS_ON,SHOW_LAST_CAR_ON,SHOW_BOUNDBOX_ON,SHOW_PREDICTION_ON,SHOW_LATEST_PATHS_ON,SHOW_TRANSIT_LEDGER_ON,SHOW_REGION_MONITORS_ON,
			SHOW_PATH_OF_CAR_X_OFF,USE_RESIZED_WITH_OVERLAYS_FRAME, startRegion, endRegion);
}


int main() 
{
	using namespace cv;

	int runTest = 1;

	// User Prompt
	std::cout << "CTDOT CARCOUNTER DEMO: \n";
	std::cout << "This a simple demo application using the CTDOT car counting API. \n\n";

	std::cout << "Note: Feel free to play around with the runWrapper file to get an idea for how to use this API. \n\n";

	std::cout << "The following test runs are available: \n";
	std::cout << "1: Day Time/Going away from camera/Multithreaded \n";
	std::cout << "2: Night Time/Going towards camera/Uconn/Before Intersection \n";
	std::cout << "3: Day Time/Going towards camera/Uconn/After Intersection \n";
	std::cout << "4: Night Time/Going towards camera/Uconn/After Intersection \n";
	std::cout << "5: Night Time/Going away from camera/Uconn/Before Intersection \n";
	std::cout << "6: Day Time/Going away from camera/Uconn/Before Intersection \n";
	std::cout << "7: Day Time/Parallel/Highway/Shaky \n";
	std::cout << "8: Day to Night Time/Going towards camera/Uconn/After Intersection \n";
	std::cout << "9: Day to Night Time/Going towards camera/4-way intersection \n";
	std::cout << "10: Day Time/Going away from camera/4-way intersection \n";
	std::cout << "11: Day Time/Parallel/Highway/Stable \n";
	std::cout << "Select (1-11): ";
	std::cin >> runTest;
	
	// TEST1: Day Time/Going away from camera/Multithreaded
	if(runTest == 1)
	{
		std::thread thread1(runACounter1,1);
		std::thread thread2(runACounter2,2);
		std::thread thread3(runACounter3,3);
		std::thread thread4(runACounter4,4);
		thread1.detach();
		thread2.detach();
		thread3.detach();
		thread4.detach();
		while(true);
	}
	
	// TEST2: Night Time/Going towards camera/Uconn/Before Intersection
	if (runTest == 2){
		char fileName[100] = "Standard_SCU3JD_2014-10-21_0845.012.mp4";
		char saveImgTo[200] = "C:/Users/Vanya/Desktop/Car/";
		char dataBase[200] = "C:/Users/Vanya/Desktop/car_count_db";
		int bufferSize = 30;
		int minObjectSizeDay = 5000;
		int minObjectSizeNight = 10;
		int skip = 3;
		int learningTime = 240;
		int fps = 30;
		int expectedDist = 20;
		int horizontalBandwidth = 50;
		int nmixtures = 5;
		double backgroundratio = .6;
		bool detectShadows = false;
		bool online = true;

		cv::Point Sp1(300,250);
		cv::Point Sp4(600,250);
		cv::Point Sp2(300,300);
		cv::Point Sp3(600,300);
		
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

		Point overlay1[1][4];
		overlay1[0][0] = Point( 0, 0 );
		overlay1[0][1] = Point( 0, 480 );
		overlay1[0][2] = Point( 320, 480);
		overlay1[0][3] = Point( 240, 0 );
		

		const Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		Point overlay2[1][4];
		overlay2[0][0] = Point( 230, 0 );
		overlay2[0][1] = Point( 720, 400 );
		overlay2[0][2] = Point( 720, 400 );
		overlay2[0][3] = Point( 720, 0 );
		

		const Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(5,true);
		CC.run(bufferSize,minObjectSizeDay,minObjectSizeNight,skip,learningTime,fileName,saveImgTo,dataBase,fps,expectedDist,horizontalBandwidth,RUN_ONLINE,
			*ppt,*ppt2,npt,npt2,
			USE_MOG2_ON,nmixtures,backgroundratio,
			DETECT_SHADOWS_ON,SHOW_LAST_CAR_ON,SHOW_BOUNDBOX_ON,SHOW_PREDICTION_ON,SHOW_LATEST_PATHS_ON,SHOW_TRANSIT_LEDGER_ON,SHOW_REGION_MONITORS_ON,
			SHOW_PATH_OF_CAR_X_OFF,USE_RESIZED_WITH_OVERLAYS_FRAME, startRegion, endRegion);
		return 0;

	// TEST3: Day Time/Going towards camera/Uconn/After Intersection
	} if(runTest == 3) {
		char fileName[100] = "Standard_SCU3IP_2014-10-21_0830.010.mp4";
		char saveImgTo[200] = "C:/Users/Vanya/Desktop/Car/";
		char dataBase[200] = "C:/Users/Vanya/Desktop/car_count_db";
		int bufferSize = 30;
		int minObjectSizeDay = 5000;
		int minObjectSizeNight = 10;
		int skip = 3;
		int learningTime = 240;
		int fps = 30;
		int expectedDist = 100;
		int horizontalBandwidth = 50;
		int nmixtures = 5;
		double backgroundratio = .6;
		bool detectShadows = false;
		bool online = true;

		cv::Point Sp1(200,250);
		cv::Point Sp4(600,250);
		cv::Point Sp2(200,300);
		cv::Point Sp3(600,300);
		
		cv::vector<cv::Point> startRegion;
		startRegion.push_back(Sp1);
		startRegion.push_back(Sp2);
		startRegion.push_back(Sp3);
		startRegion.push_back(Sp4);

		cv::Point Ep1(200,325);
		cv::Point Ep4(600,325);
		cv::Point Ep2(200,375);
		cv::Point Ep3(600,375);

		vector<Point> endRegion;
		endRegion.push_back(Ep1);
		endRegion.push_back(Ep2);
		endRegion.push_back(Ep3);
		endRegion.push_back(Ep4);

		Point overlay1[1][4];
		overlay1[0][0] = Point( 0, 0 );
		overlay1[0][1] = Point( 0, 480 );
		overlay1[0][2] = Point( 320, 480);
		overlay1[0][3] = Point( 30, 0 );
		

		const Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		Point overlay2[1][4];
		overlay2[0][0] = Point( 40, 0 );
		overlay2[0][1] = Point( 720, 400 );
		overlay2[0][2] = Point( 720, 400 );
		overlay2[0][3] = Point( 720, 0 );
		

		const Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(6,false);
		CC.run(bufferSize,minObjectSizeDay,minObjectSizeNight,skip,learningTime,fileName,saveImgTo,dataBase,fps,expectedDist,horizontalBandwidth,RUN_ONLINE,
			*ppt,*ppt2,npt,npt2,
			USE_MOG2_ON,nmixtures,backgroundratio,
			DETECT_SHADOWS_ON,SHOW_LAST_CAR_ON,SHOW_BOUNDBOX_ON,SHOW_PREDICTION_ON,SHOW_LATEST_PATHS_ON,SHOW_TRANSIT_LEDGER_ON,SHOW_REGION_MONITORS_ON,
			SHOW_PATH_OF_CAR_X_OFF,USE_RESIZED_WITH_OVERLAYS_FRAME, startRegion, endRegion);
		return 0;
	// TEST4: Night Time/Going towards camera/Uconn/After Intersection
	} if(runTest == 4) {
		char fileName[100] = "Standard_SCU3IP_2014-10-21_0830.023.mp4";
		char saveImgTo[200] = "C:/Users/Vanya/Desktop/Car/";
		char dataBase[200] = "C:/Users/Vanya/Desktop/car_count_db";
		int bufferSize = 30;
		int minObjectSizeDay = 5000;
		int minObjectSizeNight = 10;
		int skip = 3;
		int learningTime = 240;
		int fps = 30;
		int expectedDist = 20;
		int horizontalBandwidth = 100;
		int nmixtures = 5;
		double backgroundratio = .6;
		bool detectShadows = false;
		bool online = true;

		cv::Point Sp1(200,250);
		cv::Point Sp4(600,250);
		cv::Point Sp2(200,300);
		cv::Point Sp3(600,300);
		
		cv::vector<cv::Point> startRegion;
		startRegion.push_back(Sp1);
		startRegion.push_back(Sp2);
		startRegion.push_back(Sp3);
		startRegion.push_back(Sp4);

		cv::Point Ep1(300,375);
		cv::Point Ep4(700,375);
		cv::Point Ep2(300,425);
		cv::Point Ep3(700,425);

		vector<Point> endRegion;
		endRegion.push_back(Ep1);
		endRegion.push_back(Ep2);
		endRegion.push_back(Ep3);
		endRegion.push_back(Ep4);

		Point overlay1[1][4];
		overlay1[0][0] = Point( 0, 0 );
		overlay1[0][1] = Point( 0, 480 );
		overlay1[0][2] = Point( 320, 480);
		overlay1[0][3] = Point( 30, 0 );

		const Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		Point overlay2[1][4];
		overlay2[0][0] = Point( 40, 0 );
		overlay2[0][1] = Point( 720, 380 );
		overlay2[0][2] = Point( 720, 380 );
		overlay2[0][3] = Point( 720, 0 );
		

		const Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(7,true);
		CC.run(bufferSize,minObjectSizeDay,minObjectSizeNight,skip,learningTime,fileName,saveImgTo,dataBase,fps,expectedDist,horizontalBandwidth,RUN_ONLINE,
			*ppt,*ppt2,npt,npt2,
			USE_MOG2_ON,nmixtures,backgroundratio,
			DETECT_SHADOWS_ON,SHOW_LAST_CAR_ON,SHOW_BOUNDBOX_ON,SHOW_PREDICTION_ON,SHOW_LATEST_PATHS_ON,SHOW_TRANSIT_LEDGER_ON,SHOW_REGION_MONITORS_ON,
			SHOW_PATH_OF_CAR_X_OFF,USE_RESIZED_WITH_OVERLAYS_FRAME, startRegion, endRegion);
		return 0;

	// TEST5: Night Time/Going away from camera/Uconn/Before Intersection/ DOESN'T WORK.
	} if(runTest == 5) {
		char fileName[100] = "Standard_SCU3IS_2014-10-21_0930.012.mp4";
		char saveImgTo[200] = "C:/Users/Vanya/Desktop/Car/";
		char dataBase[200] = "C:/Users/Vanya/Desktop/car_count_db";
		int bufferSize = 30;
		int minObjectSizeDay = 5000;
		int minObjectSizeNight = 10;
		int skip = 3;
		int learningTime = 240;
		int fps = 30;
		int expectedDist = 20;
		int horizontalBandwidth = 50;
		int nmixtures = 5;
		double backgroundratio = .6;
		bool detectShadows = false;
		bool online = true;

		cv::Point Sp1(100,325);
		cv::Point Sp4(400,325);
		cv::Point Sp2(100,375);
		cv::Point Sp3(400,375);
		
		cv::vector<cv::Point> startRegion;
		startRegion.push_back(Sp1);
		startRegion.push_back(Sp2);
		startRegion.push_back(Sp3);
		startRegion.push_back(Sp4);

		cv::Point Ep1(100,250);
		cv::Point Ep4(400,250);
		cv::Point Ep2(100,300);
		cv::Point Ep3(400,300);

		vector<Point> endRegion;
		endRegion.push_back(Ep1);
		endRegion.push_back(Ep2);
		endRegion.push_back(Ep3);
		endRegion.push_back(Ep4);

		Point overlay1[1][4];
		overlay1[0][0] = Point( 0, 0 );
		overlay1[0][1] = Point( 0, 480 );
		overlay1[0][2] = Point( 0, 480);
		overlay1[0][3] = Point( 370, 0 );
		

		const Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		Point overlay2[1][4];
		overlay2[0][0] = Point( 410, 0 );
		overlay2[0][1] = Point( 480, 480 );
		overlay2[0][2] = Point( 720, 480 );
		overlay2[0][3] = Point( 720, 0 );
		

		const Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(8,true);
		CC.run(bufferSize,minObjectSizeDay,minObjectSizeNight,skip,learningTime,fileName,saveImgTo,dataBase,fps,expectedDist,horizontalBandwidth,RUN_ONLINE,
			*ppt,*ppt2,npt,npt2,
			USE_MOG2_ON,nmixtures,backgroundratio,
			DETECT_SHADOWS_ON,SHOW_LAST_CAR_ON,SHOW_BOUNDBOX_ON,SHOW_PREDICTION_ON,SHOW_LATEST_PATHS_ON,SHOW_TRANSIT_LEDGER_ON,SHOW_REGION_MONITORS_ON,
			SHOW_PATH_OF_CAR_X_OFF,USE_RESIZED_WITH_OVERLAYS_FRAME, startRegion, endRegion);
		return 0;

	// TEST6: Day Time/Going away from camera/Uconn/Before Intersection
	} if(runTest == 6) {
		char fileName[100] = "Standard_SCU3IS_2014-10-21_0930.003.mp4";
		char saveImgTo[200] = "C:/Users/Vanya/Desktop/Car/";
		char dataBase[200] = "C:/Users/Vanya/Desktop/car_count_db";
		int bufferSize = 30;
		int minObjectSizeDay = 5000;
		int minObjectSizeNight = 10;
		int skip = 3;
		int learningTime = 240;
		int fps = 30;
		int expectedDist = 100;
		int horizontalBandwidth = 50;
		int nmixtures = 5;
		double backgroundratio = .6;
		bool detectShadows = false;
		bool online = true;

		cv::Point Sp1(100,325);
		cv::Point Sp4(500,325);
		cv::Point Sp2(100,375);
		cv::Point Sp3(500,375);
		
		cv::vector<cv::Point> startRegion;
		startRegion.push_back(Sp1);
		startRegion.push_back(Sp2);
		startRegion.push_back(Sp3);
		startRegion.push_back(Sp4);

		cv::Point Ep1(100,150);
		cv::Point Ep4(500,150);
		cv::Point Ep2(100,200);
		cv::Point Ep3(500,200);

		vector<Point> endRegion;
		endRegion.push_back(Ep1);
		endRegion.push_back(Ep2);
		endRegion.push_back(Ep3);
		endRegion.push_back(Ep4);

		Point overlay1[1][4];
		overlay1[0][0] = Point( 0, 0 );
		overlay1[0][1] = Point( 0, 480 );
		overlay1[0][2] = Point( 0, 480);
		overlay1[0][3] = Point( 370, 0 );
		

		const Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		Point overlay2[1][4];
		overlay2[0][0] = Point( 410, 0 );
		overlay2[0][1] = Point( 480, 480 );
		overlay2[0][2] = Point( 720, 480 );
		overlay2[0][3] = Point( 720, 0 );
		

		const Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(9,false);
		CC.run(bufferSize,minObjectSizeDay,minObjectSizeNight,skip,learningTime,fileName,saveImgTo,dataBase,fps,expectedDist,horizontalBandwidth,RUN_ONLINE,
			*ppt,*ppt2,npt,npt2,
			USE_MOG2_ON,nmixtures,backgroundratio,
			DETECT_SHADOWS_ON,SHOW_LAST_CAR_ON,SHOW_BOUNDBOX_ON,SHOW_PREDICTION_ON,SHOW_LATEST_PATHS_ON,SHOW_TRANSIT_LEDGER_ON,SHOW_REGION_MONITORS_ON,
			SHOW_PATH_OF_CAR_X_OFF,USE_RESIZED_WITH_OVERLAYS_FRAME, startRegion, endRegion);
		return 0;
	// TEST7: Day Time/Parallel/Highway/Shaky
	} if(runTest == 7) {
		char fileName[100] = "Standard_SCU3JD_2014-07-30_0500.002.mp4";
		char saveImgTo[200] = "C:/Users/Vanya/Desktop/Car/";
		char dataBase[200] = "C:/Users/Vanya/Desktop/car_count_db";
		int bufferSize = 30;
		int minObjectSizeDay = 200;
		int minObjectSizeNight = 10;
		int skip = 3;
		int learningTime = 1000;
		int fps = 30;
		int expectedDist = 100;
		int horizontalBandwidth = 50;
		int nmixtures = 5;
		double backgroundratio = .6;

		cv::Point Sp1(100,220);
		cv::Point Sp4(150,220);
		cv::Point Sp2(100,375);
		cv::Point Sp3(150,375);
		
		cv::vector<cv::Point> startRegion;
		startRegion.push_back(Sp1);
		startRegion.push_back(Sp2);
		startRegion.push_back(Sp3);
		startRegion.push_back(Sp4);

		cv::Point Ep1(300,220);
		cv::Point Ep4(350,220);
		cv::Point Ep2(300,375);
		cv::Point Ep3(350,375);

		vector<Point> endRegion;
		endRegion.push_back(Ep1);
		endRegion.push_back(Ep2);
		endRegion.push_back(Ep3);
		endRegion.push_back(Ep4);

		Point overlay1[1][4];
		overlay1[0][0] = Point( 0, 0 );
		overlay1[0][1] = Point( 0, 270 );
		overlay1[0][2] = Point( 720, 270);
		overlay1[0][3] = Point( 720, 0 );
		

		const Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		Point overlay2[1][4];
		overlay2[0][0] = Point( 0, 300 );
		overlay2[0][1] = Point( 0, 480 );
		overlay2[0][2] = Point( 720, 480 );
		overlay2[0][3] = Point( 720, 300 );
		

		const Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(10,false);
		CC.run(bufferSize,minObjectSizeDay,minObjectSizeNight,skip,learningTime,fileName,saveImgTo,dataBase,fps,expectedDist,horizontalBandwidth,RUN_ONLINE,
			*ppt,*ppt2,npt,npt2,
			USE_MOG2_ON,nmixtures,backgroundratio,
			DETECT_SHADOWS_ON,SHOW_LAST_CAR_ON,SHOW_BOUNDBOX_ON,SHOW_PREDICTION_ON,SHOW_LATEST_PATHS_ON,SHOW_TRANSIT_LEDGER_ON,SHOW_REGION_MONITORS_ON,
			SHOW_PATH_OF_CAR_X_OFF,USE_RESIZED_WITH_OVERLAYS_FRAME, startRegion, endRegion);
		return 0;
	// TEST8: Day to Night Time/Going towards camera/Uconn/After Intersection
	} if(runTest == 8) {
		char fileName[100] = "Standard_SCU3IP_2014-10-21_0830.010.mp4";
		char saveImgTo[200] = "C:/Users/Vanya/Desktop/Car/";
		char dataBase[200] = "C:/Users/Vanya/Desktop/car_count_db";
		int bufferSize = 30;
		int minObjectSizeDay = 5000;
		int minObjectSizeNight = 10;
		int skip = 3;
		int learningTime = 240;
		int fps = 30;
		int expectedDist = 20;
		int horizontalBandwidth = 100;
		int nmixtures = 5;
		double backgroundratio = .6;
		bool detectShadows = false;
		bool online = true;

		cv::Point Sp1(200,250);
		cv::Point Sp4(600,250);
		cv::Point Sp2(200,300);
		cv::Point Sp3(600,300);
		
		cv::vector<cv::Point> startRegion;
		startRegion.push_back(Sp1);
		startRegion.push_back(Sp2);
		startRegion.push_back(Sp3);
		startRegion.push_back(Sp4);

		cv::Point Ep1(300,375);
		cv::Point Ep4(700,375);
		cv::Point Ep2(300,425);
		cv::Point Ep3(700,425);

		vector<Point> endRegion;
		endRegion.push_back(Ep1);
		endRegion.push_back(Ep2);
		endRegion.push_back(Ep3);
		endRegion.push_back(Ep4);

		Point overlay1[1][4];
		overlay1[0][0] = Point( 0, 0 );
		overlay1[0][1] = Point( 0, 480 );
		overlay1[0][2] = Point( 320, 480);
		overlay1[0][3] = Point( 30, 0 );

		const Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		Point overlay2[1][4];
		overlay2[0][0] = Point( 40, 0 );
		overlay2[0][1] = Point( 720, 380 );
		overlay2[0][2] = Point( 720, 380 );
		overlay2[0][3] = Point( 720, 0 );
		

		const Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(11,false);
		CC.run(bufferSize,minObjectSizeDay,minObjectSizeNight,skip,learningTime,fileName,saveImgTo,dataBase,fps,expectedDist,horizontalBandwidth,RUN_ONLINE,
			*ppt,*ppt2,npt,npt2,
			USE_MOG2_ON,nmixtures,backgroundratio,
			DETECT_SHADOWS_ON,SHOW_LAST_CAR_ON,SHOW_BOUNDBOX_ON,SHOW_PREDICTION_ON,SHOW_LATEST_PATHS_ON,SHOW_TRANSIT_LEDGER_ON,SHOW_REGION_MONITORS_ON,
			SHOW_PATH_OF_CAR_X_OFF,USE_RESIZED_WITH_OVERLAYS_FRAME, startRegion, endRegion);
		return 0;
	// TEST9: Day to Night Time/Going towards camera/4-way intersection/
	} if(runTest == 9) {
		char fileName[100] = "Standard_SCU3IU_2015-01-08_1600.002.mp4";
		char saveImgTo[200] = "C:/Users/Vanya/Desktop/Car/";
		char dataBase[200] = "C:/Users/Vanya/Desktop/car_count_db";
		int bufferSize = 30;
		int minObjectSizeDay = 200;
		int minObjectSizeNight = 10;
		int skip = 3;
		int learningTime = 1000;
		int fps = 30;
		int expectedDist = 100;
		int horizontalBandwidth = 50;
		int nmixtures = 5;
		double backgroundratio = .6;

		cv::Point Sp1(20,220);
		cv::Point Sp4(60,220);
		cv::Point Sp2(20,375);
		cv::Point Sp3(60,375);
		
		cv::vector<cv::Point> startRegion;
		startRegion.push_back(Sp1);
		startRegion.push_back(Sp2);
		startRegion.push_back(Sp3);
		startRegion.push_back(Sp4);

		cv::Point Ep1(70,190);
		cv::Point Ep4(110,190);
		cv::Point Ep2(70,350);
		cv::Point Ep3(110,350);

		vector<Point> endRegion;
		endRegion.push_back(Ep1);
		endRegion.push_back(Ep2);
		endRegion.push_back(Ep3);
		endRegion.push_back(Ep4);

		Point overlay1[1][4];
		overlay1[0][0] = Point( 0, 0 );
		overlay1[0][1] = Point( 0, 0 );
		overlay1[0][2] = Point( 0, 0 );
		overlay1[0][3] = Point( 0, 0 );
		

		const Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		Point overlay2[1][4];
		overlay2[0][0] = Point( 0, 0 );
		overlay2[0][1] = Point( 0, 0 );
		overlay2[0][2] = Point( 0, 0 );
		overlay2[0][3] = Point( 0, 0 );
		

		const Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(12,false);
		CC.run(bufferSize,minObjectSizeDay,minObjectSizeNight,skip,learningTime,fileName,saveImgTo,dataBase,fps,expectedDist,horizontalBandwidth,RUN_ONLINE,
			*ppt,*ppt2,npt,npt2,
			USE_MOG2_ON,nmixtures,backgroundratio,
			DETECT_SHADOWS_ON,SHOW_LAST_CAR_ON,SHOW_BOUNDBOX_ON,SHOW_PREDICTION_ON,SHOW_LATEST_PATHS_ON,SHOW_TRANSIT_LEDGER_ON,SHOW_REGION_MONITORS_ON,
			SHOW_PATH_OF_CAR_X_OFF,USE_RESIZED_WITH_OVERLAYS_FRAME, startRegion, endRegion);
		return 0;
	// TEST10: Day Time/Going away from camera/4-way intersection/
	} if(runTest == 10) {
		char fileName[100] = "Standard_SCU026-R_2014-03-11_0700.002.mp4";
		char saveImgTo[200] = "C:/Users/Vanya/Desktop/Car/";
		char dataBase[200] = "C:/Users/Vanya/Desktop/car_count_db";
		int bufferSize = 30;
		int minObjectSizeDay = 3000;
		int minObjectSizeNight = 10;
		int skip = 3;
		int learningTime = 240;
		int fps = 30;
		int expectedDist = 20;
		int horizontalBandwidth = 100;
		int nmixtures = 5;
		double backgroundratio = .6;
		bool detectShadows = false;
		bool online = true;

		cv::Point Sp1(200,200);
		cv::Point Sp4(500,200);
		cv::Point Sp2(200,300);
		cv::Point Sp3(500,300);
		
		cv::vector<cv::Point> startRegion;
		startRegion.push_back(Sp1);
		startRegion.push_back(Sp2);
		startRegion.push_back(Sp3);
		startRegion.push_back(Sp4);

		cv::Point Ep1(300,50);
		cv::Point Ep4(700,50);
		cv::Point Ep2(300,150);
		cv::Point Ep3(700,150);

		vector<Point> endRegion;
		endRegion.push_back(Ep1);
		endRegion.push_back(Ep2);
		endRegion.push_back(Ep3);
		endRegion.push_back(Ep4);

		Point overlay1[1][4];
		overlay1[0][0] = Point( 0, 0 );
		overlay1[0][1] = Point( 0, 0 );
		overlay1[0][2] = Point( 0, 0 );
		overlay1[0][3] = Point( 0, 0 );

		const Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		Point overlay2[1][4];
		overlay2[0][0] = Point( 0, 0 );
		overlay2[0][1] = Point( 0, 0 );
		overlay2[0][2] = Point( 0, 0 );
		overlay2[0][3] = Point( 0, 0 );
		

		const Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(13,false);
		CC.run(bufferSize,minObjectSizeDay,minObjectSizeNight,skip,learningTime,fileName,saveImgTo,dataBase,fps,expectedDist,horizontalBandwidth,RUN_ONLINE,
			*ppt,*ppt2,npt,npt2,
			USE_MOG2_ON,nmixtures,backgroundratio,
			DETECT_SHADOWS_ON,SHOW_LAST_CAR_ON,SHOW_BOUNDBOX_ON,SHOW_PREDICTION_ON,SHOW_LATEST_PATHS_ON,SHOW_TRANSIT_LEDGER_ON,SHOW_REGION_MONITORS_ON,
			SHOW_PATH_OF_CAR_X_OFF,USE_RESIZED_WITH_OVERLAYS_FRAME, startRegion, endRegion);
		return 0;
	
	// TEST11: Day Time/Parallel/Highway/Stable
	} if(runTest == 11) {
		char fileName[100] = "Standard_SCU3JD_2014-08-11_0000.016.mp4";
		char saveImgTo[200] = "C:/Users/Vanya/Desktop/Car/";
		char dataBase[200] = "C:/Users/Vanya/Desktop/car_count_db";
		int bufferSize = 30;
		int minObjectSizeDay = 5000;
		int minObjectSizeNight = 10;
		int skip = 3;
		int learningTime = 240;
		int fps = 30;
		int expectedDist = 20;
		int horizontalBandwidth = 100;
		int nmixtures = 5;
		double backgroundratio = .6;
		bool detectShadows = false;
		bool online = true;

		cv::Point Sp1(50,100);
		cv::Point Sp4(200,100);
		cv::Point Sp2(50,400);
		cv::Point Sp3(200,400);
		
		cv::vector<cv::Point> startRegion;
		startRegion.push_back(Sp1);
		startRegion.push_back(Sp2);
		startRegion.push_back(Sp3);
		startRegion.push_back(Sp4);

		cv::Point Ep1(400,100);
		cv::Point Ep4(700,100);
		cv::Point Ep2(400,400);
		cv::Point Ep3(700,400);

		vector<Point> endRegion;
		endRegion.push_back(Ep1);
		endRegion.push_back(Ep2);
		endRegion.push_back(Ep3);
		endRegion.push_back(Ep4);

		Point overlay1[1][4];
		overlay1[0][0] = Point( 0, 0 );
		overlay1[0][1] = Point( 0, 0 );
		overlay1[0][2] = Point( 0, 0 );
		overlay1[0][3] = Point( 0, 0 );

		const Point* ppt[1] = { overlay1[0] };
		int npt[] = { 4 };

		Point overlay2[1][4];
		overlay2[0][0] = Point( 0, 0 );
		overlay2[0][1] = Point( 0, 0 );
		overlay2[0][2] = Point( 0, 0 );
		overlay2[0][3] = Point( 0, 0 );
		

		const Point* ppt2[1] = { overlay2[0] };
		int npt2[] = { 4 };

		carCounter CC(14,false);
		CC.run(bufferSize,minObjectSizeDay,minObjectSizeNight,skip,learningTime,fileName,saveImgTo,dataBase,fps,expectedDist,horizontalBandwidth,RUN_ONLINE,
			*ppt,*ppt2,npt,npt2,
			USE_MOG2_ON,nmixtures,backgroundratio,
			DETECT_SHADOWS_ON,SHOW_LAST_CAR_ON,SHOW_BOUNDBOX_ON,SHOW_PREDICTION_ON,SHOW_LATEST_PATHS_ON,SHOW_TRANSIT_LEDGER_ON,SHOW_REGION_MONITORS_ON,
			SHOW_PATH_OF_CAR_X_OFF,USE_RESIZED_WITH_OVERLAYS_FRAME, startRegion, endRegion);
		return 0;
	}
}