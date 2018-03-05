#pragma once
#include "NukhtaAndBaseShapeAssociationActionInput.h"
#include "NukhtaAndBaseShapeAssociationActionOutput.h"
using namespace std;

class IBaseShapeAndNukhtaAssociater {
public:
	virtual NukhtaAndBaseShapeAssociationActionOutput* associate(NukhtaAndBaseShapeAssociationActionInput* inp) = 0;
};