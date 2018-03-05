#pragma once
#include "BlobFinderActionInput.h"
#include "BlobFinderActionOutput.h"
class IBlobFinder
{
public: 
	virtual BlobFinderActionOutput* FindBlobs(BlobFinderActionInput* inp)= 0;
};