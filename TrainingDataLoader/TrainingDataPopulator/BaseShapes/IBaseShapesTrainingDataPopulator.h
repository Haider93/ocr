#pragma once
/*
 * Interface for loading nukhtas from file or folder
 */
#include "../../../TrainingDataModel/BaseShapeTrainingData.h"
class IBaseShapesTrainingDataPopulator
{
public:
	virtual BaseShapeTrainingData* populateBaseShapesTrainingData(string file,string userImageFile)= 0;
};