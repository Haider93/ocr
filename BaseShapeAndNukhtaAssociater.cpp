#include "BaseShapeAndNukhtaAssociater.h"
#include "HausdorffDistance.h"
#include <vector>

NukhtaAndBaseShapeAssociationActionOutput* BaseShapeAndNukhtaAssociater::associate(NukhtaAndBaseShapeAssociationActionInput* inp)
{
	NukhtaAndBaseShapeAssociationActionOutput *o = new NukhtaAndBaseShapeAssociationActionOutput;
	double m_start_time = clock();
	associateBaseShapesWithNuktasFromBlobs(
		inp->m_baseShapeAndNukhtaFilterOutput->nukhtaShapes,
		inp->m_baseShapeAndNukhtaFilterOutput->baseShapes,
		inp->m_baseShapeAndNukhtaFilterOutput->shapeFilesNames,
		inp->m_baseShapeAndNukhtaFilterOutput->baseShapesWithNuktasList,
		inp->m_baseShapeAndNukhtaFilterOutput->nukhtaDescription,
		inp->m_blobFinderActionOutput->m_rects,
		inp->m_blobFinderActionOutput->m_blobs);

	o->dump(inp->m_baseShapeAndNukhtaFilterOutput->nukhtaShapes,
		inp->m_baseShapeAndNukhtaFilterOutput->baseShapes,
		inp->m_baseShapeAndNukhtaFilterOutput->shapeFilesNames,
		inp->m_baseShapeAndNukhtaFilterOutput->baseShapesWithNuktasList,
		inp->m_blobFinderActionOutput->m_rects,
		inp->m_blobFinderActionOutput->m_blobs,
		inp->m_baseShapeAndNukhtaFilterOutput->path,inp->m_size);
	double m_end_time = clock();
	double time_taken=(m_end_time-m_start_time)/double(CLOCKS_PER_SEC);

	return o;
}

void BaseShapeAndNukhtaAssociater::associateBaseShapesWithNuktasFromBlobs (const vector<int> &nukhtaShapes,
	const vector<int> &baseShapes,
	const map<int,string> &shapeFilesNames,
	map <int, vector< pair<int,string> >> &baseShapesWithNuktasList,
	const map<int,NukhtaShape::Nukhtas> &nukhtaDescs,
	const std::vector<pair<int,cv::Rect>> &rects,
	const std::vector <std::vector<cv::Point2i>> &blobs)
{

	double total_time_taken = 0;
	// looping on all nukhtas we find in image
	for(size_t nukhtaIndexCounter = 0; nukhtaIndexCounter < nukhtaShapes.size(); nukhtaIndexCounter++)
	{
		string pos = "",finalpos = ""; 		
		double distance ,minDistance = 99999999;
		int maxXPixelCoverage = -99999,xPixelCoverage;
		int closeShapeIndex = -1;
		size_t baseIndexCounter = 0 ; 

		for(baseIndexCounter = 0; baseIndexCounter < baseShapes.size(); baseIndexCounter++)
		{			
			cv::Rect nukhtaR = rects.at(nukhtaShapes.at(nukhtaIndexCounter)).second; // get rect of nukhta
			cv::Rect shapeR = rects.at(baseShapes.at(baseIndexCounter)).second;    // get rect of shape

			// check if rect intersect or their interval overlap with each other
			bool intersects = ((nukhtaR & shapeR).area() > 0);
			int s1,e1,s2,e2;
			s1 = nukhtaR.x;e1=nukhtaR.x + nukhtaR.width;
			s2 = shapeR.x;e2=shapeR.x + shapeR.width;
			bool isOverLap = (s2 <= e1 && s1 <= e2);

			// if its overlap // check for rectangle overlap on edge and sharing no pixels 
			if(intersects || isOverLap)
			{				
				// get position of nukta and and its distnace from base shape
				string pos_new;		
				double distance_new;
				int xPixelCoverage_new;
				
				double m_start_time = clock();
				getPercentAndPosition_new(nukhtaShapes.at(nukhtaIndexCounter),baseShapes.at(baseIndexCounter),pos,distance,xPixelCoverage,blobs,rects);
				double m_end_time = clock();
				double time_taken=(m_end_time-m_start_time)/double(CLOCKS_PER_SEC);
				total_time_taken+=time_taken;
				
				// choose closet shape to nukhta                                   
				if(distance < minDistance && xPixelCoverage > maxXPixelCoverage && distance>0 && xPixelCoverage > 0)					
				{
					finalpos = pos;
					closeShapeIndex = baseShapes.at(baseIndexCounter);
					minDistance = distance;
					maxXPixelCoverage = xPixelCoverage;
				}
			}
		}

		if(closeShapeIndex !=-1)
			// assign this nukhta to closeShapeIndex shape
			baseShapesWithNuktasList[closeShapeIndex].push_back(make_pair(nukhtaShapes.at(nukhtaIndexCounter),finalpos));
	}
}


int compvar(const void* a, const void* b)
{
	cv::Point2i* pa = (cv::Point2i*) a;
	cv::Point2i* pb = (cv::Point2i*) b;

	if (pb->x < pa->x)
		return 1;
	else if (pa->x < pb->x)
		return -1;
	return 0;
}

inline bool cmpID( const cv::Point2i p1 , const cv::Point2i p2) 
{
    return p1.x <= p2.x; 
}

void BaseShapeAndNukhtaAssociater::getPercentAndPosition_new(const int nukhtaIndex, 
	const int shapeIndex , 
	string &pos , 
	double &percent , 
	int &xPixelCoverage , 
	const std::vector < std::vector<cv::Point2i> > &blobs ,
	const std::vector <pair<int,cv::Rect>> &rects)
{
	
	bool upper = false,down = false;			
	int lxPixelCoverage = 0;
	int mindis = 99999;

	// find tag of  nukhta and shape
	size_t nukhtaBlobI = rects.at(nukhtaIndex).first;	
	size_t shapeBlobI =  rects.at(shapeIndex).first;	

	long int iter = blobs[shapeBlobI].size()*blobs[nukhtaBlobI].size();

	// sort shape and nukhta on x axis
	std::vector<cv::Point2i> shapePixels = blobs[shapeBlobI];
	std::vector<cv::Point2i> nukhtaPixels = blobs[nukhtaBlobI];
	
	qsort(&shapePixels[0], shapePixels.size(), sizeof(cv::Point2i), compvar);
	qsort(&nukhtaPixels[0], nukhtaPixels.size(), sizeof(cv::Point2i), compvar);

	//shape pixel is firstArray ,nukhta pixel is secondArray
	int numberToFindTo = 0;
	int inwhichArraySearch = 0;

	// find the largest minimum number from both arrays
	if(shapePixels[0].x>nukhtaPixels[0].x) {
		numberToFindTo = shapePixels[0].x;inwhichArraySearch = 1;}
	else {
		numberToFindTo = nukhtaPixels[0].x;inwhichArraySearch = 0;
	}
		
	std::vector<cv::Point2i> &firstArray = shapePixels;
	std::vector<cv::Point2i> &secondArray = nukhtaPixels;
	
	// swap array if we have to serach in second array i.e nukhtas array
	if(inwhichArraySearch == 1)
	{
		firstArray.swap(secondArray);
	}
	//build target point to search using binary serach (log(n))
	cv::Point2d target;
	target.x= numberToFindTo;
	
	// find occurence of target in first array
	auto it = upper_bound(firstArray.begin(), firstArray.end(), target, cmpID);
	int index = it- firstArray.begin() ;
	int counterrrr = 0;

	for(int j = index ; j< firstArray.size(); j++)
	{	
		if(firstArray[j].x > secondArray[secondArray.size()-1].x) 
			{
				break;
		}

		for(int k = 0; k < secondArray.size();k++)
		{	
			// we have a pixel at same co-ordinate of x then check their y position to determine location
			if(firstArray[j].x == secondArray[k].x)						
			{								
				if((abs((int) (firstArray[j].y -secondArray[k].y ) )) < mindis)
					mindis = (abs((int) ( firstArray[j].y - secondArray[k].y ) ));				
				lxPixelCoverage++;
				if(firstArray[j].y > secondArray[k].y)  inwhichArraySearch == 0? upper   = true : down = true; // nukta is above
				else if (firstArray[j].y < secondArray[k].y) inwhichArraySearch == 0?down = true : upper = true; // nukta is below				
			}
			else if(firstArray[j].x<secondArray[k].x)
			{
				break;
			}
		}			
	}

	if(upper && down) pos= "middle";
	else if(upper) pos= "upper";
	else if(down) pos = "lower";
	else pos = "";
	xPixelCoverage = lxPixelCoverage;	
	percent = mindis;
}


void BaseShapeAndNukhtaAssociater::getPercentAndPosition_old(const int nukhtaIndex, 
	const int shapeIndex , 
	string &pos , 
	double &percent , 
	int &xPixelCoverage , 
	const std::vector < std::vector<cv::Point2i> > &blobs ,
	const std::vector <pair<int,cv::Rect>> &rects)
{
	bool upper = false,down = false;			
	int lxPixelCoverage = 0;
	int mindis = 99999;

	// find tag of  nukhta and shape
	size_t nukhtaBlobI = rects.at(nukhtaIndex).first;	
	size_t shapeBlobI =  rects.at(shapeIndex).first;	

	int iter = blobs[shapeBlobI].size()*blobs[nukhtaBlobI].size();

	for(int j = 0 ; j< blobs[shapeBlobI].size(); j++)
	{		
		for(int k = 0; k < blobs[nukhtaBlobI].size();k++)
		{	
			// we have a pixel at same co-ordinate of x then check their y position to determine location
			if(blobs[shapeBlobI][j].x == blobs[nukhtaBlobI][k].x)						
			{								
				if((abs((int) ( blobs[shapeBlobI][j].y - blobs[nukhtaBlobI][k].y ) )) < mindis)
					mindis = (abs((int) ( blobs[shapeBlobI][j].y - blobs[nukhtaBlobI][k].y ) ));				
				lxPixelCoverage++;
				if(blobs[shapeBlobI][j].y > blobs[nukhtaBlobI][k].y) upper   = true; // nukta is above
				else if (blobs[shapeBlobI][j].y < blobs[nukhtaBlobI][k].y) down = true; // nukta is below				
			}
		}			
	}

	if(upper && down) pos= "middle";
	else if(upper) pos= "upper";
	else if(down) pos = "lower";
	else pos = "";

	xPixelCoverage = lxPixelCoverage;	
	percent = mindis;
}