#pragma once
#include "BaseShapeAndNukhtaFilterActionInput.h"
#include "BaseShapeAndNukhtaFilterActionOutput.h"
using namespace std;

class IBaseShapeAndNukhtaFilterer
{
public:
	virtual BaseShapeAndNukhtaFilterOutput* Filter(BaseShapeAndNukhtaFilterInput* inp) = 0;
};