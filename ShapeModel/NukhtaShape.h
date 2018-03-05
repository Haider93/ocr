#pragma once
#include "AbstractShape.h"
#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/assert.hpp> 
#include <map>

using namespace boost::assign;
/**
* This class represnets a Nukhta.A nukhta has collection of points and an identifier
* associated with it
*/
class NukhtaShape : public AbstractShape
{
public:
	static enum Nukhtas { 
		B_NUKHTA,
		S_NUKHTA,
		D_NUKHTA,
		LAAM_NUKHTA,
		ALIF_NUKHTA,
		FIVE_NUKHTA,
		M_NUKHTA,
		NINE_NUKHTA,
		SIX_NUKHTA,
		R_NUKHTA,
		SLASH_NUKHTA,
		LAAM_WNUKHTA_CONNECTED,
		DNUKHTA_SNUKHTA_CONNECTED,
		T_NUKHTA,
		W_NUKHTA,
		LINE_NUKHTA,
		SLASH_DOUBLE,
		NINEB_CONNECTED,
		NINE_DNUKTA_CONNECTED,
		SLASH_WNUKTA_CONNECTED,
		SLASH_DNUKTA_CONNECTED,
		UNDEFINED
	};
private:
	Nukhtas m_whichNukta;

public:
	static string NukhtaShape::getTextualDescription(NukhtaShape::Nukhtas whichnukta,string position);
	static map<string, NukhtaShape::Nukhtas> m_nukhtaToStringMap;
	static const char *m_stringToNukhta[];
	//ctor
	NukhtaShape(std::vector<cv::Point> points,
		Nukhtas whichNukta):
	AbstractShape(points),
		m_whichNukta(whichNukta) {
	}

	//dtor
	virtual ~NukhtaShape(){
	}

	//getter setter
	Nukhtas getWhichNukta() {
		return m_whichNukta;
	}
	void setWhichNukta(int nukhta) {
		this->m_whichNukta= (Nukhtas)nukhta;
	}
};