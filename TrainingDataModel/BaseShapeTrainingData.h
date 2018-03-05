#pragma once
#include "../ShapeModel/BaseShape.h"
#include "AbstractTrainingData.h"
#include <map>
#include "../Utility\ImageUtil\CalcContour.h"
#include <tbb/concurrent_vector.h>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_hash_map.h>
#include "../TreeKD.h"

#include <nanoflann.hpp>
#include <utils.h>

using namespace nanoflann;
using namespace std;
using namespace tbb;

// construct a kd-tree index for b set:
typedef KDTreeSingleIndexAdaptor<
	L2_Simple_Adaptor<float, PointCloud<int> > ,
	PointCloud<int>,
	2 /* dim */
> my_kd_tree_t;

/**
* This is a model of lazy base shape.
* Lazy means it will contain file path instead of actual vector of points.
* It is helpful to reduce bootstrap time for OCR application
*/
class LazyBaseShape
{
private:
	BaseShapeDescription m_urduShapeDescription;
	string m_filePath;
	int m_index;
	Rect m_rect;
public:

	//ctor
	LazyBaseShape(BaseShapeDescription urduShapeDescription,
		string filePath, int index):
	m_urduShapeDescription(urduShapeDescription),
		m_filePath(filePath) , m_index(index) {
	}

	//dtor
	virtual ~LazyBaseShape(){}

	//getter setter	
	BaseShapeDescription getBaseShapeDescription(){return m_urduShapeDescription;}

	void setBaseShapeDescription(BaseShapeDescription urduShapeDescription){
		this->m_urduShapeDescription = urduShapeDescription;
	}
	string getFilePath(){return m_filePath;}
    int getIndex(){return  m_index;}
	void setFilePath(string filePath){this->m_filePath=filePath;}
	void setRectBox(Rect rect) {this->m_rect = rect;}
	Rect getRectBox() {return this->m_rect;}
};

/**
* Abstract Class to represent BasShape Training Data used in OCR for recoginiton of shapes
*/
class BaseShapeTrainingData : public AbstractTrainingData {
};

/**
* This class loads data in lazy evaluation mode.It has to provide implementation for loading orginal sjape from its image file.
* This class should be singeleton.
*/
class LazyBaseShapeTrainingData : public BaseShapeTrainingData {

public:
	concurrent_unordered_map<string , concurrent_vector<LazyBaseShape*>> baseShapesTrainingData;
	//concurrent_unordered_map<string , concurrent_vector<LazyBaseShape*>> baseShapesTrainingDataConcurrent;

public:
	
	vector<Point> PPcachedTrainingData[25000];
	//KDTreeMukul<int>* PPKDDcachedTrainingData[25000];

	my_kd_tree_t* nanoflanndata[25000];

    //dtor
	~LazyBaseShapeTrainingData() {
		for (concurrent_unordered_map<string , concurrent_vector<LazyBaseShape*> >::iterator it = baseShapesTrainingData.begin(); it != baseShapesTrainingData.end(); it++) {
			concurrent_vector<LazyBaseShape*> x = it->second;
			for (concurrent_vector<LazyBaseShape*>::iterator itt = x.begin(); itt < x.end(); itt++) {
				delete *itt;
			}
		}
		baseShapesTrainingData.clear();
	}

	void putShape(string contextDescription , LazyBaseShape *lazyBaseShape) {
		vector<vector<Point>> contours;
		CalcContour::calcContour(lazyBaseShape->getFilePath() , contours);
	
		if(contours.size() >=1) {
			
			lazyBaseShape->setRectBox(boundingRect(contours[0]));
			baseShapesTrainingData[contextDescription].push_back(lazyBaseShape);
			
			
			//PointCloud<int> *cloud = new PointCloud<int>;
			//my_kd_tree_t *index;

			//vector<Point> &b = contours[0];
			//int N = b.size();
			//// Generating Random Point Cloud
			//cloud->pts.resize(N);

			//for (size_t i = 0; i < N; i++)
			//{
			//	cloud->pts[i].x = b[i].x;
			//	cloud->pts[i].y = b[i].y;
			//}
			//index =  new my_kd_tree_t(2, *cloud, KDTreeSingleIndexAdaptorParams(1));
   //         index->buildIndex();

			//nanoflanndata[lazyBaseShape->getIndex()] = index;
			
			//string filename = "E:\\flanindex\\"+std::to_string((long double)lazyBaseShape->getIndex())+".bin";
			//FILE *f = fopen(filename.c_str(), "wb");
			//if (!f) throw std::runtime_error("Error writing index file!");
			//index->saveIndex(f);
			//fclose(f);
			//delete index;
			//cloud.pts.clear();
			PPcachedTrainingData[lazyBaseShape->getIndex()] = contours[0];
		}
		//contours.clear();
	}

	BaseShape* getBaseShapeFor(string contextDescription , int imageNo) {
		LazyBaseShape *ls = baseShapesTrainingData[contextDescription].at(imageNo);
		PPcachedTrainingData[ls->getIndex()];
		BaseShape *bs = new BaseShape(PPcachedTrainingData[ls->getIndex()],ls->getBaseShapeDescription());
		return bs;
	}

	int getCountOfBaseShapesFor(string contextDescription) {
		return baseShapesTrainingData[contextDescription].size();  
	}
};