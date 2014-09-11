
#include "gtest/gtest.h"

#include "TestsUtil.h"
#include <DrawingLoader.h>
#include <UDadosGenerico.h>

using namespace std;

class DrawingLoaderTests : public ::testing::Test
{
};



TEST_F(DrawingLoaderTests, basic)
{
    CDadosGenerico dados;
    string fileName = TestsUtil::getExePath() + "/../data/tests/example.dxf";
    DrawingLoader::loadDXF( fileName, &dados );
}
