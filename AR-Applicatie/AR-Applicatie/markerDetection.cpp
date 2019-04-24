//#include <opencv2/opencv.hpp>
//
//using namespace cv;
//using namespace std;
//
//int main(int argc, const char** argv)
//{
//	Mat frame;
//
//	VideoCapture cap(0);
//	if (!cap.isOpened()) {
//		cerr << "No camera detected on this system" << endl;
//		return -1;
//	}
//
//	while (true) {
//		cap >> frame;
//		if (frame.empty()) {
//			cerr << "Frame invalid and skipped!" << endl;
//			continue;
//		}
//		imshow("test", frame);
//		waitKey(5);
//	}
//
//	return 0;
//}