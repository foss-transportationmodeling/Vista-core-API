#include "carCounter.h"
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
_CRT_SECURE_NO_WARNINGS

#include <string>

#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h>  





using namespace cv;

// db functions
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

// PRIVATE METHODS

void carCounter::openDB(int expID,char dataBase[200]){
	char * sql;
	sql = new char[200];

	char * num;
	num = new char[15];

	char * params;
	params = new char[500];

	char *zErrMsg = 0;

	/* Open database */
	rc = sqlite3_open(dataBase, &db);
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stderr, "Opened database successfully\n");
	}

	/* Create table if it doesn't exist. */
	
	strcpy_s (sql,200,"CREATE TABLE IF NOT EXISTS CarTable_exp");
    params = "(Video TEXT   NOT NULL," \
			 "Date TEXT		NOT NULL," \
			 "SecInVid TEXT NOT NULL," \
			 "ExpID INTEGER NOT NULL," \
			 "CarID INTEGER NOT NULL," \
			 "Count TEXT	NOT NULL," \
			 "Frames TEXT	NOT NULL," \
			 "ULX TEXT		NOT NULL,"\
			 "ULY TEXT		NOT NULL,"\
			 "LRX TEXT      NOT NULL,"\
			 "LRY TEXT      NOT NULL,"\
			 "Area TEXT     NOT NULL,"\
			 "Path TEXT		NOT NULL);";

	_itoa_s(expID,num,15,10);
	strncat_s (sql, 10000, num, 15);
	strncat_s (sql,10000, params, 500);

	/* Execute SQL statement */
	bool continueTrying = true;
	while(continueTrying){
		rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
		switch(rc){
			case SQLITE_BUSY:
				sqlite3_sleep(10);
				break;
			case SQLITE_OK:
				continueTrying = false;
				break;
		}
	}
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Table created successfully\n");
	}
}

void carCounter::closeDB(){
	sqlite3_close(db);
}

void carCounter::exportCarsToDB(int expID,char fileName[100],char saveImgTo[200],std::deque<Object> &carArchive,int fps) {
	
	int index = 0;
	char *zErrMsg = 0;
	char * sql;
	sql = new char[10000];

	char * value0;
	value0 = new char[100];

	char * value1;
	value1 = new char[100];

	char * value2;
	value2 = new char[15];

	char * value3;
	value3 = new char[15];

	char * value4;
	value4 = new char[15];

	char * value5;
	value5 = new char[15];

	char * value6;
	value6 = new char[1000];

	char * value7;
	value7 = new char[1000];

	char * value8;
	value8 = new char[1000];

	char * value9;
	value9 = new char[1000];

	char * value10;
	value10 = new char[1000];

	char * value11;
	value11 = new char[1000];

	char * value12;
	value12 = new char[1000];

	char * temp;
	temp = new char[15];
	
	// File path
	strcpy_s (value0,100,"'");
	strncat_s (value0, 100, fileName,100);
	strncat_s (value0, 100, "'",1);

	// Time and Date
	time_t now = time(0);
	tm* localtm;
	localtime_s(localtm,&now);
	tm* gmtm;
	gmtime_s(gmtm, &now);
	strcpy_s (value1, 100,"'");
	strncat_s(value1,100, asctime(gmtm) , 100);
	strncat_s (value1, 100, "'", 1);
	
	while(true){
		while(index < carArchive.size()){
			// Archive Last Frame Time
			int last_frame = carArchive[index].frames[carArchive[index].frames.size()-1];
			double secsInVideo = (double) (last_frame)/(double) (fps);
			sprintf_s(value2, 15,"%f", secsInVideo);
			
			// Expirement Id
			_itoa_s(expID,value3,15,10);
			
			// Archive Car Id
			_itoa_s (carArchive[index].id,value4,15,10);

			// Archive Car Count
			_itoa_s (carArchive[index].count,value5,15,10);
			
			// Archive Frames
			strcpy_s (value6, 1000,"'");
			for(int i=1; i < carArchive[index].frames.size(); i++){
				_itoa_s (carArchive[index].frames[i],temp,15,10);
				if(i != carArchive[index].frames.size()-1){
					strncat_s (temp, 15, " ",1);
					strncat_s (value6, 1000, temp,15);
				} else {
					strncat_s (temp, 15, "'",1);
					strncat_s (value6, 1000, temp,15);
				}
			}
			
			// Archive ulX
			strcpy_s (value7,1000,"'");
			for(int i=1; i < carArchive[index].ulx.size(); i++){
				_itoa_s (carArchive[index].ulx[i],temp,15,10);
				if(i != carArchive[index].ulx.size()-1){
					strncat_s (temp, 15, " ", 1);
					strncat_s (value7, 1000, temp,15);
				} else {
					strncat_s (temp, 15, "'", 1);
					strncat_s(value7, 1000, temp,15);
				}
			}

			// Archive ulY
			strcpy_s (value8,1000,"'");
			for(int i=1; i < carArchive[index].uly.size(); i++){
				_itoa_s (carArchive[index].uly[i],temp,15,10);
				if(i != carArchive[index].uly.size()-1){
					strncat_s (temp, 15, " ",1);
					strncat_s (value8, 1000, temp,15);
				} else {
					strncat_s (temp, 15, "'",1);
					strncat_s(value8, 1000, temp,15);
				}
			}

			// Archive lrX
			strcpy_s(value9,1000,"'");
			for(int i=1; i < carArchive[index].lrx.size(); i++){
				_itoa_s (carArchive[index].lrx[i],temp,15,10);
				if(i != carArchive[index].lrx.size()-1){
					strncat_s (temp, 15, " ",1);
					strncat_s (value9, 1000, temp,15);
				} else {
					strncat_s (temp, 15, "'",1);
					strncat_s(value9,1000,temp,15);
				}
			}

			// Archive lrY
			strcpy_s (value10,1000,"'");
			for(int i=1; i < carArchive[index].lry.size(); i++){
				_itoa_s (carArchive[index].lry[i],temp,15,10);
				if(i != carArchive[index].lry.size()-1){
					strncat_s (temp, 15, " ",1);
					strncat_s (value10, 1000, temp,15);
				} else {
					strncat_s (temp,15,"'",1);
					strncat_s(value10,1000,temp,15);
				}
			}

			// Archive Area
			strcpy_s (value11,1000,"'");
			for(int i=1; i < carArchive[index].area.size(); i++){
				_itoa_s (carArchive[index].area[i],temp,15,10);
				if(i != carArchive[index].area.size()-1){
					strncat_s (temp, 15, " ",1);
					strncat_s (value11, 1000, temp,15);
				} else {
					strncat_s (temp,15,"'",1);
					strncat_s(value11,1000,temp,15);
				}
			}

			// Save image locally & save directory
			strcpy_s (value12, 1000,saveImgTo);
			strncat_s (value12, 1000, "exp",15);
			strncat_s (value12, 1000,value3,15);
			strncat_s (value12, 1000,"_",15);
			strncat_s (value12, 1000,"id",15);
			strncat_s (value12, 1000,value4,15);
			strncat_s (value12, 1000,".jpg",15);
			String value12_S = String(value12);
			imwrite(value12_S, carArchive[index].img );

			/* Create SQL statement */
			strcpy_s(sql, 10000,"INSERT INTO CarTable_exp");
			strncat_s(sql, 10000,value3,15);
			strncat_s(sql, 10000,"(Video,Date,SecInVid,expID,CarID,Count,Frames,ULX,ULY,LRX,LRY,Area,Path) VALUES (",100);
			strncat_s(sql, 10000,value0,100);
			strncat_s(sql, 10000,",",1);
			strncat_s(sql, 10000,value1,100);
			strncat_s(sql, 10000,",",1);
			strncat_s(sql, 10000,value2,15);
			strncat_s(sql, 10000,",",1);
			strncat_s(sql, 10000,value3,15);
			strncat_s(sql, 10000,",",1);
			strncat_s(sql, 10000,value4,15);
			strncat_s(sql, 10000,",",1);
			strncat_s(sql, 10000,value5,15);
			strncat_s(sql, 10000,",",1);
			strncat_s(sql, 10000,value6,1000);
			strncat_s(sql, 10000,",",1);
			strncat_s(sql, 10000,value7,1000);
			strncat_s(sql, 10000,",",1);
			strncat_s(sql, 10000,value8,1000);
			strncat_s(sql, 10000,",",1);
			strncat_s(sql, 10000,value9,1000);
			strncat_s(sql, 10000,",",1);
			strncat_s(sql, 10000,value10,1000);
			strncat_s(sql, 10000,",",1);
			strncat_s(sql, 10000,value11,1000);
			strncat_s(sql, 10000,",",1);
			strncat_s(sql, 10000,"'",1);
			strncat_s(sql, 10000,value12,100);
			strncat_s(sql, 10000,"'",1);
			strncat_s(sql, 10000,");",3);

			/* Execute SQL statement */
			bool continueTrying = true;
			while(continueTrying){
				rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
				switch(rc){
				case SQLITE_BUSY:
					sqlite3_sleep(10);
					break;
				case SQLITE_OK:
					continueTrying = false;
					break;
				}
			}
			if( rc != SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n",zErrMsg);
				sqlite3_free(zErrMsg);
			} else {
				fprintf(stdout, "Records created successfully\n");
				fprintf(stdout, "----------------------------\n");
				fprintf(stdout, "Record idx:%d\n",index);
				fprintf(stdout, "Record Video:%s\n",value0);
				fprintf(stdout, "Record Video Date:%s\n",value1);
				fprintf(stdout, "Record Seconds into Video:%s\n",value2);
				fprintf(stdout, "Record EID:%s\n",value3);
				fprintf(stdout, "Record CID:%s\n",value4);
				fprintf(stdout, "Record count:%s\n",value5);
				fprintf(stdout, "Record frames:%s\n",value6);
				fprintf(stdout, "Record ULX:%s\n",value7);
				fprintf(stdout, "Record ULY:%s\n",value8);
				fprintf(stdout, "Record LRX:%s\n",value9);
				fprintf(stdout, "Record LRY:%s\n",value10);
				fprintf(stdout, "Record Area:%s\n",value11);
				fprintf(stdout, "Path of image:%s\n",value12);
			}
			index++;
		}
	}	
}

void carCounter::detectCarBoxForNight(cv::Mat &subtract,cv::Rect &boundRect,int minObjectSize){
	cv::vector<cv::vector<cv::Point>> endRegionContours;
	cv::vector<cv::Vec4i> endRegionHierarchy;
					
	findContours(subtract, endRegionContours, endRegionHierarchy, CV_RETR_EXTERNAL,  CV_CHAIN_APPROX_NONE, cv::Point(0, 0));
	cv::vector<cv::vector<cv::Point>> endRegionContoursPoly(endRegionContours.size());
	cv::vector<cv::Rect> endRegionboundRect(endRegionContours.size());
	cv::vector<cv::Point2f> endRegionCenter(endRegionContours.size());
	cv::vector<float> endRegionRadius(endRegionContours.size());
	int j = obtainBoundBoxes(endRegionContours,endRegionContoursPoly,endRegionboundRect,endRegionCenter,endRegionRadius,2000);
	int xUL = std::numeric_limits<int>::max();
	int yUL = std::numeric_limits<int>::max();
	int xLR = 0;
	int	yLR = 0;
	for( int i = 0; i<endRegionboundRect.size(); i=endRegionHierarchy[i][0]){
		if(endRegionboundRect[i].x < xUL && endRegionboundRect[i].x != 0)
			xUL = endRegionboundRect[i].x;
		if(endRegionboundRect[i].y < yUL && endRegionboundRect[i].y != 0)
			yUL = endRegionboundRect[i].y;
		if(endRegionboundRect[i].x+endRegionboundRect[i].width > xLR)
			xLR = endRegionboundRect[i].x +endRegionboundRect[i].width;
		if(endRegionboundRect[i].y+endRegionboundRect[i].height > yLR)
			yLR = endRegionboundRect[i].y+endRegionboundRect[i].height;
	}

	boundRect = Rect(cv::Point(xUL,yUL),cv::Point(xLR,yLR));
/*  test code
	cv::rectangle(frame,cv::Point(xUL,yUL),cv::Point(xLR,yLR),CV_RGB(255,0,255),2,8,0);
	for( int i = 0; i<endRegionboundRect.size(); i=endRegionHierarchy[i][0] ){
		frame.copyTo(test);
		cv::putText(frame, std::to_string(i), cv::Point(endRegionboundRect[i].x,endRegionboundRect[i].y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255,255,0), 2.0);
		cv::rectangle(frame,endRegionboundRect[i].tl(),endRegionboundRect[i].br(),CV_RGB(255,255,0),2,8,0);
	}
	imshow("Test",test);
*/
}

void carCounter::removeShadow(cv::Mat &subtract,cv::Mat3b &subtractrgb){
	subtractrgb = subtract;
	for (Mat3b::iterator it =subtractrgb.begin(); it != subtractrgb.end(); it++) {
		if(*it != Vec3b(255, 255, 255)) {
			*it = Vec3b(0, 0, 0);
		}
	}	
}

void carCounter::cleanNoise(cv::Mat &mask, cv::Mat &extraMask,cv::Mat3b &maskrgb, cv::Mat3b &extraMaskrgb){
	cv::erode(mask,mask,cv::Mat(),Point(-1, -1), 1, 1, 1);
	cv::dilate(mask,mask,cv::Mat(),Point(-1, -1), 3, 1, 1);
	cv::medianBlur(mask,mask,21);
	removeShadow(mask,maskrgb);
			
	cv::resize(maskrgb, maskrgb, cv::Size(frame.size().width, frame.size().height)); 
	cv::cvtColor(maskrgb,mask,CV_RGB2GRAY);
			
	if(extraMask.dims > 0){
		cv::erode(extraMask,extraMask,cv::Mat(),Point(-1, -1), 1, 1, 1);
		cv::dilate(extraMask,extraMask,cv::Mat(),Point(-1, -1), 3, 1, 1);
		cv::medianBlur(extraMask,extraMask,21);
		removeShadow(extraMask,extraMaskrgb);
			
		cv::resize(extraMaskrgb, extraMaskrgb, cv::Size(frame.size().width, frame.size().height)); 
		cv::cvtColor(extraMaskrgb,extraMask,CV_RGB2GRAY);
	}
}

int carCounter::obtainBoundBoxes(cv::vector<cv::vector<cv::Point>> &contours,cv::vector<cv::vector<cv::Point>> &contours_poly, 
								  cv::vector<cv::Rect> &boundRect,
								  cv::vector<cv::Point2f> &center,cv::vector<float> &radius, int minObjectSize){
	int j = 0;
	for(int i = 0; i < contours.size(); i++){ 
		approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
		cv::minEnclosingCircle((Mat)contours_poly[i], center[j], radius[j]);
		if(cv::boundingRect(cv::Mat(contours_poly[i])).area() > minObjectSize){
			boundRect[j] = cv::boundingRect(cv::Mat(contours_poly[i]));
			contours[j] = contours[i];
			cv::minEnclosingCircle((Mat)contours_poly[i], center[j], radius[j]);		
			j++;
		}
	}
	return j;
}

void carCounter::createFrame(Frame &data, cv::vector<cv::vector<cv::Point>> &contours,
							  cv::vector<cv::vector<cv::Point>> &contours_poly,
							  cv::vector<cv::Rect> &boundRect,
							  cv::vector<cv::Point2f> &center,
						      cv::vector<float> &radius, int minObjectSize){
	
	int j = obtainBoundBoxes(contours,contours_poly,boundRect,center,radius,minObjectSize);
	
	contours.resize(j);
	boundRect.resize(j);
	center.resize(j);
	radius.resize(j);

	data.contour   = std::vector<cv::vector<cv::Point>>(contours);
	data.boundRect = std::vector<cv::Rect>(boundRect);
	data.center    = std::vector<cv::Point2f>(center);
	data.radius    = std::vector<float>(radius);

	data.speed     = std::vector<double>(j);
	data.speedX     = std::vector<double>(j);
	data.speedY     = std::vector<double>(j);
	data.start_loc = std::vector<Point2f>(j);
	data.velocity  = std::vector<double>(j);
	data.velocityX  = std::vector<double>(j);
	data.velocityY  = std::vector<double>(j);
	data.distance  = std::vector<double>(j);
	data.distanceX  = std::vector<double>(j);
	data.distanceY  = std::vector<double>(j);
	data.time      = std::vector<int>(j);
	data.id        = std::vector<int>(j);
	data.tIdx      = std::vector<int>(j);
	data.p         = std::vector<bool>(j);
	data.checked   = std::vector<bool>(j);
	data.pTraj     = std::vector<std::vector<cv::Point2f>>(j);
}

vector<Mat> carCounter::calcHistogram(Mat& img,Mat& mask){
	int bins = 256;             // number of bins
	int nc = img.channels();    // number of channels

	vector<Mat> hist(nc);       // histogram arrays

	// Initalize histogram arrays
	for (int i = 0; i < hist.size(); i++)
		hist[i] = Mat::zeros(1, bins, CV_32SC1);

	// Calculate the histogram of the image
	for (int i = 0; i < img.rows; i++){
		for (int j = 0; j < img.cols; j++){
			for (int k = 0; k < nc; k++){
				if(mask.at<Vec3b>(i,j) == Vec3b(255, 255, 255)){
					uchar val = nc == 1 ? img.at<uchar>(i,j) : img.at<Vec3b>(i,j)[k];
					if(val != 0)
						hist[k].at<int>(val) += 1;
				}
			}
		}
	}

	// For each histogram arrays, obtain the maximum (peak) value
	// Needed to normalize the display later
	int hmax[3] = {0,0,0};
	for (int i = 0; i < nc; i++){
		for (int j = 0; j < bins-1; j++)
			hmax[i] = hist[i].at<int>(j) > hmax[i] ? hist[i].at<int>(j) : hmax[i];
	}

	const char* wname[3] = { "blue", "green", "red" };
	Scalar colors[3] = { Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255) };

	vector<Mat> canvas(nc);
	return hist;
}

double carCounter::findStd(vector<Mat> &hist, int channel){
	int bins = 255;
	double total = 0;
	double avg = 0;
	double var = 0;
	double deviation = 0;
	for(int i = 0; i < bins-1; i++){
		total=total+hist[channel].at<int>(i);
	}

	for(int i = 0; i < bins-1; i++){
		avg=avg+(hist[channel].at<int>(i)/total)*i;
	}

	for(int i = 0; i < bins-1; i++){
		var=var+(hist[channel].at<int>(i)/total)*pow((i-avg),2.0);
	}

	deviation = pow(var,0.5);
	return deviation;
}

double carCounter::histogramCalcAndFindStd(cv::Mat &frame,cv::Mat &background,cv::Mat& subtract,std::deque<Frame> &buffer, int boundBoxNum){
		
	cv::Mat croppedFrame;
	cv::Mat croppedSubtract;
	cv::Mat croppedBackground;
	cv::Mat croppedDifference;

	frame(buffer[0].boundRect[boundBoxNum]).copyTo(croppedFrame);
	background(buffer[0].boundRect[boundBoxNum]).copyTo(croppedBackground);
	subtract(buffer[0].boundRect[boundBoxNum]).copyTo(croppedSubtract);

	croppedDifference = croppedFrame - croppedBackground;

	vector<Mat> hist = calcHistogram(croppedDifference,croppedSubtract);

	double std = findStd(hist,0);
	
	return std;
}

bool carCounter::safetyChecks(cv::Mat &frame,cv::Mat &background,cv::Mat &subtract,std::deque<Frame> &buffer,int boundBoxNum, double thresh){
	double std = histogramCalcAndFindStd(frame,background,subtract,buffer,boundBoxNum);
	if (std < thresh && !nightTime)
		return false;
	else
		return true;
}

// region monitor functions
void carCounter::startRegionMonitor(cv::Mat &frame, cv::Mat &background, cv::Mat &subtract,
									std::deque<Frame> &buffer,std::deque<Object> &transit,
									cv::vector<cv::Point> startRegion,int horizontalBandWidth){
	std::deque<Object> temp;
	for(int i = 0; i < buffer[0].p.size(); i++){
		bool safetyPass = safetyChecks(frame,background,subtract,buffer,i,10);
		if(pointPolygonTest(startRegion, buffer[0].center[i], false) == 1 
		   && safetyPass == true){
			
			if(!buffer[0].checked[i]){
				buffer[0].start_loc[i] = buffer[0].center[i];
				buffer[0].time[i] = 0;
			}

			Object obj;

			obj.last_seen = frame_counter;
			obj.id = buffer[0].id[i];
			obj.tIdx = buffer[0].tIdx[i];
			obj.checked = buffer[0].checked[i];
			obj.start_loc = buffer[0].center[i];
			obj.center_when_last_seen = buffer[0].center[i];
			obj.velocity_when_last_seen = buffer[0].velocity[i];
			obj.velocityX_when_last_seen = buffer[0].velocityX[i];
			obj.velocityY_when_last_seen = buffer[0].velocityY[i];
			obj.speed_when_last_seen = buffer[0].speed[i];
			obj.speedX_when_last_seen = buffer[0].speedX[i];
			obj.speedY_when_last_seen = buffer[0].speedY[i];
			obj.distance_when_last_seen = buffer[0].distance[i];
			obj.distanceX_when_last_seen = buffer[0].distanceX[i];
			obj.distanceY_when_last_seen = buffer[0].distanceY[i];
			obj.time_when_last_seen = buffer[0].time[i];
			obj.contour_before_disappearing = buffer[0].contour[i];
			obj.boundbox_before_disappearing = buffer[0].boundRect[i];

			/* First time object is seen so no path can be predicted yet based
			 * on it's previous trajectory. For correctness sake we place a
			 * "trajectory" that only indicates current positon of the object.  */

			std::vector<cv::Point2f> aVector;
			aVector.push_back(obj.start_loc);
			obj.trajectories_before_disappearing.push_back(aVector);

			obj.area.push_back(buffer[0].boundRect[i].area());
			obj.trajectory.push_back(buffer[0].center[i]);
			obj.frames.push_back(frame_counter);
			obj.bIdx = i;
		
			temp.push_front(obj);
		}
	}
	if(nightTime){
		for(int x = temp.size()-1; x > -1; x--){
			for(int y = temp.size()-1; y > -1; y--){
				if(x != y && 
					temp.at(x).center_when_last_seen.y < temp.at(y).center_when_last_seen.y+horizontalBandWidth &&
					temp.at(x).center_when_last_seen.y > temp.at(y).center_when_last_seen.y-horizontalBandWidth &&
				!temp.at(x).checked){
					if(temp.at(y).checked && !temp.at(y).hasSibling){

						temp.at(x).checked = true;
						temp.at(x).headlight = true;
						temp.at(x).hasSibling = true;
						temp.at(x).siblingID = temp.at(y).id;
						temp.at(x).id = object_counter+1;
						
						buffer[0].id[temp.at(x).bIdx] = temp.at(x).id;
						buffer[0].checked[temp.at(x).bIdx] = temp.at(x).checked;
						transit.push_front(temp.at(x));
						
						for(int i = 0; i < transit.size(); i++)
							transit[i].tIdx = i;

						temp.at(y).hasSibling = true;
						temp.at(y).siblingID = temp.at(x).id;

						transit[temp.at(y).tIdx+1].hasSibling = temp.at(y).hasSibling;
						transit[temp.at(y).tIdx+1].siblingID = temp.at(y).siblingID;

						object_counter++;

					} else if(!temp.at(y).checked && !temp.at(y).hasSibling) {
						temp.at(x).checked = true;
						temp.at(x).headlight = true;
						temp.at(x).hasSibling = true;
						temp.at(x).id = object_counter+1;
						buffer[0].id[temp.at(x).bIdx] = temp.at(x).id;
						buffer[0].checked[temp.at(x).bIdx] = temp.at(x).checked;
						object_counter++;
					
						temp.at(y).checked = true;
						temp.at(y).headlight = true;
						temp.at(y).hasSibling = true;
						temp.at(y).id = object_counter+1;
						buffer[0].id[temp.at(y).bIdx] = temp.at(y).id;
						buffer[0].checked[temp.at(x).bIdx] = temp.at(x).checked;
						object_counter++;

						temp.at(x).siblingID = temp.at(y).id;
						temp.at(y).siblingID = temp.at(x).id;

						transit.push_front(temp.at(x));
						transit.push_front(temp.at(y));

						for(int i = 0; i < transit.size(); i++)
							transit[i].tIdx = i;
					}
				}
			}
			if(!temp.at(x).checked){
				temp.at(x).checked = true;
				temp.at(x).headlight = true;
				temp.at(x).hasSibling = false;
				temp.at(x).id = object_counter+1;
				
				buffer[0].id[temp.at(x).bIdx] = temp.at(x).id;
				buffer[0].checked[temp.at(x).bIdx] = temp.at(x).checked;
				transit.push_front(temp.at(x));
				for(int i = 0; i < transit.size(); i++)
					transit[i].tIdx = i;
				
				object_counter++;
			}
		}
	} else {
		for(int x = temp.size()-1; x > -1; x--){
			if(!temp.at(x).checked){
				temp.at(x).checked = true;
				temp.at(x).headlight = false;
				temp.at(x).hasSibling = false;
				temp.at(x).id = object_counter+1;
				
				buffer[0].id[temp.at(x).bIdx] = temp.at(x).id;
				buffer[0].checked[temp.at(x).bIdx] = temp.at(x).checked;
				transit.push_front(temp.at(x));
				for(int i = 0; i < transit.size(); i++)
					transit[i].tIdx = i;

				object_counter++;
			}
		}
	}
}

void carCounter::endRegionMonitor(cv::Mat &frame,cv::Mat &background,cv::Mat &subtract,cv::Mat &subtract2, int minObjectSize,
								  int &car_counter,
								  std::deque<Frame> &buffer,std::deque<Object> &transit, std::deque<Object> &carArchive,
								  cv::vector<cv::Point> endRegion){
	
	for(int i = 0; i < buffer[0].center.size(); i++){
		bool safetyPass = safetyChecks(frame,background,subtract,buffer, i,10);
		if(pointPolygonTest(endRegion, buffer[0].center[i], false) == 1 && safetyPass == true){
			int sib = NULL;
			bool found = false;
			for(int j = 0; j < transit.size(); j++){
				if(buffer[0].id[i] == transit[j].id){
					found = true;
					if(nightTime && transit[j].hasSibling)
						sib = transit[j].siblingID;
				}
			}
			if(found == true){
				while(buffer[0].id[i] != transit[transit.size()-1].id)
					transit.pop_back();
				
				if(buffer[0].id[i] == transit[transit.size()-1].id){
					car_counter++;
					if(!nightTime){
						cv::Mat croppedBox;
						frame(buffer[0].boundRect[i]).copyTo(croppedBox);
						transit[transit.size()-1].img = croppedBox;
						transit[transit.size()-1].count = car_counter;
						carArchive.push_back(transit[transit.size()-1]);
					} else {
						detectCarBoxForNight(subtract2,buffer[0].boundRect[i],minObjectSize);
						cv::Mat croppedBox;
						frame(buffer[0].boundRect[i]).copyTo(croppedBox);
						transit[transit.size()-1].img = croppedBox;
						transit[transit.size()-1].count = car_counter;
						carArchive.push_back(transit[transit.size()-1]);
					}
					transit.pop_back();
				}
				if(nightTime && sib != NULL){
					while(sib != transit[transit.size()-1].id && transit.size() != 0)
						transit.pop_back();

					if(sib == transit[transit.size()-1].id)
						transit.pop_back();
				}
			}
		}
	}
}


void carCounter::checkMissing(int &frame_counter,std::deque<Frame> &buffer,std::deque<Object> &transit){
	// Keep track of missing objects currently in transit.
	for(int i = 0; i < transit.size(); i++)
	{
		bool found = false;
		for(int j = 0; j < buffer[0].id.size(); j++){
			if(buffer[0].id[j] == transit[i].id){
				found = true;
				transit[i].miss = false;
				transit[i].last_seen = frame_counter;
				transit[i].trajectories_before_disappearing.push_back(buffer[0].pTraj[j]);
				transit[i].time_when_last_seen =buffer[0].time[j];
				transit[i].center_when_last_seen = buffer[0].center[j];
				transit[i].velocity_when_last_seen = buffer[0].velocity[j];
				transit[i].velocityX_when_last_seen = buffer[0].velocityX[j];
				transit[i].velocityY_when_last_seen = buffer[0].velocityY[j];
				transit[i].speed_when_last_seen = buffer[0].speed[j];
				transit[i].speedX_when_last_seen = buffer[0].speedX[j];
				transit[i].speedY_when_last_seen = buffer[0].speedY[j];
				transit[i].distance_when_last_seen = buffer[0].distance[j];
				transit[i].distanceX_when_last_seen = buffer[0].distanceX[j];
				transit[i].distanceY_when_last_seen = buffer[0].distanceY[j];
				transit[i].contour_before_disappearing = buffer[0].contour[j];
				transit[i].boundbox_before_disappearing = buffer[0].boundRect[j];
				
				transit[i].area.push_back(buffer[0].boundRect[j].area());
				transit[i].trajectory.push_back(buffer[0].center[i]);
				transit[i].frames.push_back(frame_counter);
				transit[i].ulx.push_back(buffer[0].boundRect[j].x);
				transit[i].uly.push_back(buffer[0].boundRect[j].y);
				transit[i].lrx.push_back(buffer[0].boundRect[j].x+buffer[0].boundRect[j].width);
				transit[i].lry.push_back(buffer[0].boundRect[j].y+buffer[0].boundRect[j].height);
				break;
			}
		}
		if(!found){
			transit[i].miss = true;
		}
	}
}

// Persistency checking using Transit Ledger.
void carCounter::checkTransitLedgerUsingMomentAndCenter(int &frame_counter,std::deque<Frame> &buffer,std::deque<Object> &transit, int expectedDist)
{
	for(int i = 0; i < buffer[0].boundRect.size(); i++){
		if(buffer[0].p[i] != true){
			/* The loop below this comment is what controls how we examine the transit ledger.
			   We can either search front to end or end to front.*/
			for(int j = transit.size()-1; j > -1; j--){
				/* For Future: Matching is currently disabled. Need to figure out how to choose threshold for similarity.
				   Another direction to consider is matching over RGB image rather than contour. cv::MatchShape(...) > ???value???*/
				if(cv::matchShapes(buffer[0].contour[i], transit[j].contour_before_disappearing,CV_CONTOURS_MATCH_I2,0) || nightTime){
					for(int t = transit[j].trajectories_before_disappearing.size()-1; t >= 0; t--){
						for(int m = transit[j].trajectories_before_disappearing[t].size()-1; m >= 0; m--){
							if(((buffer[0].center[i].x > transit[j].trajectories_before_disappearing[t].at(m).x-expectedDist) 
								&& (transit[j].trajectories_before_disappearing[t].at(m).x+expectedDist > buffer[0].center[i].x)) 
							&& ((buffer[0].center[i].y > transit[j].trajectories_before_disappearing[t].at(m).y-expectedDist) 
								&& (transit[j].trajectories_before_disappearing[t].at(m).y+expectedDist > buffer[0].center[i].y))){
								
								buffer[0].start_loc[i] = transit[j].start_loc;
								buffer[0].id[i] = transit[j].id;

								int n_time = transit[j].time_when_last_seen + (frame_counter - transit[j].last_seen);
								buffer[0].time[i] = n_time;

								// Instantanous Metrics
								buffer[0].velocity[i] = sqrt(pow(buffer[0].center[i].x - transit[j].center_when_last_seen.x,2.0) 
								+ pow(buffer[0].center[i].y - transit[j].center_when_last_seen.y,2.0))/(frame_counter - transit[j].last_seen);

								buffer[0].velocityX[i] = (buffer[0].center[i].x - transit[j].center_when_last_seen.x)/(frame_counter - transit[j].last_seen);
								buffer[0].velocityY[i] = (buffer[0].center[i].y - transit[j].center_when_last_seen.y)/(frame_counter - transit[j].last_seen);

								// Average Metrics
								buffer[0].distance[i] = sqrt(pow(buffer[0].center[i].x - transit[j].start_loc.x,2.0) 
								+ pow(buffer[0].center[i].y - transit[j].start_loc.y,2.0));
								
								buffer[0].distanceX[i] = buffer[0].center[i].x-transit[j].start_loc.x;
								buffer[0].distanceY[i] = buffer[0].center[i].y-transit[j].start_loc.y;

								buffer[0].speed[i] = buffer[0].distance[i]/buffer[0].time[i];
								buffer[0].speedX[i] = buffer[0].distanceX[i]/buffer[0].time[i];
								buffer[0].speedY[i] = buffer[0].distanceY[i]/buffer[0].time[i];

								// Transit Ledger index
								buffer[0].tIdx[i] = transit[j].tIdx;

								// Checked Flag
								buffer[0].checked[i] = transit[j].checked;

								// Presistance Flag
								buffer[0].p[i] = true;

								int k = 0;
								// Fill predicted trajectory vector
								while(k < 30) {
									Point2f fp(buffer[0].center[i].x+(buffer[0].speedX[i])*k, buffer[0].center[i].y+(buffer[0].speedY[i])*k);
									buffer[0].pTraj[i].push_back(fp);
									k++;
								}
								break;
							}
						}
						if(buffer[0].p[i] == true)
							break;
					}
					if(buffer[0].p[i] == true)
						break;
				}
			}
		}
	}
}


void carCounter::persistenceCheck(int &frame_counter,int type,std::deque<Frame> &buffer, std::deque<Object> &transit,int expectedDist){
	if(type == 0) 
		checkTransitLedgerUsingMomentAndCenter(frame_counter,buffer,transit,expectedDist);
	else 
		assert(type < 1 && type >= 0);
	
	// Additional methods or combination of methods can be used to check for persistence between frames.
}

void carCounter::selectBackgroundSubtraction(bool useMOG2, bool nightTime,
											 cv::Mat &resizeF,cv::Mat &mask,
											 int nmixtures, double backgroundratio, bool detectShadows){
	if(!useMOG2 && !nightTime){
		bgsAdaptiveMedian->process(resizeF, mask, background);
	}else if(useMOG2 && !nightTime){ 
		MOG2.set("nmixtures", nmixtures);
		MOG2.set("backgroundRatio", backgroundratio);
		MOG2.set("detectShadows", detectShadows);
		MOG2(resizeF, mask,.005);
		MOG2.getBackgroundImage(background);
	}else{
		// Nighttime case, switch to Frame Difference.
		bgsFrameDifference->process(resizeF, mask, background);
	}
}

void carCounter::drawResult(int &car_counter, int &object_counter, cv::Mat &frame, cv::vector<cv::Vec4i> &hierarchy, std::deque<Frame> &buffer,std::deque<Object> &transit,
							bool showLastCar, bool boundBoxesOn, bool predictionOn, bool latestPathsOn, bool displayTransitLedger, bool displayFocusRegions, int showPathofId,
							cv::vector<cv::Point> startRegion,cv::vector<cv::Point> endRegion)
{
	// draw bounding boxes.
	if(boundBoxesOn){
		for( int i = 0; i<buffer[0].boundRect.size(); i=hierarchy[i][0] ){
			if(buffer[0].id[i] > 0){
				cv::Scalar color = cv::Scalar(255,0,0);
				cv::rectangle(frame,buffer[0].boundRect[i].tl(),buffer[0].boundRect[i].br(),color,2,8,0);
				cv::putText(frame, std::to_string(i), cv::Point(buffer[0].boundRect[i].x,buffer[0].boundRect[i].y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
				cv::putText(frame, "gid="+std::to_string(buffer[0].id[i])+
														"/d="+std::to_string(buffer[0].distance[i])+
														"/t="+std::to_string(buffer[0].time[i])+
														"/s="+std::to_string(buffer[0].speed[i])+
					"/c=("+std::to_string(buffer[0].start_loc[i].x)+
					+","+std::to_string(buffer[0].start_loc[i].y)+")"
					,cv::Point(buffer[0].center[i].x,buffer[0].center[i].y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255,0,0), 2.0);
			}else{
				cv::Scalar color = cv::Scalar(255,0,255);
				cv::rectangle(frame,buffer[0].boundRect[i].tl(),buffer[0].boundRect[i].br(),color,2,8,0);
				cv::putText(frame, std::to_string(i), cv::Point(buffer[0].boundRect[i].x,buffer[0].boundRect[i].y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
				cv::putText(frame, "gid="+std::to_string(buffer[0].id[i])+
														"/d="+std::to_string(buffer[0].distance[i])+
														"/t="+std::to_string(buffer[0].time[i])+
														"/s="+std::to_string(buffer[0].speed[i])+
					"/c=("+std::to_string(buffer[0].start_loc[i].x)+
					+","+std::to_string(buffer[0].start_loc[i].y)+")"
					,cv::Point(buffer[0].center[i].x,buffer[0].center[i].y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255,0,0), 2.0);
			}
		}
	}
	// draw predicted trajectory.
	if(predictionOn){
		for( int i = 0; i<buffer[0].boundRect.size(); i=hierarchy[i][0] ){
			if(buffer[0].id[i] > 0){
				cv::Scalar color = cv::Scalar(255,0,255);
				if( buffer[0].pTraj[i].size() > 2){
					for(int j = 0; j < buffer[0].pTraj[i].size()-1; j++){
						line(frame, buffer[0].pTraj[i].at(j), buffer[0].pTraj[i].at(j+1), color, 20, 8, 0);	
					}
				}
				for(int j = 0; j < buffer[0].pTraj[i].size(); j++){
					circle(frame, buffer[0].pTraj[i].at(j), 1, cv::Scalar(255,255,255), 3, 8, 0);	
				}
			}
		}
	}
	// draw recent trajectories.
	if(latestPathsOn){
		for(int i = 0; i<buffer.size(); i++){
			for( int j = 0; j<buffer[i].boundRect.size(); j=hierarchy[j][0] ){
				if(buffer[i].id[j] > 0){
					cv::Scalar color = cv::Scalar(255,0,0);
					circle(frame, buffer[i].center[j], 1, color, 3, 8, 0);
				}
			}
		}
	}

	// draw older trajectories from archive.
	if(showPathofId != SHOW_PATH_OF_CAR_X_OFF){
		int display_path_of_id = showPathofId;
		if(carArchive.size() > display_path_of_id){
			for( int j = 0; j<carArchive[display_path_of_id].trajectory.size(); j++ ){
				cv::Scalar color = cv::Scalar(0,0,255);
				circle(frame, carArchive[display_path_of_id].trajectory[j], 1, color, 3, 8, 0);
			}
		}
	}

	// display the last discovered car in a box.
	if(showLastCar && carArchive.size() != 0){
		String s = "Last_Car_";
		std::string r = s + std::to_string(id);
		imshow(r,carArchive[carArchive.size()-1].img);   
	}

	// transit ledger.
	if(displayTransitLedger){
		putText(frame, "object counter="+std::to_string(object_counter), 
			cv::Point(25,25), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255,0,0), 2.0);

		putText(frame, "car counter="+std::to_string(car_counter), 
			cv::Point(25,45), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255,0,0), 2.0);

		putText(frame, "In Transit: ", 
			cv::Point(25,65), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255,0,0), 2.0);

		for(int i = 0; i < transit.size(); i++)
			putText(frame, std::to_string(transit[i].id) +" | "+std::to_string(transit[i].miss), 
			cv::Point(25,85+i*20), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255,0,0), 2.0);
	}

	//display start and end regions
	if(displayFocusRegions){
		cv::vector<cv::vector<cv::Point>> regions;
		regions.push_back(startRegion);
		regions.push_back(endRegion);
		drawContours(frame, regions, -1, CV_RGB(255,0,0), 1, 8);
	}
}

// PUBLIC METHODS

/* constructor */
carCounter::carCounter(int expID,bool night) {
	id = expID;
	nightTime = night;
	object_counter = 0;
	frame_counter = 0;
	car_counter = 0;
}

/* make the counter start counting */
void carCounter::run(int bufferSize, int minObjectSizeDay,int minObjectSizeNight, int skip, int learningTime, 
		 char fileName[100], char saveImgTo[200], char dataBase[200],  
		 int fps, 
		 int expectedDist, int horizontalBandWidth,
		 bool online, const cv::Point* ppt, const cv::Point* ppt2,int* npt,int* npt2,
		 bool useMOG2, int nmixtures, double backgroundratio, bool detectShadows,
		 bool showLastCar, bool boundBoxesOn, bool predictionOn, bool latestPathsOn, 
		 bool displayTransitLedger, bool displayFocusRegions, int showPathofId,
		 int displayType,
		 cv::vector<cv::Point> startRegion,cv::vector<cv::Point> endRegion){

	// 0 is the id of video device.0 if you have only one camera.	
	cv::VideoCapture stream(fileName);  
	bgsAdaptiveMedian = new DPAdaptiveMedianBGS;
	bgsFrameDifference = new FrameDifferenceBGS;

	openDB(id,dataBase);

	// Background archiving process.
	std::thread DBthread(&carCounter::exportCarsToDB,*this,id,fileName,saveImgTo,std::ref(carArchive),fps);
	DBthread.detach();
	
	// Unconditional loop (keeps grabbing frames from video).     
	while (true) {     
		frame_counter++;
		if(frame_counter % skip == 0){
			// OBTAIN FRAME
			if(!(stream.read(frame)))
				break;

			// create windows of correct size.
			cv::resize(frame, resizeF, cv::Size(frame.size().width, frame.size().height));

			// set focal overlays.
			fillPoly(resizeF,&ppt,npt,1,Scalar( 0, 0, 0 ),8);
			fillPoly(resizeF,&ppt2,npt2,1,Scalar( 0, 0, 0 ),8);

			// BACKGROUND SUBTRACTION
			selectBackgroundSubtraction(useMOG2,nightTime,resizeF,mask,nmixtures,backgroundratio,detectShadows);
			mask.copyTo(extraMask); // if daytime extraMask and mask will store background subracted foreground, 
									// if nighttime mask will store threshold foreground
									// extraMask will store the background subtracted foreground.

			// THRESHOLDING
			if(nightTime){
				cv::cvtColor(resizeF,resizeFgray,COLOR_RGB2GRAY);
				threshold(resizeFgray,mask,240.0,255.0,THRESH_BINARY);
			}
			
			// CLEAN MASK
			// clear out noise using erode and dilate, also remove shadows if they are detected.
			cleanNoise(mask,extraMask,maskrgb,extraMaskrgb);

			if(frame_counter > learningTime){
				// OBJECT DETECTION
				findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL,  CV_CHAIN_APPROX_NONE, cv::Point(0, 0));
				
				// Approximate contours to polygons + get bounding rects and circles.
				cv::vector<cv::vector<cv::Point>> contours_poly(contours.size());
				cv::vector<cv::Rect> boundRect(contours.size());
				cv::vector<cv::Point2f> center(contours.size());
				cv::vector<float> radius(contours.size());

				Frame data;
				int minObjectSize;
				if(nightTime)
					minObjectSize = minObjectSizeNight;
				else
					minObjectSize = minObjectSizeDay;

				// initialize the frame object data.
				carCounter::createFrame(data, contours,
					contours_poly,
					boundRect,
					center,
					radius, minObjectSize);

				// OBJECT TRACKING

				// buffer frame data (last X frames are placed in a queue).
				// buffer flood.
				if(buffer.size() < bufferSize){
					buffer.push_front(data);		
					// persistence checks only make sense if you have more than one frame.
					if(buffer.size() > 1){
						persistenceCheck(frame_counter,0,buffer,transit,expectedDist);
						startRegionMonitor(frame,background,maskrgb,buffer,transit,startRegion,horizontalBandWidth);		
						checkMissing(frame_counter,buffer,transit);
						endRegionMonitor(frame,background,maskrgb,extraMask,minObjectSizeDay,car_counter,buffer,transit,carArchive,endRegion);
						frameArchive.push_front(buffer[0]);
					}
				}

				// full buffer.
				else{
					buffer.push_front(data);
					buffer.pop_back();

					persistenceCheck(frame_counter,0,buffer,transit,expectedDist);
					startRegionMonitor(frame,background,maskrgb,buffer,transit,startRegion,horizontalBandWidth);
					checkMissing(frame_counter,buffer,transit);
					endRegionMonitor(frame,background,maskrgb,extraMask,minObjectSizeDay,car_counter,buffer,transit,carArchive,endRegion);
					frameArchive.push_back(buffer[0]);
				}

				// VISUALIZATION/CMD OUTPUT

				// if online mode is on a window is produced otherwise runs through console with no visualization.
				if(online){
					if(displayType == USE_BACKGROUND)
						background.copyTo(display);
					
					else if(displayType == USE_ORGINAL_FRAME)
						frame.copyTo(display);
					
					else if(displayType == USE_RESIZED_WITH_OVERLAYS_FRAME)
						resizeF.copyTo(display);
					
					else if(displayType == USE_SUBTRACTED)
						maskrgb.copyTo(display);
					
					drawResult(car_counter,object_counter,display,hierarchy,buffer,transit,
						showLastCar,boundBoxesOn, predictionOn, latestPathsOn, 
						displayTransitLedger, displayFocusRegions, showPathofId, 
						startRegion,endRegion);

					// display original with bounding boxes.
					String s = "Display_";
					std::string r = s + std::to_string(id);
					imshow(r, display);
				}
			}

			if (cv::waitKey(30) >= 0)     
				break;
		}
	}
	closeDB();
}
