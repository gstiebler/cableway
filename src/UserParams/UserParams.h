#ifndef USER_PARAMS_H__
#define USER_PARAMS_H__

#include <set>
#include <string>
#include "UDefines.h"

struct DrawingParams
{
	std::string fileName;
	double elevation;
};

struct UserParams
{
	std::set<std::string> equipmentLevels;
	std::set<std::string> bandeirolaLevels;
	std::set<std::string> tagLevels;
	std::set<std::string> cableLevels;
	
	std::vector<DrawingParams> drawingsParams;

	TTipoElemento getTipoElemento( std::string level );
};

#endif
