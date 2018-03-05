#pragma once
#include "ActionOutput.h"
#include <opencv\cv.h>
using namespace cv;

class NukhtaMatchOutput :public ActionOutput
{
private:
	bool m_isNukhtaMatched;
	NukhtaShape::Nukhtas m_whichNukhta;

public:
	NukhtaMatchOutput(NukhtaShape::Nukhtas whichNukhta ,bool isNukhtaMatched = false):
	  m_whichNukhta(whichNukhta),m_isNukhtaMatched(isNukhtaMatched){}
	~NukhtaMatchOutput(){}
	bool getIsNukhtaMatched(){return m_isNukhtaMatched;}
	NukhtaShape::Nukhtas getWhichNukhta(){return m_whichNukhta;}
};