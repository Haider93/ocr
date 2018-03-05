#include "NukhtaMatchAction.h"
#include "NukhtaMatcher.h"
#include "NukhtaMatchOutput.h"
#include "INukhtaMatcher.h"

INukhtaMatcher* NukhtaMatchAction::m_NukhtaMatcher = new NukhtaMatcher;
NukhtaMatchOutput* NukhtaMatchAction::execute(NukhtaMatchInput* in)
{
	this->startClock();
	NukhtaMatchOutput* o=m_NukhtaMatcher->Match(in);
	this->endClock();
	return o;
}

