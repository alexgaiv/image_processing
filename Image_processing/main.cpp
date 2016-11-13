
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


	Mat image1 = imread("2.jpg");
	imshow("first in", image1);

	WatershedTobo watershed(&image1);
	watershed.segmentation();
	imshow("Result", image1);
	cvWaitKey();


	Mat image = imread("test_Image.jpg");
	imshow("in", image);

	Interpolation interpolation(&image, 3);
	interpolation.resample();
	interpolation.showResult();

	return 0;
}