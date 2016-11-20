
#include "ImageCompressor.h"
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
	

	Vec3sb p1 = image.at<Vec3sb>(0, 0);
	Vec3sb p2 = inputImage.at<Vec3sb>(0, 0);
	minError = Vec3sb(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
	maxError = minError;
		
	for (int j = 0; j < image.rows; j++)
	{
		for (int i = 0; i < image.cols; i++)
		{
			Vec3sb p1 = image.at<Vec3sb>(j, i);
			Vec3sb p2 = inputImage.at<Vec3sb>(j, i);

			Vec3sb err = Vec3sb(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
			result.at<Vec3sb>(j, i) = err;

			for (int k = 0; k < 3; k++)
			{
				if (err[k] < minError[k])
					minError[k] = err[k];
				if (err[k] > maxError[k])
					maxError[k] = err[k];
				//cout << "ERROR[" << k << "] " << err << endl;
			}
		}
	}

	for (int k = 0; k < 3; k++)
	{
		//cout << "Min error " << (int) minError[k] << endl;
		//cout << "Max error " << (int) maxError[k] << endl;

		avgError[k] = (minError[k] + maxError[k]) / 2;
		
	}
	cout << "Avg Error " << avgError << endl;
	cout << "Max Error " << maxError << endl;
	cout << "Min Error " << minError << endl;

	cout << endl;
	interval = maxError - minError;
	cout<< "Interval: " << interval << endl;
	return result;
}

Mat ImageCompressor::Quantizate(const Mat &image)
{
	Mat result(Size(image.cols, image.rows), CV_8SC3);



	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			Vec3sb error = image.at<Vec3sb>(i, j);
			for (int k = 0; k < 3; k++)
			{
				if (error[k] < (minError[k] + interval[k]/3))
					result.at<Vec3sb>(i, j)[k] = (minError[k] + interval[k] / 3);
				else
					if (error[k] >= (minError[k] + interval[k] / 3) && error[k] <= (minError[k] + 2 * interval[k] / 3))
						result.at<Vec3sb>(i, j)[k] = avgError[k];
					else
						result.at<Vec3sb>(i, j)[k] = (minError[k] + 2 * interval[k] / 3);

			}
			cout << result.at<Vec3sb>(i, j) << endl;
		}
	}
	cout << "LOWER AND HIGHER GATEWAYS " << minError[0] + interval[0] / 3 << " :: " << minError[0] + 2 * interval[0] / 3 << endl;
	cout << "LOWER AND HIGHER GATEWAYS " << minError[1] + interval[1] / 3 << " :: " << minError[1] + 2 * interval[1] / 3 << endl;
	cout << "LOWER AND HIGHER GATEWAYS " << minError[2] + interval[2] / 3 << " :: " << minError[2] + 2 * interval[2] / 3 << endl;

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