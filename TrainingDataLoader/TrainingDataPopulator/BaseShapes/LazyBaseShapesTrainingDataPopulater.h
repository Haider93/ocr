#pragma once
#include "IBaseShapesTrainingDataPopulator.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
class LazyBaseShapesTrainingDataPopulator : public IBaseShapesTrainingDataPopulator
{
public:
	LazyBaseShapeTrainingData* populateBaseShapesTrainingData(string file,string userImageFile);
	
private:
	void init(string filePath);
	void populateLazyBaseShapes(LazyBaseShapeTrainingData *lazyBaseShapeTrainingData);
	void populateParallelLazyBaseShapes(LazyBaseShapeTrainingData *lazyBaseShapeTrainingData);
	void ProcessLineData(int index,string lines,LazyBaseShapeTrainingData *lazyBaseShapeTrainingData);
	void cleanUp();
};