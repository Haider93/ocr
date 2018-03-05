#pragma once
#include "ActionOutput.h"
#include <opencv\cv.h>
using namespace cv;

class LineBreakerActionOutput : public ActionOutput
{
private:
	vector<pair<string,string>> lines;
public:
	LineBreakerActionOutput(vector<pair<string,string>> lines):lines(lines){}
	vector<pair<string,string>> getOutputOfLineBreaker(){return lines;}
};