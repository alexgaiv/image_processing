#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>

#define THRESHOLD 0.5
#define lowPoint 7000

/*
TODO:
-which peaks are bad?
-cleaning from bad peaks
-calculate threshold
-cleaning peaks under threshold
*/


int calculateIntensity(cv::Vec3b &pixel);
void showImage(cv::Mat* image);

class Histogram {
public:
	int hist[256];
	std::vector<int> vecMax;
	std::vector<int> vecMin;

	std::vector<int> intervals;

	Histogram(cv::Mat* image) {
		for (int i = 0; i < 254; i++) { hist[i] = 0; }
		for (int i = 0; i < image->cols; i++) {
			for (int j = 0; j < image->rows; j++) {
				cv::Vec3b pixColor = image->at<cv::Vec3b>(cv::Point(i, j));
				int intensity = calculateIntensity(pixColor);
				hist[intensity]++;
			}
		}
	}

	~Histogram() {

	}
	void showHistorgam() const {
		int hist_w = 300;
		int hist_h = 200;
		int histSize = 255;

		cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
		for (int i = 0; i < histSize - 1; i++){
			line(histImage, cv::Point(i, hist_h), cv::Point(i, hist_h - hist[i] / histSize), cv::Scalar(255, 255, 255), 1, 8, 0);
		}
		showImage(&histImage);
	}

	void searchLocalMax() {
		if (hist[0] > hist[1])
			vecMax.push_back(0);
		for (int i = 1; i < 255; i++) {
			if (hist[i] > hist[i - 1] && hist[i] >= hist[i + 1])
				vecMax.push_back(i);
		}
		if (hist[255] > hist[254])
			vecMax.push_back(255);
	}

	void printLocalMax() const {
		std::cout << "Local maximas:\n";
		for (int i : vecMax) {
			std::cout << i << std::endl;
		}
	}
	
	void searchLocalMin() {
		if (hist[0] < hist[1])
			vecMin.push_back(0);
		for (int i = 1; i < 255; i++) {
			if (hist[i] < hist[i - 1] && hist[i] <= hist[i + 1])
				vecMin.push_back(i);
		}
		if (hist[255] < hist[254])
			vecMin.push_back(255);
	}

	void printLocalMin() const {
		std::cout << "Local minimas:\n";
		for (int i : vecMin) {
			std::cout << i << std::endl;
		}
	}

	double calculatePeakMeasure(int &peakNum) const {
		double peakMesure = 0.0;
		double square = 0.0;
		if (peakNum == 0) {
			for (int i = peakNum; i <= vecMin[0]; i++) {
				square += hist[i];
			}
			peakMesure = (1 - hist[vecMin[0]] / (2 * hist[peakNum]))*(1 - square / ((vecMin[0] - peakNum+1)*hist[peakNum]));
		}
		if (peakNum == 255) {
			for (int i = vecMin[vecMin.size() - 1]; i <= peakNum; i++) {
				square += hist[i];
			}
			peakMesure = (1 - hist[vecMin[vecMin.size() - 1]] / (2 * hist[peakNum]))*(1 - square / ((peakNum - vecMin[vecMin.size() - 1]+1)*hist[peakNum]));
		}
		if (peakNum > 0 && peakNum < 255) {
			int leftMin = 0;
			int rightMin = 0;
			while (peakNum > vecMin[leftMin + 1]) {
				leftMin++;
			}
			rightMin = leftMin + 1;
			for (int i = vecMin[leftMin]; i <= vecMin[rightMin]; i++) {
				square += hist[i];
			}
			peakMesure = (1 - (hist[vecMin[leftMin]] + hist[vecMin[rightMin]]) / (2 * hist[peakNum]))*(1 - square / ((vecMin[rightMin] - vecMin[leftMin]+1)*hist[peakNum]));
		}
		std::setprecision(3);
		return peakMesure;
	}

	void peakAnalyse() {
		double measure = 0.0;
		for (int i = 0; i < vecMax.size(); i++) {
			if (calculatePeakMeasure(vecMax[i]) < THRESHOLD) {
				vecMax.erase(vecMax.begin()+i);
			}
		}

		for (int i = 0; i < vecMax.size()-1; i++){
			int minI = vecMax[i];
			for (int j = vecMax[i]; j < vecMax[i + 1]; j++){
				if (hist[minI] > hist[j])
					minI = j;
			}
			intervals.push_back(minI);
		}
	}

	void smooth(int numPasses) { // сглаживание (см. презентацию)
		int newHist[256] = {};

		for (int k = 0; k < numPasses; k++)
		{
			for (int i = 1; i < 255; i++) {
				newHist[i] = (hist[i - 1] + hist[i] + hist[i + 1]) / 3;
			}
			newHist[0] = (hist[0] + hist[1]) / 2;
			newHist[255] = (hist[254] + hist[255]) / 2;
			memcpy(hist, newHist, 256 * sizeof(int));
		}
	}
};



int main() {


	cv::Mat image;
	std::string filename = "test_Image.jpg";
	image = cv::imread(filename);
	Histogram ImageHist(&image);

	ImageHist.smooth(5);

	ImageHist.searchLocalMax();
	ImageHist.printLocalMax();
	
	ImageHist.searchLocalMin();
	ImageHist.printLocalMin();


	
	ImageHist.peakAnalyse();
	srand(time(NULL));
	std::vector<cv::Vec3b> colors;
	for (int i = 0; i < ImageHist.intervals.size(); i++){
		
		cv::Vec3b p;
		p[0] = rand() % 255;
		p[1] = rand() % 255;
		p[1] = rand() % 255;
		colors.push_back(p);
	}

	for (int i = 0; i < image.cols; i++) {
		for (int j = 0; j < image.rows; j++) {
			cv::Vec3b pixColor = image.at<cv::Vec3b>(cv::Point(i, j));

			int intensity = calculateIntensity(pixColor);
			int k;
			for (k = 0; k < ImageHist.intervals.size() - 1; k++){
				if (intensity > ImageHist.intervals[k] && intensity < ImageHist.intervals[k + 1]){
					break;
				}
			}
			image.at<cv::Vec3b>(cv::Point(i, j)) = colors[k];
		}
	}

	showImage(&image);
	
	exit(0);
}



int calculateIntensity(cv::Vec3b &pixel) {
	int tmp = (int)(pixel[0] * 0.299 + pixel[1] * 0.587 + pixel[2] * 0.184);
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