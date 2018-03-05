#include "KDHausdorff.h"
#include <nanoflann.hpp>
#include <utils.h>

using namespace nanoflann;

double totalTimeSpentInTreeBuildingNano = 0;
double totalTimeSpentInKDMatchingNano = 0;

double KDHausdorff::distance_hausdorff_kdTree( const std::vector<cv::Point> & a, const std::vector<cv::Point> & b)
{
	int maxDistAB = distance_2_kdTree( a, b );
	int maxDistBA = distance_2_kdTree( b, a );   
	int maxDist = maxint(maxDistAB,maxDistBA);
	return std::sqrt((double)maxDist);
}

int KDHausdorff::distance_2_kdTree( const std::vector<cv::Point> & a, const std::vector<cv::Point> & b)
{
	double m_start_time1 = clock();		
	PointCloud<float> cloud;
	int N = b.size();

	// Generating Random Point Cloud
	cloud.pts.resize(N);

	for (size_t i = 0; i < N; i++)
	{
		cloud.pts[i].x = b[i].x;
		cloud.pts[i].y = b[i].y;
	}

	// construct a kd-tree index for b set:
	typedef KDTreeSingleIndexAdaptor<
		L2_Simple_Adaptor<float, PointCloud<float> > ,
		PointCloud<float>,
		2 /* dim */
	> my_kd_tree_t;

	my_kd_tree_t   index(2 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(1/* max leaf */) );
	index.buildIndex();
	double m_end_time1 = clock();
    double time_taken1=(m_end_time1-m_start_time1)/double(CLOCKS_PER_SEC);
	totalTimeSpentInTreeBuildingNano+= time_taken1;

	double m_start_time2 = clock();	
	int maxDistAB = 0;

	for (size_t i = 0; i<a.size(); i++)
	{	
		const PointCloud<float> &cloud = index.dataset;


		// do a knn search
		const size_t num_results = 1;
		size_t ret_index;
		float out_dist_sqr;
		float query_pt[2] = { a[i].x, a[i].y};
		index.knnSearch(&query_pt[0],1,&ret_index, &out_dist_sqr);
		int dx2 = (a[i].x - b[ret_index].x);     
		int dy2 = (a[i].y -  b[ret_index].y);     
		int tmpDist = dx2*dx2 + dy2*dy2;
		int minB = tmpDist;
		maxDistAB += minB;
	}
	double m_end_time2 = clock();
    double time_taken2=(m_end_time2-m_start_time2)/double(CLOCKS_PER_SEC);
	totalTimeSpentInKDMatchingNano+= time_taken2;
	return maxDistAB;
}


