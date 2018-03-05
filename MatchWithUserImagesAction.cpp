#include "MatchWithUserImagesAction.h"
#include "MatchWithUserImagesActionInput.h"
#include "MatchWithUserImagesActionOutput.h"
#include "MatchWithUserImages.h"
IMatchWithUserImages* MatchWithUserImagesAction::m_MatchWithUserImages=0;
MatchWithUserImagesActionOutput* MatchWithUserImagesAction::execute(MatchWithUserImagesActionInput* inp)
{
	this->startClock();
	MatchWithUserImagesActionOutput* output=m_MatchWithUserImages->matchUserImages(inp);
	this->endClock();
	return output;
}