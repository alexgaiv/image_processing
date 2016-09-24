/*
THAT CODE WAS MADE BY POVELIKIN ROSTISLAV, GRECHUHIN NIKITA, GAIVAN'UK ALEXANDER
*/

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
	std::string filename = "test_Image.jpg";
	image = imread(filename);
	Histogram ImageHist(image);

	ImageHist.smooth(5);

	ImageHist.searchLocalMax();
	ImageHist.printLocalMax();
	
	ImageHist.searchLocalMin();
	ImageHist.printLocalMin();


	
	ImageHist.peakAnalyse();
	srand(time(NULL));
	std::vector<Vec3b> colors;
	for (int i = 0; i < ImageHist.intervals.size(); i++){
		
		Vec3b p;
		p[0] = rand() % 255;
		p[1] = rand() % 255;
		p[2] = rand() % 255;
		colors.push_back(p);
	}

	for (int i = 0; i < image.cols; i++) {
		for (int j = 0; j < image.rows; j++) {
			Vec3b pixColor = image.at<Vec3b>(Point(i, j));

			int intensity = calculateIntensity(pixColor);
			int k;
			for (k = 0; k < ImageHist.intervals.size() - 1; k++){
				if (intensity > ImageHist.intervals[k] && intensity < ImageHist.intervals[k + 1]){
					break;
				}
			}
			image.at<Vec3b>(Point(i, j)) = colors[k];
		}
	}

	showImage(&image);
	
	exit(0);
}



uchar calculateIntensity(Vec3b &pixel) {
	uchar tmp = (int)(pixel[0] * 0.299 + pixel[1] * 0.587 + pixel[2] * 0.184);
	if (tmp < 0) tmp = 0;
	if (tmp > 255) tmp = 255;
	return(tmp);
}

void showImage(Mat* image) {
	static char windowname[] = "Show";
	imshow(windowname, *image);
	cvWaitKey();
	return;
}