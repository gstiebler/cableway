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
	std::vector<int> equipmentLevels;
	std::vector<int> bandeirolaLevels;
	std::vector<int> tagLevels;
	std::vector<int> cableLevels;
	
	std::vector<DrawingParams> drawingsParams;
};

#endif
