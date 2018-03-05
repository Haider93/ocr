#include "TrainingDataPopulator\Nukhtas\NukhtaTrainingDataPopulater.h"

/**
* This class is responsible for loading NukhtaTrainingData by various means.
* Its uses a decorator pattern to load Nukhtas.
* Pass it a INukhtasTrainingDataPopulator implemenation which will be used to load NukhtaTraining Data
*/

class NukhtasTrainingDataLoader
{
private:

	// Inject loader which you want to use
	INukhtasTrainingDataPopulator *trainingDataPopulator;
	static NukhtasTrainingDataLoader *trainingDataLoader;

	NukhtasTrainingDataLoader(INukhtasTrainingDataPopulator *trainingDataPopulator) {
		this->trainingDataPopulator = trainingDataPopulator;
	}

public:

	~NukhtasTrainingDataLoader(){
		delete trainingDataPopulator;
	}
	/**
	* @param
	* @return
	*/
	NukhtaTrainingData* load(string trainFile) {
		return trainingDataPopulator->populateNukhtasTrainingData(trainFile);
	}

	/**
	* @param
	* @return
	*/
	static NukhtasTrainingDataLoader* getInstance(INukhtasTrainingDataPopulator *trainingDataPopulator = new NukhtaTrainingDataPopulater) {
		if (!trainingDataLoader)
			trainingDataLoader = new NukhtasTrainingDataLoader(trainingDataPopulator);
		return trainingDataLoader;
	}
};

//trainingDataLoader is a static pointer to class NukhtasTrainingDataLoader------defined outside class also
NukhtasTrainingDataLoader* NukhtasTrainingDataLoader::trainingDataLoader;