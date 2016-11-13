#include "ImageCompressor.h"
#include "utils.h"
#include <iostream>

Mat ImageCompressor::Compress(const Mat &image)
{
	inputImage = image;
	return Huffman(LengthCoding(Quantizate(CalcError(Predict(image)))));
}

Mat ImageCompressor::Predict(const Mat &image)
{
	Mat result(Size(image.cols, image.rows), image.type());

	for (int j = 0; j < image.rows; j++)
	{
		for (int i = 0; i < image.cols; i++)
		{
			int i2, j2;
			if (i == 0) {
				i2 = image.cols - 1;
				j2 = j - 1;
			}
			else {
				i2 = i - 1;
				j2 = j;
			}

			Vec3b p1;
			if (i2 > 0 && j2 > 0)
				p1 = image.at<Vec3b>(j2, i2);
			else p1 = Vec3b(0, 0, 0);

			Vec3b p2 = image.at<Vec3b>(j, i);

			result.at<Vec3b>(j, i) = (p1 + p2) / 2;
		}
	}

	return result;
}

Mat ImageCompressor::CalcError(const Mat &image)
{
	Mat result(Size(image.cols, image.rows), CV_8SC3);
	Vec3sb minError, maxError;

	for (int j = 0; j < image.rows; j++)
	{
		for (int i = 0; i < image.cols; i++)
		{
			Vec3sb p1 = image.at<Vec3sb>(j, i);
			Vec3sb p2 = inputImage.at<Vec3sb>(j, i);

			Vec3sb err = Vec3sb(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
			result.at<Vec3sb>(j, i) = err;
			avgError += err;
		}
	}

	avgError /= (image.cols * image.rows);

	return result;
}

Mat ImageCompressor::Quantizate(const Mat &image)
{
	Mat result(Size(image.cols, image.rows), image.type());

	return result;
}

Mat ImageCompressor::LengthCoding(const Mat &image)
{
	Mat result(Size(image.cols, image.rows), image.type());

	return result;
}

Mat ImageCompressor::Huffman(const Mat &image)
{
	Mat result(Size(image.cols, image.rows), image.type());

	return result;
}