
#include "ImageCompressor.h"
#include <iostream>

Mat ImageCompressor::Compress(const Mat &image)
{
	inputImage = image;
	LengthCoding(Quantizate(CalcError(Predict(image))));
	return Mat();
}

Mat ImageCompressor::Predict(const Mat &image)
{
	Mat result(Size(image.cols, image.rows), image.type());

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			int i2, j2;
			if (j == 0) {
				i2 = i - 1;
				j2 = image.cols - 1;
			}
			else {
				i2 = i;
				j2 = j - 1;
			}

			Vec3b p1;
			if (i2 > 0 && j2 > 0)
				p1 = image.at<Vec3b>(i2, j2);
			else p1 = Vec3b(0, 0, 0);

			Vec3b p2 = image.at<Vec3b>(i, j);

			result.at<Vec3b>(i, j) = (p1 + p2) / 2;
		}
	}

	return result;
}

Mat ImageCompressor::CalcError(const Mat &image)
{
	Mat result(Size(image.cols, image.rows), CV_8SC3);

	Vec3sb p1 = image.at<Vec3sb>(0, 0);
	Vec3sb p2 = inputImage.at<Vec3sb>(0, 0);
	minError = maxError = Vec3sb(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
		
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			Vec3sb p1 = image.at<Vec3sb>(i, j);
			Vec3sb p2 = inputImage.at<Vec3sb>(i, j);

			Vec3sb err = Vec3sb(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
			result.at<Vec3sb>(i, j) = err;

			for (int k = 0; k < 3; k++)
			{
				if (err[k] < minError[k])
					minError[k] = err[k];
				if (err[k] > maxError[k])
					maxError[k] = err[k];
			}
		}
	}

	avgError = (minError + maxError) / 2;
	interval = maxError - minError;

	cout << "Avg Error " << avgError << endl;
	cout << "Max Error " << maxError << endl;
	cout << "Min Error " << minError << endl;

	cout << endl;
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
				if (error[k] < (minError[k] + interval[k] / 3))
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

void ImageCompressor::LengthCoding(const Mat &image)
{
	vector<RunLengthPair> pairs[3];

	for (int k = 0; k < 3; k++)
	{
		vector<RunLengthPair> &pk = pairs[k];
		pk.push_back(RunLengthPair(0, image.at<Vec3sb>(0, 0)[k]));

		for (int i = 0; i < image.rows; i++)
		{
			for (int j = 0; j < image.rows; j++)
			{
				Vec3sb p = image.at<Vec3sb>(i, j);

				if (p[k] == avgError[k])
					pk[pk.size() - 1].tail++;
				else {
					pk.push_back(RunLengthPair(0, p[k]));
				}
			}
		}

		int last;
		for (last = pk.size() - 1; last >= 0; last--)
			if (pk[last].value != avgError[k]) break;

		if (last < pk.size() - 2) {
			pk.erase(pk.begin() + last + 2, pk.end());
			pk[pk.size() - 1] = RunLengthPair(0, 0); // EOB
		}

		for (int i = 0; i < pk.size() - 1; i++)
			FillBits(k, pk[i]);
	}
}

void ImageCompressor::FillBits(int channel, const RunLengthPair &p)
{
	byte b = p.value;
	vector<bool> convert;
	byte binaryLength = 8;
	for (int i = 7; i >= 0; i--) {
		if ((1 << i) & b) {
			convert.push_back(true);
		}
		else {
			convert.push_back(false);
		}
	}
	for (int i = convert.size(); i > 0; i--) {
		if (convert[i] == false) {
			binaryLength--;
		}
		else
			break;
	}


	//bits[channel].push_back()

	// todo: fill bits array
	return;
}