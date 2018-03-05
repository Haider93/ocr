#pragma once
#include "Action.h"
#include "BlobFinderActionInput.h"
#include "BlobFinderActionOutput.h"
#include "IBlobFinder.h"
#include "IBaseShapeAndNukhtaFilterer.h"
class BlobFinderAction : public Action<BlobFinderActionInput,BlobFinderActionOutput>
{
public:
	BlobFinderActionOutput* execute( BlobFinderActionInput* in);
	
	// Implementation of actual Blob fnder need to be injected
	static IBlobFinder *m_blobFinder;
	~BlobFinderAction() {}
};
