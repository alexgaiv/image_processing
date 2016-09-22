#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <iomanip>

int calcIntensity(cv::Vec3b &pixel);
void showImage(cv::Mat* image);

class Histogram {
public:
	int hist[256];
	std::vector<int> vec;
	Histogram(cv::Mat* image) {
		for (int i = 0; i < 254; i++) { hist[i] = 0; }
		for (int i = 0; i < image->cols; i++) {
			for (int j = 0; j < image->rows; j++) {
				cv::Vec3b pixColor = image->at<cv::Vec3b>(cv::Point(i, j));
				int intensity = calcIntensity(pixColor);
				hist[intensity]++;
			}
		}
	}
	void showHistorgam() {
		int hist_w = 300;
		int hist_h = 200;
		int histSize = 255;

		cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
		for (int i = 0; i < histSize - 1; i++){
			line(histImage, cv::Point(i, hist_h), cv::Point(i, hist_h - hist[i] / histSize), cv::Scalar(255, 255, 255), 1, 8, 0);
		}
		showImage(&histImage);
	}
	void searchLocalMax(){
		if (hist[0]>hist[1])
			vec.push_back(hist[0]);
		for (int i = 1; i < 255; i++){
			if (hist[i]>hist[i - 1] && hist[i]>hist[i + 1])
				vec.push_back(hist[i]);
		}
		if (hist[255] > hist[254])
			vec.push_back(hist[255]);
	}
	void printLocalMax(){
		for (int i : vec){
			std::cout<< i << std::endl;
		}
	}
	
};



int main() {
	cv::Mat image;
	std::string filename = "test_Image.jpg";
	image = cv::imread(filename);
	//showImage(&image);
	Histogram ImageHist(&image);
	//ImageHist.showHistorgam();
	ImageHist.searchLocalMax();
	ImageHist.printLocalMax();

	exit(0);
}

int calcIntensity(cv::Vec3b &pixel) {
	int tmp = (int)(pixel[0] * 0.184 + pixel[1] * 0.587 + pixel[2] * 0.299);
	if (tmp < 0) tmp = 0;
	if (tmp > 255) tmp = 255;
	return(tmp);
}

void showImage(cv::Mat* image) {
	static char windowname[] = "Show";
	cv::imshow(windowname, *image);
	cvWaitKey();
	return;
}