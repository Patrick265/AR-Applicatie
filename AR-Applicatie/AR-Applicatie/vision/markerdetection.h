
	//Global defines
	const int MARKERDETECTION_WITH_OPENCV = 0;
	const int MARKERDETECTION_WITH_MOUSE = 1;

	struct Point2D {
		float x, y;
	};

//Global functions

/*
//	This function starts the openCV module.
//
//	@param int input is for switching the mode between mouse and openCV
//
*/
int runMarkerDetection(int input);

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
int getDetectionMode();

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
