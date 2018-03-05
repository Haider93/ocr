#include "ParallelUrduShapeMatcher.h"
#include "HausdorffDistance.h"

#include "Utility\FileUtil\FileUtil.h"
#include "Utility\ImageUtil\CalcContour.h"
#include "ShapeModel\NukhtaShape.h"
#include "HausdorffDistance.h"
#include <vector>
#include <tbb\tbb.h>

using namespace std;
UrduShapeMatchActionOutput* ParallelUrduShapeMatcher::matchShapes(UrduShapeMatchActionInput* inp)
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

void ParallelUrduShapeMatcher::matchFinalShapes(vector<int> &nukhtaShapes,
	vector<int> &baseShapes,
	map<int,string> &shapeFilesNames,
	map <int, vector< pair<int,string> >> &baseShapesWithNuktasList,
	const map<int,NukhtaShape::Nukhtas> &nukhtaDescs,
	std::vector<pair<int,cv::Rect>> &rects,
	std::vector <std::vector<cv::Point2i>> &blobs,
	LazyBaseShapeTrainingData *shapesProcessedData,
	string &path)
{
	string fullImageFolder = path + "\\FullShapes";
	FileUtil ::createDirectoryIfNotExist(fullImageFolder);	

	tbb::parallel_for(size_t(0), size_t(baseShapes.size()),
		[&] (size_t baseIndexCounter) {

			// form its textual description
			string textualDesc = "";
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

			int recordedWidthdiff = 0,recordedheightdiff = 0;
			string fontMatch="";
			string seatMatch="";

			// if we find an entry in process mapped based on its textual description
			if (shapesProcessedData->getCountOfBaseShapesFor(textualDesc)) 
			{		
				// find its closet matching shape and its font and seat number
				double min = 9999; string fontAndSeatMatched = "";	

				int totalImages = shapesProcessedData->getCountOfBaseShapesFor(textualDesc);

				for(int mapIndexCounter = 0 ; mapIndexCounter< shapesProcessedData->getCountOfBaseShapesFor(textualDesc); mapIndexCounter++)
				{		
					BaseShape *bShape =  shapesProcessedData->getBaseShapeFor(textualDesc , mapIndexCounter);

					vector <Point> &TrainShapeContour = bShape->getPoints();
					int font = bShape->getBaseShapeDescription().getFontNo();
					int seat = bShape->getBaseShapeDescription().getSeatNo();
					string font_seat = std::to_string(((long double)font))+"_"+std::to_string((long double)seat);
					int shapeSize1 = shapeContour.size();
					int trainShapeSize = TrainShapeContour.size();

					Rect r1 = boundingRect(TrainShapeContour);
					Rect r2 = boundingRect(shapeContour);

					// filter images with no match using rect and pixel count
					if(abs(r1.width - r2.width) <=12 && abs(r1.height - r2.height)<=12)							
					{
						double diff = abs(HausdorffDistance::distance_hausdorff(TrainShapeContour,shapeContour));
						//double diff = abs(HausdorffDistance::distance_hausdorff_kdTree(TrainShapeContour,shapeContour));

						if(diff < min) 
						{
							min = diff;fontAndSeatMatched = font_seat;sizeMatched = trainShapeSize;
							fontMatch=std::to_string(((long double)font));
							seatMatch=std::to_string(((long double)seat));
							recordedWidthdiff = abs(r1.width - r2.width); recordedheightdiff = abs(r1.height - r2.height);
						}
					}
					delete bShape;
				}	

				// append shape matched font and seat number to output variable
				//matchedData +=fontAndSeatMatched+"\n";
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

			if(lmin==9999) writeTo="cd = "+textualDesc+"\n";
			string fullImage = path + "\\FullShapes\\Shape"+std::to_string((long long)baseIndexCounter)+".txt";
			FileUtil::writeStringToFile(fullImage,writeTo);
	});
}