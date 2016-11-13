
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

	/*/
	Watershed watershedVincentShoille;
	Mat result = watershedVincentShoille.processImage(image, 3, 12);
	imshow("Result", result);
	cvWaitKey();
	//*/
	Mat image_compressor = imread("4.2.07.jpg");
	imshow("in", image_compressor);
	ImageCompressor c;
	c.Compress(image_compressor);

	return 0;
}