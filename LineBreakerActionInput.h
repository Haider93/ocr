#pragma once
#include "ActionInput.h"
#include <opencv\cv.h>
using namespace cv;

class LineBreakerActionInput : public ActionInput
{
private:
	string pathOfDocument;
	int noOfLines;
public:
	LineBreakerActionInput(string path,int lines):pathOfDocument(path),noOfLines(lines){}
	string getPathOfDocument(){return pathOfDocument;}
	int getNoOfLines(){return noOfLines;}
};