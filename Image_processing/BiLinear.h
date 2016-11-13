#ifndef INTER
#define INTER
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>

using namespace std;
using namespace cv;

struct UnitSquare{
	int x1, x2;
	int y1, y2;
};

class Interpolation{
private:
	Mat image;
	Mat result;
	int koefOfUpsempling;
public:
	Interpolation(Mat *image, int koefOfUpsempling){
		this->image = *image;
		this->koefOfUpsempling = koefOfUpsempling;
		this->result = Mat(koefOfUpsempling * this->image.rows, koefOfUpsempling * this->image.cols, CV_8UC3);
	}
	~Interpolation(){}

	void resample(){
		int h, w;
		float tmp;
		float u, t;
		for (int j = 0; j < result.rows; j++){
			tmp = (float)(j) / (float)(result.rows - 1)*(image.rows - 1);
			h = Clamp((int)floor(tmp), 0, image.rows - 2);
			u = tmp - h;

			for (int i = 0; i < result.cols; i++){
				tmp = (float)(i) / (float)(result.cols - 1)*(image.cols - 1);
				w = Clamp((int)floor(tmp), 0, image.cols - 2);
				t = tmp - w;

				/*coefficients*/
				float d1 = (1 - t)*(1 - u);
				float d2 = t*(1 - u);
				float d3 = t*u;
				float d4 = (1 - t) * u;

				/*surrounding pixels*/
				Vec3b p1 = (image.at<Vec3b>(Point(Clamp(w, 0, image.cols - 2), Clamp(h, 0, image.rows - 2)))); //f00
				Vec3b p2 = (image.at<Vec3b>(Point(Clamp(w + 1, 0, image.cols - 2), Clamp(h, 0, image.rows - 2)))); //f10
				Vec3b p3 = (image.at<Vec3b>(Point(Clamp(w + 1, 0, image.cols - 2), Clamp(h + 1, 0, image.rows - 2)))); //f01
				Vec3b p4 = (image.at<Vec3b>(Point(Clamp(w, 0, image.cols - 2), Clamp(h + 1, 0, image.rows - 2)))); //f11

				/*new pixel*/
				Vec3b component = p1*d1 + p2*d2 + p3*d3 + p4*d4;

				result.at<Vec3b>(Point(i, j)) = component;
			}
		}
	}

	void showResult() const {
		static char windowname[] = "Result";
		imshow(windowname, result);
		cvWaitKey();
	}
};

#endif