#pragma once
#include "AbstractShape.h"
#include "../TreeKD.h"
/**
* This is internal data which is specific to Inpage
* This hold values which ic used internally in OCR matching process
*/

class BaseShapeDescription {
private:
	int m_fontNo;
	int m_seatNo;
	wstring m_unicodeValue;
	string m_shapeDescription;

public:

	//ctor
	BaseShapeDescription(
		int fontNo,
		int seatNo,
		wstring unicodeValue,
		string shapeDescription):
	    m_fontNo(fontNo),	
		m_seatNo(seatNo),
		m_unicodeValue(unicodeValue),
		m_shapeDescription(shapeDescription){
	}

	BaseShapeDescription(){}

	//dtor
	~BaseShapeDescription(){}

	//getter setter
	int getFontNo(){return m_fontNo;}
	void setFontNo(int f){this->m_fontNo=f;}
	int getSeatNo(){return m_seatNo;}
	void setSeatNo(int s){this->m_seatNo=s;}
	wstring getUnicodeText(){return m_unicodeValue;}
	void setUnicodeText(wstring uv){this->m_unicodeValue=uv;}
	string getShapeDescription(){return m_shapeDescription;}
	void setShapeDescription(string sd){this->m_shapeDescription=sd;}
};

/**
* This class represents a Urdu Base Shape.Base shape is one which does not have any nukhtas associated 
* with it.This Shape is used as a traning data in OCR to match baseShapes regadrless of nukhats in any Urdu ligature Shape.
*/
class BaseShape
{
private:
	std::vector<cv::Point> &m_points;
	BaseShapeDescription m_baseShapeDescription;
public:
	//ctor
	BaseShape(std::vector<cv::Point> &points,
		BaseShapeDescription baseShapeDescription)
		:m_points(points),
		m_baseShapeDescription(baseShapeDescription) {
	}
    
	//getter setter
	vector<cv::Point>& getPoints(){
		return m_points;
	}

	//dtor
	virtual ~BaseShape()
	{}

	//getter setter
	BaseShapeDescription& getBaseShapeDescription() {
		return m_baseShapeDescription;
	}
	void setBaseShapeDescription(BaseShapeDescription baseShapeDesc){
		this->m_baseShapeDescription=baseShapeDesc;
	}
};