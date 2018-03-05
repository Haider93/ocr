#pragma once
#include "Action.h"
#include "UrduShapeMatchActionInput.h"
#include "UrduShapeMatchActionOutput.h"
#include "IUrduShapeMatcher.h"

class UrduShapeMatchAction : public Action<UrduShapeMatchActionInput,UrduShapeMatchActionOutput>
{
public:
	UrduShapeMatchActionOutput* execute(UrduShapeMatchActionInput* in);
private:
	static IUrduShapeMatcher *m_urduShapeMatcher;
};