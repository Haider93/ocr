#pragma once
#include <iostream>
#include <vector>
#include <opencv\cv.h>
#include "TreeKD.h"

class HausdorffDistance
{
private:
	static int distance_2( const std::vector<cv::Point> &a, const std::vector<cv::Point>  &b);
	static int distance_2_kdTree( const std::vector<cv::Point> & a, const std::vector<cv::Point> & b);
	static int maxint(int a , int b){ if(a>b) return a; return b;}
	static int HausdorffDistance::modified_distance_2_kdTree( KDTreeMukul<int> *trainTreeForb ,const std::vector<cv::Point> & a, const std::vector<cv::Point> & b);
public:
	static double distance_hausdorff( const std::vector<cv::Point> & a, const std::vector<cv::Point> & b);
	static double distance_hausdorff_kdTree( const std::vector<cv::Point> & a, const std::vector<cv::Point> & b);
	static double HausdorffDistance::modified_distance_hausdorff_kdTree(KDTreeMukul<int> *trainTree , const std::vector<cv::Point> & train, const std::vector<cv::Point> & scan);
	
};

