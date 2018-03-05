#pragma once
#include "IUrduShapeMatcher.h"
#include "ShapeModel\NukhtaShape.h"
#include "TrainingDataModel\BaseShapeTrainingData.h"
using namespace std;

class UrduShapeMatcher : public IUrduShapeMatcher
{
public:
	UrduShapeMatchActionOutput* matchShapes(UrduShapeMatchActionInput* inp);
private:

	void writeFinalShapes(vector<int> &nukhtaShapes,
	vector<int> &baseShapes,
	map<int,string> &shapeFilesNames,
	map <int, vector< pair<int,string> >> &baseShapesWithNuktasList,
	map<int,string> &nukhtaDescs,
	std::vector<pair<int,cv::Rect>> &rects,
	std::vector <std::vector<cv::Point2i>> &blobs,
	LazyBaseShapeTrainingData *shapesProcessedData,
	string &path,cv::Size size);

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