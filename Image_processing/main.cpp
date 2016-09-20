#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <iostream>
#include <iomanip>

int calcIntensity(cv::Vec3b &pixel) {
	int tmp = (int)(pixel[0] * 0.184 + pixel[1] * 0.587 + pixel[2] * 0.299);
	if (tmp < 0) tmp = 0;
	if (tmp > 255) tmp = 255;
	return(tmp);
}

class histogram {
public:
	float hist[255];
	histogram(cv::Mat* image) {
		for (int i = 0; i < 254; i++) { hist[i] = 0; }
		for (int i = 0; i < image->cols; i++) {
			for (int j = 0; j < image->rows; j++) {
				cv::Vec3b pixColor = image->at<cv::Vec3b>(cv::Point(i, j));
				int intensity = calcIntensity(pixColor);
				hist[intensity]++;
			}
		}
		int pixelAmount = image->rows*image->cols;
		/*for (int i = 0; i < 254; i++) { 
			std::setprecision(2);
			hist[i] /= pixelAmount;
			hist[i] *= 100;
		}
		*/
	}
	void showHistorgam() {
		for (int i = 0; i < 254; i++) { 
			std::cout << "intensity " << i << ": " << hist[i] << std::endl;
		}
	}
};

void showImage(cv::Mat* image) {
	static char windowname[] = "Show";
	cv::imshow(windowname, *image);
	cvWaitKey();
	return;
}

int main() {
	cv::Mat image;
	std::string filename = "E:/! Important/! Other information/THE CHOICE/SUKHAN AUTUMN 2016/899898.PNG";
	image = cv::imread(filename);
	//showImage(&image);
	histogram ImageHist(&image);
	ImageHist.showHistorgam();
}