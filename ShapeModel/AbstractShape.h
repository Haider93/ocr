#pragma once
#include "opencv2/core/core.hpp"
#include <vector>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

/**
* This class represent basic shape object Shape is a collection of {x,y} pairs
*/
class AbstractShape
{
private:
	std::vector<cv::Point> m_points;
	int m_width;
	int m_height;

	virtual void toString() {
	}

public:
	//ctor
	AbstractShape(std::vector<cv::Point> &points,
		int height= 0,
		int width= 0):
	m_points(points),
		m_width(width),
		m_height(height) {
	}

	AbstractShape(){}
	//dtor
	virtual ~AbstractShape() {
		m_points.clear();
	}

	virtual void dump() {
		cout<<"I am dumping myself";
	}

	//getter setter
	vector<cv::Point>& getPoints(){
		return m_points;
	}

	void setHeight(int height){
		this->m_height = height;
	}

	void setWidth(int width){
		this->m_width = width;
	}

	int getHeight() {return m_height;}
	int getWidth() {return m_width;}
};