#include "NukhtaAndBaseShapeAssociationAction.h"
#include "BaseShapeAndNukhtaAssociater.h"

// Default Associater Injection
IBaseShapeAndNukhtaAssociater* NukhtaAndBaseShapeAssociationAction::m_baseShapeAndNukhtaAssociater = new BaseShapeAndNukhtaAssociater;

NukhtaAndBaseShapeAssociationActionOutput* NukhtaAndBaseShapeAssociationAction::execute(NukhtaAndBaseShapeAssociationActionInput* inp)
{
	this->startClock();
	NukhtaAndBaseShapeAssociationActionOutput *output = m_baseShapeAndNukhtaAssociater->associate(inp);
	this->endClock();
	return output;
}