
#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>
#include "TestsUtil.h"
#include "MainExecution.h"
#include "file/CsvReader.h"


using namespace std;

class File2FileTests : public ::testing::Test
{
};



TEST_F(File2FileTests, test1)
{
    string xlsFileName = TestsUtil::getDataPath() + "\\tests\\complete_test1\\user_params.xls";
    string inputCircuitsFileName = TestsUtil::getDataPath() + "\\tests\\complete_test1\\input_circuits.xls";
	string csvFileName = TestsUtil::getDataPath() + "\\tests\\complete_test1\\CircuitsReport_fixtures.csv";

    MainExecution mainExecution( xlsFileName );
    mainExecution.execute( inputCircuitsFileName );

	vector< vector< string > > circuitsFixtures;
	CsvReader::readCsv( csvFileName, circuitsFixtures );

	// removes the header
	circuitsFixtures.erase( circuitsFixtures.begin() );

	ASSERT_EQ( 12, circuitsFixtures.size() );
	ASSERT_EQ( circuitsFixtures.size(), mainExecution._resultCircuits.size() );

	const int LENGTH_COLUMN_INDEX = 5;
	const int ROUTE_COLUMN_INDEX = 6;
	const int ERROR_COLUMN_INDEX = 7;
	for( int i(0); i < mainExecution._resultCircuits.size(); ++i )
	{
		if( circuitsFixtures[i][LENGTH_COLUMN_INDEX] == "" )
		{
			EXPECT_EQ( 0.0, mainExecution._resultCircuits[i].length );
		}
		else
		{
			double fixtureLength = stod( circuitsFixtures[i][LENGTH_COLUMN_INDEX] );
			EXPECT_FLOAT_EQ( mainExecution._resultCircuits[i].length, fixtureLength );
		}

		EXPECT_STREQ( circuitsFixtures[i][ROUTE_COLUMN_INDEX].c_str(), InputCircuit::getFormatedRoute( mainExecution._resultCircuits[i].route ).c_str() );
		EXPECT_STREQ( circuitsFixtures[i][ERROR_COLUMN_INDEX].c_str(), mainExecution._resultCircuits[i].errorMessage.c_str() );
	}
}