#include "markerdetection.h"
#include <iostream>
#include <string>


/*
//	This class is used for marker detection
//	Author: Tim de Booij, Max van Noordennen, Tom Martens
*/

//Local defines
#define SCREEN_DIVIDER_RATIO 5
#define SCREEN_RIGHT_SIDE_BOUND_START 4

//Global variables
cv::VideoCapture cap(0);
cv::Mat frame, frame_HSV, frame_threshold;
cv::Point markerPosition;
cv::Ptr<cv::SimpleBlobDetector> detector;
cv::Mat blobImg;
cv::SimpleBlobDetector::Params params;
std::vector<cv::KeyPoint> myBlobs;
cv::Mat originalBlobImg;
markerdetection::DetectionMode mode;
int lowerS = -1;
int upperS = -1;

//Variables for HSV
const int maxValueH = 360 / 2;
const int maxValue = 255;
const std::string windowCaptureName = "Video Capture";
const std::string windowDetectionName = "Object Detection";
int lowH = 0, lowS = 200, lowV = 115;
int highH = 7, highS = maxValue, highV = maxValue;

//Variables for camera
int width;
int height;

// Mouse
bool newMousePosition = false;

/*
//	This function is used for getting the detection mode.
//
//  @return returns the Markerdetection mode. 0 for OpenCV and 1 for Mouse detection
*/
markerdetection::DetectionMode markerdetection::getDetectionMode() 
{
	return mode;
}

/*
//	This function is used for changing the detection mode of the OpenCV module.
//
*/
void markerdetection::changeDetectionMode() {
	if (mode == markerdetection::DetectionMode::opencv) 
	{
		mode = markerdetection::DetectionMode::mouse;
	}
	else {
		mode = markerdetection::DetectionMode::opencv;
	}
}

/*
//	This function is used for terminating the OpenCV windows.
//
*/
void markerdetection::terminateDetection() 
{
	cv::destroyAllWindows();
}

/*
// This method is used for setting the parameters of the blobdetector.
//
*/
void markerdetection::resetBlobDetector() 
{
	params.minDistBetweenBlobs = 1.0;   
	params.filterByArea = true;           
	params.filterByColor = false;        
	params.minArea = 1000;        
	params.maxArea = 70000;      
	params.thresholdStep = 100;            
	params.blobColor = 0;              
	params.filterByCircularity = true;  
	params.filterByInertia = false;       
	params.filterByConvexity = true;    
	params.minConvexity = 0.5;
	params.minCircularity = 0.15;

	//Creating a detector with the settings above
	detector = cv::SimpleBlobDetector::create(params);
}


/*
//	This function is used for getting the coordinates of the OpenCV application..
//
//  @return Struct of Point2D with x and y coordinates
*/
markerdetection::Point2D markerdetection::getCoordinates()
{
	markerdetection::Point2D point = { markerPosition.x / float(width), markerPosition.y / float(height)};
	return point;
}

/*
//	This function is used for checking the bounds of the debug screen
//
//	@param cv::Point point1 is used for checking the x and the y value of the first point
//	@param cv::Point point2 is used for checking the x and the y value of the second point
//
//	@return returnvalue returns the value 0 or 1, 0 for not in bounds and 1 for in bounds
//
*/
int markerdetection::checkBounds(cv::Point point1, cv::Point point2) 
{
	int returnValue = 0;
	if (markerPosition.x >= point1.x && markerPosition.x <= point2.x && markerPosition.y >= point1.y && markerPosition.y <= point2.y) {
		//std::cout << "point is in bounds" << std::endl;
		returnValue = 1;
	}

	return returnValue;
}

/*
//	This function is used for detecting the marker
*/
void markerdetection::detectMarker() 
{
	int size = 0;
	for (cv::KeyPoint k : myBlobs)
	{
		if (k.size > size) {
			markerdetection::Point2D points{ k.pt.x, k.pt.y };
			markerPosition.x = k.pt.x;
			markerPosition.y = k.pt.y;
			size = k.size;
			
		}
	}
}

/*
//	This function is a callback used for using the mouse on the openCV screen.
//
*/
void mouseCallback(int  event, int  x, int  y, int  flag, void *param) 
{
	markerPosition.x = x;
	markerPosition.y = y;
	newMousePosition = true;
}

bool markerdetection::hasNewMousePosition()
{
	if (newMousePosition)
	{
		newMousePosition = false;
		return true;
	}
	return false;
}

/*
//	This function is used for drawing the borders on the openCV screen.
//
*/
void markerdetection::drawBounds(cv::Mat drawImg) 
{
	//Draw Horizontal raster
	cv::line(drawImg, cv::Point(0, height / SCREEN_DIVIDER_RATIO), cv::Point(width, height / SCREEN_DIVIDER_RATIO), CV_RGB(255, 255, 255), 2);
	cv::line(drawImg, cv::Point(0, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), cv::Point(width, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), CV_RGB(255, 255, 255), 2);

	//Draw Vertical raster
	cv::line(drawImg, cv::Point(width / SCREEN_DIVIDER_RATIO, 0), cv::Point(width / SCREEN_DIVIDER_RATIO, height), CV_RGB(255, 255, 255), 2);
	cv::line(drawImg, cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, 0), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height), CV_RGB(255, 255, 255), 2);

}

/*
//	This function is used for checking if the x and the y values of the object are in the bounds.
//
*/
void markerdetection::checkAllBounds(cv::Mat drawImg) 
{
	//Check left bound
	if (checkBounds(cv::Point(width / SCREEN_DIVIDER_RATIO, 0), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO)) == 1) {
		cv::rectangle(drawImg, cv::Point(width / SCREEN_DIVIDER_RATIO, 0), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO), CV_RGB(255, 0, 0), 5, 1, 0);
	}

	//Check Upper bound
	if (checkBounds(cv::Point(0, height / SCREEN_DIVIDER_RATIO), cv::Point(width / SCREEN_DIVIDER_RATIO, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START)) == 1) {
		cv::rectangle(drawImg, cv::Point(0, height / 5), cv::Point(width / SCREEN_DIVIDER_RATIO, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), CV_RGB(255, 0, 0), 5, 1, 0);
	}

	//Check right bound
	if (checkBounds(cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO), cv::Point(width, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START)) == 1) {
		cv::rectangle(drawImg, cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO), cv::Point(width, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), CV_RGB(255, 0, 0), 5, 1, 0);
	}

	//Check lower bound
	if (checkBounds(cv::Point(width / 5, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height)) == 1) {
		cv::rectangle(drawImg, cv::Point(width / SCREEN_DIVIDER_RATIO, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height), CV_RGB(255, 0, 0), 5, 1, 0);
	}

	//Check middle bound
	if (checkBounds(cv::Point(width / SCREEN_DIVIDER_RATIO, height / SCREEN_DIVIDER_RATIO), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START)) == 1) {
		cv::rectangle(drawImg, cv::Point(width / SCREEN_DIVIDER_RATIO, height / SCREEN_DIVIDER_RATIO), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), CV_RGB(255, 0, 0), 5, 1, 0);
	}
}

/*
//	This function is used for calibrating the tracked object.
//
*/
void markerdetection::calibrate() 
{
	int counterUp = 0;
	int counterDown = 0;
	while ((lowerS == -1 || upperS == -1) && lowS >= 0) {
		cap >> frame;
		if (frame.empty()) {
			std::cerr << "Frame invalid and skipped!" << std::endl;
			continue;
		}
		flip(frame, frame, 1);

		// Convert from BGR to HSV colorspace
		cvtColor(frame, frame_HSV, cv::COLOR_BGR2HSV);
		// Detect the object based on HSV Range Values
		inRange(frame_HSV, cv::Scalar(lowH, lowS, lowV), cv::Scalar(highH, highS, highV), frame_threshold);

		//Detecting the blobs
		detector->detect(frame_threshold, myBlobs);
		
		if (myBlobs.size() == 1 && upperS == -1) {
			counterUp++;
			if (counterUp == 3) {
				upperS = lowS;
				
				counterUp = 0;
			}
		}
		else {
			counterUp = 0;
		}
		if ((myBlobs.size() == 0 || myBlobs.size() > 1) && upperS != -1) {
			counterDown++;
			if (counterDown == 3) {
				lowerS = lowS;
				
			}
		}
		else {
			counterDown = 0;
		}
		lowS -= 2;
	}
	lowS = (upperS + lowerS) / 2;
	
}

/*
//	This function starts the mouse detection modus.
//
*/
void markerdetection::excecuteMouseDetection() 
{

	originalBlobImg = cv::imread("Resources/Vision/Black_Picture.jpg");

	blobImg = originalBlobImg;

	cv::imshow("binair beeld", blobImg);

	cv::setMouseCallback("binair beeld", mouseCallback);

	width = 640;
	height = 480;

	blobImg = originalBlobImg.clone();
	drawBounds(blobImg);

	checkAllBounds(blobImg);

	//Showing the text
	cv::imshow("binair beeld", blobImg);
	cv::resizeWindow("binair beeld", 640, 480);


		if (cv::waitKey(5) == 32) {
			terminateDetection();
			changeDetectionMode();
			runMarkerDetection(getDetectionMode());
		}
	
}

/*
//	This function starts the OpenCV detection modus.
//
*/
void markerdetection::excecuteOpenCVDetection() 
{
		cap >> frame;
		if (frame.empty()) {
			std::cerr << "Frame invalid and skipped!" << std::endl;
			return;
		}
		flip(frame, frame, 1);

		// Convert from BGR to HSV colorspace
		cvtColor(frame, frame_HSV, cv::COLOR_BGR2HSV);
		// Detect the object based on HSV Range Values
		inRange(frame_HSV, cv::Scalar(lowH, lowS, lowV), cv::Scalar(highH, highS, highV), frame_threshold);

		//Detecting the blobs
		detector->detect(frame_threshold, myBlobs);

		//Drawing keypoints (red circles)
		drawKeypoints(frame_threshold, myBlobs, blobImg, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		detectMarker();

		//Showing the text
		cv::imshow("binair beeld", blobImg);


		if (cv::waitKey(5) == 32) {
			terminateDetection();
			changeDetectionMode();
			runMarkerDetection(getDetectionMode());
		}
	
}

/*
//	This function starts the openCV module.
//
//	@param int input is for switching the mode between mouse and openCV
//
*/
int markerdetection::runMarkerDetection(markerdetection::DetectionMode mode)
{
	if (mode == markerdetection::DetectionMode::opencv) {

		resetBlobDetector();

		if (!cap.isOpened()) {
			std::cerr << "No camera detected on this system" << std::endl;
			return -1;
		}

		width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
		height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

		calibrate();

		excecuteOpenCVDetection();
	}
	if (mode == markerdetection::DetectionMode::mouse) {

		excecuteMouseDetection();
	}
	return 0;
}

