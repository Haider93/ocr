#pragma once
#include "BaseShape.h"
#include "NukhtaShape.h"
#include <vector>
using namespace std;

// posible postion of a nukhta in a Urdu ligature shape
enum NukhtaPosition {LOWER,MIDDLE,UPPER};

/**
 * This class combine information of a Nukhta and its postion across an Urdu Shape
 */
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

/**
 * This class represents a complete URDU shape.
 * An urdu shape consist one base shape and more than one possible nukhtas
 * Every UrduShape will have a base shape description which will be used in
 * OCR matching process.It will also have nukhtas with their alignement across shape
 * [LOWER,MIDDLE,UPPER]
 */
class UrduShape : public BaseShape {
private:
	vector<unique_ptr<NukhtaShapeAndPosition>> nukhtasForShape; // represent nukhtas belongs to this base shape
	
	// Disable copying and assignment
	UrduShape(const UrduShape &other) {}
	UrduShape& operator=(UrduShape other){}

public:
	
	//Mutator methods
	int getNumberOfNukhtas() {
		return nukhtasForShape.size();
	}

	vector<unique_ptr<NukhtaShapeAndPosition>>& getNukhtasForShape() {
		return nukhtasForShape;
	}
};