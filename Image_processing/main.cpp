
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <cassert>
#include <ctime>
#include <iomanip>
#include "Histogram.h"
#include "WatershedTobogganing.h"
#include "BiLinear.h"


#include "ImageCompressor.h"
#include "watershed.h"

using namespace std;
using namespace cv;

int main() {

	Mat image_compressor = imread("1.jpg");
	imshow("in", image_compressor);
	ImageCompressor c;
	c.Compress(image_compressor);

	return 0;
}