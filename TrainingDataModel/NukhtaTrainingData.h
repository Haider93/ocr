#pragma once
#include "../ShapeModel/NukhtaShape.h"
#include "AbstractTrainingData.h"

/**
* This is a singleton class which will hold data for all nukhtas
* used in URDU OCR.
* It will have a vector of Nukhtas Shapes
*/
using namespace std;
class NukhtaTrainingData : public AbstractTrainingData 
{
	vector<NukhtaShape*> nuktaTrainingData;

public:
	// ctor
	NukhtaTrainingData(int size) {
		nuktaTrainingData.reserve(size);
	}

	void putShape(NukhtaShape *nukhtaShape) {
		nuktaTrainingData.push_back(nukhtaShape);
	}
	
	vector<NukhtaShape*> getNukhtaTrainingData(){return nuktaTrainingData;}
	
	//dtor
	virtual ~NukhtaTrainingData() {
		vector<NukhtaShape*>::iterator it;
		for (it = nuktaTrainingData.begin(); it < nuktaTrainingData.end(); it++) {
			delete (*it);
		}
		nuktaTrainingData.clear();
	}
};