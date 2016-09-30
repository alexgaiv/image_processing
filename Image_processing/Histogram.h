/*
THAT CODE WAS MADE BY POVELIKIN ROSTISLAV, GRECHUHIN NIKITA, GAIVAN'UK ALEXANDER
*/

#ifndef HISTOGRAM
#define HISTOGRAM

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
using namespace std;
using namespace cv;

//TODO: make a constructor with string of filename!

#define THRESHOLD 0.5

//Uchar is 0 to 255 range. That's the best to use RGB model
uchar calculateIntensity(Vec3b &pixel);

class Histogram {
	int hist[256];
	vector<int> vecMax;
	vector<int> vecMin;
	vector<int> intervals;
	Mat image;
public:
	Histogram(Mat &image);

	~Histogram() { }

	void showImage() const;

	void showHistorgam() const;

	void searchLocalMax();

	void printLocalMax() const;

	void searchLocalMin();

	void printLocalMin() const;

	float calculatePeakMeasure(int &peakNum) const;

	void peakAnalyse();

	void smooth(const int &numPasses);

	void segmentation();
};

#endif 