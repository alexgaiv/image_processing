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
public:
	Interpolation(Mat *image){
		this->image = *image;
		result = Mat(3 * this->image.rows, 3 * this->image.cols, CV_8UC3);
	}
	~Interpolation(){}
	void doInterpolation(){
		for (int i = 1; i < image.rows; i++){
			UnitSquare unit;
			for (int j = 1; j < image.cols; j++){
				unit.x1 = Clamp(j - 1, 0, this->image.cols - 1);
				unit.y1 = Clamp(i - 1, 0, this->image.rows - 1);
				unit.x2 = Clamp(j + 1, 0, this->image.cols - 1);
				unit.y2 = Clamp(i + 1, 0, this->image.rows - 1);
				int idX1 = Clamp(j*3 - 1, 0, result.cols);
				int idY1 = Clamp(i*3 - 1, 0, result.rows);
				int idX2 = Clamp(j*3 + 1, 0, result.cols);
				int idY2 = Clamp(i*3 + 1, 0, result.rows);
				Vec3b f00 = this->image.at<Vec3b>(Point(unit.x1, unit.y2));
				Vec3b f01 = this->image.at<Vec3b>(Point(unit.x1, unit.y1));
				Vec3b f11 = this->image.at<Vec3b>(Point(unit.x2, unit.y1));
				Vec3b f10 = this->image.at<Vec3b>(Point(unit.x2, unit.y2));
				result.at<Vec3b>(Point(idX1, idY2)) = f00;
				result.at<Vec3b>(Point(idX1, idY1)) = f01;
				result.at<Vec3b>(Point(idX2, idY1)) = f11;
				result.at<Vec3b>(Point(idX2, idY2)) = f10;
				Vec3b color = (f00)*(1 - (double)j / (double)unit.x2)*(1 - (double)i / unit.y2) + (f10)*((double)j / unit.x2)*(1 - (double)i / unit.y2) + (f01)*(1 - (double)j / unit.x2)*((double)i / unit.y2) + (f11)*((double)j / unit.x2)*((double)i / unit.y2);
				result.at<Vec3b>(Point(j*3 , i * 3 )) = color;
				result.at<Vec3b>(Point(idX1,  i*3 )) = color;
				result.at<Vec3b>(Point(idX2, i*3)) = color;
				result.at<Vec3b>(Point(j*3 , idY1)) = color;
				result.at<Vec3b>(Point(j*3 , idY2)) = color;
			}
		}
	}
	void showResult(){
		static char windowname[] = "Result";
		imshow(windowname, result);
		cvWaitKey();
	}
};

#endif