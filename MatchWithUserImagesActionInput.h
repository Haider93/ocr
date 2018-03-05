#pragma once
#include "ActionInput.h"
#include "BaseShapeAndNukhtaFilterActionOutput.h"
#include "BlobFinderActionOutput.h"
#include <opencv\cv.h>
#include <string>
#include <vector>
using namespace cv;
using namespace std;

class MatchWithUserImagesActionInput:public ActionInput
{
public:
	BaseShapeAndNukhtaFilterOutput *m_baseShapeAndNukhtaFilterOutput;
	BlobFinderActionOutput *m_blobFinderActionOutput;
	string file;//file containing absolute path of user saved images from front end 
	MatchWithUserImagesActionInput(string file):file(file){}
};