#pragma once
#include "LineBreakerActionInput.h"
#include "LineBreakerActionOutput.h"

class ILineBreaker
{
public:
	virtual LineBreakerActionOutput* lineBreak(LineBreakerActionInput* inp)=0;
};