#include "markerdetection.h"
#include "../util/Exceptions.h"
#include <iostream>
#include <string>

/*
//	This class is used for marker detection
//	Author: Tim de Booij, Max van Noordennen, Tom Martens
*/

//Global variables
cv::VideoCapture cap(0);
markerdetection::Point2D marker_position;
markerdetection::DetectionMode mode;

bool newMousePosition = false;

//Variables for camera
int width;
int height;

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
	params.minCircularity = 0.15f;

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
	markerdetection::Point2D point = { marker_position.x / float(width), marker_position.y / float(height)};
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
	if (marker_position.x >= point1.x && marker_position.x <= point2.x && marker_position.y >= point1.y && marker_position.y <= point2.y) {
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
	newMousePosition = true;
	int size = 0;
	for (cv::KeyPoint k : my_blobs)
	{
		if (k.size > size) {
			Point2D points{ k.pt.x, k.pt.y };
			marker_position.x = static_cast<float>(k.pt.x);
			marker_position.y = static_cast<float>(k.pt.y);
			size = static_cast<int>(k.size);
			
		}
	}
}

/*
//	This function is a callback used for using the mouse on the openCV screen.
//
*/
void mouseCallback(int event, int x, int y, int flag, void *param) 
{
	marker_position.x = static_cast<float>(x);
	marker_position.y = static_cast<float>(y);
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
void markerdetection::drawBounds(cv::Mat draw_img) 
{
	//Draw Horizontal raster
	cv::line(draw_img, cv::Point(0, height / SCREEN_DIVIDER_RATIO), cv::Point(width, height / SCREEN_DIVIDER_RATIO), CV_RGB(255, 255, 255), 2);
	cv::line(draw_img, cv::Point(0, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), cv::Point(width, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), CV_RGB(255, 255, 255), 2);

	//Draw Vertical raster
	cv::line(draw_img, cv::Point(width / SCREEN_DIVIDER_RATIO, 0), cv::Point(width / SCREEN_DIVIDER_RATIO, height), CV_RGB(255, 255, 255), 2);
	cv::line(draw_img, cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, 0), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height), CV_RGB(255, 255, 255), 2);
}

/*
//	This function is used for checking if the x and the y values of the object are in the bounds.
//
*/
void markerdetection::checkAllBounds(cv::Mat draw_img) 
{
	//Check left bound
	if (checkBounds(cv::Point(width / SCREEN_DIVIDER_RATIO, 0), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO)) == 1) {
		cv::rectangle(draw_img, cv::Point(width / SCREEN_DIVIDER_RATIO, 0), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO), CV_RGB(255, 0, 0), 5, 1, 0);
	}

	//Check Upper bound
	if (checkBounds(cv::Point(0, height / SCREEN_DIVIDER_RATIO), cv::Point(width / SCREEN_DIVIDER_RATIO, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START)) == 1) {
		cv::rectangle(draw_img, cv::Point(0, height / 5), cv::Point(width / SCREEN_DIVIDER_RATIO, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), CV_RGB(255, 0, 0), 5, 1, 0);
	}

	//Check right bound
	if (checkBounds(cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO), cv::Point(width, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START)) == 1) {
		cv::rectangle(draw_img, cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO), cv::Point(width, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), CV_RGB(255, 0, 0), 5, 1, 0);
	}

	//Check lower bound
	if (checkBounds(cv::Point(width / 5, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height)) == 1) {
		cv::rectangle(draw_img, cv::Point(width / SCREEN_DIVIDER_RATIO, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height), CV_RGB(255, 0, 0), 5, 1, 0);
	}

	//Check middle bound
	if (checkBounds(cv::Point(width / SCREEN_DIVIDER_RATIO, height / SCREEN_DIVIDER_RATIO), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START)) == 1) {
		cv::rectangle(draw_img, cv::Point(width / SCREEN_DIVIDER_RATIO, height / SCREEN_DIVIDER_RATIO), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), CV_RGB(255, 0, 0), 5, 1, 0);
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
	while ((thresholdLower == -1 || thresholdUpper == -1) && threshold >= 0) {
		
		cap >> frame;
		if (frame.empty()) {
			try
			{
				exception.noCameraDetected();
			}
			catch (Exceptions e)
			{
				std::cout << "Exception: " << e.getExceptionMessage() << std::endl;
			}
		}
		cv::flip(frame, frame, 1);

		//Filter the frame on it's red value
		cv::Mat bgr[3];
		cv::split(frame, bgr);

		cv::Mat red = bgr[2] - (bgr[0] / 2 + bgr[1] / 2);

		cv::threshold(red, red, threshold, 255, cv::THRESH_BINARY);

		//Detecting the blobs
		detector->detect(red, my_blobs);
		
		if (my_blobs.size() == 1 && thresholdUpper == -1) {
			counterUp++;
			if (counterUp == 3) {
				thresholdUpper = threshold;
				
				counterUp = 0;
			}
		}
		else {
			counterUp = 0;
		}
		if ((my_blobs.size() == 0 || my_blobs.size() > 1) && thresholdUpper != -1) {
			counterDown++;
			if (counterDown == 3) {
				thresholdLower = threshold;
				
			}
		}
		else {
			counterDown = 0;
		}
		threshold -= 2;
	}
	threshold = (thresholdUpper + thresholdLower) / 2.0f;
}

/*
//	This function starts the mouse detection modus.
//
*/
void markerdetection::excecuteMouseDetection() 
{
	original_blob_img = cv::imread("Resources/Vision/Black_Picture.jpg");

	blob_img = original_blob_img;

	cv::setMouseCallback("binair beeld", mouseCallback);

	width = 640;
	height = 480;

	blob_img = original_blob_img.clone();
	
	//Showing the text
	cv::imshow("binair beeld", blob_img);
	cv::resizeWindow("binair beeld", width, height);

		if (cv::waitKey(1) == 32) {
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
		cap.read(frame);
		if (frame.empty()) {
			try
			{
				exception.invalidFrames();
			}
			catch (Exceptions e)
			{
				std::cout << "Exception: " << e.getExceptionMessage() << std::endl;
				return;
			}
		}
		flip(frame, frame, 1);

		//Filter the frame on it's red value
		cv::Mat bgr[3];
		cv::split(frame, bgr);

		cv::Mat red = bgr[2] - (bgr[0]/2 + bgr[1]/2);

		cv::threshold(red, red, threshold, 255, cv::THRESH_BINARY);

		//Detecting the blobs
		detector->detect(red, my_blobs);
			   
		//Drawing keypoints (red circles)
		drawKeypoints(red, my_blobs, blob_img, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		detectMarker();

		//Showing the text
		cv::imshow("binair beeld", blob_img);

		if (cv::waitKey(1) == 32) {
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
void markerdetection::runMarkerDetection(markerdetection::DetectionMode mode)
{
	if (mode == markerdetection::DetectionMode::opencv) {
			resetBlobDetector();

			if (!cap.isOpened()) {
				try
				{
					exception.noCameraDetected();
				}
				catch (Exceptions e)
				{
					std::cout << "Exception: " << e.getExceptionMessage() << std::endl;
				}
			}

			width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
			height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

			calibrate();

			excecuteOpenCVDetection();
	}
	if (mode == markerdetection::DetectionMode::mouse) {
		excecuteMouseDetection();
	}
}

