#ifndef USER_PARAMS_H__
#define USER_PARAMS_H__

#include <vector>
#include <string>

struct DrawingParams
{
	std::string fileName;
	double elevation;
};

struct UserParams
{
	std::vector<std::string> equipmentLevels;
	std::vector<std::string> bandeirolaLevels;
	std::vector<std::string> tagLevels;
	std::vector<std::string> cableLevels;
	
	std::vector<DrawingParams> drawingsParams;
};

#endif
