#pragma once
#include "ActionInput.h"
#include <opencv\cv.h>
using namespace cv;

class BlobFinderActionInput :public ActionInput
{
public:
	cv::Mat img;
public:
	void setActionInputImage(cv::Mat &img,string path){this->img=img;}
	cv::Mat& getActionInputImage(){return img;}

	// 1-arg ctor
	BlobFinderActionInput(cv::Mat &img){this->img=img;}
	//defualt ctor
	BlobFinderActionInput(){}
	//dtor
	~BlobFinderActionInput() {
		img.release();
	}
};