/*
THAT CODE WAS MADE BY POVELIKIN ROSTISLAV, GRECHUHIN NIKITA, GAIVAN'UK ALEXANDER
*/

#ifndef WATERSHED
#define WATERSHED
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

int Clamp(int value, int min, int max);

struct PointWithMin{
public:
	int x;
	int y;
	Vec3b min;
	Vec3b color;
};

class Watershed{
private:
	Mat image;
	PointWithMin *segments;
	int rows;
	vector<Vec3b> colors;
public:
	Watershed(Mat *image){
		this->image = *image;
		segments = new PointWithMin[(*image).cols*(*image).rows];
		for (int i = 0; i < 256; i++){
			Vec3b p;
			p[0] = i;
			p[1] = (256 - i) % 256;
			p[2] = (i*2) % 256;
			colors.push_back(p);
		}
	}

	~Watershed(){
		delete[]segments;
	}

	Vec3b searchMin(int x, int y){
		Vec3b minVec = this->image.at<Vec3b>(Point(x, y));
		Vec3b review = minVec;
		uchar minIntensity = calculateIntensity(minVec);
		
		while (true){
			Vec3b top = this->image.at<Vec3b>(Point(x, Clamp(y - 1, 0, this->image.rows)));
			Vec3b bottom = this->image.at<Vec3b>(Point(x, Clamp(y + 1, 0, this->image.rows - 1)));
			Vec3b left = this->image.at<Vec3b>(Point(Clamp(x - 1, 0, this->image.cols), y));
			Vec3b right = this->image.at<Vec3b>(Point(Clamp(x + 1, 0, this->image.cols - 1), y));

			if (minIntensity > calculateIntensity(top)){
				minIntensity = calculateIntensity(top);
				minVec = top;
			}

			if (minIntensity > calculateIntensity(bottom)){
				minIntensity = calculateIntensity(bottom);
				minVec = bottom;
			}

			if (minIntensity > calculateIntensity(left)){
				minIntensity = calculateIntensity(left);
				minVec = left;
			}

			if (minIntensity > calculateIntensity(right)){
				minIntensity = calculateIntensity(right);
				minVec = right;
			}

			if (minVec == review)
				break;
			else{
				if (minVec == top)
					y = Clamp(y + 1, 0, this->image.rows - 1);
				if (minVec == bottom)
					y = Clamp(y - 1, 0, this->image.rows);
				if (minVec == left)
					x = Clamp(x - 1, 0, this->image.cols);
				if (minVec == right)
					x = Clamp(x + 1, 0, this->image.cols - 1);
				review = minVec;
			}
		}
		return minVec;
	}

	void segmentation(){
		int amount = 0;
		for (int i = 0; i < image.cols; i++){
			for (int j = 0; j < image.rows; j++){
				Vec3b pix = searchMin(i, j);
				segments[amount].x = i;
				segments[amount].y = j;
				segments[amount].min = pix;
				segments[amount].color = colors[calculateIntensity(pix)];
				amount++;
			}
		}

		for (int i = 0; i < amount; i++){
			Point point(segments[i].x, segments[i].y);
			image.at<Vec3b>(point) = segments[i].color;
		}

		return;
	}

};

int Clamp(int value, int min, int max){
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

#endif