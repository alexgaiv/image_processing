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

#define THRESHOLD 0.5

//Uchar is 0 to 255 range. That's the best to use for RGB model
uchar calculateIntensity(Vec3b &pixel);
void showImage(Mat* image);

class Histogram {
public:
	int hist[256];
	vector<int> vecMax;
	vector<int> vecMin;
	vector<int> intervals;

	Histogram(const Mat &image);

	~Histogram() { }

	void showHistorgam() const;

	void searchLocalMax();

	void printLocalMax() const;

	void searchLocalMin();

	void printLocalMin() const;

	double calculatePeakMeasure(int &peakNum) const;

	void peakAnalyse();

	void smooth(int numPasses);
};

#endif 
