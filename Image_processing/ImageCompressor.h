#ifndef _IMAGE_COMPRESSOR_H_
#define _IMAGE_COMPRESSOR_H_

#include <opencv2\core\core.hpp>

using namespace std;
using namespace cv;

typedef char byte;
typedef Vec<byte, 3> Vec3sb;

class ImageCompressor
{
public:
	Mat Compress(const Mat &image);
private:
	struct RunLengthPair
	{
		RunLengthPair() { tail = value = 0; }
		RunLengthPair(int tail, byte value) :
			tail(tail), value(value) { }

		int tail;
		byte value;
	};

	Mat inputImage;

	Vec3sb minError, maxError;
	Vec3sb avgError;
	Vec3sb interval;

	vector<bool> bits[3];

	Mat Predict(const Mat &image);
	Mat CalcError(const Mat &image);
	Mat Quantizate(const Mat &image);
	void LengthCoding(const Mat &image);
	void FillBits(int channel, const RunLengthPair &p);
};

#endif // _IMAGE_COMPRESSOR_H_