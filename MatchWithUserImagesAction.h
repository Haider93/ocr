#pragma once
#include "Action.h"
#include "MatchWithUSerImagesActionInput.h"
#include "MatchWithUSerImagesActionOutput.h"
#include "IMatchWithUserImages.h"

class MatchWithUserImagesAction:public Action<MatchWithUserImagesActionInput,MatchWithUserImagesActionOutput>
{
public:
	MatchWithUserImagesActionOutput* execute(MatchWithUserImagesActionInput* in);
	static IMatchWithUserImages* m_MatchWithUserImages;
	~MatchWithUserImagesAction(){}
};