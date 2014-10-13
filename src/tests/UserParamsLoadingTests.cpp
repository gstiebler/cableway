
#include "gtest/gtest.h"

#include <UserParams/UserParams.h>
#include <UserParams/LoadUserParams.h>
#include "TestsUtil.h"

using namespace std;

class UserParamsLoadingTests : public ::testing::Test
{
};



TEST_F(UserParamsLoadingTests, basic)
{
    UserParams userParams;
    string fileName = TestsUtil::getExePath() + "/../data/tests/user_params.xls";
    loadUserParams( fileName, &userParams );

    ASSERT_EQ( 2, (int) userParams.drawingsParams.size() );

    EXPECT_FLOAT_EQ( 100.0, userParams.drawingsParams[0].elevation );
    EXPECT_STREQ( "drawing1.dxf", userParams.drawingsParams[0].fileName.c_str() );

    EXPECT_FLOAT_EQ( 135.0, userParams.drawingsParams[1].elevation );
    EXPECT_STREQ( "drawing2.dxf", userParams.drawingsParams[1].fileName.c_str() );

    ASSERT_EQ( 1, (int) userParams.bandeirolaLevels.size());
    ASSERT_EQ( 1, (int) userParams.tagLevels.size());
    ASSERT_EQ( 3, (int) userParams.equipmentLevels.size());
    ASSERT_EQ( 2, (int) userParams.cableLevels.size());

    EXPECT_STREQ( "10", userParams.bandeirolaLevels[0].c_str() );
    EXPECT_STREQ( "15", userParams.tagLevels[0].c_str() );
    EXPECT_STREQ( "25", userParams.equipmentLevels[0].c_str() );
    EXPECT_STREQ( "26", userParams.equipmentLevels[1].c_str() );
    EXPECT_STREQ( "equip", userParams.equipmentLevels[2].c_str() );
    EXPECT_STREQ( "51", userParams.cableLevels[0].c_str() );
    EXPECT_STREQ( "52", userParams.cableLevels[1].c_str() );
}

