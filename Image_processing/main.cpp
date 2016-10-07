
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include <ctime>
#include <iomanip>
#include "Histogram.h"
#include "WatershedTobogganing.h"
using namespace std;
using namespace cv;

int main() { 
	Mat image;
	string filename = "4.2.07.jpg";
	image = imread(filename);
	Mat dst = image;
	
	//blur(image, dst, Size(3, 3));
	//medianBlur(dst, dst, 3);
	
	Watershed watershed(&dst);

	watershed.segmentation();

	static char windowname[] = "Show";
	imshow(windowname, dst);
	cvWaitKey();

	exit(0);
	return 0;
}