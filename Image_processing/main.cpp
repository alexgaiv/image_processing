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
	ImageHist.showHistorgam();
	exit(0);
}