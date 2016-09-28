#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include "Histogram.h"
#include "watershed.h"
using namespace std;
using namespace cv;

int main() {

	Mat image = imread("test_Image3.png");
	cvtColor(image, image, CV_BGR2GRAY);
	/* todo:
	compute image gradient
	compute final segmented image from WatershedStructure::pixels
	understand the algorithm
	*/

	WatershedAlgorithm watershed;
	watershed.run(image);

	/*
	Mat image;
	string filename = "test_Image.jpg";
	Histogram ImageHist(filename);

	ImageHist.smooth(5);

	ImageHist.searchLocalMax();
	ImageHist.printLocalMax();

	ImageHist.searchLocalMin();
	ImageHist.printLocalMin();



	ImageHist.peakAnalyse();

	ImageHist.segmentation();

	ImageHist.showImage();
	ImageHist.showHistorgam();
	*/

	return 0;
}