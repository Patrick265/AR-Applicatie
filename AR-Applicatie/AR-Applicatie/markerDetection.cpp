#include <opencv2/opencv.hpp>
#include <string>

#define SCREEN_DIVIDER_RATIO 5
#define SCREEN_RIGHT_SIDE_BOUND_START 4


struct Points {
	float x, y;
};

//Global variables
cv::Mat frame, frame_HSV, frame_threshold;
cv::Point markerPosition;
cv::Ptr<cv::SimpleBlobDetector> detector;
cv::Mat blobImg;
cv::SimpleBlobDetector::Params params;
std::vector<cv::KeyPoint> myBlobs;

//Variables for HSV
const int maxValueH = 360 / 2;
const int maxValue = 255;
const std::string windowCaptureName = "Video Capture";
const std::string windowDetectionName = "Object Detection";
int lowH = 68, lowS = 110, lowV = 171;
int highH = 93, highS = maxValue, highV = maxValue;

//Variables for camera
int width;
int height;

//Variables for areabar
int areaSliderMin = 1000;
int areaSliderMax = 70000;



void areaBar(int, void*) {
	params.minDistBetweenBlobs = 1.0;    //Minimum 1 pixel between blobs
	params.filterByArea = true;            //Checking for area
	params.filterByColor = false;        //We're doing a binary detection, we don't want color
	params.minArea = areaSliderMin;        //Minimum value of the area
	params.maxArea = areaSliderMax;        //Maximum value of the area
	params.thresholdStep = 100;            //Slider steps
	params.blobColor = 0;                //Color we're checking for
	params.filterByCircularity = false;    //We dont check for circularity
	params.filterByInertia = false;        //We dont check for Intertia
	params.filterByConvexity = false;    //We dont check for convexity

	//Creating a detector with the settings above
	 detector = cv::SimpleBlobDetector::create(params);

}

static void onLowHThreshTrackbar(int, void *)
{
	lowH = cv::min(highH - 1, lowH);
	cv::setTrackbarPos("Low H", windowDetectionName, lowH);
}
static void onHighHThreshTrackbar(int, void *)
{
	highH = cv::max(highH, lowH + 1);
	cv::setTrackbarPos("High H", windowDetectionName, highH);
}
static void onLowSThreshTrackbar(int, void *)
{
	lowS = cv::min(highS - 1, lowS);
	cv::setTrackbarPos("Low S", windowDetectionName, lowS);
}
static void onHighSThreshTrackbar(int, void *)
{
	highS = cv::max(highS, lowS + 1);
	cv::setTrackbarPos("High S", windowDetectionName, highS);
}
static void onLowVThreshTrackbar(int, void *)
{
	lowV = cv::min(highV - 1, lowV);
	cv::setTrackbarPos("Low V", windowDetectionName, lowV);
}
static void onHighVThreshTrackbar(int, void *)
{
	highV = cv::max(highV, lowV + 1);
	cv::setTrackbarPos("High V", windowDetectionName, highV);
}

int checkBounds(cv::Point point1, cv::Point point2) {
	int returnValue = 0;
	if (markerPosition.x >= point1.x && markerPosition.x <= point2.x && markerPosition.y >= point1.y && markerPosition.y <= point2.y) {
		std::cout << "point is in bounds" << std::endl;
		returnValue = 1;
	}

	return returnValue;
}

int main(int argc, const char** argv)
{

	params.minDistBetweenBlobs = 1.0;    //Minimum 1 pixel between blobs
	params.filterByArea = true;            //Checking for area
	params.filterByColor = false;        //We're doing a binary detection, we don't want color
	params.minArea = areaSliderMin;        //Minimum value of the area
	params.maxArea = areaSliderMax;        //Maximum value of the area
	params.thresholdStep = 100;            //Slider steps
	params.blobColor = 0;                //Color we're checking for
	params.filterByCircularity = false;    //We dont check for circularity
	params.filterByInertia = false;        //We dont check for Intertia
	params.filterByConvexity = false;    //We dont check for convexity

	//Creating a detector with the settings above
	detector = cv::SimpleBlobDetector::create(params);


	cv::namedWindow(windowCaptureName);
	cv::namedWindow(windowDetectionName);

	// Trackbars to set thresholds for HSV values
	cv::createTrackbar("Low H", windowDetectionName, &lowH, maxValueH, onLowHThreshTrackbar);
	cv::createTrackbar("High H", windowDetectionName, &highH, maxValueH, onHighHThreshTrackbar);
	cv::createTrackbar("Low S", windowDetectionName, &lowS, maxValue, onLowSThreshTrackbar);
	cv::createTrackbar("High S", windowDetectionName, &highS, maxValue, onHighSThreshTrackbar);
	cv::createTrackbar("Low V", windowDetectionName, &lowV, maxValue, onLowVThreshTrackbar);
	cv::createTrackbar("High V", windowDetectionName, &highV, maxValue, onHighVThreshTrackbar);

	cv::createTrackbar("area", windowDetectionName, &areaSliderMin, areaSliderMax, areaBar);


	cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
		std::cerr << "No camera detected on this system" << std::endl;
		return -1;
	}

	width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	while (true) {
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
		// Show the frames
		imshow(windowCaptureName, frame);
		imshow(windowDetectionName, frame_threshold);
		

		//Detecting the blobs
		detector->detect(frame_threshold, myBlobs);

		//Drawing keypoints (red circles)
		drawKeypoints(frame_threshold, myBlobs, blobImg, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

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
		//For text drawing purposes
		for (cv::KeyPoint k : myBlobs)
		{
			putText(blobImg, std::to_string(k.size), cv::Point(k.pt.x, k.pt.y), cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 255, 0), 2.0);
		}

		//Draw Horizontal raster
		cv::line(blobImg, cv::Point(0, height/SCREEN_DIVIDER_RATIO), cv::Point(width,height/SCREEN_DIVIDER_RATIO), CV_RGB(255, 255, 255), 2);
		cv::line(blobImg, cv::Point(0, height /SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), cv::Point(width, height /SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), CV_RGB(255, 255, 255), 2);

		//Draw Vertical raster
		cv::line(blobImg, cv::Point(width / SCREEN_DIVIDER_RATIO, 0), cv::Point(width / SCREEN_DIVIDER_RATIO, height ), CV_RGB(255, 255, 255), 2);
		cv::line(blobImg, cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, 0), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height), CV_RGB(255, 255, 255), 2);


		//Check left bound
		if (checkBounds(cv::Point(width / SCREEN_DIVIDER_RATIO, 0), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO)) == 1) {
			cv::rectangle(blobImg, cv::Point(width / SCREEN_DIVIDER_RATIO, 0), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO), CV_RGB(255, 0, 0), 5, 1, 0);
		}

		//Check Upper bound
		if (checkBounds(cv::Point(0, height / SCREEN_DIVIDER_RATIO), cv::Point(width / SCREEN_DIVIDER_RATIO, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START)) == 1) {
			cv::rectangle(blobImg, cv::Point(0, height/5), cv::Point(width/ SCREEN_DIVIDER_RATIO, height/ SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), CV_RGB(255, 0, 0), 5, 1, 0);
		}

		//Check right bound
		if (checkBounds(cv::Point(width/ SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO), cv::Point(width, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START)) == 1) {
			cv::rectangle(blobImg, cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO), cv::Point(width, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), CV_RGB(255, 0, 0), 5, 1, 0);
		}

		//Check lower bound
		if (checkBounds(cv::Point(width/5, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height)) == 1) {
			cv::rectangle(blobImg, cv::Point(width / SCREEN_DIVIDER_RATIO, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height), CV_RGB(255, 0, 0), 5, 1, 0);
		}

		//Check middle bound
		if (checkBounds(cv::Point(width / SCREEN_DIVIDER_RATIO, height / SCREEN_DIVIDER_RATIO), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height/ SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START)) == 1) {
			cv::rectangle(blobImg, cv::Point(width / SCREEN_DIVIDER_RATIO, height / SCREEN_DIVIDER_RATIO), cv::Point(width / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START, height / SCREEN_DIVIDER_RATIO * SCREEN_RIGHT_SIDE_BOUND_START), CV_RGB(255, 0, 0), 5, 1, 0);
		}
		
		
		//Showing the text
		cv::imshow("binair beeld", blobImg);

		//imshow("test", frame);
		cv::waitKey(5);
	}

	return 0;
}