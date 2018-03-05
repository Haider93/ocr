#pragma once
#include "Action.h"
#include "BaseShapeAndNukhtaFilterActionInput.h"
#include "BaseShapeAndNukhtaFilterActionOutput.h"
#include "IBaseShapeAndNukhtaFilterer.h"
class BaseShapeAndNukhtaFilterAction : public Action<BaseShapeAndNukhtaFilterInput,BaseShapeAndNukhtaFilterOutput>
{
public:
	BaseShapeAndNukhtaFilterOutput* execute(BaseShapeAndNukhtaFilterInput* in);
private:
	static IBaseShapeAndNukhtaFilterer *m_baseShapeAndNukhtaFilterer;

public:
	~BaseShapeAndNukhtaFilterAction()
	{
	}
};
