#pragma once
#include "UrduShapeMatchActionInput.h"
#include "UrduShapeMatchActionOutput.h"
using namespace std;

class IUrduShapeMatcher {
public:
	virtual UrduShapeMatchActionOutput* matchShapes(UrduShapeMatchActionInput* inp) = 0;
};