#include "gtest/gtest.h"
#include "TestsUtil.h"

int main (int argc, char *argv[])
{
    TestsUtil::exeFileName = argv[0];
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
