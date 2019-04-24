#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

struct Points {
	float x, y;
};
//Global variables
Mat frame, frame_HSV, frame_threshold;
Point markerPosition;

//Variables for HSV
const int max_value_H = 360 / 2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
int low_H = 0, low_S = 200, low_V = 115;
int high_H = 180, high_S = max_value, high_V = max_value;

//Variables for camera
int width;
int height;

//Variables for areabar
int areaSliderMin = 1000;
int areaSliderMax = 70000;

cv::Ptr<cv::SimpleBlobDetector> detector;
cv::Mat blobImg;
cv::SimpleBlobDetector::Params params;
std::vector<cv::KeyPoint> myBlobs;
cv::Mat inputImg;

void areaBar(int, void*) {
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

static void on_low_H_thresh_trackbar(int, void *)
{
	low_H = min(high_H - 1, low_H);
	setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
	high_H = max(high_H, low_H + 1);
	setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
	low_S = min(high_S - 1, low_S);
	setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
	high_S = max(high_S, low_S + 1);
	setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
	low_V = min(high_V - 1, low_V);
	setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
	high_V = max(high_V, low_V + 1);
	setTrackbarPos("High V", window_detection_name, high_V);
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
	params.filterByCircularity = true;    //We dont check for circularity
	params.filterByInertia = false;        //We dont check for Intertia
	params.filterByConvexity = true;    //We dont check for convexity
	params.minConvexity = 0.5;
	params.minCircularity = 0.15;


	//Creating a detector with the settings above
	detector = cv::SimpleBlobDetector::create(params);


	namedWindow(window_capture_name);
	namedWindow(window_detection_name);

	// Trackbars to set thresholds for HSV values
	createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
	createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
	createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
	createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
	createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
	createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);

	createTrackbar("area", window_detection_name, &areaSliderMin, areaSliderMax, areaBar);


	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cerr << "No camera detected on this system" << endl;
		return -1;
	}

	width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	while (true) {
		cap >> frame;
		if (frame.empty()) {
			cerr << "Frame invalid and skipped!" << endl;
			continue;
		}
		flip(frame, frame, 1);


		// Convert from BGR to HSV colorspace
		cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
		// Detect the object based on HSV Range Values
		inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
		// Show the frames
		imshow(window_capture_name, frame);
		imshow(window_detection_name, frame_threshold);
		

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

		cv::line(blobImg, cv::Point(0, height/5), cv::Point(width,height/5), CV_RGB(255, 255, 255), 2);
		cv::line(blobImg, cv::Point(0, height /5 * 4), cv::Point(width, height / 5 * 4), CV_RGB(255, 255, 255), 2);

		cv::line(blobImg, cv::Point(width / 5, 0), cv::Point(width / 5, height ), CV_RGB(255, 255, 255), 2);
		cv::line(blobImg, cv::Point(width / 5 * 4, 0), cv::Point(width / 5 * 4, height), CV_RGB(255, 255, 255), 2);

		if (checkBounds(cv::Point(width / 5, 0), cv::Point(width / 5 * 4, height / 5)) == 1) {
			cv::rectangle(blobImg, cv::Point(width / 5, 0), cv::Point(width / 5 * 4, height / 5), CV_RGB(255, 0, 0), 5, 1, 0);
		}
		if (checkBounds(cv::Point(0, height / 5), cv::Point(width / 5, height / 5 * 4)) == 1) {
			cv::rectangle(blobImg, cv::Point(0, height/5), cv::Point(width/5, height/5*4), CV_RGB(255, 0, 0), 5, 1, 0);
		}
		if (checkBounds(cv::Point(width/5*4, height / 5), cv::Point(width, height / 5 * 4)) == 1) {
			cv::rectangle(blobImg, cv::Point(width / 5 * 4, height / 5), cv::Point(width, height / 5 * 4), CV_RGB(255, 0, 0), 5, 1, 0);
		}
		if (checkBounds(cv::Point(width/5, height / 5*4), cv::Point(width / 5*4, height)) == 1) {
			cv::rectangle(blobImg, cv::Point(width / 5, height / 5 * 4), cv::Point(width / 5 * 4, height), CV_RGB(255, 0, 0), 5, 1, 0);
		}
		if (checkBounds(cv::Point(width / 5, height / 5), cv::Point(width / 5 * 4, height/5*4)) == 1) {
			cv::rectangle(blobImg, cv::Point(width / 5, height / 5), cv::Point(width / 5 * 4, height / 5 * 4), CV_RGB(255, 0, 0), 5, 1, 0);
		}
		
		
		//Showing the text
		cv::imshow("binair beeld", blobImg);

		//imshow("test", frame);
		waitKey(5);
	}

	return 0;
}