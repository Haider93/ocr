#include "LineBreaker.h"
#include "LineBreakerActionInput.h"
#include "LineBreakerActionOutput.h"
#include <time.h>
#include <map>
#include <windows.h>
#include <stdio.h>
#include <io.h>

#include <tbb\tbb.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/flann/flann.hpp"
#include "ShapeModel\NukhtaShape.h"

#include "NukhtaAndBaseShapeAssociationAction.h"
#include "UrduShapeMatchAction.h"

#include "kdTree.h"
#include "kdTreeNode.h"
#include <ShlObj.h>
#include <math.h>


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //Thanks to Alessandro
#include <opencv2/nonfree/nonfree.hpp>

using namespace cv;
using namespace tbb;

bool LineBreaker::createDirectoryIfNotExist(const std::string& dirPath)
{
	bool isCreateDir = false;
	if (!dirPath.empty())
	{
		// Reset the error
		::SetLastError(0);
		LRESULT result = SHCreateDirectoryExA(NULL, dirPath.c_str(), NULL);
		if ((result != ERROR_SUCCESS) && (result != ERROR_ALREADY_EXISTS))
		{
		}
		else
		{
			isCreateDir = true;
		}
	}
	return isCreateDir;
}

int LineBreaker::findmin(vector<int> nukhtaShapes, int &index)
{

	int min=9999;

	int i = 0;
	for(vector<int> :: iterator it = nukhtaShapes.begin(); it != nukhtaShapes.end(); it++)
		//while(it!=nukhtaShapes.end())
	{
		if(min>*it){
			min = *it;
			index=i;
		}		
		//it++;
		i++;
	}

	return min;
}

int LineBreaker::lineBreakPoint(int number,vector <pair<int,int>> blackWhite)
{
	vector<int> add;
	vector<int> subtract;
	int min=0;
	int index1=0;
	int index2=0;
	for(int i=number;i<=number+10;i++)
	{
		if(i<blackWhite.size())
		{
			add.push_back(blackWhite.at(i).first);
		}

	}
	for(int i=number;i>=number-10;i--)
	{
		if(i<blackWhite.size())
		{
			subtract.push_back(blackWhite.at(i).first);
		}
	}
	int addNum=findmin(add,index1);
	int subNum=findmin(subtract,index2);
	//find(add.begin(),add.end(),addNum)-
	if(addNum<subNum)
	{
		min=index1;
	}
	else
	{
		min=-index2;
	}
	return min;
}
string LineBreaker::getFolderName(string path)
{	
	if(!path.empty())
	{
		int pos = path.find_last_of("\\/");
		if(pos != string::npos)
		{
			path = path.substr(0, pos);
		}
	}

	return path;
}

std::vector<pair<int,int>> LineBreaker::calc(cv::Mat &srcImage)
{
	std::vector<pair<int,int>> blackWhite;
	int m = srcImage.rows;
	int n = srcImage.cols;	
	bool isWhiteLine = true;
	for(int row=0; row<m; row++)		
	{
		isWhiteLine = true;
		int black =0;
		int white =0;
		for(int col=0; col<n; col++)
		{
			uchar c = srcImage.at<uchar>(row,col);
			if(c == 0)
			{
				isWhiteLine = false;
				black++;
				/*break;*/
			}// found a black Pixel
			else
			{
				white++;
			}
			
		}

		blackWhite.push_back(std::make_pair(black,white));
		/*if(isWhiteLine) lineData.push_back(1);
		else lineData.push_back(0);*/
	}
	return blackWhite;
}

vector<pair<string,string>> LineBreaker::linebreakIt(string file,int noOfLines)
{
    vector<pair<string,string>> lines;
	std::vector<pair<int,int>> blackWhite;
	Mat imagegray1=cvLoadImage(file.c_str(),0);			
	if(!imagegray1.data) 
	{	
		printf("Document Image not found\n.");
		return lines;
	}

	string path = getFolderName(file);   
	blackWhite=calc(imagegray1);
	//divide by number of lines in page
	int numberLinesPerPage=noOfLines;
	int number=blackWhite.size()/numberLinesPerPage;
	/*int number=blackWhite.size()/26+1;*/
	int lineNo = 0;
	int point=0;
	int initial=0;
	int startMark = 0 , endMark =0;
	for(int i = 0 ; i<blackWhite.size(); )
	{
		startMark=i;
		initial=number+i;
		point=lineBreakPoint(initial,blackWhite);
		endMark=initial+point;
		if(endMark<blackWhite.size())
		{
			
			char buf[10];
			itoa(lineNo++,buf,10);
			cv::Mat croppedImg = cv::Mat::zeros(imagegray1.size().width,endMark-startMark+1, CV_8UC3);
			/*imagegray1(cv::Rect(0,startMark,imagegray1.size().width,endMark-startMark+1)).copyTo(croppedImg);*/	
			imagegray1(cv::Rect(0,startMark,imagegray1.size().width,endMark-startMark)).copyTo(croppedImg);
			string folderPath=path+"\\Line"+string(buf);
			createDirectoryIfNotExist(folderPath);
			String fName = folderPath+"\\Line"+string(buf)+".bmp";
			//cvSaveImage(fName.c_str(), &(IplImage(croppedImg)));
			cv::imwrite(fName.c_str(),croppedImg);			
			lines.push_back(make_pair(fName,folderPath));
			//lines.push_back(fName) ;
			i=endMark;
		}
		else
		{
			endMark=blackWhite.size();
			char buf[10];
			itoa(lineNo++,buf,10);
			cv::Mat croppedImg = cv::Mat::zeros(imagegray1.size().width,endMark-startMark+1, CV_8UC3);
			/*imagegray1(cv::Rect(0,startMark,imagegray1.size().width,endMark-startMark+1)).copyTo(croppedImg);*/	
			imagegray1(cv::Rect(0,startMark,imagegray1.size().width,endMark-startMark)).copyTo(croppedImg);
			string folderPath=path+"\\Line"+string(buf);
			createDirectoryIfNotExist(folderPath);
			String fName = folderPath+"\\Line"+string(buf)+".bmp";
			//cv::imwrite(fName.c_str(),croppedImg);			
			cvSaveImage(fName.c_str(), &(IplImage(croppedImg)));
			lines.push_back(make_pair(fName,folderPath));
			//lines.push_back(fName) ;
			i=endMark;
		}
	}
	return lines;
}

LineBreakerActionOutput* LineBreaker::lineBreak(LineBreakerActionInput* inp)
{
	vector<pair<string,string>> lines = linebreakIt(inp->getPathOfDocument(),inp->getNoOfLines());//slice lines from document
	LineBreakerActionOutput* out=new LineBreakerActionOutput(lines);
	return out;
}