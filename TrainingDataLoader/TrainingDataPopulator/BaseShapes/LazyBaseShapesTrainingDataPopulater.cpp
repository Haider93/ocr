#include "../../../Utility/FileUtil/FileUtil.h"
#include "LazyBaseShapesTrainingDataPopulater.h"
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <map>
#include <string>
#include <time.h>
//#include <tbb/compat/ppl.h>
//#include <ppl.h>

#include <tbb\tbb.h>

//using namespace Concurrency;
using namespace cv;
using namespace std;
using namespace boost;
//using namespace tbb;

const int shapesTotalSample = 30000;
vector<string> shapeFiles(shapesTotalSample);

/*
Load paths of contextdescription file for each ligarture
*/
void LazyBaseShapesTrainingDataPopulator::init(string filePath)
{
	int start_time = clock();
	shapeFiles = FileUtil::getAllLines(filePath);//loading baseshapes from database
	int stop_time = clock();
	double time_taken1=(stop_time-start_time)/double(CLOCKS_PER_SEC);
	return;
}

/* This function loads list of image files of nukhtas and their identifier
* @param
*/
LazyBaseShapeTrainingData* LazyBaseShapesTrainingDataPopulator::populateBaseShapesTrainingData(string filePath,string userImageFile)
{
	init(filePath);
	//contains baseshapes of user saved images along with their contextual description
	vector<string> userImageShapeFiles=FileUtil::getAllLines(userImageFile);
	//added the user image context description firl path and cd to main shapeFiles
	for(int i=0;i!=userImageShapeFiles.size();i++)
		shapeFiles.push_back(userImageShapeFiles.at(i));
	userImageShapeFiles.clear();

	LazyBaseShapeTrainingData *lazyBaseShapeTrainingData = new LazyBaseShapeTrainingData;
	//populateLazyBaseShapes(lazyBaseShapeTrainingData);
	//int size1 = lazyBaseShapeTrainingData->getCountOfBaseShapesFor("<shape>");
	//LazyBaseShapeTrainingData *lazyBaseShapeTrainingData1 = new LazyBaseShapeTrainingData;
	populateParallelLazyBaseShapes(lazyBaseShapeTrainingData);
	//int size2 = lazyBaseShapeTrainingData->getCountOfBaseShapesFor("<shape>");
	cleanUp();
	return lazyBaseShapeTrainingData;
}

void LazyBaseShapesTrainingDataPopulator::ProcessLineData(int index, string lines,LazyBaseShapeTrainingData *lazyBaseShapeTrainingData)
{
	const int nElements = 2;
	string line[nElements];
	int pos = lines.find_last_of(",");
	if (pos != string::npos)
	{
		line[0] = lines.substr(0, pos);
		line[1] = lines.substr(pos + 1, lines.length());
	}

	int font = 0,seat = 0;
	string contextFilePath = line[0];		
	string folder = FileUtil::getFolderName(contextFilePath);
	string cdfileName = FileUtil::getFileName(contextFilePath);
	sscanf(cdfileName.c_str(),"ContextDescription_%d_%d.bmp",&font,&seat);		
	string imageShapeFile = folder + "\\0_BaseShape0.bmp";		
	string contextDesc = line[1];

	if(contextDesc.length()<=0) return;

	BaseShapeDescription bsd(font,seat,L"",contextDesc);
	LazyBaseShape *lazyBaseShape = new LazyBaseShape(bsd,imageShapeFile,index);
	lazyBaseShapeTrainingData->putShape(contextDesc,lazyBaseShape);
}

void LazyBaseShapesTrainingDataPopulator::populateLazyBaseShapes(LazyBaseShapeTrainingData *lazyBaseShapeTrainingData)
{
	int start_time = clock();
	for(vector<string>::iterator it = shapeFiles.begin() ; it < shapeFiles.end(); it++) {
		//ProcessLineData((*it), lazyBaseShapeTrainingData);
	}

	int stop_time = clock();
	double time_taken=(stop_time-start_time)/double(CLOCKS_PER_SEC);
	int i = 0;
}

void LazyBaseShapesTrainingDataPopulator::populateParallelLazyBaseShapes(LazyBaseShapeTrainingData *lazyBaseShapeTrainingData)
{
	int start_time = clock();
	
	tbb::parallel_for(size_t(0), size_t(shapeFiles.size()),
		[&] (size_t index) { ProcessLineData(index,shapeFiles.at(index), lazyBaseShapeTrainingData);});

	int stop_time = clock();
	double time_taken=(stop_time-start_time)/double(CLOCKS_PER_SEC);
	int i = 0;
}

/*
* Clean data which is no more required
*/
void LazyBaseShapesTrainingDataPopulator::cleanUp()
{
	shapeFiles.clear();
}

