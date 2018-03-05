#pragma once
#include "Action.h"
#include "NukhtaAndBaseShapeAssociationActionInput.h"
#include "NukhtaAndBaseShapeAssociationActionOutput.h"
#include "IBaseShapeAndNukhtaAssociater.h"

class NukhtaAndBaseShapeAssociationAction:public Action<NukhtaAndBaseShapeAssociationActionInput,NukhtaAndBaseShapeAssociationActionOutput>
{
public:
	NukhtaAndBaseShapeAssociationActionOutput* execute(NukhtaAndBaseShapeAssociationActionInput* in);
private:
	static IBaseShapeAndNukhtaAssociater *m_baseShapeAndNukhtaAssociater;

public:
	~NukhtaAndBaseShapeAssociationAction(){}
};