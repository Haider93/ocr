#pragma once
#include "ActionInput.h"
#include <opencv\cv.h>
#include "TrainingDataModel\NukhtaTrainingData.h"
using namespace cv;

class NukhtaMatchInput :public ActionInput
{
private:
	NukhtaTrainingData *nukhtaTrainingData;
	string nukhtaShapeFile;
public:
	NukhtaMatchInput(){}
	NukhtaMatchInput(NukhtaTrainingData *nukhtaTrainingData,string file):
	nukhtaTrainingData(nukhtaTrainingData),nukhtaShapeFile(file){}
	~NukhtaMatchInput(){}
	NukhtaTrainingData* getNukhtaTrainingData(){return nukhtaTrainingData;}
	string getFilePath(){return nukhtaShapeFile;}
};