#define USE_BACKGROUND 0
#define USE_ORGINAL_FRAME 1
#define USE_RESIZED_WITH_OVERLAYS_FRAME 2
#define USE_SUBTRACTED 3

#define DETECT_SHADOWS_ON true
#define DETECT_SHADOWS_OFF false

#define SHOW_LAST_CAR_ON true
#define SHOW_LAST_CAR_OFF false
#define SHOW_PREDICTION_ON true
#define SHOW_PREDICTION_OFF false
#define SHOW_LATEST_PATHS_ON true
#define SHOW_LATEST_PATHS_OFF false
#define SHOW_TRANSIT_LEDGER_ON true
#define SHOW_TRANSIT_LEDGER_OFF false
#define SHOW_BOUNDBOX_ON true
#define SHOW_BOUNDBOX_OFF false
#define SHOW_REGION_MONITORS_ON true
#define SHOW_REGION_MONITORS_OFF false

#define SHOW_PATH_OF_CAR_X_OFF -1

#define RUN_ONLINE true
#define RUN_OFFLINE false

#define USE_MOG2_ON true
#define USE_MOG2_OFF false

#define _VARIADIC_MAX 10

#ifndef CARCOUNTER_H
#include <stdio.h>  
#include <iostream>
#include <ctime>
#include <thread>
#include <limits>
#include <vector>
#include <queue> 

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "sqlite3.h" 
#include "DPAdaptiveMedianBGS.h"
#include "FrameDifferenceBGS.h"

/* Frame datatype */
/* Holistic representation of all the objects captured in the given frame. */ 
struct Frame
{
	// Geometric description
	cv::vector<cv::vector<cv::Point>> contour;
	cv::vector<cv::Rect> boundRect;
	cv::vector<cv::Point2f> center;
	cv::vector<float> radius;
	
	// Instantaneous metrics (based on current frame and the last)  
	cv::vector<double> velocity;
	cv::vector<double> velocityX;
	cv::vector<double> velocityY;

	// Average metrics (based on the entire known history of the object)
	cv::vector<double> speed;
	cv::vector<double> speedX;
	cv::vector<double> speedY;
	cv::vector<double> distance;
	cv::vector<double> distanceX;
	cv::vector<double> distanceY;

	// Meta-data
	cv::vector<int> time;
	cv::vector<int> id;
	cv::vector<int> tIdx;
	cv::vector<bool> p;
	cv::vector<bool> checked;
	cv::vector<cv::Point2f> start_loc;

	// Calculated prediction of where the object should be in the future
	cv::vector<std::vector<cv::Point2f>> pTraj;
};

/* Object datatype */
/* Representation of object. */
struct Object
{	
	// Target data
	int id;	
	int count;
	cv::Point2f start_loc;
	cv::vector<cv::Point2f> trajectory;
	cv::vector<int> area;
	cv::vector<int> frames;
	cv::vector<int> ulx;
	cv::vector<int> uly;
	cv::vector<int> lrx;
	cv::vector<int> lry;
	cv::Mat img;
	
	// Simple flags
	bool checked;
	bool hasSibling;
	bool headlight;
	bool miss;
	
	// Headlight sibling
	int siblingID;

	// Variables to track last know state of object.
	int last_seen;
	cv::vector<std::vector<cv::Point2f>> trajectories_before_disappearing;
	cv::vector<cv::Point> contour_before_disappearing;
	cv::Rect boundbox_before_disappearing;
	cv::Point2f center_when_last_seen;
	double velocity_when_last_seen;
	double velocityX_when_last_seen;
	double velocityY_when_last_seen;
	double speed_when_last_seen;
	double speedX_when_last_seen;
	double speedY_when_last_seen;
	double distance_when_last_seen;
	double distanceX_when_last_seen;
	double distanceY_when_last_seen;
	int time_when_last_seen;
	
	// Buffer index
	int bIdx;

	// Transit index
	int tIdx;

	// Set default boolean values
	Object() {
		checked = false;
		hasSibling = false;
		headlight = false;
		miss = false;
  }
};

/* An instance of carCounter can run the algorithm.
 * carCounter also stores result data on cars and all
 * processed frames. An alternative to getting the data
 * from program memory is to pull it from a database. A 
 * carCounter will keep table data updated as the algorithm
 * runs. 
 *
 * note: Anything marked with (*) is specific to night time mode only.
 *
 * while(true)
 *	- obtainframe < (opencv)
 *		- obtainBoundBoxes
 *  - createFrame
 *  - threshold   < (opencv)
 *  - selectBackgroundSubtraction																
 *		- MOG2 < (opencv) || - Adaptive Median < (BGS) || - (*) FrameDifference < (BGS)				
 *	- dilate      < (opencv)
 *  - erode       < (opencv)
 *  - medianblur  < (opencv) 
 *  - removeShadow
 *	- persistenceCheck
 *		- checkTransitLedgerUsingMomentAndCenter
 *  - startRegionMonitor																														
 *		- safetyChecks																			
 *			- histogramCalcAndFindStd
 *				- calcHistogram
 *				- findStd
 *  - checkMissing
 *  - endRegionMonitor																			 
 *		- safetyChecks
 *			- histogramClacAndFindStd
 *				- calcHistogram
 *				- findStd
 *		- (*) detectCarBoxForNight
 *  - drawResult
 *
 */

class carCounter{

private:

	// Identifier
	int id;

	// Counters
	int frame_counter;
	int object_counter;
	int car_counter;

	// Flow control (If nighttime is selected algorithm will take a slightly alternate path each cycle)
	bool nightTime;

	// Temporal Workspace
	std::deque<Frame> buffer;	
	std::deque<Object> transit;
	
	// Result
	std::deque<Object> carArchive;
	std::deque<Frame>  frameArchive;

	// Testing
	std::clock_t start;         // timer
	double duration;		    // timer
	cv::Mat test;			    // isn't used in calc, output to this to see what something looks like				
	
	// Frame
	cv::Mat frame;				// current frame  
	cv::Mat resizeF;			// current frame resized
	cv::Mat resizeFgray;		// current frame resized (grayscale)	
	
	// Display Matrix
	cv::Mat display;		    // can use this to draw whatever you want on it.
	
	// Foreground Matrices
	cv::Mat mask;				// fg mask generated by MOG2/Adaptive Median or Threshold for night time.
	cv::Mat3b maskrgb;          // fg mask with 3 channels.
	cv::Mat extraMask;			// fg mask generated by frame difference (during night time).
	cv::Mat3b extraMaskrgb;		// fg mask with 3 channels.

	// Background Matrix
	cv::Mat background;	

	cv::vector<cv::vector<cv::Point>> contours;
	cv::vector<cv::Vec4i> hierarchy;
	cv::vector<cv::Vec3f> vecCircles;               
	cv::vector<cv::Vec3f>::iterator itrCircles;

	cv::BackgroundSubtractorMOG2 MOG2;  // MOG Background subtractor.
	IBGS *bgsAdaptiveMedian;
	IBGS *bgsFrameDifference;
	
	// db variables
	sqlite3 *db;
	int rc;

	/*  Initializes the frame object once background subtraction is done. */

	void createFrame(Frame &data, cv::vector<cv::vector<cv::Point>> &contours,
					 cv::vector<cv::vector<cv::Point>> &contours_poly,
					 cv::vector<cv::Rect> &boundRect,
					 cv::vector<cv::Point2f> &center,
					 cv::vector<float> &radius, int minObjectSize);

	/* Select and apply BackgroundSubtraction. 
	   For daytime use either MOG2 or a modified DP Adaptive Median.
 	   For nighttime we use frame difference. (currently not implemented) */

	void carCounter::selectBackgroundSubtraction(bool useMOG2, bool nightTime,
												 cv::Mat &resizeF,cv::Mat &mask,
												 int nmixtures, double backgroundratio, bool detectShadows);

	/* Checks what objects in the current frame correspond to the ones 
	   in the previous. Will check against the transit ledger and properly update
	   the most current frame object in the buffer. */

	void persistenceCheck(int &frame_counter,int type,std::deque<Frame> &buffer, std::deque<Object> &transit,int expectedDist);
	
	/* This method keeps tabs on objects that may have went missing. 
	   If object transit ledger is updated with information from current frame. 
	   Else the object is simply marked as missing and the transit ledger
	   will store info from the previous time the object was seen. */
	
	void checkMissing(int &frame_counter,
					  std::deque<Frame> &buffer,std::deque<Object> &transit);

	/* Checks for new objects entering the transit region.
	   Objects in the transit region are suspected cars, if they enter they should also leave
	   at some point. When this happens they are identified as being cars. In the start region objects
	   are assigned their initial values. During night time we attempt to "match" multiple objects to one
	   another since we look at headlights. (2 headlights belong to 1 car)*/

	void startRegionMonitor(cv::Mat &frame,cv::Mat &background, cv::Mat &subtract,
						    std::deque<Frame> &buffer,std::deque<Object> &transit, 
							cv::vector<cv::Point> startRegion,
							int horizontalBandWidth);
	
	/* Checks for objects exiting the transit region. We flush all objects up to 
	   the last one that exited. If A,B, and C entered the transit region in 
	   that order and C ends up crossing first then A and B are disregarded.*/

	void endRegionMonitor(cv::Mat &frame,cv::Mat &background,cv::Mat &subtract, cv::Mat &subtract2, int minObjectSize,
						  int &car_counter,
						  std::deque<Frame> &buffer,std::deque<Object> &transit,std::deque<Object> &carArchive,
						  cv::vector<cv::Point> endRegion);

	/* Internal functionality for endRegionMonitor */

	void detectCarBoxForNight(cv::Mat &subtract,cv::Rect &boundRect,int minObjectSize);

	
	/* Internal functionality for persistenceCheck */
	
	void checkTransitLedgerUsingMomentAndCenter(int &frame_counter,std::deque<Frame> &buffer,std::deque<Object> &transit,int expectDist);

	/* Internal functionality for startRegionMonitor and endRegionMonitor */
	
	bool safetyChecks(cv::Mat &frame,cv::Mat &background,cv::Mat &subtract,
					  std::deque<Frame> &buffer,
					  int boundBoxNum,double thresh);
	
	/* Internal functionality for safetyChecks */
	
	double histogramCalcAndFindStd(cv::Mat &frame,cv::Mat &background,cv::Mat &subtract,
											   std::deque<Frame> &buffer,int boundBoxNum);

	/* Internal functionality for histogramCalcAndFindMax */

	cv::vector<cv::Mat> calcHistogram(cv::Mat& img,cv::Mat& mask);
	
	double findStd(cv::vector<cv::Mat> &hist, int channel);
	
	/* Visualization */

	void drawResult(int &car_counter, int &object_counter, cv::Mat &frame, cv::vector<cv::Vec4i> &hierarchy, std::deque<Frame> &buffer,std::deque<Object> &transit,
								bool showLastCar, bool boundBoxesOn, bool predictionOn, bool latestPathsOn, bool displayTransitLedger, bool displayFocusRegions, int showPathofId,
								cv::vector<cv::Point> startRegion,
								cv::vector<cv::Point> endRegion);
	/* Custom Utility */

	int obtainBoundBoxes(cv::vector<cv::vector<cv::Point>> &contours,cv::vector<cv::vector<cv::Point>> &contours_poly, 
									  cv::vector<cv::Rect> &boundRect,
									  cv::vector<cv::Point2f> &center,cv::vector<float> &radius, int minObjectSize);

	/* Applies median blur, dilate/erode and shadow removal */
	void cleanNoise(cv::Mat &mask, cv::Mat &extraMask,cv::Mat3b &maskrgb, cv::Mat3b &extraMaskrgb);

	/* Internal to clean noise, can remove shadows if MOG2 shadow detection is being used*/
	void removeShadow(cv::Mat &subtract,cv::Mat3b &subtractrgb);
	
	/* DB functions */
	void openDB(int expID,char dataBase[200]);
	void closeDB();

	/* Async method for filling the database */
	void exportCarsToDB(int expID,char fileName[100], char saveImgTo[200], std::deque<Object> &carArchive, int fps);

public:
	// constructor
	carCounter(int expID,bool night);

	/* Start the counter
	 *  NAME:				:DESCRIPTION																															:UNITS (IF APPLICABLE)
	 *  bufferSize:			 Local workspace (how many of the latest frames of the video to store)
	 *  minObjectSizeDay:	 Minimum required size for an object to be considered (ignore things that are too small to be a car)									Pixels Squared
	(*) minObjectSizeNight:	 Minimum required size for an object to be considered (ignore things that are too small to be a headlight)								Pixels Squared
	 *  skip:				 Skips frames from video (potential performance improvement)																			Frames
	 *  learningTime:		 Give time for BGS to settle																											Frames
	 *  fileName:			 Video filename that you want run the algorithm on.
	 *  saveImgTo:			 Directory in which cropped images of counted cars will be saved.
	 *  dataBase			 The local sqlite database to which you want to save data. (absolute location)
	 *  fps:				 Ignore this for now (set the frames per second of this video. Can be used to track how many seconds into the video the car is detect)  Frames/second
	 *  expectedDist:		 Tells the algorithm the threshold on how far along in pixels the object will be next frame.											Pixels
	 *  horizontalBandWidth: If looking for a sibling headlight object this tells the algorithm how wide the search area band should be horizontally.				Pixels
	 *  online:				 Whether to display frames
	 *  ppt,ppt2,npt,npt2:	 Set an overlay mask on the video (optional)
	 *  useMOG2:			 Choose between MOG2 or Adaptive Median for background subtraction 
	 						 (Adaptive median is more primitive but is able to handle stationary objects).
	 						 Check MOG2 for details. 
	 *  startRegion:		 Set points for start Region.
	 *  endRegion:			 Set points for end Region.
	 
	   Other parameters are settings for displaying if online mode is chosen.
	 *
	 * note: Anything marked with (*) is specific to night time mode only.
	 */
	void run(int bufferSize, int minObjectSizeDay, int minObjectSizeNight, int skip, int learningTime, 
		char fileName[100], char saveImgTo[200], char dataBase[200], 
		int fps,
		int expectedDist, int horizontalBandWidth,
		bool online, const cv::Point* ppt, const cv::Point* ppt2,int* npt,int* npt2, bool useMOG2,
		int nmixtures, double backgroundratio, bool detectShadows, bool showLastCar,
		bool boundBoxesOn, bool predictionOn, bool latestPathsOn, 
		bool displayTransitLedger, bool displayFocusRegions, int showPathofId,
		int displayType,
		cv::vector<cv::Point> startRegion,
		cv::vector<cv::Point> endRegion);

	/* Getters for the results */
	void getFrameArchiveData(int* frames);
	void getCarArchiveData(int* cars);
};

static int callback(void *NotUsed, int argc, char **argv, char **azColName);

#endif