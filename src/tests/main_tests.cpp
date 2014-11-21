#include "gtest/gtest.h"
#include "TestsUtil.h"

int main (int argc, char *argv[])
{
	TestsUtil::initExeFileName();

    //TestsUtil::exeFileName = std::string( argv[0] );
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
