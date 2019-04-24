#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//Global variables
Mat frame, frame_HSV, frame_threshold;

//Variables for HSV
const int max_value_H = 360 / 2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
int low_H = 68, low_S = 110, low_V = 171;
int high_H = 93, high_S = max_value, high_V = max_value;

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
	params.filterByCircularity = false;    //We dont check for circularity
	params.filterByInertia = false;        //We dont check for Intertia
	params.filterByConvexity = false;    //We dont check for convexity

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

	while (true) {
		cap >> frame;
		if (frame.empty()) {
			cerr << "Frame invalid and skipped!" << endl;
			continue;
		}


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

		//For text drawing purposes
		for (cv::KeyPoint k : myBlobs)
		{
			putText(blobImg, std::to_string(k.size), cv::Point(k.pt.x, k.pt.y), cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 255, 0), 2.0);
		}

		//Showing the text
		cv::imshow("binair beeld", blobImg);

		//imshow("test", frame);
		waitKey(5);
	}

	return 0;
}