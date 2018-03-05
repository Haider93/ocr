#pragma once
#include <iostream>
#include <vector>
#include <opencv\cv.h>
#include <nanoflann.hpp>
#include <utils.h>

using namespace nanoflann;

// construct a kd-tree index for b set:
typedef KDTreeSingleIndexAdaptor<
	L2_Simple_Adaptor<float, PointCloud<int> > ,
	PointCloud<int>,
	2 /* dim */
> my_kd_tree_t;

class StoredKDTree
{
private:
	static int distance_2_kdTree( const std::vector<cv::Point> & a, const std::vector<cv::Point> & b,
	my_kd_tree_t *aTree,
	my_kd_tree_t* bTree);
	static int maxint(int a , int b){ if(a>b) return a; return b;}
public:
	static double distance_hausdorff_kdTree ( const std::vector<cv::Point> & a, const std::vector<cv::Point> & b , my_kd_tree_t *aTree,
	my_kd_tree_t* bTree);
};

