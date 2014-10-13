
#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>
#include <string>
#include <vector>

#include "../DwgLoader.h"
#include "../UDadosGenerico.h"
#include "../UInfoCelula.h"
#include "../UListaCelulas.h"
#include "../UListaV.h"

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

    ASSERT_EQ( 4, (int) dados.Multipoint[0].pontos.size() );
    EXPECT_FLOAT_EQ( 2250.0, dados.Multipoint[0].pontos[0].x );
    EXPECT_FLOAT_EQ( 2750.0, dados.Multipoint[0].pontos[1].x );
    EXPECT_FLOAT_EQ( 2750.0, dados.Multipoint[0].pontos[2].x );
    EXPECT_FLOAT_EQ( 2250.0, dados.Multipoint[0].pontos[3].x );
    EXPECT_FLOAT_EQ( 1740.0, dados.Multipoint[0].pontos[0].y );
    EXPECT_FLOAT_EQ( 1740.0, dados.Multipoint[0].pontos[1].y );
    EXPECT_FLOAT_EQ( 1510.0, dados.Multipoint[0].pontos[2].y );
    EXPECT_FLOAT_EQ( 1510.0, dados.Multipoint[0].pontos[3].y );

    ASSERT_EQ( 4, (int) dados.Multipoint[1].pontos.size() );

    EXPECT_STREQ( "Equipamento 1", dados.Textos[0].texto.c_str() );
    EXPECT_FLOAT_EQ( 2297.237381, dados.Textos[0].origem.x );
    EXPECT_FLOAT_EQ( 1644.897681, dados.Textos[0].origem.y );

    EXPECT_STREQ( "Equipamento 2", dados.Textos[1].texto.c_str() );

    ASSERT_EQ( 2, dados.InfoCelula.ListaCelulasInstrumentos->Tamanho() );

    TListaItensCelula *firstCell = dados.InfoCelula.ListaCelulasInstrumentos->getItem(0);
    ASSERT_EQ( 1, firstCell->Tamanho() );
    ASSERT_EQ( 0, firstCell->getItem(0)->Indice );
    ASSERT_EQ( VMULTIPOINT, firstCell->getItem(0)->TipoVetorCW );

    TListaItensCelula *secondCell = dados.InfoCelula.ListaCelulasInstrumentos->getItem(1);
    ASSERT_EQ( 1, (int) secondCell->iTextos.size() );
    ASSERT_EQ( 1, (int) secondCell->iTextos[0] );

    ASSERT_EQ( 1, secondCell->Tamanho() );
    ASSERT_EQ( 1, (int) secondCell->iTextos.size() );
    ASSERT_EQ( 1, (int) secondCell->iTextos[0] );
}
