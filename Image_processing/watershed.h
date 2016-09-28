#ifndef _WATERSHED_H
#define _WATERSHED_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;
using namespace cv;

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
	uchar height; // = intensity

	// ID of basin that pixel belongs to or one of the special values listed above
	int label;

	// kakaya-to huynya
	int dist;

	vector<WatershedPixel *> neighbours;

	WatershedPixel(int x, int y, uchar height)
		: x(x), y(y), height(height)
	{
		label = INIT;
		dist = 0;
		neighbours.reserve(8);
	}

	WatershedPixel()
	{
		x = y = 0;
		height = 0;
		label = FICT;
		dist = 0;
	}

	bool areAllNeighboursWSHED() {
		for (WatershedPixel *n : neighbours) {
			if (n->label != WSHED) return false;
		}
		return true;
	}

	bool isFictitious() const {
		return label == FICT;
	}
};

/* static */ WatershedPixel WatershedPixel::fictPixel;

class WatershedStructure
{
public:
	WatershedStructure(const Mat &image) // image is one-channel grayscale of CV_8U1C format
	{
		if (image.channels() != 1) return;
		pixels.reserve(image.rows * image.cols);

		for (int j = 0; j < image.rows; j++)
		{
			const uchar *row = image.ptr<uchar>(j);
			for (int i = 0; i < image.cols; i++) {
				pixels.push_back(new WatershedPixel(i, j, row[i]));
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

				if (x + 1 < image.cols)
				{
					curPixel->neighbours.push_back(pixels[x + 1 + offset]);
					if (y - 1 >= 0)
						curPixel->neighbours.push_back(pixels[x + 1 + bottomOffset]);
					if (y + 1 < image.rows)
						curPixel->neighbours.push_back(pixels[x + 1 + topOffset]);
				}

				if (x - 1 >= 0)
				{
					curPixel->neighbours.push_back(pixels[x - 1 + offset]);
					if (y - 1 >= 0)
						curPixel->neighbours.push_back(pixels[x - 1 + bottomOffset]);
					if (y + 1 < image.rows)
						curPixel->neighbours.push_back(pixels[x - 1 + topOffset]);
				}

				if (y - 1 >= 0)
					curPixel->neighbours.push_back(pixels[x + bottomOffset]);

				if (y + 1 < image.rows)
					curPixel->neighbours.push_back(pixels[x + topOffset]);
			}
		}

		// sort pixels according to their heights
		std::sort(pixels.begin(), pixels.end(),
			[](const WatershedPixel *p1, const WatershedPixel *p2) -> bool {
				return p1->height < p2->height;
			}
		);
	}

	~WatershedStructure() {
		for (WatershedPixel *p : pixels)
			delete p;
	}

	WatershedStructure(const WatershedStructure &); // no impl.
	WatershedStructure &operator=(const WatershedStructure &); // no impl.

	WatershedPixel *get(int i) {
		return pixels[i];
	}

	int size() const {
		return (int)pixels.size();
	}

private:
	vector<WatershedPixel *> pixels;
};

class WatershedAlgorithm
{
public:
	void run(const Mat &image)
	{
		WatershedStructure watershedStructure(image);
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

				for (WatershedPixel *q : p->neighbours)
				{
					// ???
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

				// nihuya ne ponyatno
				for (WatershedPixel *q : p->neighbours)
				{
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

						for (WatershedPixel *r : q->neighbours)
						{
							if (r->label == WatershedPixel::MASK)
							{
								r->label = curLabel; // mark neighbour as belonging to the same basing as q
								queue.push(r); // .. and add it to queue to process later
							}
						}
					}
				}
			}
		} /* End of flooding */



	}
};

#endif // _WATERSHED_H_