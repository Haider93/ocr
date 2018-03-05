#include "TrainingDataPopulator\BaseShapes\IBaseShapesTrainingDataPopulator.h"
#include "TrainingDataPopulator\BaseShapes\LazyBaseShapesTrainingDataPopulater.h"
class BaseShapeTrainingDataLoader 
{

private:
	// Inject loader which you want to use [lazy , cached , full]
	IBaseShapesTrainingDataPopulator *trainingDataPopulator;

	static BaseShapeTrainingDataLoader* trainingDataLoader;

	BaseShapeTrainingDataLoader(IBaseShapesTrainingDataPopulator *trainingDataPopulator){
		this->trainingDataPopulator = trainingDataPopulator;
	}

public:

	//ctor
	BaseShapeTrainingDataLoader(){}

	//dtor
	~BaseShapeTrainingDataLoader(){
		delete trainingDataPopulator;
	}

	// call adapter to load base shapes training data
	BaseShapeTrainingData* load(string trainFile,string userImageFile) {
		return trainingDataPopulator->populateBaseShapesTrainingData(trainFile,userImageFile);
	}

	/**
	* @param
	* @return
	*/
	static BaseShapeTrainingDataLoader* getInstance(IBaseShapesTrainingDataPopulator *trainingDataPopulator = new LazyBaseShapesTrainingDataPopulator) {
		if (!trainingDataLoader)
			trainingDataLoader = new BaseShapeTrainingDataLoader(trainingDataPopulator);
		return trainingDataLoader;
	}
};
BaseShapeTrainingDataLoader* BaseShapeTrainingDataLoader::trainingDataLoader;