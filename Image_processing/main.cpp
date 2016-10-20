
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

using namespace std;
using namespace cv;

int main() {
	Mat image = imread("test_Image.jpg");
	imshow("in", image);

	//blur(image, image, Size(3, 3), Point(-1, -1), 4);

	Interpolation interpolation(&image, 3);
	interpolation.resample();
	interpolation.showResult();

	return 0;
}