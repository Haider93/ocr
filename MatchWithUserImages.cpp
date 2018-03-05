#include "MatchWithUserImages.h"

MatchWithUserImagesActionOutput* MatchWithUserImages::matchUserImages(MatchWithUserImagesActionInput* inp)
{
	MatchWithUserImagesActionOutput* out=new MatchWithUserImagesActionOutput;
	matchUserSavedImages(
		inp->m_baseShapeAndNukhtaFilterOutput->baseShapes,
		inp->m_baseShapeAndNukhtaFilterOutput->shapeFilesNames,
		inp->m_baseShapeAndNukhtaFilterOutput->baseShapesWithNuktasList,
		inp->m_baseShapeAndNukhtaFilterOutput->nukhtaDescription,
		inp->m_blobFinderActionOutput->m_rects,
		inp->m_blobFinderActionOutput->m_blobs,
		inp->m_baseShapeAndNukhtaFilterOutput->path
		)
	return out;
}

void matchUserSavedImages(vector<int> &nukhtaShapes,
	vector<int> &baseShapes,
	map<int,string> &shapeFilesNames,
	map <int, vector< pair<int,string> >> &baseShapesWithNuktasList,
	const map<int,NukhtaShape::Nukhtas> &nukhtaDescs,
	std::vector<pair<int,cv::Rect>> &rects,
	std::vector <std::vector<cv::Point2i>> &blobs,
	string &path)
{

}