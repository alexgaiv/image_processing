
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
#include "watershed.h"

using namespace std;
using namespace cv;

int main() {
	Mat image = imread("test_Image3.jpg");
	if (!image.data) return false;
	//blur(image, image, Size(3, 3), Point(-1, -1), 4);

	WatershedTobo watershed(&image);
	watershed.segmentation();
	imshow("Result", image);

	Watershed watershedVincentShoille;
	Mat result = watershedVincentShoille.processImage(image, 3, 12);
	imshow("Result", result);
	cvWaitKey();
	return 0;
}