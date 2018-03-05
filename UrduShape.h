#pragma once
#include "BaseShape.h"
#include "NukhtaShape.h"
#include <vector>
using namespace std;

enum NukhtaPosition {LOWER,MIDDLE,UPPER};

class NukhtaShapeAndPosition
{
private:
	NukhtaShape nukhtaShape;
	NukhtaPosition nukhtaPosition;

	// Disable copying and assignment
	NukhtaShapeAndPosition(const NukhtaShapeAndPosition &other) {}
	NukhtaShapeAndPosition& operator=(NukhtaShapeAndPosition other){}

public:
	NukhtaShape& getNukhtaShape(){
		return nukhtaShape;
	}
};

class UrduShapeDiscripton {
private:
	wstring unicodeValue;
	string shapeDiscription;
	int fontNo;
	int seatNo;

public:
	int getFontNo(){return fontNo;}
	int getSeatNo(){return seatNo;}
	wstring getUnicodeText(){return unicodeValue;}
	string getShapeDescription(){return shapeDiscription;}
};

class UrduShape {

private:
	BaseShape baseShape;
	vector<unique_ptr<NukhtaShapeAndPosition>> nukhtasForShape;
	UrduShapeDiscripton urduShapeDiscripton;

	// Disable copying and assignment
	UrduShape(const UrduShape &other) {}
	UrduShape& operator=(UrduShape other){}

public:
	int getNumberOfNukhtas() {
		return nukhtasForShape.size();
	}

	vector<unique_ptr<NukhtaShapeAndPosition>>& getNukhtasForShape() {
		return nukhtasForShape;
	}

	BaseShape& getBaseShape() {
		 return (baseShape);
	}

	UrduShapeDiscripton& getUrduShapeDescription() {
		return urduShapeDiscripton;
	}
};