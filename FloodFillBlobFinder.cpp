#include "FloodFillBlobFinder.h"
BlobFinderActionOutput* FloodFillBlobFinder::FindBlobs(BlobFinderActionInput* inp) {
	std::vector <std::vector<cv::Point2i>> blobs;
	prr rects;
	this->findBlobs(inp->img,blobs,rects);
	BlobFinderActionOutput *output = new BlobFinderActionOutput(blobs,rects);
	return output;
}

bool sortBlobsXX(pair<int,cv::Rect> a, pair<int,cv::Rect> b)
{
	cv::Rect first = a.second;
	cv::Rect second = b.second;    
	return first.x < second.x;
}

void FloodFillBlobFinder:: findBlobs(const cv::Mat &img, std::vector < std::vector<cv::Point2i> > &blobs ,prr &rects)
{
	
	cv::threshold(img, img, 0.0, 255.0, cv::THRESH_BINARY_INV);
	// apply thresold to image
	cv::Mat binary;
	cv::threshold(img, binary, 0.0, 1.0, cv::THRESH_BINARY);
	blobs.clear();

	/* Fill the label_image with the blobs
	0  - background
	1  - unlabelled foreground
	2+ - labelled foreground*/

	cv::Mat label_image;
	binary.convertTo(label_image, CV_32SC1);
	int label_count = 2; // starts at 2 because 0,1 are used already
	int counter = 0;
	for(int y=0; y < label_image.rows; y++) {
		int *row = (int*)label_image.ptr(y);
		for(int x=0; x < label_image.cols; x++) {
			if(row[x] != 1) {
				continue;
			}

			// fill in rect information
			cv::Rect rect;
			cv::floodFill(label_image, cv::Point(x,y), label_count, &rect, 0, 0, 8);				            								
			rects.push_back(make_pair(counter++,rect));						
			std::vector <cv::Point2i> blob;	
			for(int i=rect.y; i < (rect.y+rect.height); i++) {
				int *row2 = (int*)label_image.ptr(i);
				for(int j=rect.x; j < (rect.x+rect.width); j++) {
					if(row2[j] != label_count) {
						continue;
					}					
					blob.push_back(cv::Point2i(j,i));
				}
			}
			blobs.push_back(blob);
			label_count++;
		}
	}

	// sort rects according to their x position
	std::sort(rects.begin(), rects.end(), sortBlobsXX);	
}