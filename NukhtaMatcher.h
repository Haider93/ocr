#pragma once
#include <string>
#include <iostream>
#include <opencv\cv.h>
#include "INukhtaMatcher.h"
#include "ShapeModel\NukhtaShape.h"
using namespace std;


class NukhtaMatcher : public INukhtaMatcher
{
public:		
	static double thresold;
	static double heightThresold;
    static double widthThresold;
	static NukhtaShape::Nukhtas getMatchedNukta(const string &file,NukhtaTrainingData* nukhtaTrainingData);
	NukhtaMatchOutput* Match(NukhtaMatchInput* inp);
};
