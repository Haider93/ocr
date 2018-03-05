#include "StoredUrduShapeMatcher.h"
#include "HausdorffDistance.h"

#include "Utility\FileUtil\FileUtil.h"
#include "Utility\ImageUtil\CalcContour.h"
#include "ShapeModel\NukhtaShape.h"
#include "HausdorffDistance.h"
#include "KDHausdorff.h"
#include "StoredKDTree.h"
#include <vector>
#include <tbb\tbb.h>
#include <nanoflann.hpp>
#include <utils.h>

using namespace nanoflann;
using namespace std;

double totalTimeSpentInTreeBuildingFlannNano = 0;
double totalTimeSpentInTreeBuildingFlannNano1 = 0;

// construct a kd-tree index for b set:
typedef KDTreeSingleIndexAdaptor<
	L2_Simple_Adaptor<float, PointCloud<int> > ,
	PointCloud<int>,
	2 /* dim */
> my_kd_tree_t;


my_kd_tree_t* nanoflanndataCache[25000];


UrduShapeMatchActionOutput* StoredUrduShapeMatcher::matchShapes(UrduShapeMatchActionInput* inp)
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

void StoredUrduShapeMatcher::matchFinalShapes(vector<int> &nukhtaShapes,
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

	double totalTimeInKd = 0;
	double totalTimeNonKd = 0;

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

		/*************************************************************************************************************************************/
		// building kd-tree for scanned shape

		double m_start_time_indexBuilding = clock();
		PointCloud<int> *cloud = new PointCloud<int>;
		my_kd_tree_t *scanShapeIndex;

		vector<Point> &b = shapecontour[0];
		int N = b.size();
		// Generating Random Point Cloud
		cloud->pts.resize(N);

		for (size_t i = 0; i < N; i++)
		{
			cloud->pts[i].x = b[i].x;
			cloud->pts[i].y = b[i].y;
		}
		scanShapeIndex =  new my_kd_tree_t(2, *cloud, KDTreeSingleIndexAdaptorParams(1));
		scanShapeIndex->buildIndex();
		double m_end_time_indexBuilding = clock();
		double time_taken_indexBuilding=(m_end_time_indexBuilding-m_start_time_indexBuilding)/double(CLOCKS_PER_SEC);
		totalTimeSpentInTreeBuildingFlannNano1+=time_taken_indexBuilding;
		/*************************************************************************************************************************************/


		string lfs = ""; double lmin = 0;int sizeMatched = 0;
		string fontMatch="";
		string seatMatch="";
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
				string font_seat = std::to_string(((long double)font))+"_"+std::to_string((long double)seat);

				int shapeSize1 = shapeContour.size();
				int trainShapeSize = TrainShapeContour.size();

				Rect r1 = boundingRect(TrainShapeContour);
				Rect r2 = boundingRect(shapeContour);

				// filter images with no match using rect and pixel count
				if(abs(r1.width - r2.width) <=12 && abs(r1.height - r2.height)<=12)							
				{
					/*************************************************************************************************************************************/
					// building index for training shape using stored index

					double m_start_time_indexBuilding = clock();

					if(!nanoflanndataCache[ls->getIndex()])
					{
					PointCloud<int> *cloud = new PointCloud<int>;
					my_kd_tree_t *storedShapeIndex;

					vector<Point> &b = TrainShapeContour;
					int N = b.size();
					// Generating Random Point Cloud
					cloud->pts.resize(N);

					for (size_t i = 0; i < N; i++)
					{
						cloud->pts[i].x = b[i].x;
						cloud->pts[i].y = b[i].y;
					}
					storedShapeIndex =  new my_kd_tree_t(2, *cloud, KDTreeSingleIndexAdaptorParams(1));
					string filename = "E:\\flanindex\\"+std::to_string((long double)ls->getIndex())+".bin";
					FILE *f = fopen(filename.c_str(), "rb");
					if (!f) throw std::runtime_error("Error reading index file!");
					storedShapeIndex->loadIndex(f);
					fclose(f);

					nanoflanndataCache[ls->getIndex()] = storedShapeIndex;
					} 
					else {//printf("\nCache Hit");
					}
					double m_end_time_indexBuilding = clock();
					double time_taken_indexBuilding=(m_end_time_indexBuilding-m_start_time_indexBuilding)/double(CLOCKS_PER_SEC);
					totalTimeSpentInTreeBuildingFlannNano+=time_taken_indexBuilding;


					/*************************************************************************************************************************************/
					double m_start_time1 = clock();
					double diff = abs(StoredKDTree::distance_hausdorff_kdTree(TrainShapeContour,shapeContour,
						nanoflanndataCache[ls->getIndex()]
					/*shapesProcessedData->nanoflanndata[ls->getIndex()]*/
					/*storedShapeIndex*/,
					scanShapeIndex));
					
					double m_end_time1 = clock();
					double time_taken1=(m_end_time1-m_start_time1)/double(CLOCKS_PER_SEC);
					//cloud->pts.clear();
					//delete cloud;
					
					totalTimeInKd += time_taken1;
					if(diff < min) 
					{
						min = diff;fontAndSeatMatched = font_seat;sizeMatched = trainShapeSize;
						fontMatch=std::to_string(((long double)font));
						seatMatch=std::to_string(((long double)seat));
						recordedWidthdiff = abs(r1.width - r2.width); recordedheightdiff = abs(r1.height - r2.height);
					}

					//delete storedShapeIndex;
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

		if(lmin == 9999) writeTo="cd = "+textualDesc+"\n";string fullImage = path + "\\FullShapes\\Shape"+std::to_string((long long)shapeNumber++)+".txt";
		FileUtil::writeStringToFile(fullImage,writeTo);

		cloud->pts.clear();
		delete cloud;
		delete scanShapeIndex;
	}

	// finally write output to some file and scan it later
	string outFile = path +"\\"+"Unicode.txt";
	FileUtil::writeStringToFile(outFile,matchedData);
}