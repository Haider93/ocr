#pragma once
#include "ActionInput.h"
#include"BaseShapeAndNukhtaFilterActionOutput.h"
#include <opencv\cv.h>
#include"BlobFinderActionOutput.h"
#include "TrainingDataModel\NukhtaTrainingData.h"
using namespace cv;

class BaseShapeAndNukhtaFilterInput :public ActionInput
{
private:
	BlobFinderActionOutput* blobFinderActionOutput;
	NukhtaTrainingData* nukhtaTrainingData;
	string path;
public:
	BaseShapeAndNukhtaFilterInput(){}
	BaseShapeAndNukhtaFilterInput(BlobFinderActionOutput* mblobFinderActionOutput,NukhtaTrainingData* nukhtasTrainingData,string path):
	blobFinderActionOutput(mblobFinderActionOutput), nukhtaTrainingData(nukhtasTrainingData),path(path){}
	BlobFinderActionOutput* getBlobFinderOutput(){return blobFinderActionOutput;}
	NukhtaTrainingData* getNukhtaTrainingData(){return nukhtaTrainingData;}
	string getPath(){return path;}

	~BaseShapeAndNukhtaFilterInput()
	{
	}
};