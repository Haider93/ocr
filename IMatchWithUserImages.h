#pragma once
#include "MatchWithUserImagesActionInput.h"
#include "MatchWithUserImagesActionOutput.h"

class IMatchWithUserImages
{
public:
	virtual MatchWithUserImagesActionOutput* matchUserImages(MatchWithUserImagesActionInput* inp)=0;
};