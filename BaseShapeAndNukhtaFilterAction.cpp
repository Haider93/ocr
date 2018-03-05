#include "BaseShapeAndNukhtaFilterAction.h"
#include "BaseShapeAndNukhtaFilterer.h"

// Default Filterer Injection
IBaseShapeAndNukhtaFilterer* BaseShapeAndNukhtaFilterAction::m_baseShapeAndNukhtaFilterer = new BaseShapeAndNukhtaFilterer;

BaseShapeAndNukhtaFilterOutput* BaseShapeAndNukhtaFilterAction::execute(BaseShapeAndNukhtaFilterInput* inp)
{
	this->startClock();
	BaseShapeAndNukhtaFilterOutput *output = m_baseShapeAndNukhtaFilterer->Filter(inp);
	this->endClock();
	return output;
}