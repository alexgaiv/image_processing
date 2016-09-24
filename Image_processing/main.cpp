

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include "Histogram.h"
using namespace std;
using namespace cv;

int main() {
	Mat image;
	string filename = "test_Image.jpg";
	image = imread(filename);
	Histogram ImageHist(image);

	ImageHist.smooth(5);

	ImageHist.searchLocalMax();
	ImageHist.printLocalMax();
	
	ImageHist.searchLocalMin();
	ImageHist.printLocalMin();


	
	ImageHist.peakAnalyse();

	ImageHist.segmentation();

	showImage(&image);
	
	exit(0);
}



uchar calculateIntensity(Vec3b &pixel) {
	uchar tmp = (int)(pixel[0] * 0.299 + pixel[1] * 0.587 + pixel[2] * 0.184);
	if (tmp < 0) tmp = 0;
	if (tmp > 255) tmp = 255;
	return(tmp);
}

void showImage(Mat* image) {
	static char windowname[] = "Show";
	imshow(windowname, *image);
	cvWaitKey();
	return;
}