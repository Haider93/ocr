#include "UrduShapeMatchAction.h"
#include "UrduShapeMatcher.h"
#include "ParallelUrduShapeMatcher.h"
#include "StoredUrduShapeMatcher.h"

// Default Associater Injection
IUrduShapeMatcher* UrduShapeMatchAction::m_urduShapeMatcher = new UrduShapeMatcher;
//IUrduShapeMatcher* UrduShapeMatchAction::m_urduShapeMatcher = new StoredUrduShapeMatcher;
//IUrduShapeMatcher* UrduShapeMatchAction::m_urduShapeMatcher = new ParallelUrduShapeMatcher;
UrduShapeMatchActionOutput* execute(UrduShapeMatchActionInput* in);

UrduShapeMatchActionOutput* UrduShapeMatchAction::execute(UrduShapeMatchActionInput* inp)
{
	this->startClock();
	UrduShapeMatchActionOutput *output = m_urduShapeMatcher->matchShapes(inp); 
	this->endClock();
	return output;
}