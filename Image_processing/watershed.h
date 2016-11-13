#ifndef _WATERSHED_H
#define _WATERSHED_H

#include <opencv2/core/core.hpp>

class Watershed {
public:
	cv::Mat processImage(const cv::Mat &image, int blurRadius = 0, int thresh = 0);
//private:
	cv::Mat preProccess(const cv::Mat &image, int blurRadius = 0, int thresh = 0);
};

#endif // _WATERSHED_H_