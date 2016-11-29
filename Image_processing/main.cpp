
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <cassert>
#include <ctime>
#include <iomanip>
#include "Histogram.h"
#include "WatershedTobogganing.h"
#include "BiLinear.h"
#include "Huffman.h"


#include "ImageCompressor.h"
#include "Watershed.h"

using namespace std;
using namespace cv;

int main() {

	Mat image_compressor = imread("test_images/test_Image3.jpg");
	imshow("in", image_compressor);
	ImageCompressor c;
	c.Compress(image_compressor);

	getchar();
	return 0;
}