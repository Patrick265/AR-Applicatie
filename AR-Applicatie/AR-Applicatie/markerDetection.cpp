#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include "markerDetection.h"

/*
//	This class is used for marker detection
//	Author: Tim de Booij, Max van Noordennen, Tom Martens
*/

//Local defines
#define SCREEN_DIVIDER_RATIO 5
#define SCREEN_RIGHT_SIDE_BOUND_START 4


struct Points {
	float x, y;
};

//Global variables
cv::VideoCapture cap(0);
cv::Mat frame, frame_HSV, frame_threshold;
cv::Point markerPosition;
cv::Ptr<cv::SimpleBlobDetector> detector;
cv::Mat blobImg;
cv::SimpleBlobDetector::Params params;
std::vector<cv::KeyPoint> myBlobs;
cv::Mat originalBlobImg;
int isRunning = 1;
int detectionMode = 1;
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

//Variables for areabar
int areaSliderMin = 1000;
int areaSliderMax = 70000;

int getDetectionMode() {
	return detectionMode;
}

void changeDetectionMode() {
	if (detectionMode == 0) {
		detectionMode = 1;
	}
	else {
		detectionMode = 0;
	}
}

/*
//	This function is a callback and used for setting the params and changing the params.
//
//	@param int is for callback
//	@param void* is for the callback
//	
*/
void terminateDetection() {
	isRunning = 0;
	cv::destroyAllWindows();
}

void resetBlobDetector() {
	params.minDistBetweenBlobs = 1.0;    //Minimum 1 pixel between blobs
	params.filterByArea = true;            //Checking for area
	params.filterByColor = false;        //We're doing a binary detection, we don't want color
	params.minArea = areaSliderMin;        //Minimum value of the area
	params.maxArea = areaSliderMax;        //Maximum value of the area
	params.thresholdStep = 100;            //Slider steps
	params.blobColor = 0;                //Color we're checking for
	params.filterByCircularity = true;    //We dont check for circularity
	params.filterByInertia = false;        //We dont check for Intertia
	params.filterByConvexity = true;    //We dont check for convexity
	params.minConvexity = 0.5;
	params.minCircularity = 0.15;

	//Creating a detector with the settings above
	detector = cv::SimpleBlobDetector::create(params);
}

void areaBar(int, void*) {
	resetBlobDetector();

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
int checkBounds(cv::Point point1, cv::Point point2) {
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
void detectMarker() {
	int size = 0;
	for (cv::KeyPoint k : myBlobs)
	{
		if (k.size > size) {
			Points points{ k.pt.x, k.pt.y };
			markerPosition.x = k.pt.x;
			markerPosition.y = k.pt.y;
			size = k.size;
		}
	}
}

void mouseCallback(int  event, int  x, int  y, int  flag, void *param) {
	markerPosition.x = x;
	markerPosition.y = y;
}

void drawBounds(cv::Mat drawImg) {
	//Draw Horizontal raster
	cv::line(drawImg, cv::Point(0, height / SCREEN_DIVIDER_RATIO), cv::Point(width, height / SCREEN_DIVIDER_RATIO), CV_RGB(255, 255, 255), 2);
	cv::line(drawImg, cv::Point(0, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), cv::Point(width, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), CV_RGB(255, 255, 255), 2);

	//Draw Vertical raster
	cv::line(drawImg, cv::Point(width / SCREEN_DIVIDER_RATIO, 0), cv::Point(width / SCREEN_DIVIDER_RATIO, height), CV_RGB(255, 255, 255), 2);
	cv::line(drawImg, cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, 0), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height), CV_RGB(255, 255, 255), 2);

}

void checkAllBounds(cv::Mat drawImg) {
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

void calibrate() {
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
		std::cout << "amount of blobs: " << myBlobs.size() << std::endl;
		if (myBlobs.size() == 1 && upperS == -1) {
			counterUp++;
			if (counterUp == 3) {
				upperS = lowS;
				std::cout << "set upper" << upperS << std::endl;
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
				std::cout << "set lower" << lowerS << std::endl;
			}
		}
		else {
			counterDown = 0;
		}
		lowS -= 2;
	}
	lowS = (upperS + lowerS) / 2;
	std::cout << "average" << lowS << std::endl;
}

void excecuteMouseDetection() {

	originalBlobImg = cv::imread("Black_Picture.jpg");

	blobImg = originalBlobImg;

	cv::imshow("binair beeld", blobImg);

	cv::setMouseCallback("binair beeld", mouseCallback);

	width = 640;
	height = 480;

	while (isRunning == 1) {
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
}

void excecuteOpenCVDetection() {

	while (isRunning == 1) {
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

		cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));
		cv::erode(frame, frame, element);

		//Detecting the blobs
		detector->detect(frame_threshold, myBlobs);

		//Drawing keypoints (red circles)
		drawKeypoints(frame_threshold, myBlobs, blobImg, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		detectMarker();

		//For text drawing purposes
		for (cv::KeyPoint k : myBlobs)
		{
			putText(blobImg, std::to_string(k.size), cv::Point(k.pt.x, k.pt.y), cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 255, 0), 2.0);
		}

		drawBounds(blobImg);

		checkAllBounds(blobImg);

		//Showing the text
		cv::imshow("binair beeld", blobImg);

		if (cv::waitKey(5) == 32) {
			terminateDetection();
			changeDetectionMode();
			runMarkerDetection(getDetectionMode());
		}
	}
}

int runMarkerDetection(int input)
{
	isRunning = 1;
	if (input == MARKERDETECTION_WITH_OPENCV) {
		
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
	if (input == MARKERDETECTION_WITH_MOUSE) {

		excecuteMouseDetection();
	}
	return 0;
}