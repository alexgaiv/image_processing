#ifndef _IMAGE_COMPRESSOR_H_
#define _IMAGE_COMPRESSOR_H_

#include <opencv2\core\core.hpp>

using namespace std;
using namespace cv;

class ImageCompressor
{
public:
	Mat Compress(const Mat &image);
private:
	typedef Vec<char, 3> Vec3sb;

	Mat inputImage;
	Vec3sb minError, maxError, avgError;

	Mat Predict(const Mat &image);
	Mat CalcError(const Mat &image);
	Mat Quantizate(const Mat &image);
	Mat LengthCoding(const Mat &image);
	Mat Huffman(const Mat &image);
};

#endif // _IMAGE_COMPRESSOR_H_