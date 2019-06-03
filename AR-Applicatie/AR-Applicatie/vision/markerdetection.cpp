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
markerdetection::Point2D markerPosition;
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
	if (mode == DetectionMode::opencv)
	{
		mode = DetectionMode::mouse;
	}
	else {
		mode = DetectionMode::opencv;
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
	const Point2D point = { markerPosition.x / float(width), markerPosition.y / float(height) };
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
bool markerdetection::checkBounds(const cv::Point point1, const cv::Point point2) const
{
	return markerPosition.x >= point1.x && markerPosition.x <= point2.x
		&& markerPosition.y >= point1.y && markerPosition.y <= point2.y;
}

/*
//	This function is used for detecting the marker
*/
void markerdetection::detectMarker()
{
	newMousePosition = true;
	auto size = 0;
	for (const auto keyPoint : myBlobs)
	{
		if (keyPoint.size > size)
		{
			Point2D points{ keyPoint.pt.x, keyPoint.pt.y };
			markerPosition.x = static_cast<float>(keyPoint.pt.x);
			markerPosition.y = static_cast<float>(keyPoint.pt.y);
			size = static_cast<int>(keyPoint.size);
		}
	}
}

/*
//	This function is a callback used for using the mouse on the openCV screen.
//
*/
void mouseCallback(int event, const int x, const int y, int flag, void *param)
{
	markerPosition.x = static_cast<float>(x);
	markerPosition.y = static_cast<float>(y);
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
//	This function is used for calibrating the tracked object.
*/
void markerdetection::calibrate()
{
	auto counterUp = 0;
	auto counterDown = 0;
	while ((thresholdLower == -1 || thresholdUpper == -1) && threshold >= 0)
	{
		cap >> frame;
		if (frame.empty())
		{
			try
			{
				exception.noCameraDetected();
			}
			catch (Exceptions e)
			{
				std::cout << "Exception: " << e.getExceptionMessage() << std::endl;
			}
		}
		flip(frame, frame, 1);

		//Filter the frame on it's red value
		cv::Mat bgr[3];
		split(frame, bgr);

		cv::Mat red = bgr[2] - (bgr[0] / 2 + bgr[1] / 2);

		cv::threshold(red, red, threshold, 255, cv::THRESH_BINARY);

		//Detecting the blobs
		detector->detect(red, myBlobs);

		if (myBlobs.size() == 1 && thresholdUpper == -1)
		{
			counterUp++;
			if (counterUp == 3)
			{
				thresholdUpper = threshold;

				counterUp = 0;
			}
		}
		else
		{
			counterUp = 0;
		}
		if (myBlobs.size() != 1 && thresholdUpper != -1)
		{
			counterDown++;
			if (counterDown == 3)
			{
				thresholdLower = threshold;
			}
		}
		else
		{
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
void markerdetection::executeMouseDetection()
{
	originalBlobImg = cv::imread("Resources/Vision/Black_Picture.jpg");

	blobImg = originalBlobImg;

	cv::setMouseCallback("binair beeld", mouseCallback);

	width = 640;
	height = 480;

	blobImg = originalBlobImg.clone();

	//Showing the text
	imshow("binair beeld", blobImg);
	cv::resizeWindow("binair beeld", width, height);

	if (cv::waitKey(1) == 32)
	{
		terminateDetection();
		changeDetectionMode();
		runMarkerDetection(getDetectionMode());
	}
}

/*
//	This function starts the OpenCV detection modus.
*/
void markerdetection::executeOpenCVDetection()
{
	cap.read(frame);
	if (frame.empty())
	{
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
	split(frame, bgr);

	cv::Mat red = bgr[2] - (bgr[0] / 2 + bgr[1] / 2);

	cv::threshold(red, red, threshold, 255, cv::THRESH_BINARY);

	//Detecting the blobs
	detector->detect(red, myBlobs);

	//Drawing keypoints (red circles)
	drawKeypoints(red, myBlobs, blobImg, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	detectMarker();

	//Showing the text
	imshow("binair beeld", blobImg);

	if (cv::waitKey(1) == 32)
	{
		terminateDetection();
		changeDetectionMode();
		runMarkerDetection(getDetectionMode());
	}
}

/*
//	This function starts the openCV module.
//
//	@param int input is for switching the mode between mouse and openCV
*/
void markerdetection::runMarkerDetection(const DetectionMode mode)
{
	if (mode == DetectionMode::opencv)
	{
		resetBlobDetector();

		if (!cap.isOpened())
		{
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

		executeOpenCVDetection();
	}
	if (mode == DetectionMode::mouse)
	{
		executeMouseDetection();
	}
}
