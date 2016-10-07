#include "watershed.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;
using namespace cv;

// WatershedPixel and WatershedStructure classes are not used by external code, so their definitions moved in cpp

class WatershedPixel
{
public:
	enum
	{
		INIT = -1, // default value of this->label
		MASK = -2, //
		WSHED = 0, // this value indicates that pixel belongs to watershed wall
		FICT = -3  // 
	};

	static WatershedPixel fictPixel;

	int x, y;
	uchar height; // == intensity

	// ID of basin that pixel belongs to or one of the special values listed above
	int label;

	int dist;

	// not using vector<WatershedPixel *> here because deallocating
	// an array of WatershedPixel takes very long time in that case (for some reason)
	WatershedPixel *neighbours[8];
	int numNeighbours;

	WatershedPixel();
	WatershedPixel(int x, int y, uchar height);

	bool areAllNeighboursWSHED();

	bool isFictitious() const {
		return label == FICT;
	}
};

class WatershedStructure
{
public:
	WatershedStructure(const Mat &image); // image is one-channel grayscale of CV_8UC1 format
	~WatershedStructure();

	WatershedStructure(const WatershedStructure &); // no impl.
	WatershedStructure &operator=(const WatershedStructure &); // no impl.

	WatershedPixel *get(int i) {
		return pixels[i];
	}
	const WatershedPixel *get(int i) const {
		return pixels[i];
	}
	int size() const {
		return (int)pixels.size();
	}

	Mat computeImage();
private:
	vector<WatershedPixel *> pixels; // contains pointers to objects that are stored in pixelData
	WatershedPixel *pixelData; // array of pixels, used to reduce number of new & delete calls
	int imageWidth, imageHeight; // input image dimensions, used in computeImage
};


/*** WatershedPixel class ***/

/* static */ WatershedPixel WatershedPixel::fictPixel;

WatershedPixel::WatershedPixel(int x, int y, uchar height)
	: x(x), y(y), height(height)
{
	label = INIT;
	dist = 0;
	numNeighbours = 0;
}

WatershedPixel::WatershedPixel()
{
	x = y = 0;
	height = 0;
	label = FICT;
	dist = 0;
	numNeighbours = 0;
}

bool WatershedPixel::areAllNeighboursWSHED() {
	for (int i = 0; i < numNeighbours; i++) {
		if (neighbours[i]->label != WSHED) return false;
	}
	return true;
}


/*** WatershedStructure class ***/

WatershedStructure::WatershedStructure(const Mat &image) // image is one-channel grayscale of CV_8UC1 format
{
	if (image.channels() != 1) return;

	imageWidth = image.cols;
	imageHeight = image.rows;

	int s = imageWidth * imageHeight;
	pixelData = new WatershedPixel[s];
	pixels.reserve(s);

	for (int j = 0; j < image.rows; j++)
	{
		int offset = j*image.cols;
		const uchar *row = image.ptr<uchar>(j);
		for (int i = 0; i < image.cols; i++)
		{
			pixelData[i + offset] = WatershedPixel(i, j, row[i]);
			pixels.push_back(&pixelData[i + offset]);
		}
	}

	// compute neighbours of each pixel
	for (int y = 0; y < image.rows; y++)
	{
		int offset = y*image.cols;
		int topOffset = offset + image.cols;
		int bottomOffset = offset - image.cols;

		for (int x = 0; x < image.cols; x++)
		{
			WatershedPixel *curPixel = pixels[x + offset];
			int &nn = curPixel->numNeighbours;

			if (x + 1 < image.cols)
			{
				curPixel->neighbours[nn++] = pixels[x + 1 + offset];
				if (y - 1 >= 0)
					curPixel->neighbours[nn++] = pixels[x + 1 + bottomOffset];
				if (y + 1 < image.rows)
					curPixel->neighbours[nn++] = pixels[x + 1 + topOffset];
			}

			if (x - 1 >= 0)
			{
				curPixel->neighbours[nn++] = pixels[x - 1 + offset];
				if (y - 1 >= 0)
					curPixel->neighbours[nn++] = pixels[x - 1 + bottomOffset];
				if (y + 1 < image.rows)
					curPixel->neighbours[nn++] = pixels[x - 1 + topOffset];
			}

			if (y - 1 >= 0)
				curPixel->neighbours[nn++] = pixels[x + bottomOffset];

			if (y + 1 < image.rows)
				curPixel->neighbours[nn++] = pixels[x + topOffset];
		}
	}

	// sort pixels according to their heights
	std::sort(pixels.begin(), pixels.end(),
		[](const WatershedPixel *p1, const WatershedPixel *p2) -> bool {
			return p1->height < p2->height;
		}
	);
}

WatershedStructure::~WatershedStructure() {
	pixels.clear();
	delete[] pixelData;
}

Mat WatershedStructure::computeImage()
{
	vector<Vec3b> colors;
	Mat output(imageHeight, imageWidth, CV_8UC3);
	output.setTo(Scalar(0, 0, 0));

	for (WatershedPixel *p : pixels)
	{
		if (p->label == WatershedPixel::WSHED)
			output.at<Vec3b>(p->y, p->x) = Vec3b(255, 255, 255);
		else if (p->label > 0)
		{
			if ((int)colors.size() <= p->label - 1)
				colors.push_back(Vec3b(rand() % 256, rand() % 256, rand() % 256));
			output.at<Vec3b>(p->y, p->x) = colors[p->label - 1];
		}
	}
	return output;
}


/*** Watershed class ***/

Mat Watershed::preProccess(const Mat &image, int blurRadius, int thresh)
{
	Mat result;
	Mat grad_x, grad_y;

	if (blurRadius)
		GaussianBlur(image, result, Size(blurRadius, blurRadius), 0, 0, BORDER_DEFAULT);
	cvtColor(result, result, CV_BGR2GRAY);

	Sobel(result, grad_x, CV_16S, 1, 0);
	convertScaleAbs(grad_x, grad_x); // convert to CV_8UC1

	Sobel(result, grad_y, CV_16S, 0, 1);
	convertScaleAbs(grad_y, grad_y); // convert to CV_8UC1

	addWeighted(grad_x, 1, grad_y, 1, 0, result);

	if (thresh)
		threshold(result, result, thresh, 0, THRESH_TOZERO);

	return result;
}

Mat Watershed::processImage(const Mat &image, int blurRadius, int thresh)
{
	Mat image2 = preProccess(image, blurRadius, thresh);
	WatershedStructure watershedStructure(image2);

	queue<WatershedPixel *> queue;
	int curLabel = 0;
	int heightIndex1 = 0;
	int heightIndex2 = 0;

	/* Start flooding */
	for (int h = 0; h < 256; h++)
	{
		// set label=MASK for all pixels at level h
		for (int pix = heightIndex1; pix < watershedStructure.size(); pix++)
		{
			WatershedPixel *p = watershedStructure.get(pix);

			if (p->height != h) { // pixel is at level h+1, exit loop
				heightIndex1 = pix;
				break;
			}

			p->label = WatershedPixel::MASK;

			for (int i = 0; i < p->numNeighbours; i++)
			{
				// ???
				WatershedPixel *q = p->neighbours[i];
				if (q->label >= 0) {
					p->dist = 1;
					queue.push(p);
					break;
				}
			}
		}

		int curDist = 1;
		queue.push(&WatershedPixel::fictPixel);

		// extend basins
		while (true)
		{
			WatershedPixel *p = queue.front();
			queue.pop();

			if (p->isFictitious()) {
				if (queue.empty())
					break;
				else {
					// ???
					queue.push(&WatershedPixel::fictPixel);
					curDist++;
					p = queue.front();
					queue.pop();
				}
			}

			for (int i = 0; i < p->numNeighbours; i++)
			{
				WatershedPixel *q = p->neighbours[i];

				if (q->dist <= curDist && // ???
					q->label >= 0 // q belongs to existing basin (>0) or it is a watershed(=0)
					)
				{
					if (q->label > 0) // q belongs to existing basin
					{
						if (p->label == WatershedPixel::MASK)
							p->label = q->label;
						else if (p->label != q->label)
							p->label = WatershedPixel::WSHED;
					}
					else if (p->label == WatershedPixel::MASK)
					{
						p->label = WatershedPixel::WSHED;
					}
				}
				else if (q->label == WatershedPixel::MASK && q->dist == 0)
				{
					q->dist = curDist + 1;
					queue.push(q);
				}
			}
		}

		// detect and process new minima at level h
		for (int pix = 0; pix < watershedStructure.size(); pix++)
		{
			WatershedPixel *p = watershedStructure.get(pix);

			if (p->height != h) { // pixel is at level h+1, exit loop
				heightIndex2 = pix;
				break;
			}

			p->dist = 0;

			if (p->label == WatershedPixel::MASK)
			{
				curLabel++; // create a new basin
				p->label = curLabel; // .. and mark p as belonging to this basin
				queue.push(p);

				while (!queue.empty())
				{
					WatershedPixel *q = queue.front();
					queue.pop();

					for (int i = 0; i < q->numNeighbours; i++)
					{
						WatershedPixel *r = q->neighbours[i];

						if (r->label == WatershedPixel::MASK)
						{
							r->label = curLabel; // mark neighbour as belonging to the same basin as q
							queue.push(r); // .. and add it to queue to process later
						}
					}
				}
			}
		}
	} /* End of flooding */

	return watershedStructure.computeImage();
}