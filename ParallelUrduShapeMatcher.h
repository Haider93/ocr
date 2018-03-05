#pragma once
#include "IUrduShapeMatcher.h"
#include "ShapeModel\NukhtaShape.h"
#include "TrainingDataModel\BaseShapeTrainingData.h"
using namespace std;

class ParallelUrduShapeMatcher : public IUrduShapeMatcher
{
public:
	UrduShapeMatchActionOutput* matchShapes(UrduShapeMatchActionInput* inp);
private:
 void matchFinalShapes(vector<int> &nukhtaShapes,
	vector<int> &baseShapes,
	map<int,string> &shapeFilesNames,
	map <int, vector< pair<int,string> >> &baseShapesWithNuktasList,
	const map<int,NukhtaShape::Nukhtas> &nukhtaDescs,
	std::vector<pair<int,cv::Rect>> &rects,
	std::vector <std::vector<cv::Point2i>> &blobs,
	LazyBaseShapeTrainingData *shapesProcessedData,
	string &path);
};