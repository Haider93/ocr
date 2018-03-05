#include "UrduShapeMatcher.h"
#include "HausdorffDistance.h"
#include <algorithm>
#include "Utility\FileUtil\FileUtil.h"
#include "Utility\ImageUtil\CalcContour.h"
#include "ShapeModel\NukhtaShape.h"
#include "HausdorffDistance.h"
#include "KDHausdorff.h"
#include "StoredKDTree.h"
#include <vector>
#include <tbb\tbb.h>
using namespace std;

UrduShapeMatchActionOutput* UrduShapeMatcher::matchShapes(UrduShapeMatchActionInput* inp)
{
	UrduShapeMatchActionOutput *o = new UrduShapeMatchActionOutput;
	matchFinalShapes(
		inp->m_baseShapeAndNukhtaFilterOutput->nukhtaShapes,
		inp->m_baseShapeAndNukhtaFilterOutput->baseShapes,
		inp->m_baseShapeAndNukhtaFilterOutput->shapeFilesNames,
		inp->m_baseShapeAndNukhtaFilterOutput->baseShapesWithNuktasList,
		inp->m_baseShapeAndNukhtaFilterOutput->nukhtaDescription,
		inp->m_blobFinderActionOutput->m_rects,
		inp->m_blobFinderActionOutput->m_blobs,
		inp->lazyBaseShapeTrainingData,
		inp->m_baseShapeAndNukhtaFilterOutput->path);
	return o;
}

/***********/
bool isSmaller(double x,vector<pair<double, pair<string,string>>> vec)
{
	for(int i=0;i<vec.size();i++)
	{
		if(vec.at(i).first<x)
			return false;
	}
	return true;
}

/**********/

void UrduShapeMatcher::matchFinalShapes(vector<int> &nukhtaShapes,
	vector<int> &baseShapes,
	map<int,string> &shapeFilesNames,
	map <int, vector< pair<int,string> >> &baseShapesWithNuktasList,
	const map<int,NukhtaShape::Nukhtas> &nukhtaDescs,
	std::vector<pair<int,cv::Rect>> &rects,
	std::vector <std::vector<cv::Point2i>> &blobs,
	LazyBaseShapeTrainingData *shapesProcessedData,
	string &path)
{

	string textualDesc = "";		
	string matchedData = "";

	int shapeNumber = 0;	
	string fullImageFolder = path + "\\FullShapes";
	FileUtil ::createDirectoryIfNotExist(fullImageFolder);	

	// loop over all shapes to match it from preprocessed data
	for(int baseIndexCounter=0; baseIndexCounter < baseShapes.size(); baseIndexCounter++)
	{		
		// form its textual description
		textualDesc = "";
		textualDesc += "<shape>";
		int baseShapeIndex = baseShapes.at(baseIndexCounter);
		for(int shapesandnuktasIndexCounter = 0; shapesandnuktasIndexCounter < baseShapesWithNuktasList[baseShapeIndex].size(); shapesandnuktasIndexCounter++)
		{		
			if(baseShapesWithNuktasList[baseShapeIndex].at(shapesandnuktasIndexCounter).first != -1)
			{							
				int nuktaIndex = baseShapesWithNuktasList[baseShapeIndex].at(shapesandnuktasIndexCounter).first;
				NukhtaShape::Nukhtas whichnukta =  nukhtaDescs.at(nuktaIndex);
				string position   = baseShapesWithNuktasList[baseShapeIndex].at(shapesandnuktasIndexCounter).second;
				textualDesc += NukhtaShape::getTextualDescription(whichnukta,position);									
			}
		}

		// get file name on disk
		string ShapefileName = shapeFilesNames[baseShapes.at(baseIndexCounter)];		

		vector<vector<Point>> shapecontour;		
		CalcContour::calcContour(ShapefileName,shapecontour); // calculate its contour
		vector<Point> shapeContour = shapecontour[0];

		string lfs = ""; double lmin = 0;int sizeMatched = 0;
		string fontMatch="";
		string seatMatch="";
		/***********************/
		/*It holds top 5 closest match using hausdorff distance*/
		vector<pair<double, pair<string,string>>> bestClosestHausdorff;
		bestClosestHausdorff.reserve(5);
		/**********************/

		int recordedWidthdiff = 0,recordedheightdiff = 0;

		// if we find an entry in process mapped based on its textual description
		if (shapesProcessedData->getCountOfBaseShapesFor(textualDesc)) 
		{		
			// find its closet matching shape and its font and seat number
			double min = 9999; string fontAndSeatMatched = "";	string test="";		

			int totalImages = shapesProcessedData->getCountOfBaseShapesFor(textualDesc);

			for(int mapIndexCounter = 0 ; mapIndexCounter< shapesProcessedData->getCountOfBaseShapesFor(textualDesc); mapIndexCounter++)
			{		
				LazyBaseShape *ls = shapesProcessedData->baseShapesTrainingData[textualDesc].at(mapIndexCounter);
				vector <Point> &TrainShapeContour = shapesProcessedData->PPcachedTrainingData[ls->getIndex()];;
				int font = ls->getBaseShapeDescription().getFontNo();
				int seat = ls->getBaseShapeDescription().getSeatNo();

				if(font==85&&seat==182)
				{
					int i=0;
				}

				string font_seat = std::to_string(((long double)font))+"_"+std::to_string((long double)seat);

				int shapeSize1 = shapeContour.size();
				int trainShapeSize = TrainShapeContour.size();

				Rect r1 = boundingRect(TrainShapeContour);
				Rect r2 = boundingRect(shapeContour);

				// filter images with no match using rect and pixel count
				if(abs(r1.width - r2.width) <=15 && abs(r1.height - r2.height)<=15)							
				{
					double m_start_time = clock();
					double diff= abs(HausdorffDistance::distance_hausdorff(TrainShapeContour,shapeContour));
					double m_end_time = clock();
					double time_taken=(m_end_time-m_start_time)/double(CLOCKS_PER_SEC);

					if(diff < min) 
					{
						min = diff;fontAndSeatMatched = font_seat;sizeMatched = trainShapeSize;
						fontMatch=std::to_string(((long double)font));
						seatMatch=std::to_string(((long double)seat));
						recordedWidthdiff = abs(r1.width - r2.width); recordedheightdiff = abs(r1.height - r2.height);
						if(isSmaller(diff,bestClosestHausdorff))
							bestClosestHausdorff.push_back(make_pair(diff,make_pair(fontMatch,seatMatch)));
					}
				}
			}	

			// append shape matched font and seat number to output variable
			matchedData +=fontAndSeatMatched+"\n";
			lfs = fontAndSeatMatched;
			lmin = min;
		}
		else
		{
			// write it to missing folder please and write its best matcg we found with its textual description			 			
			// can we write its whole rect to missing folder files
			;// this shape mismatch we encountered a new shape with nuktas
		}

		int shapeSize = shapeContour.size();
		string writeTo = "cd = "+textualDesc+"\n"+"font = "+fontMatch+"\n"+"seat = "+seatMatch+"\n"+"distance = "+std::to_string((long double)lmin)+"\npixels = "+std::to_string((long double)shapeSize)+"\nmPixels ="+std::to_string((long double)sizeMatched);
		writeTo += "\nwDiff = "+std::to_string((long double)recordedWidthdiff)+ "\nhDiff = "+std::to_string((long double)recordedheightdiff);
		
		/*******/
		std::sort(bestClosestHausdorff.begin(), bestClosestHausdorff.end());
		for(int i=0;i<bestClosestHausdorff.size();i++)
		{
			int j=i+1;
			writeTo+="\nDistance"+std::to_string((long double)j)+" = "+std::to_string((long double)bestClosestHausdorff.at(i).first);
			writeTo+="\nfont"+std::to_string((long double)j)+" = "+bestClosestHausdorff.at(i).second.first;
			writeTo+="\nseat"+std::to_string((long double)j)+" = "+bestClosestHausdorff.at(i).second.second;
		}
		/******/


		if(lmin == 9999) writeTo="cd = "+textualDesc+"\n";string fullImage = path + "\\FullShapes\\Shape"+std::to_string((long long)shapeNumber++)+".txt";
		FileUtil::writeStringToFile(fullImage,writeTo);
	}

	// finally write output to some file and scan it later
	string outFile = path +"\\"+"Unicode.txt";
	FileUtil::writeStringToFile(outFile,matchedData);
}