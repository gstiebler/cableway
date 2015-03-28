
#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "file/CweLoader.h"
#include "UDadosGenerico.h"
#include "UDefines.h"
#include "UInfoCelula.h"
#include "UListaItensCelula.h"
#include "UserParams/UserParams.h"
#include "TestsUtil.h"

using namespace std;

class DrawingLoaderTests : public ::testing::Test
{
};



TEST_F(DrawingLoaderTests, basic)
{
    UserParams userParams;
    userParams.equipmentLevels.insert( "25" );
    userParams.tagLevels.insert( "15" );
    userParams.cableLevels.insert( "51" );

    std::shared_ptr<CDadosGenerico> dados( new CDadosGenerico() );
    string fileName = TestsUtil::getDataPath() + "/tests/drawing2.cwe";
    CweLoader *loader = new CweLoader( fileName, dados, &userParams );

    ASSERT_EQ( 0, (int) dados->Arcos.size() );
    ASSERT_EQ( 3, (int) dados->Multipoint.size() );
    ASSERT_EQ( 2, (int) dados->Textos.size() );

    EXPECT_EQ( INSTRUMENTO, dados->Textos[0]->Nivel );
    EXPECT_EQ( INSTRUMENTO, dados->Textos[1]->Nivel );
    EXPECT_EQ( INSTRUMENTO, dados->Multipoint[0]->Nivel );
    EXPECT_EQ( INSTRUMENTO, dados->Multipoint[1]->Nivel );
    EXPECT_EQ( CABO, dados->Multipoint[2]->Nivel );

    ASSERT_EQ( 4, (int) dados->Multipoint[0]->pontos.size() );
    EXPECT_FLOAT_EQ( 2644.9602, dados->Multipoint[0]->pontos[0].x );
    EXPECT_FLOAT_EQ( 3144.9602, dados->Multipoint[0]->pontos[1].x );
    EXPECT_FLOAT_EQ( 3144.9602, dados->Multipoint[0]->pontos[2].x );
    EXPECT_FLOAT_EQ( 2644.9602, dados->Multipoint[0]->pontos[3].x );
    EXPECT_FLOAT_EQ( 1911.5696, dados->Multipoint[0]->pontos[0].y );
    EXPECT_FLOAT_EQ( 1911.5696, dados->Multipoint[0]->pontos[1].y );
    EXPECT_FLOAT_EQ( 1681.5696, dados->Multipoint[0]->pontos[2].y );
    EXPECT_FLOAT_EQ( 1681.5696, dados->Multipoint[0]->pontos[3].y );

    ASSERT_EQ( 4, (int) dados->Multipoint[1]->pontos.size() );

    EXPECT_STREQ( "Equipamento 1", dados->Textos[0]->texto.c_str() );
    EXPECT_FLOAT_EQ( 2778.2468, dados->Textos[0]->origem.x );
    EXPECT_FLOAT_EQ( 1815.6892, dados->Textos[0]->origem.y );

    EXPECT_STREQ( "Equipamento 2", dados->Textos[1]->texto.c_str() );

	ASSERT_EQ( 2, dados->InfoCelula.ListaCelulasInstrumentos.size() );
}
