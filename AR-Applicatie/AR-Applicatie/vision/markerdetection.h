
	//Global defines
	const int MARKERDETECTION_WITH_OPENCV = 0;
	const int MARKERDETECTION_WITH_MOUSE = 1;

	struct Point2D {
		float x, y;
	};

	//Global functions
	int runMarkerDetection(int input);
	void terminateDetection();
	int getDetectionMode();
	void changeDetectionMode();
	Point2D getCoordinates();
