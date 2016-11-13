
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
#include "BiLinear.h"
#include "watershed.h"

using namespace std;
using namespace cv;

int main() {
	
	

	Mat image_interpolation = imread("2.jpg");
	if (!image_interpolation.data) return false;
	//blur(image, image, Size(3, 3), Point(-1, -1), 4);

	Interpolation interpolation(&image_interpolation, 3);
	interpolation.resample();
	interpolation.showResult();

	Mat image = imread("2.jpg");
	imshow("Input", image);
	if (!image.data) return false;
	//*
	Watershed a;
	//image = a.preProccess(image, 3, 12);
	//cvtColor(image, image, CV_GRAY2RGB);
	WatershedTobo watershed(&image);
	watershed.segmentation();
	imshow("Result", image);
	cvWaitKey();
	/*/
	Watershed watershedVincentShoille;
	Mat result = watershedVincentShoille.processImage(image, 3, 12);
	imshow("Result", result);
	cvWaitKey();
	//*/
	return 0;
}