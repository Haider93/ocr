#include "NukhtaMatcher.h"
#include "HausdorffDistance.h"
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "Utility\ImageUtil\CalcContour.h"
#include "NukhtaMatchOutput.h"
#include "ShapeModel\NukhtaShape.h"

using namespace cv;
double NukhtaMatcher::thresold = 25.0;
double NukhtaMatcher::heightThresold = 10.0;
double NukhtaMatcher::widthThresold = 10.0;

NukhtaMatchOutput* NukhtaMatcher::Match(NukhtaMatchInput* in)
{
	NukhtaTrainingData* nukhtaTrainingData=in->getNukhtaTrainingData();
	NukhtaShape::Nukhtas whichNukta = NukhtaMatcher::getMatchedNukta(in->getFilePath(),nukhtaTrainingData);
	NukhtaMatchOutput* out=new NukhtaMatchOutput(whichNukta, whichNukta != NukhtaShape::Nukhtas::UNDEFINED);
	return out;
}

NukhtaShape::Nukhtas NukhtaMatcher::getMatchedNukta(const string &file,NukhtaTrainingData* nukhtaTrainingData)
{
	double min = std::numeric_limits<int>::max();
	NukhtaShape::Nukhtas whichNukta = NukhtaShape::Nukhtas::UNDEFINED;

	vector<vector<Point>>contours1;	
	CalcContour::calcContour(file,contours1);			
		
	vector<NukhtaShape*>::iterator it;
	vector<NukhtaShape*> nukhtaVector=nukhtaTrainingData->getNukhtaTrainingData();
	double diff = 9999;
	for(it = nukhtaVector.begin() ; it < nukhtaVector.end(); it++) 
	{				
		vector<Point> &contours2 = (*it)->getPoints();	

		Rect r1 = boundingRect(contours2);
		Rect r2 = boundingRect(contours1[0]);

		// filter images with no match using rect and pixel count
		if(abs(r1.width - r2.width) <= NukhtaMatcher::widthThresold && abs(r1.height - r2.height)<= NukhtaMatcher::heightThresold)		
		{
			double diff = abs( HausdorffDistance::distance_hausdorff(contours1[0],contours2));
			//double diff = abs( HausdorffDistance::distance_hausdorff_kdTree(contours1[0],contours2));
		
			if(diff < min) {min = diff;whichNukta = (*it)->getWhichNukta();}		
		}
	}

	if(abs(min) > NukhtaMatcher::thresold) return NukhtaShape::Nukhtas::UNDEFINED;
	return whichNukta;
}