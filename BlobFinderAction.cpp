#include "BlobFinderAction.h"
#include "BlobFinderActionInput.h"
#include "BlobFinderActionOutput.h"
#include "FloodFillBlobFinder.h"

IBlobFinder* BlobFinderAction::m_blobFinder = new FloodFillBlobFinder;
BlobFinderActionOutput* BlobFinderAction::execute(BlobFinderActionInput* inp)
{
	this->startClock();
	BlobFinderActionOutput *output = m_blobFinder->FindBlobs(inp);
	this->endClock();
	return output;
}