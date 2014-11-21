#ifndef TESTSUTIL_H_
#define TESTSUTIL_H_

#include <string>

class TestsUtil
{
public:
    static std::string exeFileName;

	static void initExeFileName();

    static std::string getExePath();

    static std::string getDataPath();
};

#endif /* TESTSUTIL_H_ */
