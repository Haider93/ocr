#include "NukhtaTrainingDataPopulater.h"
#include "../../../ShapeModel/NukhtaShape.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "../../../Utility/FileUtil/FileUtil.h"
#include <vector>
#include "../../../Utility/ImageUtil/CalcContour.h"
using namespace cv;
using namespace boost;
using namespace std;

/**
* Implementation for NukhtasShape populator using a text file
*/
// Internal vector for storing all file names. This should get cleared in the end of 
// populating nukhtas data
vector<pair<string, string>> NukhtaTrainingDataPopulater::files;
const int nukhtaTotalSample = 100;

/**
* @param
* @return
*/
NukhtaTrainingData* NukhtaTrainingDataPopulater::populateNukhtasTrainingData(string filePath)
{
	init(filePath);
	NukhtaTrainingData *nukhtaTrainingData =  new NukhtaTrainingData(nukhtaTotalSample);
	populateNukhtaContours(nukhtaTrainingData);
	cleanUp();
	return nukhtaTrainingData;
}

/** This function loads list of image files of nukhtas and their identifier
* @param
*/
void NukhtaTrainingDataPopulater::init(string filePath)
{
	std::string data = FileUtil::getFileContents(filePath);
	std::vector<std::string> lines(nukhtaTotalSample);
	boost::split(lines,data,boost::is_any_of("\n\r"));

	for (std::vector<std::string>::iterator pos = lines.begin(); pos != lines.end(); ++pos) { 
		std::vector<std::string> line;
		boost::split(line,*pos,boost::is_any_of(","));
		if(line.size() >=2)
			files.push_back(make_pair(line[0],line[1]));
	}
	lines.clear();
}

/*
This function converts string to enum of type Nukhtas
*/
NukhtaShape::Nukhtas getNukhtaEnum(string nukhtaName)
{
	boost::algorithm::trim(nukhtaName);
	map<std::string, NukhtaShape::Nukhtas> ::iterator it;
	it = NukhtaShape::m_nukhtaToStringMap.find(nukhtaName);
	if (it != NukhtaShape::m_nukhtaToStringMap.end())
		return it->second;
	return NukhtaShape::UNDEFINED;
}

/**
* This function populate nukhta training data
*/
void NukhtaTrainingDataPopulater::populateNukhtaContours(NukhtaTrainingData *nukhtaTrainingData)
{
	for (vector<pair<string, string>>::iterator it = files.begin();it < files.end(); it++)
	{
		vector<vector<Point>>contours;
		CalcContour::calcContour((*it).first, contours);
		if(contours.size() > 0){
			NukhtaShape *nukhta = new NukhtaShape(contours[0],getNukhtaEnum((*it).second));
			nukhtaTrainingData->putShape(nukhta);
			contours.clear();
		}
	}
}

/**
* Clean data which is no more required
*/
void NukhtaTrainingDataPopulater::cleanUp()
{
	files.clear();
}

