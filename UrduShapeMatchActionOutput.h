#pragma once
#include "ActionOutput.h"
#include <opencv\cv.h>
#include <vector>
#include "ShapeModel\NukhtaShape.h"
#include "Utility\FileUtil\FileUtil.h"

class UrduShapeMatchActionOutput :public ActionOutput
{
public:
	std::vector < std::vector<cv::Point2i> > m_blobs;
	prr m_rects;

public:

	UrduShapeMatchActionOutput()
	{}

	UrduShapeMatchActionOutput(std::vector < std::vector<cv::Point2i> >& blobs,
		prr& rects):m_blobs(blobs),m_rects(rects) {}
	~UrduShapeMatchActionOutput(){
		m_blobs.clear();
		m_rects.clear();
	}
};