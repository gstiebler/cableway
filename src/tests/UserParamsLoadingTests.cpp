
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
    string fileName = TestsUtil::getDataPath() + "/tests/user_params.xls";
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

    EXPECT_NE( userParams.bandeirolaLevels.end(), userParams.bandeirolaLevels.find( "10" ) );
    EXPECT_NE( userParams.tagLevels.end(), userParams.tagLevels.find( "15" ) );
    EXPECT_NE( userParams.equipmentLevels.end(), userParams.equipmentLevels.find( "25" ) );
    EXPECT_NE( userParams.equipmentLevels.end(), userParams.equipmentLevels.find( "26" ) );
    EXPECT_NE( userParams.equipmentLevels.end(), userParams.equipmentLevels.find( "equip" ) );
    EXPECT_NE( userParams.cableLevels.end(), userParams.cableLevels.find( "51" ) );
    EXPECT_NE( userParams.cableLevels.end(), userParams.cableLevels.find( "52" ) );
}

