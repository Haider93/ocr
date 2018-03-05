#include "HausdorffDistance.h"
#include "TreeKD.h"
#include "opencv2/flann/flann.hpp"

double totalTimeSpentInTreeBuilding = 0;
double totalTimeSpentInKDMatching = 0;
double totalTimeSpentInNonKDMTCHING = 0;
int HausdorffDistance ::distance_2( const std::vector<cv::Point> &a, const std::vector<cv::Point>  &b)
{
	double m_start_time2 = clock();
	int maxDistAB = 0;
	for (size_t i=0; i<a.size(); i++)
	{
		int minB = 1000000;
		for (size_t j=0; j<b.size(); j++)
		{
			int dx = (a[i].x - b[j].x);     
			int dy = (a[i].y - b[j].y);     
			int tmpDist = dx*dx + dy*dy;

			if (tmpDist < minB)
			{
				minB = tmpDist;
			}
			if ( tmpDist == 0 )
			{
				break; // can't get better than equal.
			}
		}
		maxDistAB += minB;
	}

	double m_end_time2 = clock();
    double time_taken2=(m_end_time2-m_start_time2)/double(CLOCKS_PER_SEC);
	totalTimeSpentInNonKDMTCHING+= time_taken2;
	return maxDistAB;
}

double HausdorffDistance::distance_hausdorff( const std::vector<cv::Point> & a, const std::vector<cv::Point> & b)
{
	//printf("\n Approx size = %d , %d , %d" , a.size(),b.size(),a.size()*b.size());
	int maxDistAB = distance_2( a, b );
	int maxDistBA = distance_2( b, a );
	int maxDist = maxint(maxDistAB,maxDistBA);
	return std::sqrt((double)maxDist);
}

double HausdorffDistance::distance_hausdorff_kdTree( const std::vector<cv::Point> & a, const std::vector<cv::Point> & b)
{
	int maxDistAB = distance_2_kdTree( a, b );
	int maxDistBA = distance_2_kdTree( b, a );   
	int maxDist = maxint(maxDistAB,maxDistBA);
	return std::sqrt((double)maxDist);
}

int HausdorffDistance::distance_2_kdTree( const std::vector<cv::Point> & a, const std::vector<cv::Point> & b)
{
	double m_start_time1 = clock();		
	cv::Mat_<float> features(0,2);
	for(size_t ii = 0; ii<b.size(); ii++) {
		//Fill matrix
		cv::Mat row = (cv::Mat_<float>(1, 2) <<  b[ii].x, b[ii].y);
		features.push_back(row);

	}
	double m_end_time1 = clock();
    double time_taken1=(m_end_time1-m_start_time1)/double(CLOCKS_PER_SEC);
	cv::flann::Index flann_index(features, cv::flann::KDTreeIndexParams(4));
	totalTimeSpentInTreeBuilding+=time_taken1;
	
	double m_start_time2 = clock();	
	int maxDistAB = 0;
	

	cv::Mat_<float> query(0,2);
	
	for (size_t i = 0; i<a.size(); i++)
	{
		//Fill matrix
		cv::Mat row = (cv::Mat_<float>(1, 2) <<  a[i].x, a[i].y);
		query.push_back(row);
	}
	
	cv::Mat indices, dists; //neither assume type nor size here !
	unsigned int max_neighbours = 1;
	flann_index.knnSearch(query, indices, dists, max_neighbours,  cv::flann::SearchParams(1));
	/*for (size_t i = 0; i<a.size(); i++)
	{	
		unsigned int max_neighbours = 1;
		cv::Mat query = (cv::Mat_<float>(1, 2) << a[i].x, a[i].y);
		

		flann_index.knnSearch(query, indices, dists, max_neighbours,  cv::flann::SearchParams(1));
		
		int index = indices.at<int>(0,0);
		int dx2 = (a[i].x - b[index].x);     
		int dy2 = (a[i].y -  b[index].y);     
		int tmpDist = dx2*dx2 + dy2*dy2;
		int minB = tmpDist;
		maxDistAB += minB;
	}*/
	double m_end_time2 = clock();
    double time_taken2=(m_end_time2-m_start_time2)/double(CLOCKS_PER_SEC);
	totalTimeSpentInKDMatching+= time_taken2;
	return 0;
}









































double HausdorffDistance::modified_distance_hausdorff_kdTree(KDTreeMukul<int> *trainTree , const std::vector<cv::Point> & train, const std::vector<cv::Point> & scan)
{
	int maxDistAB = distance_2_kdTree(train,scan);
	int maxDistBA = modified_distance_2_kdTree(trainTree, scan, train );   
	int maxDist = maxint(maxDistAB,maxDistBA);
	return std::sqrt((double)maxDist);
}

int HausdorffDistance::modified_distance_2_kdTree( KDTreeMukul<int> *trainTreeForb ,const std::vector<cv::Point> & a, const std::vector<cv::Point> & b)
{
	int maxDistAB = 0;
	for (size_t i = 0; i<a.size(); i++)
	{
		int pTarget [2];
		pTarget[0] = a[i].x;
		pTarget[1] = a[i].y;
		KDNode<int>* nearest = trainTreeForb->find_nearest(pTarget);
		int dx2 = (a[i].x - nearest->x[0]);     
		int dy2 = (a[i].y -  nearest->x[1]);     
		int tmpDist = dx2*dx2 + dy2*dy2;
		int minB = tmpDist;
		maxDistAB += minB;
	}
	return maxDistAB;
}


