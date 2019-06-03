#pragma once

#include <opencv2/opencv.hpp>
#include "../util/Exceptions.h"

class markerdetection
{
	void resetBlobDetector();
	bool checkBounds(cv::Point point1, cv::Point point2) const;
	void detectMarker();
	void drawBounds(cv::Mat drawImg) const;
	void checkAllBounds(cv::Mat drawImg);
	void calibrate();
	void executeMouseDetection();
	void executeOpenCVDetection();

	const int SCREEN_DIVIDER_RATIO = 5;
	const int SCREEN_RIGHT_SIDE_BOUND_START = 4;

	cv::Mat frame, frameThreshold;
	Exceptions exception;
	cv::Mat blobImg;
	cv::Mat originalBlobImg;
	int thresholdLower = -1;
	int thresholdUpper = -1;
	int threshold = 200;

	cv::Ptr<cv::SimpleBlobDetector> detector;
	cv::SimpleBlobDetector::Params params;
	std::vector<cv::KeyPoint> myBlobs;

public:
	// typesafe enums
	enum class DetectionMode { opencv, mouse };

	//struct for passing the x and y
	struct Point2D
	{
		float x, y;
	};

	//Global functions

	/*
	//	This function starts the openCV module.
	//
	//	@param int input is for switching the mode between mouse and openCV
	//
	*/
	void runMarkerDetection(DetectionMode mode);

	/*
	//	This function is used for terminating the OpenCV windows.
	//
	*/
	void terminateDetection();

	/*
	//	This function is used for getting the detection mode.
	//
	//  @return returns the Markerdetection mode. 0 for OpenCV and 1 for Mouse detection
	*/
	DetectionMode getDetectionMode();

	/*
	//	This function is used for changing the detection mode of the OpenCV module.
	//
	*/
	void changeDetectionMode();

	/*
	//	This function is used for getting the coordinates of the OpenCV application..
	//
	//  @return Struct of Point2D with x and y coordinates
	*/
	Point2D getCoordinates();

	bool hasNewMousePosition();
};
