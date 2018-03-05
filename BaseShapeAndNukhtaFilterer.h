#pragma once
#include "IBaseShapeAndNukhtaFilterer.h"
#include"BlobFinderActionOutput.h"
using namespace std;

class BaseShapeAndNukhtaFilterer : public IBaseShapeAndNukhtaFilterer
{
public:
	BaseShapeAndNukhtaFilterOutput* Filter(BaseShapeAndNukhtaFilterInput* inp);
	
private:
	void extractShapesFromBlobs(vector<int> &nukhtaShapes,
	vector<int> &baseShapes,
	map<int,string> &shapeFilesNames,
	map <int, vector< pair<int,string> >> &baseShapesWithNuktasList,
	map<int,NukhtaShape::Nukhtas> &nukhtaDescs,
	BlobFinderActionOutput* out,
	string &path,NukhtaTrainingData* nukhtaTrainingData);
};