#include <gtest/gtest.h>

#include "file/InputCircuitsReader.h"
#include "TestsUtil.h"

using namespace std;

class InputCircuitsReaderTests : public ::testing::Test
{
};



TEST_F(InputCircuitsReaderTests, basic)
{
    vector<InputCircuit> inputCircuits;
    string fileName = TestsUtil::getDataPath() + "/tests/input_circuits.xls";
    readInputCircuits( fileName, inputCircuits );

    ASSERT_EQ( 2, (int)inputCircuits.size() );
    EXPECT_STREQ( "primeiro", inputCircuits[0].name.c_str() );
    EXPECT_STREQ( "LP1", inputCircuits[0].source.c_str() );
    EXPECT_STREQ( "LP2", inputCircuits[0].dest.c_str() );
    EXPECT_STREQ( "c1", inputCircuits[0].cable.c_str() );
    ASSERT_EQ( 0, (int)inputCircuits[0].route.size() );

    EXPECT_STREQ( "segundo", inputCircuits[1].name.c_str() );
    EXPECT_STREQ( "Equipamento 1", inputCircuits[1].source.c_str() );
    EXPECT_STREQ( "Equipamento 2", inputCircuits[1].dest.c_str() );
    EXPECT_STREQ( "Cabo 2 x 1.5", inputCircuits[1].cable.c_str() );
    ASSERT_EQ( 2, (int)inputCircuits[1].route.size() );
    EXPECT_STREQ( "Band 1", inputCircuits[1].route[0].c_str() );
    EXPECT_STREQ( "Band 2", inputCircuits[1].route[1].c_str() );
}
