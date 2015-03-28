
#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>
#include "TestsUtil.h"
#include "MainExecution.h"


using namespace std;

class File2FileTests : public ::testing::Test
{
};



TEST_F(File2FileTests, test1)
{
    string xlsFileName = TestsUtil::getDataPath() + "\\tests\complete_test1\\user_params.xls";
    string inputCircuitsFileName = TestsUtil::getDataPath() + "\\tests\complete_test1\\input_circuits.xls";

    MainExecution mainExecution( xlsFileName );
    mainExecution.execute( inputCircuitsFileName );

	for( int i(0); i < mainExecution._resultCircuits.size(); ++i )
	{
	}


    ASSERT_EQ( 3, (int) mainExecution._resultCircuits.size() );

    EXPECT_STREQ( "Equipamento 1/Equipamento 2", InputCircuit::getFormatedRoute( mainExecution._resultCircuits[0].route ).c_str() );
    EXPECT_FLOAT_EQ( 500.0, mainExecution._resultCircuits[0].length );
	EXPECT_STREQ( "", mainExecution._resultCircuits[0].errorMessage.c_str() );

    EXPECT_STREQ( "Equipamento 2/Equipamento 1", InputCircuit::getFormatedRoute( mainExecution._resultCircuits[1].route ).c_str() );
    EXPECT_FLOAT_EQ( 500.0, mainExecution._resultCircuits[1].length );
	EXPECT_STREQ( "", mainExecution._resultCircuits[1].errorMessage.c_str() );

	EXPECT_STREQ( "O texto de origem não existe nos desenhos; O texto de destino não existe nos desenhos.", mainExecution._resultCircuits[2].errorMessage.c_str() );
}