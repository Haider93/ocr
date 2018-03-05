#include "StoredKDTree.h"
using namespace nanoflann;

double StoredKDTree::distance_hausdorff_kdTree( const std::vector<cv::Point> & a, const std::vector<cv::Point> & b,
	my_kd_tree_t *aTree,
	my_kd_tree_t* bTree)
{
	int maxDistAB = distance_2_kdTree( a, b ,aTree, bTree);
	int maxDistBA = distance_2_kdTree( b, a , bTree, aTree);   
	int maxDist = maxint(maxDistAB,maxDistBA);
	return std::sqrt((double)maxDist);
}

int StoredKDTree::distance_2_kdTree( const std::vector<cv::Point> & a, const std::vector<cv::Point> & b , my_kd_tree_t *aTree,
	my_kd_tree_t* bTree)
{	
	int maxDistAB = 0;
	for (size_t i = 0; i<a.size(); i++)
	{	
		// do a knn search
		const size_t num_results = 1;
		size_t ret_index;
		float out_dist_sqr;
		float query_pt[2] = { a[i].x, a[i].y};
		bTree->knnSearch(&query_pt[0],1,&ret_index, &out_dist_sqr);
		int dx2 = (a[i].x - b[ret_index].x);     
		int dy2 = (a[i].y -  b[ret_index].y);     
		int tmpDist = dx2*dx2 + dy2*dy2;
		int minB = tmpDist;
		maxDistAB += minB;
	}
	return maxDistAB;
}


