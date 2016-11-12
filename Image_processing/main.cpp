
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
#include "ImageCompressor.h"

using namespace std;
using namespace cv;

int main() {
	Mat image = imread("4.2.07.jpg");
	imshow("in", image);

	/*Interpolation interpolation(image, 3);
	interpolation.resample();
	interpolation.showResult();*/

	ImageCompressor c;
	c.Compress(image);

	return 0;
}