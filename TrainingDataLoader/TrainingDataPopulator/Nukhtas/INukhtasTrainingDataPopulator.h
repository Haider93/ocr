#pragma once
#include "../../../TrainingDataModel/NukhtaTrainingData.h"

/**
 * Interface for loading nukhtas from file or folder
 */
class INukhtasTrainingDataPopulator
{
public:
	virtual NukhtaTrainingData* populateNukhtasTrainingData(string file)= 0;
	virtual ~INukhtasTrainingDataPopulator(){}
};