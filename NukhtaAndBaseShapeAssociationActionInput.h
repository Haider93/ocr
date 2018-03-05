#pragma once
#include "ActionInput.h"
#include "BaseShapeAndNukhtaFilterActionOutput.h"
#include "BlobFinderActionOutput.h"
#include <opencv\cv.h>
#include <map>
#include <vector>
using namespace cv;

class NukhtaAndBaseShapeAssociationActionInput :public ActionInput
{
public:
	BaseShapeAndNukhtaFilterOutput *m_baseShapeAndNukhtaFilterOutput;
	BlobFinderActionOutput *m_blobFinderActionOutput;
	cv::Size m_size;

	~NukhtaAndBaseShapeAssociationActionInput()
	{
	}
};