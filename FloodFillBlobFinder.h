#pragma once
#include <iostream>
#include <vector>
#include <opencv\cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "IBlobFinder.h"

using namespace std;
using namespace cv;

typedef std::vector<pair<int,cv::Rect>> prr;
class FloodFillBlobFinder : public IBlobFinder
{
public:
	BlobFinderActionOutput* FloodFillBlobFinder::FindBlobs(BlobFinderActionInput* inp);
private:
	void findBlobs(const cv::Mat &binary, 
		std::vector < std::vector<cv::Point2i> > &blobs ,prr &rects);
};