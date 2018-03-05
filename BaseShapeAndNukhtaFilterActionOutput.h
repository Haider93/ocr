#pragma once
#include "ActionOutput.h"
#include <opencv\cv.h>
#include "ShapeModel\NukhtaShape.h"
using namespace cv;

class BaseShapeAndNukhtaFilterOutput :public ActionOutput
{
public:
	vector<int> nukhtaShapes;
	vector<int> baseShapes;
	map<int,string> shapeFilesNames;
	map <int, vector< pair<int,string>>> baseShapesWithNuktasList;
	map<int,NukhtaShape::Nukhtas> nukhtaDescription;
	string path;
public:
	
	// default ctor
	BaseShapeAndNukhtaFilterOutput(){}
	
	//ctor
	BaseShapeAndNukhtaFilterOutput(vector<int> nukhtaShapes,
		vector<int> baseShapes,
		map<int,string> shapeFilesNames,
		map <int, vector< pair<int,string>>> baseShapesWithNuktasList,
		map<int,NukhtaShape::Nukhtas> nukhtaDescs,
		string path):
	    nukhtaShapes(nukhtaShapes),
		baseShapes(baseShapes),
		shapeFilesNames(shapeFilesNames),
		baseShapesWithNuktasList(baseShapesWithNuktasList),
		nukhtaDescription(nukhtaDescs),
		path(path){}

	//dtor
	~BaseShapeAndNukhtaFilterOutput(){
		baseShapesWithNuktasList.clear();
		nukhtaDescription.clear();
		baseShapes.clear();
		nukhtaShapes.clear();
		shapeFilesNames.clear();
	}
	
	//getter and setter
	map<int,NukhtaShape::Nukhtas> getNukhtaTrainingDataDescription(){return nukhtaDescription;}
	vector<int> getBaseShapesVector(){return baseShapes;}
	map <int, vector< pair<int,string>>> getBaseShapesWithNukhtasList(){return baseShapesWithNuktasList;}
	map<int,NukhtaShape::Nukhtas> getNukhtaDescription(){return nukhtaDescription;}
};