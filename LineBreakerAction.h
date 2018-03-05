#pragma once
#include "LineBreakerActionInput.h"
#include "LineBreakerActionOutput.h"
#include "ILineBreaker.h"
#include "LineBreaker.h"
#include "Action.h"


class LineBreakerAction : public Action<LineBreakerActionInput,LineBreakerActionOutput>
{
public:
	LineBreakerActionOutput* execute(LineBreakerActionInput* inp);
	static ILineBreaker* m_LineBreaker;
	~LineBreakerAction(){}
};