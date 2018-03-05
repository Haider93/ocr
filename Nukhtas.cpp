#include "ShapeModel\NukhtaShape.h"

map<std::string, NukhtaShape::Nukhtas> NukhtaShape::m_nukhtaToStringMap = map_list_of
	("B_NUKHTA", NukhtaShape::B_NUKHTA)
	("S_NUKHTA", NukhtaShape::S_NUKHTA)
	("D_NUKHTA" ,NukhtaShape::D_NUKHTA)
	("LAAM_NUKHTA", NukhtaShape::LAAM_NUKHTA)
	("ALIF_NUKHTA", NukhtaShape::ALIF_NUKHTA)
	("FIVE_NUKHTA" ,NukhtaShape::FIVE_NUKHTA)
	("NINE_NUKHTA", NukhtaShape::NINE_NUKHTA)
	("SIX_NUKHTA", NukhtaShape::SIX_NUKHTA)
	("R_NUKHTA" ,NukhtaShape::R_NUKHTA)
	("LAAM_WNUKHTA_CONNECTED", NukhtaShape::LAAM_WNUKHTA_CONNECTED)
	("DNUKHTA_SNUKHTA_CONNECTED", NukhtaShape::DNUKHTA_SNUKHTA_CONNECTED)
	("T_NUKHTA" ,NukhtaShape::T_NUKHTA)
	("M_NUKHTA" ,NukhtaShape::M_NUKHTA)
	("SLASH_NUKHTA" ,NukhtaShape::SLASH_NUKHTA)
	("LINE_NUKHTA" ,NukhtaShape::LINE_NUKHTA)
	("W_NUKHTA" ,NukhtaShape::W_NUKHTA)
	("SLASH_DOUBLE" ,NukhtaShape::SLASH_DOUBLE)
	("NINEB_CONNECTED" ,NukhtaShape::NINEB_CONNECTED)
	("NINE_DNUKTA_CONNECTED" ,NukhtaShape::NINE_DNUKTA_CONNECTED)
	("SLASH_WNUKTA_CONNECTED" ,NukhtaShape::SLASH_WNUKTA_CONNECTED)
	("SLASH_DNUKTA_CONNECTED" ,NukhtaShape::SLASH_DNUKTA_CONNECTED);
	
const char * NukhtaShape ::m_stringToNukhta[] = { "B_NUKHTA",
		"S_NUKHTA",
		"D_NUKHTA",
		"LAAM_NUKHTA",
		"ALIF_NUKHTA",
		"FIVE_NUKHTA",
		"M_NUKHTA",
		"NINE_NUKHTA",
		"SIX_NUKHTA",
		"R_NUKHTA",
		"SLASH_NUKHTA",
		"LAAM_WNUKHTA_CONNECTED",
		"DNUKHTA_SNUKHTA_CONNECTED",
		"T_NUKHTA",
		"W_NUKHTA",
		"LINE_NUKHTA",
		"SLASH_DOUBLE",
		"NINEB_CONNECTED",
		"NINE_DNUKTA_CONNECTED",
		"SLASH_WNUKTA_CONNECTED",
		"SLASH_DNUKTA_CONNECTED",
		"UNDEFINED"
};

 string NukhtaShape::getTextualDescription(NukhtaShape::Nukhtas whichnukta,string position)
{
	string textualDesc = "";
	
	/*this is a patch for special nukta we found*/
	//<upper_SLASH_NUKTA><upper_SLASH_NUKTA>
	//<lower_SLASH_NUKTA><lower_SLASH_NUKTA>
	//<middle_SLASH_NUKTA><lower_middle_NUKTA>
	if(whichnukta == NukhtaShape::SLASH_DOUBLE)
	{
		// special case if its double laam nukhta with connected shape
		if(position == "upper")    textualDesc += "<upper_SLASH_NUKTA><upper_SLASH_NUKTA>";				
		else if(position == "lower") textualDesc += "<lower_SLASH_NUKTA><lower_SLASH_NUKTA>";				
		else textualDesc += "<middle_SLASH_NUKTA><middle_SLASH_NUKTA>";				
	}
	else if(whichnukta == NukhtaShape::NINEB_CONNECTED)
	{
		if(position == "upper")    textualDesc += "<upper_NINE_NUKTA><upper_B_NUKTA>";				
		else if(position == "lower") textualDesc += "<lower_NINE_NUKTA><lower_B_NUKTA>";
		else textualDesc += "<middle_NINE_NUKTA><middle_B_NUKTA>";				
	}

	else if(whichnukta ==  NukhtaShape::NINE_DNUKTA_CONNECTED)
	{
		if(position == "upper")    textualDesc += "<upper_D_NUKTA><upper_NINE_NUKTA>";				
		else if(position == "lower") textualDesc += "<lower_D_NUKTA><lower_NINE_NUKTA>";
		else textualDesc += "<middle_D_NUKTA><middle_NINE_NUKTA>";				
	}

	else if(whichnukta ==  NukhtaShape::SLASH_WNUKTA_CONNECTED)
	{
		if(position == "upper")    textualDesc += "<upper_SLASH_NUKTA><upper_W_NUKTA>";				
		else if(position == "lower") textualDesc += "<lower_SLASH_NUKTA><lower_W_NUKTA>";
		else textualDesc += "<middle_SLASH_NUKTA><middle_W_NUKTA>";				
	}

	else if(whichnukta ==  NukhtaShape::SLASH_DNUKTA_CONNECTED)
	{
		if(position == "upper")    textualDesc += "<upper_SLASH_NUKTA><upper_D_NUKTA>";				
		else if(position == "lower") textualDesc += "<lower_SLASH_NUKTA><lower_D_NUKTA>";
		else textualDesc += "<middle_SLASH_NUKTA><middle_D_NUKTA>";					
	}
	else
	{
		textualDesc += "<"+position+"_"+m_stringToNukhta[whichnukta]+">";
	}
	return textualDesc;
}