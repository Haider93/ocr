#pragma once
#include <iostream>
#include <vector>
#include <opencv\cv.h>
#include "TreeKD.h"

class KDHausdorff
{
private:
	static int  distance_2_kdTree( const std::vector<cv::Point> & a, const std::vector<cv::Point> & b);
	static int maxint(int a , int b){ if(a>b) return a; return b;}
	
public:

	static double distance_hausdorff_kdTree( const std::vector<cv::Point> & a, const std::vector<cv::Point> & b);
};

