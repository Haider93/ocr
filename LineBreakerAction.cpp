#include "LineBreakerAction.h"
#include "LineBreakerActionInput.h"
#include "LineBreakerActionOutput.h"
#include "LineBreaker.h"

ILineBreaker* LineBreakerAction::m_LineBreaker=new LineBreaker;
LineBreakerActionOutput* LineBreakerAction::execute(LineBreakerActionInput* inp)
{
	this->startClock();
	LineBreakerActionOutput *output = m_LineBreaker->lineBreak(inp);
	this->endClock();
	return output;
}