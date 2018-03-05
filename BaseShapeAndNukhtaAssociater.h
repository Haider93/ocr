#pragma once
#include "IBaseShapeAndNukhtaAssociater.h"
#include "ShapeModel\NukhtaShape.h"
using namespace std;

class BaseShapeAndNukhtaAssociater : public IBaseShapeAndNukhtaAssociater
{
public:
	NukhtaAndBaseShapeAssociationActionOutput* associate(NukhtaAndBaseShapeAssociationActionInput* inp);

private:
	void getPercentAndPosition_new(int nukhtaIndex, 
		const int shapeIndex , 
		string &pos , 
		double &percent , 
		int &xPixelCoverage , 
		const std::vector < std::vector<cv::Point2i> > &blobs ,
		const std::vector <pair<int,cv::Rect>> &rects);

	void getPercentAndPosition_old(int nukhtaIndex, 
		const int shapeIndex , 
		string &pos , 
		double &percent , 
		int &xPixelCoverage , 
		const std::vector < std::vector<cv::Point2i> > &blobs ,
		const std::vector <pair<int,cv::Rect>> &rects);

	void associateBaseShapesWithNuktasFromBlobs (
		const vector<int> &nukhtaShapes,
		const vector<int> &baseShapes,
		const map<int,string> &shapeFilesNames,
		map <int, vector< pair<int,string> >> &baseShapesWithNuktasList,
		const map<int,NukhtaShape::Nukhtas> &nukhtaDescs,
		const std::vector<pair<int,cv::Rect>> &rects,
		const std::vector <std::vector<cv::Point2i>> &blobs);
};