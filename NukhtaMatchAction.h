#pragma once
#include "Action.h"
#include "NukhtaMatchInput.h"
#include "NukhtaMatchOutput.h"
#include "INukhtaMatcher.h"
class NukhtaMatchAction : public Action<NukhtaMatchInput,NukhtaMatchOutput>
{
public:
	NukhtaMatchOutput* execute(NukhtaMatchInput* in);
private:
	static INukhtaMatcher *m_NukhtaMatcher;
public:
};