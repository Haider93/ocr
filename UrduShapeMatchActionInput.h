#pragma once
#include "ActionInput.h"
#include "BaseShapeAndNukhtaFilterActionOutput.h"
#include "BlobFinderActionOutput.h"
#include "TrainingDataModel\BaseShapeTrainingData.h"
#include <opencv\cv.h>
#include <map>
#include <vector>
using namespace cv;

class UrduShapeMatchActionInput :public ActionInput
{
public:
	BaseShapeAndNukhtaFilterOutput *m_baseShapeAndNukhtaFilterOutput;
	BlobFinderActionOutput *m_blobFinderActionOutput;
	LazyBaseShapeTrainingData *lazyBaseShapeTrainingData;
	cv::Size m_size;
};