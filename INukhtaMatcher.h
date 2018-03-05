#pragma once
#include "NukhtaMatchInput.h"
#include "NukhtaMatchOutput.h"
using namespace std;

class INukhtaMatcher
{
public:
	virtual NukhtaMatchOutput* Match(NukhtaMatchInput* inp) = 0;
};