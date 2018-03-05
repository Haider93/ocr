#include "BaseShapeAndNukhtaFilterer.h"
#include "BaseShapeAndNukhtaFilterActionInput.h"
#include "BaseShapeAndNukhtaFilterActionOutput.h"
#include "NukhtaMatcher.h"
#include "NukhtaMatchAction.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ShapeModel\NukhtaShape.h"
using namespace cv;

BaseShapeAndNukhtaFilterOutput* BaseShapeAndNukhtaFilterer::Filter(BaseShapeAndNukhtaFilterInput *inp)
{
	vector<int> nukhtas,baseShapes;
	map <int, vector< pair<int,string>>> baseShapesWithNuktasList;
	map<int,string> shapeFilesNames;
	map<int,NukhtaShape::Nukhtas> nukhtaDescs;
	string path=inp->getPath();
	NukhtaTrainingData* nukhtaTrainingData=inp->getNukhtaTrainingData();
	extractShapesFromBlobs(nukhtas,baseShapes,shapeFilesNames,baseShapesWithNuktasList,nukhtaDescs,inp->getBlobFinderOutput(),path,nukhtaTrainingData);
	BaseShapeAndNukhtaFilterOutput *o = new BaseShapeAndNukhtaFilterOutput(nukhtas,baseShapes,shapeFilesNames,baseShapesWithNuktasList,nukhtaDescs,path);
	return o;
}

void BaseShapeAndNukhtaFilterer::extractShapesFromBlobs (vector<int> &nukhtaShapes,
	vector<int> &baseShapes,
	map<int,string> &shapeFilesNames,
	map <int, vector< pair<int,string> >> &baseShapesWithNuktasList,
	map<int,NukhtaShape::Nukhtas> &nukhtaDescs,
	BlobFinderActionOutput* out,
	string &path,
	NukhtaTrainingData* nukhtaTrainingData)
{
	nukhtaShapes.reserve(100);
	baseShapes.reserve(100);

	// looping on rect accroding to their x position
	prr rects=out->m_rects;
	std::vector <std::vector<cv::Point2i>> blobs=out->m_blobs;
	
	//Nukhta match action input and output pointers
	NukhtaMatchInput* in;
	NukhtaMatchAction* nukhtaMatchAction = new NukhtaMatchAction;
	NukhtaMatchOutput* nukhtaMatchOutput(false);
	double total_time_taken = 0;
	for(size_t rectIndexCounter = 0; rectIndexCounter < rects.size(); rectIndexCounter++)
	{					
		// find index from rect which point to blob in blobs array
		size_t blobIndex = rects.at(rectIndexCounter).first;			
		cv::Size s = cv::Size(rects.at(rectIndexCounter).second.width+2,rects.at(rectIndexCounter).second.height+2); // allocate an image of size rect
		
		IplImage* src = cvCreateImage(s, 8, 1);
		Mat output1(src);//no copy					
		output1.setTo(0);

		// find its rect and fill all pixels
		for(size_t j=0; j < blobs[blobIndex].size(); j++) 
		{
			int x = blobs[blobIndex][j].x;
			int y = blobs[blobIndex][j].y;		
			x = x -  rects.at(rectIndexCounter).second.x;
			y = y - rects.at(rectIndexCounter).second.y;
			x++;y++;
			output1.at<uchar>(y,x) = 255;								
		}

		// apply thresold to output image
		cv::threshold (output1, output1, 0, 255, CV_THRESH_BINARY_INV);			

		int width = rects.at(rectIndexCounter).second.width; // get width of rect
		int height = rects.at(rectIndexCounter).second.height; // get height of rect

		if(width <=2 || height <=2) {continue;} // if this rect is a fake shape then continue

		string file = path+"\\c_"+std::to_string((long long)rectIndexCounter)+".bmp"; // get file name of rect				
		//cv::imwrite(file.c_str(),output1); // save file to disk		
		cvSaveImage(file.c_str(), &(IplImage(output1)));
		output1.release();
		cvReleaseImage(&src);		
		shapeFilesNames.insert(make_pair(rectIndexCounter,file)); // put this physical file path on map to further processing

		double m_start_time = clock();
				
		//Nukhta Matching Action for each rect
		in = new NukhtaMatchInput(nukhtaTrainingData,file);
		nukhtaMatchOutput = nukhtaMatchAction->execute(in);

		NukhtaShape::Nukhtas nuktaTag;
		
		// if its a match to nukhtas		
		if(nukhtaMatchOutput->getIsNukhtaMatched())
		{	
			nukhtaShapes.push_back(rectIndexCounter); // push this file to nukhta vector
			nukhtaDescs.insert(make_pair(rectIndexCounter,nukhtaMatchOutput->getWhichNukhta()));
		}
		else
		{	
			// its a dust particle or broken image
			if(width <=5 && height <=5) {
				continue;
			}

			// we detect is as a shape
			baseShapes.push_back(rectIndexCounter);	 // push this file to base shapes vector 
			baseShapesWithNuktasList[rectIndexCounter].push_back(make_pair(-1,"Undefined")); // dummy entry for this shape for making its final form
		}	

		double m_end_time = clock();
		double time_taken=(m_end_time-m_start_time)/double(CLOCKS_PER_SEC);
		total_time_taken+=time_taken;
		delete in;delete nukhtaMatchOutput;
	}
	delete nukhtaMatchAction;
}