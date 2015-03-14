#include "carCounter.h"

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

void carCounter::openDB(){

	/* Open database */
	rc = sqlite3_open("car_count_db", &db);
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stderr, "Opened database successfully\n");
	}
}

void carCounter::closeDB(){
	sqlite3_close(db);
}

void carCounter::exportCarsToDB(int expID, std::deque<Object> &carArchive) {
	
	int index = 0;
	char *zErrMsg = 0;
	char * sql;
	sql = new char[5000];

	char * value1;
	value1 = new char[15];

	char * value2;
	value2 = new char[15];

	char * value3;
	value3 = new char[1000];

	char * value4;
	value4 = new char[4];

	char * temp;
	temp = new char[15];
	
	while(true){
		while(index < carArchive.size()){

			// Expirement Id
			itoa (expID,value1,10);
			
			// Archive Car Id
			itoa (carArchive[index].id,value2,10);

			// Archive Frames
			strcpy (value3,"'");
			for(int i=0; i < carArchive[index].frames.size(); i++){
				itoa (carArchive[index].frames[i],temp,10);
				if(i != carArchive[index].frames.size()-1){
					strncat (temp," ",1);
					strncat (value3,temp,15);
				} else {
					strncat (temp,"'",1);
					strncat(value3,temp,15);
				}
			}

			// Save image locally & save directory
			strcpy (value4,"car/");
			strncat (value4,value2,15);
			strncat (value4,".jpg",15);
			String value4_S = String(value4);
			imwrite(value4_S, carArchive[index].img );

			/* Create SQL statement */
			strcpy(sql,"INSERT INTO CarTable (expID,CarID,Frames,Path) VALUES (");
			strncat(sql,value1,15);
			strncat(sql,",",1);
			strncat(sql,value2,15);
			strncat(sql,",",1);
			strncat(sql,value3,1000);
			strncat(sql,",",1);
			strncat(sql,"'",1);
			strncat(sql,value4,100);
			strncat(sql,"'",1);
			strncat(sql,");",3);

			/* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
			if( rc != SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n",zErrMsg);
				sqlite3_free(zErrMsg);
			} else {
				fprintf(stdout, "Records created successfully\n");
				fprintf(stdout, "----------------------------\n");
				fprintf(stdout, "Record idx:%d\n",index);
				fprintf(stdout, "Record CID:%s\n",value2);
				fprintf(stdout, "Record frames:%s\n",value3);
				fprintf(stdout, "Path of image:%s\n",value4);
			}
			index++;
		}
	}	

//	delete[] sql;
//  delete[] temp;
//	delete[] value1;
//	delete[] value2;
//  delete[] value3;
//  delete[] value4;
}

void carCounter::createFrame(Frame &data, cv::vector<cv::vector<cv::Point>> &contours,
							  cv::vector<cv::vector<cv::Point>> &contours_poly,
							  cv::vector<cv::Rect> &boundRect,
							  cv::vector<cv::Point2f> &center,
						      cv::vector<float> &radius, int minObjectSize)
{
	int j = 0;

	for(int i = 0; i < contours.size(); i++)
	{ 
		approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
		cv::minEnclosingCircle((Mat)contours_poly[i], center[j], radius[j]);

		if(cv::boundingRect(cv::Mat(contours_poly[i])).area() > minObjectSize)
		{
			boundRect[j] = cv::boundingRect(cv::Mat(contours_poly[i]));
			contours[j] = contours[i];
			cv::minEnclosingCircle((Mat)contours_poly[i], center[j], radius[j]);		
			j++;
		}
	}

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
	data.p         = std::vector<bool>(j);
	data.pTraj     = std::vector<std::vector<cv::Point2f>>(j);
}


// region monitor functions
void carCounter::startRegionMonitor(std::deque<Frame> &buffer,std::deque<Object> &transit,
									cv::vector<cv::Point> startRegion){
	for(int i = 0; i < buffer[0].p.size(); i++){
		if(buffer[0].p[i] != true
			&& pointPolygonTest(startRegion, buffer[0].center[i], false) == 1 && val < .75){
			
			buffer[0].start_loc[i] = buffer[0].center[i];
			buffer[0].id[i] = object_counter+1;
			buffer[0].time[i] = 0;
			object_counter++;

			Object obj;

			obj.miss = false;
			obj.last_seen = frame_counter;
			obj.id = buffer[0].id[i];
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
		
			transit.push_front(obj);
		}
	}
}

void carCounter::endRegionMonitor(cv::Mat &frame,int &car_counter,std::deque<Frame> &buffer,std::deque<Object> &transit, std::deque<Object> &carArchive,
									cv::vector<cv::Point> endRegion)
{
	for(int i = 0; i < buffer[0].center.size(); i++){
		if(pointPolygonTest(endRegion, buffer[0].center[i], false) == 1 && val < .75)
		{
			bool found = false;
			for(int j = 0; j < transit.size(); j++){
				if(buffer[0].id[i] == transit[j].id){
					found = true;
				}
			}
			if(found == true){
				while(buffer[0].id[i] != transit[transit.size()-1].id){
					transit.pop_back();
				}
				if(buffer[0].id[i] == transit[transit.size()-1].id){
					
					cv::Mat croppedBox;
					frame(buffer[0].boundRect[i]).copyTo(croppedBox);
					transit[transit.size()-1].img = croppedBox;

					carArchive.push_back(transit[transit.size()-1]);
					transit.pop_back();
					car_counter++;
				}
			}
		}
	}
}


void carCounter::checkMissing(int &frame_counter,std::deque<Frame> &buffer,std::deque<Object> &transit)
{
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
				break;
			}
		}
		if(found == false){
			transit[i].miss = true;
		}
	}
}

// Persistency checking using Transit Ledger

void carCounter::checkTransitLedgerUsingMomentAndCenter(cv::Mat &frame, int &car_counter, int &frame_counter, int &object_counter,std::deque<Frame> &buffer,std::deque<Object> &transit,		
							cv::vector<cv::Point> startRegion)
{
	for(int i = 0; i < buffer[0].boundRect.size(); i++)
	{
		if(buffer[0].p[i] != true)
		{
			for(int j = transit.size()-1; j >= 0 ;j--)
			{
				if(matchShapes(buffer[0].contour[i], transit[j].contour_before_disappearing,CV_CONTOURS_MATCH_I2,0))
				{
					for(int t = transit[j].trajectories_before_disappearing.size()-1; t >= 0; t--)
					{
						for(int m = transit[j].trajectories_before_disappearing[t].size()-1; m >= 0; m--)
						{
							if(((buffer[0].center[i].x > transit[j].trajectories_before_disappearing[t].at(m).x-15) && (transit[j].trajectories_before_disappearing[t].at(m).x+15 > buffer[0].center[i].x)) 
							&& ((buffer[0].center[i].y > transit[j].trajectories_before_disappearing[t].at(m).y-15) && (transit[j].trajectories_before_disappearing[t].at(m).y+15 > buffer[0].center[i].y)))
							{
								buffer[0].start_loc[i] = transit[j].start_loc;
								buffer[0].id[i] = transit[j].id;

								int n_time = transit[j].time_when_last_seen + (frame_counter - transit[j].last_seen);
								buffer[0].time[i] = n_time;

								//Instantanous Metrics

								buffer[0].velocity[i] = sqrt(pow(buffer[0].center[i].x - transit[j].center_when_last_seen.x,2.0) 
								+ pow(buffer[0].center[i].y - transit[j].center_when_last_seen.y,2.0))/(frame_counter - transit[j].last_seen);

								buffer[0].velocityX[i] = (buffer[0].center[i].x - transit[j].center_when_last_seen.x)/(frame_counter - transit[j].last_seen);
								buffer[0].velocityY[i] = (buffer[0].center[i].y - transit[j].center_when_last_seen.y)/(frame_counter - transit[j].last_seen);

								//Average Metrics

								buffer[0].distance[i] = sqrt(pow(buffer[0].center[i].x - transit[j].start_loc.x,2.0) 
								+ pow(buffer[0].center[i].y - transit[j].start_loc.y,2.0));
								
								buffer[0].distanceX[i] = buffer[0].center[i].x-transit[j].start_loc.x;
								buffer[0].distanceY[i] = buffer[0].center[i].y-transit[j].start_loc.y;

								buffer[0].speed[i] = buffer[0].distance[i]/buffer[0].time[i];
								buffer[0].speedX[i] = buffer[0].distanceX[i]/buffer[0].time[i];
								buffer[0].speedY[i] = buffer[0].distanceY[i]/buffer[0].time[i];

								// Presistance Flag
								buffer[0].p[i] = true;

								int k = 0;
								// fill predicted trajectory vector
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


void carCounter::persistenceCheck(cv::Mat &frame, int &car_counter, int &frame_counter, int &object_counter,int type,std::deque<Frame> &buffer, std::deque<Object> &transit,
								  cv::vector<cv::Point> startRegion){
	if(type == 0) 
		checkTransitLedgerUsingMomentAndCenter(frame, car_counter,frame_counter,object_counter,buffer,transit,startRegion);
	else 
		assert(type < 1 && type >= 0);
	
	// Additional methods or combination of methods can be added as extra types.
}

void carCounter::drawResult(int &car_counter, int &object_counter, cv::Mat &frame, cv::vector<cv::Vec4i> &hierarchy, std::deque<Frame> &buffer,std::deque<Object> &transit,
							bool showLastCar, bool boundBoxesOn, bool predictionOn, bool latestPathsOn, bool displayTransitLedger, bool displayFocusRegions, int showPathofId,
							cv::vector<cv::Point> startRegion,cv::vector<cv::Point> endRegion)
{
	if(boundBoxesOn == true)
	{
		// Draw polygonal contour + bonding rects.
		for( int i = 0; i<buffer[0].boundRect.size(); i=hierarchy[i][0] )
		{
			if(buffer[0].id[i] > 0)
			{
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
			}
			else
			{
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
	if(predictionOn == true)
	{
		for( int i = 0; i<buffer[0].boundRect.size(); i=hierarchy[i][0] )
		{
			if(buffer[0].id[i] > 0)
			{
				cv::Scalar color = cv::Scalar(255,0,255);
				if( buffer[0].pTraj[i].size() > 2)
				{
					for(int j = 0; j < buffer[0].pTraj[i].size()-1; j++)
					{
						line(frame, buffer[0].pTraj[i].at(j), buffer[0].pTraj[i].at(j+1), color, 20, 8, 0);	
					}
				}
				for(int j = 0; j < buffer[0].pTraj[i].size(); j++)
				{
					circle(frame, buffer[0].pTraj[i].at(j), 1, cv::Scalar(255,255,255), 3, 8, 0);	
				}
			}
		}
	}
	// draw recent trajectories.
	if(latestPathsOn == true)
	{
		for(int i = 0; i<buffer.size(); i++)
		{
			for( int j = 0; j<buffer[i].boundRect.size(); j=hierarchy[j][0] )
			{
				if(buffer[i].id[j] > 0)
				{
					cv::Scalar color = cv::Scalar(255,0,0);
					circle(frame, buffer[i].center[j], 1, color, 3, 8, 0);
				}
			}
		}
	}

	// draw older trajectories from archive.
	if(showPathofId >= 0)
	{
		int display_path_of_id = showPathofId;
		if(carArchive.size() > display_path_of_id)
		{
			for( int j = 0; j<carArchive[display_path_of_id].trajectory.size(); j++ )
			{
				cv::Scalar color = cv::Scalar(0,0,255);
				circle(frame, carArchive[display_path_of_id].trajectory[j], 1, color, 3, 8, 0);
			}
		}
	}

	// display the last discovered car in a box.
	if(showLastCar == true && carArchive.size() != 0){
		
		imshow("Last Car",carArchive[carArchive.size()-1].img);   
	}

	// transit ledger.
	if(displayTransitLedger == true){
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
	if(displayFocusRegions == true)
	{
		cv::vector<cv::vector<cv::Point>> regions;
		regions.push_back(startRegion);
		regions.push_back(endRegion);
		drawContours(frame, regions, -1, CV_RGB(255,0,0), 1, 8);
	}
}

// PUBLIC METHODS

/* constructor */
carCounter::carCounter(int expID) {
	id = expID;
	object_counter = 0;
	frame_counter = 0;
	car_counter = 0;
}

/* primary input params: buffersize, filename, skip frames, min object size, learning time, start & end region
 * secondary input params: overlayers (selected focus regions) -> selected through outer interface, offline/online
 * MOG2 params: nmixtures, backgroundratio, detectShadows */	
void carCounter::run(int bufferSize, int minObjectSize, int skip, int learningTime, char fileName[100], 
		 bool online, const cv::Point* ppt, const cv::Point* ppt2,int* npt,int* npt2,
		 int nmixtures, double backgroundratio, bool detectShadows,
		 bool showLastCar, bool boundBoxesOn, bool predictionOn, bool latestPathsOn, 
		 bool displayTransitLedger, bool displayFocusRegions, int showPathofId,
		 int displayType,
		 cv::vector<cv::Point> startRegion,cv::vector<cv::Point> endRegion)
{		
	//0 is the id of video device.0 if you have only one camera.	
	cv::VideoCapture stream(fileName);  

	openDB();

	std::thread DBthread(&carCounter::exportCarsToDB,*this,id,std::ref(carArchive));
	DBthread.detach();
	
	//unconditional loop (keeps grabbing frames from video).     
	while (true) 
	{     
		frame_counter++;
		if(frame_counter % skip == 0)
		{
			// OBTAIN FRAME
			if(!(stream.read(frame)))
				break;

			// create windows of correct size.
			cv::resize(frame, resizeF, cv::Size(frame.size().width, frame.size().height));

			// set focal overlays.
			fillPoly(resizeF,&ppt,npt,1,Scalar( 255, 255, 255 ),8);
			fillPoly(resizeF,&ppt2,npt2,1,Scalar( 255, 255, 255 ),8);

			// set other variables.
			MOG2.set("history", 1000); //doesn't work
			MOG2.set("nmixtures", nmixtures);
			MOG2.set("backgroundRatio", backgroundratio);
			MOG2.set("detectShadows", false);

			// BACKGROUND SUBTRACTION

			// input frame i, output foreground mask i, learning rate.
			MOG2(resizeF, fgMaskMOG2,.005);
			MOG2.getBackgroundImage(background);

			// CLEAN MASK

			// clear out noise using erode and dilate.
			cv::erode(fgMaskMOG2,fgMaskMOG2,cv::Mat());
			cv::dilate(fgMaskMOG2,fgMaskMOG2,cv::Mat());
			cv::medianBlur(fgMaskMOG2,fgMaskMOG2,21);

			// remove shadows.
			fgMaskMOG2rgb = fgMaskMOG2;

			for (Mat3b::iterator it = fgMaskMOG2rgb.begin(); it != fgMaskMOG2rgb.end(); it++) 
			{
				if(*it != Vec3b(255, 255, 255)) 
				{
					*it = Vec3b(0, 0, 0);
				}
			}

			cv::resize(fgMaskMOG2rgb, fgMaskMOG2rgb, cv::Size(frame.size().width, frame.size().height)); 
			cv::cvtColor(fgMaskMOG2rgb,fgMaskMOG2,CV_RGB2GRAY);

			if(frame_counter > learningTime)
			{

				// OBJECT DETECTION
				findContours(fgMaskMOG2, contours, hierarchy, CV_RETR_TREE,  CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

				// Approximate contours to polygons + get bounding rects and circles.
				cv::vector<cv::vector<cv::Point>> contours_poly(contours.size());
				cv::vector<cv::Rect> boundRect(contours.size());
				cv::vector<cv::Point2f> center(contours.size());
				cv::vector<float> radius(contours.size());

				Frame data;

				// initialize the frame object data.
				carCounter::createFrame(data, contours,
					contours_poly,
					boundRect,
					center,
					radius, minObjectSize);

				// OBJECT TRACKING

				// buffer frame data (last 30 frames are placed in a queue).
				// buffer flood.
				if(buffer.size() < bufferSize)
				{
					buffer.push_front(data);		

					// persistence checks only make sense if you have more than one frame.
					if(buffer.size() > 1)
					{
						persistenceCheck(frame, car_counter,frame_counter,object_counter,0,buffer,transit, startRegion);
						startRegionMonitor(buffer,transit,startRegion);
						
						checkMissing(frame_counter,buffer,transit);

						endRegionMonitor(frame,car_counter,buffer,transit,carArchive,endRegion);
						frameArchive.push_front(buffer[0]);
					}
				}

				// full buffer.
				else
				{
					buffer.push_front(data);
					buffer.pop_back();

					persistenceCheck(frame, car_counter,frame_counter,object_counter,0,buffer,transit,startRegion);
					startRegionMonitor(buffer,transit,startRegion);

					checkMissing(frame_counter,buffer,transit);

					endRegionMonitor(frame,car_counter,buffer,transit,carArchive,endRegion);
					frameArchive.push_back(buffer[0]);
				}

				// VISUALIZATION

				// if online mode is on a window is produced otherwise runs through console with no visualization.
				if(online == true){
					// use background.
					if(displayType == 0){
						eMOG2.set("history", 1000);
						eMOG2.set("nmixtures", nmixtures);
						eMOG2.set("backgroundRatio", backgroundratio);
						eMOG2.set("detectShadows", false);
						eMOG2(frame, efgMaskMOG2,.005);
						eMOG2.getBackgroundImage(display);
					}

					// use original frame.
					else if(displayType == 1)
						display = frame;
					
					// use original frame and overlays.
					else if(displayType == 2)
						display = resizeF;
					
					// subtracted.
					else if(displayType == 3)
						display = fgMaskMOG2;
					

					drawResult(car_counter,object_counter,display,hierarchy,buffer,transit,
						showLastCar,boundBoxesOn, predictionOn, latestPathsOn, 
						displayTransitLedger, displayFocusRegions, showPathofId, 
						startRegion,endRegion);

					// display original with bounding boxes.
					imshow("Display", display);
				} else {
					std::cout << car_counter << std::endl;
				}
			}

			if (cv::waitKey(30) >= 0)     
				break;
		}
	}
	closeDB();
}

/*	HISTOGRAM COMPARISON
		cv::Mat croppedFrame;
		cv::Mat hsv_croppedFrame;
		cv::Mat croppedBackground;
		cv::Mat hsv_croppedBackground;

		frame(buffer[0].boundRect[i]).copyTo(croppedFrame);
		background(buffer[0].boundRect[i]).copyTo(croppedBackground);
		
		cvtColor(croppedFrame,hsv_croppedFrame,CV_RGB2HSV);
		cvtColor(croppedBackground,hsv_croppedBackground,CV_RGB2HSV);

		// Quantize the hue to 30 levels
		// and the saturation to 32 levels
		int hbins = 30, sbins = 32;
		int histSize[] = {hbins, sbins};
		
		// hue varies from 0 to 179, see cvtColor
		float hranges[] = { 0, 180 };
		
		// saturation varies from 0 (black-gray-white) to
		// 255 (pure spectrum color)
		float sranges[] = { 0, 256 };
		const float* ranges[] = { hranges, sranges };
		MatND hist_frame;
		MatND hist_background;
		
		// we compute the histogram from the 0-th and 1-st channels
		int channels[] = {0, 1, 2, 3};

		calcHist( &hsv_croppedFrame, 1, channels, Mat(), // do not use mask
             hist_frame, 2, histSize, ranges,
             true, // the histogram is uniform
             false );

		normalize( hist_frame, hist_frame, 0, 1, NORM_MINMAX, -1, Mat() );

		calcHist( &hsv_croppedBackground, 1, channels, Mat(), // do not use mask
             hist_background, 2, histSize, ranges,
             true, // the histogram is uniform
             false );

		normalize( hist_background, hist_background, 0, 1, NORM_MINMAX, -1, Mat() );
		
		double val = compareHist( hist_frame, hist_background, CV_COMP_CORREL );
*/