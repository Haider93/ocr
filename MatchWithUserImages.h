#pragma once
#include <iostream>
#include <vector>
#include <opencv\cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "IMatchWithUserImages.h"

class MatchWithUserImages:public IMatchWithUserImages
{
public:
	MatchWithUserImagesActionOutput* MatchWithUserImages::matchUserImages(MatchWithUserImagesActionInput* inp);
	void matchUserSavedImages(vector<int> &nukhtaShapes,
	vector<int> &baseShapes,
	map<int,string> &shapeFilesNames,
	map <int, vector< pair<int,string> >> &baseShapesWithNuktasList,
	const map<int,NukhtaShape::Nukhtas> &nukhtaDescs,
	std::vector<pair<int,cv::Rect>> &rects,
	std::vector <std::vector<cv::Point2i>> &blobs,
	string &path);

};