#pragma once
#include "INukhtasTrainingDataPopulator.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
class NukhtaTrainingDataPopulater : public INukhtasTrainingDataPopulator
{
private:
	static vector<pair<string, string>> files;
	void init(string filePath);
	void populateNukhtaContours(NukhtaTrainingData *nukhtaTrainingData);
	void cleanUp();
public:
	NukhtaTrainingData* populateNukhtasTrainingData(string file);
	virtual ~NukhtaTrainingDataPopulater(){}
};