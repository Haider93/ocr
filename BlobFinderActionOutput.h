#pragma once
#include "ActionOutput.h"
#include <opencv\cv.h>
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;
typedef std::vector<pair<int,cv::Rect>> prr;

class BlobFinderActionOutput :public ActionOutput
{
public:
	std::vector < std::vector<cv::Point2i> > m_blobs;
	prr m_rects;

public:

	BlobFinderActionOutput()
	{}

	BlobFinderActionOutput(std::vector < std::vector<cv::Point2i> >& blobs,
		prr& rects):m_blobs(blobs),m_rects(rects) {}
	~BlobFinderActionOutput(){
		m_blobs.clear();
		m_rects.clear();
	}

	void dumpBlobFinderActionOutput(string path)
	{
		prr rects = this->m_rects;
		std::vector < std::vector<cv::Point2i> > blobs  = this->m_blobs;
		int start_time = clock();

		//writing their rectangles only
		int counter = 0;	
		for(size_t ii= 0; ii < rects.size(); ii++)
		{			
			// find tag of  blob
			size_t i = rects.at(ii).first;	
			cv::Size s = cv::Size(rects.at(ii).second.width+2,rects.at(ii).second.height+2);		
			IplImage* src = cvCreateImage(s, 8, 1);
			Mat output1(src);//no copy					
			output1.setTo(0);
			
			// find its rect and fill pixels
			for(size_t j=0; j < blobs[i].size(); j++) 
			{
				int x = blobs[i][j].x;
				int y = blobs[i][j].y;		
				x = x -  rects.at(ii).second.x;
				y = y - rects.at(ii).second.y;
				x++;y++;
				output1.at<uchar>(y,x) = 255;								
			}
			cv::threshold (output1, output1, 0, 255, CV_THRESH_BINARY_INV);			
			int width = rects.at(ii).second.width;
			int height = rects.at(ii).second.height;

			char buf[10];
			itoa(counter++,buf,10);		
			string file = path+"\\c_"+string(buf)+".bmp";				
			//cv::imwrite(file.c_str(),output1);
			cvSaveImage(file.c_str(), &(IplImage(output1)));
			output1.release();
			cvReleaseImage(&src);
		}
	}
};