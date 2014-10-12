
#include "gtest/gtest.h"

#include "TestsUtil.h"
#include <DwgLoader.h>
#include <UDadosGenerico.h>

using namespace std;

class DrawingLoaderTests : public ::testing::Test
{
};



TEST_F(DrawingLoaderTests, basic)
{
    CDadosGenerico dados;
    string fileName = "../data/tests/drawing2.dwg";//TestsUtil::getExePath() + "/../data/tests/drawing2.dwg";
    DwgLoader *loader = new DwgLoader( fileName, &dados );

    ASSERT_EQ( 0, (int) dados.Arcos.size() );
    ASSERT_EQ( 3, (int) dados.Multipoint.size() );
    ASSERT_EQ( 2, (int) dados.Textos.size() );
}
