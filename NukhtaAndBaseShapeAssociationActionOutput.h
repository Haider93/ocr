#pragma once
#include "ActionOutput.h"
#include <opencv\cv.h>
#include <vector>
#include "ShapeModel\NukhtaShape.h"
#include "Utility\FileUtil\FileUtil.h"

using namespace std;
using namespace cv;
typedef std::vector<pair<int,cv::Rect>> prr;

class NukhtaAndBaseShapeAssociationActionOutput :public ActionOutput
{
public:
	std::vector < std::vector<cv::Point2i> > m_blobs;
	prr m_rects;

public:

	NukhtaAndBaseShapeAssociationActionOutput()
	{}

	NukhtaAndBaseShapeAssociationActionOutput(std::vector < std::vector<cv::Point2i> >& blobs,
		prr& rects):m_blobs(blobs),m_rects(rects) {}
	~NukhtaAndBaseShapeAssociationActionOutput(){
		m_blobs.clear();
		m_rects.clear();
	}

	void NukhtaAndBaseShapeAssociationActionOutput::dump(vector<int> &nukhtaShapes,
		vector<int> &baseShapes,
		map<int,string> &shapeFilesNames,
		map <int, vector< pair<int,string> >> &baseShapesWithNuktasList,
		std::vector<pair<int,cv::Rect>> &rects,
		std::vector <std::vector<cv::Point2i>> &blobs,
		string &path,cv::Size size)
	{
		// writing full shapes also
		// rename files also with their textual description
		int shapeNumber = 0;		

		string fullImageFolder = path + "\\FullShapes";
		FileUtil ::createDirectoryIfNotExist(fullImageFolder);	

		// loop over all shapes just to name them so we can check what shapes they belong to
		for(int baseIndexCounter=0; baseIndexCounter < baseShapes.size(); baseIndexCounter++)
		{	
			int xmin = 9999, ymin = 9999; 
			int xmax = -1, ymax = -1;
			int baseShapeIndex = baseShapes.at(baseIndexCounter);
			IplImage* src = cvCreateImage(cv::Size(size.width+2,size.height+2), 8, 1);
			Mat fullShape(src);//no copy					
			fullShape.setTo(0);
			
			
			int blobIndex = rects.at(baseShapeIndex).first;

			// find its rect and fill all pixels
			for(size_t j= 0; j < blobs[blobIndex].size(); j++) 
			{
				int x = blobs[blobIndex][j].x;
				int y = blobs[blobIndex][j].y;				
				x++;y++;

				if(x < xmin) xmin = x;
				if(y < ymin) ymin = y;

				if(x > xmax) xmax = x;
				if(y > ymax) ymax = y;

				fullShape.at<uchar>(y,x) = 255;								
			}

			int componentNumber = 0;
			for(int shapesandnuktasIndexCounter = 0; shapesandnuktasIndexCounter < baseShapesWithNuktasList[baseShapeIndex].size(); shapesandnuktasIndexCounter++)
			{		
				if(baseShapesWithNuktasList[baseShapeIndex].at(shapesandnuktasIndexCounter).first != -1)
				{							
					int nuktaIndex = baseShapesWithNuktasList[baseShapeIndex].at(shapesandnuktasIndexCounter).first;

					// find its rect and fill all pixels
					int blobIndex = rects.at(nuktaIndex).first;				
					for(size_t j= 0; j < blobs[blobIndex].size(); j++) 
					{
						int x = blobs[blobIndex][j].x;
						int y = blobs[blobIndex][j].y;				
						x++;y++;
						if(x < xmin) xmin = x;
						if(y < ymin) ymin = y;

						if(x > xmax) xmax = x;
						if(y > ymax) ymax = y;
						fullShape.at<uchar>(y,x) = 255;								
					}
					componentNumber++;
				}
			}

			cv::threshold(fullShape, fullShape, 0.0, 255.0, cv::THRESH_BINARY_INV);
			//cv::Mat FcroppedImg = cv::Mat::zeros(xmax-xmin+1,ymax-ymin+1, CV_8UC3);			
			IplImage* src1 = cvCreateImage(cv::Size(xmax-xmin+1,ymax-ymin+1), 8, 1);
			Mat FcroppedImg(src1);//no copy
			fullShape(cv::Rect(xmin,ymin,xmax-xmin+1,ymax-ymin+1)).copyTo(FcroppedImg);	
			copyMakeBorder( FcroppedImg, FcroppedImg, 1, 1, 1, 1, BORDER_CONSTANT,255);	
			string fullImage = path + "\\FullShapes\\Shape"+std::to_string((long long)shapeNumber)+".bmp";
			//cv::imwrite(fullImage.c_str(),FcroppedImg);
			cvSaveImage(fullImage.c_str(), &(IplImage(FcroppedImg)));
			FcroppedImg.release();
			fullShape.release();
			cvReleaseImage(&src);
			cvReleaseImage(&src1);
			shapeNumber++;
		}
	}
};