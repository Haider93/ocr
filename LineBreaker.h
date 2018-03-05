#pragma once
#include <iostream>
#include <vector>
#include <opencv\cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "ILineBreaker.h"
#include "LineBreakerActionOutput.h"


using namespace std;
using namespace cv;

class LineBreaker : public ILineBreaker
{
public:
	LineBreakerActionOutput* lineBreak(LineBreakerActionInput* inp);
	bool createDirectoryIfNotExist(const std::string& dirPath);
	int lineBreakPoint(int number,vector <pair<int,int>> blackWhite);
	string getFolderName(string path);
	std::vector<pair<int,int>> calc(cv::Mat &srcImage);
	vector<pair<string,string>> linebreakIt(string file,int noOfLines);
	int findmin(vector<int> nukhtaShapes, int &index);

};