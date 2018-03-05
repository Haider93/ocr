#include "TrainingDataLoader\NukhtasTrainingDataLoader.hpp"
#include "TrainingDataModel\NukhtaTrainingData.h"
#include "TrainingDataLoader\BaseShapeTrainingDataLoader.hpp"
#include "TrainingDataLoader\TrainingDataPopulator\BaseShapes\LazyBaseShapesTrainingDataPopulater.h"
#include "BlobFinderAction.h"
#include "BaseShapeAndNukhtaFilterAction.h"
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
#include <ShlObj.h>
#include <math.h>
#include "kdTreeNode.h"


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //Thanks to Alessandro
#include <opencv2/nonfree/nonfree.hpp>
#include "LineBreakerAction.h"
#include "INIReader.h"

#include <activemq\library\ActiveMQCPP.h>
#include <decaf\lang\Thread.h>
#include <decaf\lang\Runnable.h>
#include <decaf\util\concurrent\CountDownLatch.h>
#include <decaf\lang\Integer.h>
#include <decaf\lang\Long.h>
#include <decaf\lang\System.h>
#include <activemq\core\ActiveMQConnectionFactory.h>
#include <activemq\util\Config.h>
#include <cms\Connection.h>
#include <cms\Session.h>
#include <cms\TextMessage.h>
#include <cms\BytesMessage.h>
#include <cms\MapMessage.h>
#include <cms\ExceptionListener.h>
#include <cms\MessageListener.h>
#include <memory>
#include <json\json.h>
#include <json\reader.h>
#include <WinBase.h>


//#include <nanoflann.hpp>
//#include <utils.h>

//using namespace nanoflann;
using namespace cv;
using namespace tbb;
using namespace activemq::core;
using namespace decaf::util::concurrent;
using namespace decaf::util;
using namespace decaf::lang;
using namespace cms;
using namespace std;


NukhtaTrainingData *nukhtaTrainingData;
LazyBaseShapeTrainingData *lazyBaseShapeTrainingData;


void init_global_data()
{
	cout<<"Loading Training Data..."<<endl;
	//Load data from ini file
	/*INIReader reader("config.ini");
    if (reader.ParseError() < 0) {
		std::cout << reader.ParseError()<<" Can't load 'config.ini'\n";
        return;
    }
    cout  << reader.Get("Training_Files", "nukhtaTrainingFile", "UNKNOWN")<<endl;
	cout << reader.Get("Training_Files", "baseTrainingFile", "UNKNOWN")<<endl;*/
	// Load nukhtas training data
	double m_start_time = clock();
	string nukhtaTrainingFile = "D:\\600dpi_16fs\\Nukhtas.txt";
	//string nukhtaTrainingFile = reader.Get("Training_Files", "nukhtaTrainingFile", "UNKNOWN");
	NukhtasTrainingDataLoader *nukhtasTrainingDataLoader = NukhtasTrainingDataLoader::getInstance();
	nukhtaTrainingData  = nukhtasTrainingDataLoader->load(nukhtaTrainingFile);
	double m_end_time = clock();
	double time_taken=(m_end_time-m_start_time)/double(CLOCKS_PER_SEC);
	delete nukhtasTrainingDataLoader;

	double m_start_time1 = clock();
	// Load base shapes training data
	string baseTrainingFile = "D:\\600dpi_16fs\\ShapesComplete.txt";
	//string baseTrainingFile = reader.Get("Training_Files", "baseTrainingFile", "UNKNOWN");
	string userImageFile="D:\\600dpi_16fs\\userImages.txt";
	//string userImageFile=reader.Get("Training_Files", "userImageFile", "UNKNOWN");;
	BaseShapeTrainingDataLoader *baseShapeTrainingDataLoader = BaseShapeTrainingDataLoader::getInstance();
	lazyBaseShapeTrainingData = (LazyBaseShapeTrainingData*)baseShapeTrainingDataLoader->load(baseTrainingFile,userImageFile);
	delete baseShapeTrainingDataLoader;
	double m_end_time1 = clock();
	double time_taken1=(m_end_time1-m_start_time1)/double(CLOCKS_PER_SEC);
}


//void test_knn()
//{
//	Mat matches; //This mat will contain the index of nearest neighbour as returned by Kd-tree
//	Mat distances; //In this mat Kd-Tree return the distances for each nearest neighbour
//	Mat ClusterMembers; //This Set A
//	Mat ClusterCenters;  //This set B
//	const cvflann::SearchParams params(32); //How many leaves to search in a tree
//	cv::flann::GenericIndex< cvflann::L2<int> > *kdtrees; // The flann searching tree
//
//	// Create matrices
//	ClusterMembers.create(cvSize(2,100), CV_32S); // The set A
//	randu(ClusterMembers, Scalar::all(0), Scalar::all(1000));
//	ClusterCenters.create(cvSize(2,5000), CV_32S); // The set B
//	randu(ClusterCenters, Scalar::all(0), Scalar::all(1000));
//	matches.create(cvSize(1,100), CV_32SC1);
//	distances.create(cvSize(1,100), CV_32FC1);
//	kdtrees =  new flann::GenericIndex< cvflann::L2<int> >(ClusterCenters, cvflann::KDTreeSingleIndexParams (4)); // a 4 k-d tree
//
//	// Search KdTree
//	kdtrees->knnSearch(ClusterMembers, matches, distances, 1,  cvflann::SearchParams(8));
//	int NN_index;
//	float dist;
//	for(int i = 0; i < 100; i++) {
//		NN_index = matches.at<int>(i,0);
//		dist = distances.at<float>(i, 0);
//	}
//}
//
//
//
//void test_knn_3()
//{
//	// Sample data
//	std::vector<cv::Point2d> points1;
//	points1.emplace_back(cv::Point2d(438.6,268.8));
//	points1.emplace_back(cv::Point2d(439.1,268.6));
//	points1.emplace_back(cv::Point2d(438.2,268.1));
//	points1.emplace_back(cv::Point2d(498.3,285.9));
//	points1.emplace_back(cv::Point2d(312.9,245.9));
//	points1.emplace_back(cv::Point2d(313.4,245.7));
//	points1.emplace_back(cv::Point2d(313.1,245.5));
//	points1.emplace_back(cv::Point2d(312.5,245.4));
//	points1.emplace_back(cv::Point2d(297.6,388.1));
//	points1.emplace_back(cv::Point2d(291.7,309.8));
//	points1.emplace_back(cv::Point2d(194.1,369.8));
//	points1.emplace_back(cv::Point2d(439.9,314.9));
//	points1.emplace_back(cv::Point2d(312.8,246.0));
//	
//	cv::Mat_<float> features(0,2);
//	for(int i = 0; i <points1.size();i++) {
//		//Fill matrix
//		cv::Mat row = (cv::Mat_<float>(1, 2) <<  points1.at(i).x, points1.at(i).y);
//		features.push_back(row);
//
//	}
//
//
//	//cvflann::Index<cvflann::L2_Simple<float>> index;
//	//index.buildIndex();
//	//cvflann::KDTreeSingleIndexParams
//
//	//cvflann::Index
//
//    //cvflann::Index<cvflann::L2_Simple<float> > index(features, cvflann::KDTreeSingleIndexParams(2, false));
//    
//	cv::flann::GenericIndex< cvflann::L2<float> > *kdtrees; // The flann searching tree
//	//kdtrees =  new flann::GenericIndex(features, cvflann::KDTreeSingleIndexParams (2)); // a 4 k-d tree
//	kdtrees =  new flann::GenericIndex< cvflann::L2<float> >(features, cvflann::KDTreeSingleIndexParams(4,false,2)); // a 4 k-d tree
//
//	// cv::flann::Index< cvflann::L2<float> > *kdtrees; // The flann searching tree
//	//kdtrees =  new flann::Index< cvflann::L2<float> >(features, cvflann::KDTreeSingleIndexParams (2)); // a 4 k-d tree
//
//	//cv::flann::Index flann_index1(features, cvflann::KDTreeSingleIndexParams (2));
//	
//	
//	// Create an empty Mat for the features that includes dimensional
//	// space for an x and y coordinate
//	//std::cout << features << std::endl;
//    cv::flann::Index flann_index(features, cv::flann::KDTreeIndexParams(1));
//
//	unsigned int max_neighbours = 1;
//	
//	cv::Mat_<float> query(0,2);
//	cv::Mat row1 = (cv::Mat_<float>(1, 2) <<  315.8,246.0);
//	cv::Mat row2 = (cv::Mat_<float>(1, 2) <<  316.8,246.0);
//	
//	query.push_back(row1);
//	query.push_back(row2);
//
//	std::cout << query << std::endl;
//	//cv::Mat query = (cv::Mat_<float>(1, 2) << 312.8,246.0);
//	cv::Mat indices, dists; //neither assume type nor size here !
//
//	flann_index.knnSearch(query, indices, dists, max_neighbours,  cv::flann::SearchParams(32));
//	cout<< indices.at<int>(0,0)<<endl;
//	cout<< dists.at<float>(0,0)<<endl;
//    cout<< indices.at<int>(1,0)<<endl;
//	cout<< dists.at<float>(1,0)<<endl;
//
//	cv::Mat indices1, dists1; //neither assume type nor size here !
//	kdtrees->knnSearch(query, indices1, dists1, max_neighbours);
//    cout<< indices1.at<int>(0,0)<<endl;
//	cout<< dists1.at<float>(0,0)<<endl;
//    cout<< indices1.at<int>(1,0)<<endl;
//	cout<< dists1.at<float>(1,0)<<endl;
//
//	cerr << indices.type() << endl << indices << endl;
//	cerr << dists.type() << endl << dists << endl;
//	int i  = 0;
//}


//void test_knn_1()
//{
//	test_knn_3();
//	std::vector<cv::Point2d> points1;
//	points1.push_back(cv::Point2d(5.0,4.0));
//	points1.push_back(cv::Point2d(5.0,4.0));
//	
//	Mat ClusterCenters;  //This set B
//	ClusterCenters.create(cvSize(2,100), CV_32S);
//	randu(ClusterCenters, Scalar::all(0), Scalar::all(1000));
//
//	imshow("test",ClusterCenters);
//	waitKey();
//	//for(int i = 0; i <points1.size();i++) {
//		//Fill matrix
//		//cv::Mat row = (cv::Mat_<double>(1, 2) << points1.at(i).x, points1.at(i).y);
//		//ClusterCenters.at<uchar>(points1.at(i).y,points1.at(i).x) = 255;
//	//}
//
//	Mat ClusterMembers; //This Set A
//
//	Mat matches; //This mat will contain the index of nearest neighbour as returned by Kd-tree
//	Mat distances; //In this mat Kd-Tree return the distances for each nearest neighbour
//
//	const cvflann::SearchParams params(32); //How many leaves to search in a tree
//	cv::flann::GenericIndex< cvflann::L2<int> > *kdtrees; // The flann searching tree
//
//	// Create matrices
//	//ClusterMembers.create(cvSize(2,100), CV_32S); // The set A
//	//randu(ClusterMembers, Scalar::all(0), Scalar::all(1000));
//	//ClusterCenters.create(cvSize(2,5000), CV_32S); // The set B
//	//randu(ClusterCenters, Scalar::all(0), Scalar::all(1000));
//	//matches.create(cvSize(1,100), CV_32SC1);
//	//distances.create(cvSize(1,100), CV_32FC1);
//	kdtrees =  new flann::GenericIndex< cvflann::L2<int> >(ClusterCenters, cvflann::KDTreeSingleIndexParams (4)); // a 4 k-d tree
//
//	// Search KdTree
//	kdtrees->knnSearch(ClusterMembers, matches, distances, 1,  cvflann::SearchParams(8));
//	int NN_index;
//	float dist;
//	for(int i = 0; i < 100; i++) {
//		NN_index = matches.at<int>(i,0);
//		dist = distances.at<float>(i, 0);
//	}
//}



void scanSingleLine(string imageFile , string folderPath)
{
	IplImage* src= cvLoadImage(imageFile.c_str(),0);
    Mat imagegray(src); //no copy

	if(!imagegray.data) {		
		printf("File Not Found %s\n",imageFile.c_str());
		return;
	}
	
	// convert it to full binary image
	cv::threshold(imagegray, imagegray, 150.0, 255.0, cv::THRESH_BINARY);
	
	// Find all  connected blob in provided image
	BlobFinderActionInput *inp1 = new BlobFinderActionInput(imagegray);
	BlobFinderAction *act1 = new BlobFinderAction;
	BlobFinderActionOutput *out1= act1->execute(inp1);
	double time1 = act1->getExecutionTime();
	
	// Filter Base Shape And nukhtas into speparate collection
	BaseShapeAndNukhtaFilterInput *inp2 = new BaseShapeAndNukhtaFilterInput(out1,nukhtaTrainingData,folderPath);
	BaseShapeAndNukhtaFilterAction *act2 = new BaseShapeAndNukhtaFilterAction;
	BaseShapeAndNukhtaFilterOutput *out2 = act2->execute(inp2);
	double time2 = act2->getExecutionTime();

	// Associate base shape with nukhtas
	NukhtaAndBaseShapeAssociationActionInput *inp3 = new NukhtaAndBaseShapeAssociationActionInput;
	inp3->m_baseShapeAndNukhtaFilterOutput = out2;
	inp3->m_blobFinderActionOutput = out1;
	inp3->m_size = imagegray.size();
	NukhtaAndBaseShapeAssociationAction *act3 = new NukhtaAndBaseShapeAssociationAction;
	NukhtaAndBaseShapeAssociationActionOutput *out3 = act3->execute(inp3);
	double time3 = act3->getExecutionTime();

	// Finally match baseshapes and write them
	UrduShapeMatchActionInput *inp4 = new UrduShapeMatchActionInput;
	inp4->m_baseShapeAndNukhtaFilterOutput = out2;
	inp4->m_blobFinderActionOutput = out1;
	inp4->lazyBaseShapeTrainingData = lazyBaseShapeTrainingData;
	UrduShapeMatchAction *act4 = new UrduShapeMatchAction;
	UrduShapeMatchActionOutput *out4 = act4->execute(inp4);
	double time4 = act4->getExecutionTime();

	printf("\nFile %s, 1 = %lf , 2 = %lf , 3 = %lf , 4 = %lf",imageFile.c_str(),time1,time2,time3,time4);
	
	delete inp1;delete act1;delete out1;
	delete inp2;delete act2;delete out2;
	delete inp3;delete act3;delete out3;
	delete inp4;delete act4;delete out4;
	
	imagegray.release();
	cvReleaseImage(&src);
}

void deskew(string file,double angle)
{
	Mat src=imread(file,0);//original image with angle of rotation
	bitwise_not(src,src);
	//Canny(src, src, 50, 200, 3);
	vector<Point> points;
	Mat_<uchar>::iterator it=src.begin<uchar>();
	Mat_<uchar>::iterator end=src.end<uchar>();
	for(;it!=end;++it)
		if(*it)
		points.push_back(it.pos());

	RotatedRect box=minAreaRect(Mat(points));
	//Rotation
	Mat rot_mat=getRotationMatrix2D(box.center,angle,1);
	Mat rotated;
	warpAffine(src,rotated,rot_mat,src.size(),INTER_CUBIC,BORDER_REPLICATE);//rotated image with that angle
	imwrite(file,rotated);
	//cropping
	Size box_size=box.size;
	if(box.angle<-45.)
		//swap(box_size.width,box_size.height);
		box.angle=-(90+box.angle);
	else
		box.angle=-box.angle;
	Mat cropped;
	getRectSubPix(rotated,box_size,box.center,cropped);//cropped image after rotation
	imwrite(file,cropped);
	return;
}

//compute skew angle2
double compute_skew(Mat &img)
{

    // Binarize
    cv::threshold(img, img, 225, 255, cv::THRESH_BINARY);

    // Invert colors
    cv::bitwise_not(img, img);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 3));
    cv::erode(img, img, element);

    std::vector<cv::Point> points;
    cv::Mat_<uchar>::iterator it = img.begin<uchar>();
    cv::Mat_<uchar>::iterator end = img.end<uchar>();
    for (; it != end; ++it)
        if (*it)
            points.push_back(it.pos());

    cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));

    double angle = box.angle;
    if (angle < -45.)
        angle += 90.;

    cv::Point2f vertices[4];
    box.points(vertices);
    for(int i = 0; i < 4; ++i)
        cv::line(img, vertices[i], vertices[(i + 1) % 4], cv::Scalar(255, 0, 0), 1, CV_AA);

    return angle;
}


//detect skew angle
double computeSkew(string file)
{
	Mat src=imread(file,0);
	 if (src.empty()) //check whether the image is loaded or not
     {
          cout << "Error : Image cannot be loaded..!!" << endl;
          return 0;
     }
	//Mat dst;
	//Canny(src, dst, 50, 200, 3);
	Size size=src.size();
	bitwise_not(src,src);
	vector<Vec4i> lines;
	//HoughLinesP(dst,lines,1,CV_PI/180,50,50,10);
	HoughLinesP(src,lines,1,CV_PI/180,80,30,10);
	//Mat disp_lines(size,CV_8UC1,Scalar(0,0,0));
	double angle=0;
	unsigned n_lines=lines.size();
	for(unsigned i=0;i<n_lines;i++)
	{
		//line(disp_lines,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(255,0,0));
		angle+=atan2((double)lines[i][3]-lines[i][1],
					(double)lines[i][2]-lines[i][0]);
	}
	angle/=n_lines;
	cout<<lines.size()<<endl;
	cout<<"Angle of skew in document "<<file<<" is "<<angle*180/CV_PI<<endl;
	//deskew the doc image
	deskew(file,angle);

	//imshow(file,disp_lines);
	//waitKey(0);
	return angle;
}

void just_for_debugging_lines(string pathOfDocument,int linesInDocument)
//void just_for_debugging_lines()
{
	//string pathOfDocument="D:\\doc3\\doc3.bmp";
	//int linesInDocument=21;
	//double angle=computeSkew(pathOfDocument);//compute doc image tilt
	
	//skew detection version 2
	/*Mat img=imread(pathOfDocument,0);
	double angle = compute_skew(img);
	deskew(pathOfDocument,angle);*/
	
	//vector<pair<string,string>> lines = linebreak(pathOfDocument,21);//slice lines from document

	LineBreakerActionInput* inp=new LineBreakerActionInput(pathOfDocument,linesInDocument);
	LineBreakerAction* act=new LineBreakerAction;
	LineBreakerActionOutput* out=act->execute(inp);
	vector<pair<string,string>> lines=out->getOutputOfLineBreaker();
	double time = act->getExecutionTime();
	cout<<"Time taken in line splitting : "<<time<<endl;


	//init_global_data();//load training data
	int x;
	cout<<"\n Starting Scan other ....";

	tbb::task_arena limited_arena(3);
	double m_start_time = clock();
	
	limited_arena.execute([&]{ 
		tbb::parallel_for(0, (int)lines.size(),
			[&] (int &i) {
				scanSingleLine(lines.at(i).first,lines.at(i).second);
		}); 
	});
	
	/*tbb::parallel_for(size_t(0), size_t(lines.size()),
		[&] (size_t i) {
			scanSingleLine(lines.at(i).first,lines.at(i).second);
	});
	double m_end_time = clock();
	double time_taken=(m_end_time-m_start_time)/double(CLOCKS_PER_SEC);*/
	
	//double m_start_time = clock();
	// test purpose
	//scanSingleLine(lines.at(0).first,lines.at(0).second);
	//double m_start_time = clock();
	
	//for(int i = 0 ; i<lines.size();i++){
		//scanSingleLine(lines.at(i).first,lines.at(i).second);
	//}

	double m_end_time = clock();
	double time_taken=(m_end_time-m_start_time)/double(CLOCKS_PER_SEC);

	extern double totalTimeSpentInTreeBuildingFlannNano;
	extern double totalTimeSpentInTreeBuildingFlannNano1;
    extern double totalTimeSpentInKDMatchingNano;
    extern double totalTimeSpentInNonKDMTCHING;
	
	printf("\n Time Spent in building kd-tree for traindata %lf",totalTimeSpentInTreeBuildingFlannNano);
	printf("\n Time Spent in building kd-tree for Scanned %lf",totalTimeSpentInTreeBuildingFlannNano1);
	//printf("\n Time Spent in matching kd-tree %lf",totalTimeSpentInKDMatchingNano);
	//printf("\n Time Spent in matching non %lf",totalTimeSpentInNonKDMTCHING);
	
	cout<<"\n Total Time taken = "<<time_taken;
	//delete nukhtaTrainingData;
	//delete lazyBaseShapeTrainingData;
}


void just_for_debugging()
{
	string pathOfDocument="E:\\test\\Shape1.bmp";
	string folder = "E:\\test";
	init_global_data();

	double m_start_time = clock();
	scanSingleLine(pathOfDocument,folder);
	double m_end_time = clock();
	double time_taken=(m_end_time-m_start_time)/double(CLOCKS_PER_SEC);

	delete nukhtaTrainingData;
	delete lazyBaseShapeTrainingData;
}
//
//
//void test_knn_3()
//{
//	// Sample data
//	std::vector<cv::Point2d> points1;
//	points1.emplace_back(cv::Point2d(438.6,268.8));
//	points1.emplace_back(cv::Point2d(439.1,268.6));
//	points1.emplace_back(cv::Point2d(438.2,268.1));
//	points1.emplace_back(cv::Point2d(498.3,285.9));
//	points1.emplace_back(cv::Point2d(312.9,245.9));
//	points1.emplace_back(cv::Point2d(313.4,245.7));
//	points1.emplace_back(cv::Point2d(313.1,245.5));
//	points1.emplace_back(cv::Point2d(312.5,245.4));
//	points1.emplace_back(cv::Point2d(297.6,388.1));
//	points1.emplace_back(cv::Point2d(291.7,309.8));
//	points1.emplace_back(cv::Point2d(194.1,369.8));
//	points1.emplace_back(cv::Point2d(439.9,314.9));
//	points1.emplace_back(cv::Point2d(312.8,246.0));
//
//	float query_pt[2] = { 312.8, 246.0};
//
//	double m_start_time1 = clock();		
//	PointCloud<float> cloud;
//	int N = points1.size();
//
//	// Generating Random Point Cloud
//	cloud.pts.resize(N);
//	for (size_t i = 0; i < N; i++)
//	{
//		cloud.pts[i].x = points1[i].x;
//		cloud.pts[i].y = points1[i].y;
//	}
//
//	// construct a kd-tree index for b set:
//	typedef KDTreeSingleIndexAdaptor<
//		L2_Simple_Adaptor<float, PointCloud<float> > ,
//		PointCloud<float>,
//		2 /* dim */
//	> my_kd_tree_t;
//
//	my_kd_tree_t   index(2 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(1/* max leaf */) );
//	index.buildIndex();
//
//	// do a knn search
//	const size_t num_results = 1;
//	size_t ret_index;
//	float out_dist_sqr;
//
//	index.knnSearch(&query_pt[0],1,&ret_index, &out_dist_sqr);
//    int yy = 0;
//
//
//}








//template <typename num_t>
//void kdtree_demo(const size_t N)
//{
//	PointCloud<num_t> cloud;
//
//	// Generate points:
//	generateRandomPointCloud(cloud, N);
//
//	num_t query_pt[3] = { 0.5, 0.5, 0.5 };
//
//	// construct a kd-tree index:
//	typedef KDTreeSingleIndexAdaptor<
//		L2_Simple_Adaptor<num_t, PointCloud<num_t> > ,
//		PointCloud<num_t>,
//		3 /* dim */
//		> my_kd_tree_t;
//
//	dump_mem_usage();
//
//	my_kd_tree_t   index(3 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(10 /* max leaf */) );
//	index.buildIndex();
//
//	dump_mem_usage();
//	{
//		// do a knn search
//		const size_t num_results = 1;
//		size_t ret_index;
//		num_t out_dist_sqr;
//		nanoflann::KNNResultSet<num_t> resultSet(num_results);
//		resultSet.init(&ret_index, &out_dist_sqr );
//		index.findNeighbors(resultSet, &query_pt[0], nanoflann::SearchParams(10));
//
//		std::cout << "knnSearch(nn="<<num_results<<"): \n";
//		std::cout << "ret_index=" << ret_index << " out_dist_sqr=" << out_dist_sqr << endl;
//	}
//	{
//		// Unsorted radius search:
//		const num_t radius = 1;
//		std::vector<std::pair<size_t, num_t> > indices_dists;
//		RadiusResultSet<num_t,size_t> resultSet(radius, indices_dists);
//
//		index.findNeighbors(resultSet, query_pt, nanoflann::SearchParams());
//
//		// Get worst (furthest) point, without sorting:
//		std::pair<size_t,num_t> worst_pair = resultSet.worst_item();
//		cout << "Worst pair: idx=" << worst_pair.first << " dist=" << worst_pair.second << endl;
//	}
//
//}

void test_sift()
{
	string file = "E:\\600dpi_new_database\\600dpi_16fs\\Nukhtas\\dnukhta_1_62.bmp";
	const cv::Mat input = cv::imread("E:\\600dpi_new_database\\600dpi_16fs\\Nukhtas\\dnukhta_1_62.bmp", 0); //Load as grayscale

	cv::threshold(input, input, 150.0, 255.0, cv::THRESH_BINARY);
    //cv::SiftFeatureDetector detector;
    std::vector<cv::KeyPoint> keypoints;
    


	//vector<vector<Point>> shapecontour;		
	//CalcContour::calcContour(file,shapecontour); // calculate its contour
	//vector<Point> shapeContour = shapecontour[0];
	//

	//int baseShapeIndex = baseShapes.at(baseIndexCounter);
	//		IplImage* src = cvCreateImage(cv::Size(size.width+2,size.height+2), 8, 1);
	//		Mat fullShape(src);//no copy					
	//		fullShape.setTo(0);
	//		
	//		
	//		int blobIndex = rects.at(baseShapeIndex).first;

	//		// find its rect and fill all pixels
	//		for(size_t j= 0; j < blobs[blobIndex].size(); j++) 
	//		{
	//			int x = blobs[blobIndex][j].x;
	//			int y = blobs[blobIndex][j].y;				
	//			x++;y++;

	//			if(x < xmin) xmin = x;
	//			if(y < ymin) ymin = y;

	//			if(x > xmax) xmax = x;
	//			if(y > ymax) ymax = y;

	//			fullShape.at<uchar>(y,x) = 255;								
	//		}


	
	
	//detector.detect(input, keypoints);

	// initialize detector and extractor
    FeatureDetector* detector;

	detector  = new SiftFeatureDetector(
                                     5, // nFeatures
                                     3, // nOctaveLayers
                                     0.04, // contrastThreshold
                                     200, //edgeThreshold
                                     1.6 //sigma
                                     );


	detector->detect(input, keypoints);

    // Add results to image and save.
    cv::Mat output;
    cv::drawKeypoints(input, keypoints, output);
    cv::imwrite("E:\\test\\c_14_features.bmp", output);
	cv::imshow("test",output);
	cv::waitKey(0);
    return ;
}

/***************/
string path="";
int lines1=0;

vector<string> paths;
vector<int> lineCount;

//extract path and number of lines in document
void extractPathAndLineCount(string text)
{
	int pos=text.find_first_of(',');
	path=text.substr(0,pos);
	lines1=stoi(text.substr(pos+1));
	return;
}
/**************/

/********************/
//Consumer Class Definition
class HelloWorldConsumer : public ExceptionListener,
                           public MessageListener,
                           public Runnable {

private:

    CountDownLatch latch;
    CountDownLatch doneLatch;
    Connection* connection;
    Session* session;
    Destination* destination;
    MessageConsumer* consumer;
    long waitMillis;
    bool useTopic;
    bool sessionTransacted;
    std::string brokerURI;

private:

    HelloWorldConsumer(const HelloWorldConsumer&);
    HelloWorldConsumer& operator=(const HelloWorldConsumer&);

public:

    HelloWorldConsumer(const std::string& brokerURI, int numMessages, bool useTopic = false, bool sessionTransacted = false, int waitMillis = 30000) :
        latch(1),
        doneLatch(numMessages),
        connection(NULL),
        session(NULL),
        destination(NULL),
        consumer(NULL),
        waitMillis(waitMillis),
        useTopic(useTopic),
        sessionTransacted(sessionTransacted),
        brokerURI(brokerURI) {
    }

    virtual ~HelloWorldConsumer() {
        cleanup();
    }

    void close() {
        this->cleanup();
    }

    void waitUntilReady() {
        latch.await();
    }

    virtual void run() {

        try {

            // Create a ConnectionFactory
            auto_ptr<ConnectionFactory> connectionFactory(
                ConnectionFactory::createCMSConnectionFactory(brokerURI));

            // Create a Connection
            connection = connectionFactory->createConnection("admin","admin");
            connection->start();
            connection->setExceptionListener(this);

            // Create a Session
            if (this->sessionTransacted == true) {
                session = connection->createSession(Session::SESSION_TRANSACTED);
            } else {
                session = connection->createSession(Session::AUTO_ACKNOWLEDGE);
            }

            // Create the destination (Topic or Queue)
            if (useTopic) {
                destination = session->createTopic("ocr_queue");
            } else {
                destination = session->createQueue("ocr_queue");
            }

            // Create a MessageConsumer from the Session to the Topic or Queue
            consumer = session->createConsumer(destination);

            consumer->setMessageListener(this);

            std::cout.flush();
            std::cerr.flush();

            // Indicate we are ready for messages.
            latch.countDown();

            // Wait while asynchronous messages come in.
            doneLatch.await(waitMillis);

        } catch (CMSException& e) {
            // Indicate we are ready for messages.
            latch.countDown();
            e.printStackTrace();
        }
    }


    // Called from the consumer since this class is a registered MessageListener.
    virtual void onMessage(const Message* message) {

        static int count = 0;

        try {
            count++;
            const TextMessage* textMessage = dynamic_cast<const TextMessage*> (message);			
            string text = "";

            if (textMessage != NULL) {
                text = textMessage->getText();
            } else {
                text = "NOT A TEXTMESSAGE!";
            }

            printf("Message #%d Received: %s\n", count, text.c_str());

			//find path and number of lines in page
			extractPathAndLineCount(text);
			//call OCR and sleep consumer Thread for 15 seconds
			/*cout<<"Path of Document : "<<path<<endl;
			cout<<"Lines in document : "<<lines<<endl;*/
			paths.push_back(path);
			lineCount.push_back(lines1);
			//if(path!=""&&lines!=0)
			just_for_debugging_lines(path,lines1);//execution starts
			

        } catch (CMSException& e) {
            e.printStackTrace();
        }

        // Commit all messages.
        if (this->sessionTransacted) {
            session->commit();
        }

        // No matter what, tag the count down latch until done.
        doneLatch.countDown();
    }

    // If something bad happens you see it here as this class is also been
    // registered as an ExceptionListener with the connection.
    virtual void onException(const CMSException& ex AMQCPP_UNUSED) {
        printf("CMS Exception occurred.  Shutting down client.\n");
        ex.printStackTrace();
        exit(1);
    }

private:

    void cleanup() {
        if (connection != NULL) {
            try {
                connection->close();
            } catch (cms::CMSException& ex) {
                ex.printStackTrace();
            }
        }

        // Destroy resources.
        try {
            delete destination;
            destination = NULL;
            delete consumer;
            consumer = NULL;
            delete session;
            session = NULL;
            delete connection;
            connection = NULL;
        } catch (CMSException& e) {
            e.printStackTrace();
        }
    }
};
/*******************/

int i=1;
int m=0;
void runBroker()
{
	activemq::library::ActiveMQCPP::initializeLibrary();
	{
	string brokerURI =
        "failover:(tcp://localhost:61616"
//		?wireFormat=stomp"
//        "&transport.useInactivityMonitor=false"
//        "&connection.alwaysSyncSend=true"
//        "&connection.useAsyncSend=true"
//        "?transport.commandTracingEnabled=true"
//        "&transport.tcpTracingEnabled=true"
//        "&wireFormat.tightEncodingEnabled=true"
        ")";


	bool useTopics = false;
    bool sessionTransacted = false;
    int numMessages = 2;
	m=numMessages;
	HelloWorldConsumer consumer(brokerURI, numMessages, useTopics, sessionTransacted);
	Thread consumerThread(&consumer);
    consumerThread.start();

	while(i++<=m)
	consumerThread.run();

    // Wait for the consumer to indicate that its ready to go.
    consumer.waitUntilReady();
	consumerThread.join();
	consumer.close();

	}
	//activemq::library::ActiveMQCPP::shutdownLibrary();

	delete nukhtaTrainingData;
	delete lazyBaseShapeTrainingData;

}



#include <ctime>
#include <cstdlib>
#include <iostream>

//int i=1;
//int m=0;
int main(int argc, char** argv)
{
	//int i = 0;

	//Sleep(10000);
	init_global_data();//load training data

	
	//test_sift();
	//just_for_debugging_lines();
	/*if(argc==3){
		just_for_debugging_lines(argv[1],atoi(argv[2]));
	}*/

	//while(i++<100)
	runBroker();
	

	//just_for_debugging();
	// Randomize Seed
	//srand(time(NULL));
	//kdtree_demo<float>(100);
	//kdtree_demo<double>(1000000);

	//test_knn_1();
	//test_knn_3();
	//return 0;

}