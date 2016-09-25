#include "Histogram.h"

uchar calculateIntensity(Vec3b &pixel) {
	uchar tmp = (int)(pixel[0] * 0.299 + pixel[1] * 0.587 + pixel[2] * 0.184);
	if (tmp < 0) tmp = 0;
	if (tmp > 255) tmp = 255;
	return(tmp);
}

void Histogram::showImage() const {
	static char windowname[] = "Show";
	imshow(windowname, image);
	cvWaitKey();
	return;
}

Histogram::Histogram(Mat &image) {
	this->image = image;
	for (int i = 0; i < 256; i++) { hist[i] = 0; }
	for (int i = 0; i < this->image.cols; i++) {
		for (int j = 0; j < this->image.rows; j++) {
			Vec3b pixColor = this->image.at<Vec3b>(Point(i, j));
			int intensity = calculateIntensity(pixColor);
			hist[intensity]++;
		}
	}
}

void Histogram::showHistorgam() const {
	int hist_w = 300;
	int hist_h = 200;
	int histSize = 255;

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	for (int i = 0; i < histSize - 1; i++) {
		line(histImage, Point(i, hist_h), Point(i, hist_h - hist[i] / histSize), Scalar(255, 255, 255), 1, 8, 0);
	}
	imshow("Histohram", histImage);
	cvWaitKey();
}

void Histogram::searchLocalMax() {
	int l = 0, r = 255;
	while (hist[l] == 0) l++;
	while (hist[r] == 0) r--;
	if (hist[l] > hist[l + 1])
		vecMax.push_back(l);
	for (int i = 1; i < 255; i++) {
		if (hist[i] > hist[i - 1] && hist[i] >= hist[i + 1])
			vecMax.push_back(i);
	}
	if (hist[255] > hist[254])
		vecMax.push_back(255);
	if (hist[r] > hist[r - 1])
		vecMax.push_back(r);
}

void Histogram::printLocalMax() const {
	cout << "Local maximas:\n";
	for (int i : vecMax) {
		cout << i << endl;
	}
}

void Histogram::searchLocalMin() {
	int l = 0, r = 255;
	while (hist[l] == 0) l++;
	while (hist[r] == 0) r--;

	if (hist[l] < hist[l + 1])
		vecMin.push_back(l);
	for (int i = l + 1; i < r - 1; i++) {
		if (hist[i] < hist[i - 1] && hist[i] <= hist[i + 1])
			vecMin.push_back(i);
	}
	if (hist[r] < hist[r - 1])
		vecMin.push_back(r);
}

void Histogram::printLocalMin() const {
	cout << "Local minimas:\n";
	for (int i : vecMin) {
		cout << i << endl;
	}
}

float Histogram::calculatePeakMeasure(int &peakNum) const {
	double peakMesure = 0.0;
	double square = 0.0;
	if (peakNum == 0) {
		for (int i = peakNum; i <= vecMin[0]; i++) {
			square += hist[i];
		}
		peakMesure = (1 - hist[vecMin[0]] / (2 * hist[peakNum]))*(1 - square / ((vecMin[0] - peakNum + 1)*hist[peakNum]));
	}
	if (peakNum == 255) {
		for (int i = vecMin[vecMin.size() - 1]; i <= peakNum; i++) {
			square += hist[i];
		}
		peakMesure = (1 - hist[vecMin[vecMin.size() - 1]] / (2 * hist[peakNum]))*(1 - square / ((peakNum - vecMin[vecMin.size() - 1] + 1)*hist[peakNum]));
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
		peakMesure = (1 - (hist[vecMin[leftMin]] + hist[vecMin[rightMin]]) / (2 * hist[peakNum]))*(1 - square / ((vecMin[rightMin] - vecMin[leftMin] + 1)*hist[peakNum]));
	}
	setprecision(3);
	return peakMesure;
}

void Histogram::peakAnalyse() {
	double measure = 0.0;
	for (int i = 0; i < vecMax.size(); i++) {
		if (calculatePeakMeasure(vecMax[i]) < THRESHOLD) {
			vecMax.erase(vecMax.begin() + i);
		}
	}

	for (int i = 0; i < vecMax.size() - 1; i++) {
		int minI = vecMax[i];
		for (int j = vecMax[i]; j < vecMax[i + 1]; j++) {
			if (hist[minI] > hist[j])
				minI = j;
		}
		intervals.push_back(minI);
	}
}

void Histogram::smooth(const int &numPasses) {
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

void Histogram::segmentation() {
	srand(time(NULL));
	vector<Vec3b> colors;
	for (int i = 0; i < intervals.size(); i++) {

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
			for (k = 0; k < intervals.size() - 1; k++) {
				if (intensity > intervals[k] && intensity < intervals[k + 1]) {
					break;
				}
			}
			image.at<Vec3b>(Point(i, j)) = colors[k];
		}
	}
}