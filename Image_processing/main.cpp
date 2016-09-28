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
	if (!image.data) return false;

	Watershed watershed;
	Mat result = watershed.processImage(image, 3, 12);
	imshow("Result", result);
	cvWaitKey();

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