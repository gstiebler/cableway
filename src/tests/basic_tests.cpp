
#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>
#include <string>
#include <vector>

#include "../DwgLoader.h"
#include "../UContainerDesenhos.h"
#include "../UDadosGenerico.h"
#include "../UDefines.h"
#include "../UInfoCelula.h"
#include "../UInfoCircuitos.h"
#include "../UItemCelula.h"
#include "../UListaCelulas.h"
#include "../UListaItensCelula.h"
#include "../UListaV.h"
#include "../UNiveisProjeto.h"
#include "../UserParams/LoadUserParams.h"
#include "../UserParams/UserParams.h"
#include "../UTCallbackStatusCarregamento.h"
#include "TestsUtil.h"

namespace {

class BasicTest : public ::testing::Test {
 protected:

  BasicTest() {}

  virtual ~BasicTest() {}
  virtual void SetUp() {}

  virtual void TearDown() {}

  void createFloor1( CDadosGenerico &dados );
  void createFloor2( CDadosGenerico &dados );
};

void BasicTest::createFloor1( CDadosGenerico &dados )
{
    dados.IndiceDesenho = 0;
    dados.IDArquivo = 0;
	// equipment 1
	{
		TMultipoint equip1;
		equip1.pontos.push_back( TPonto( 10.0, 10.0 ) );
		equip1.pontos.push_back( TPonto( 20.0, 10.0 ) );
		equip1.pontos.push_back( TPonto( 20.0, 20.0 ) );
		equip1.pontos.push_back( TPonto( 10.0, 20.0 ) );
		equip1.Nivel = INSTRUMENTO;
		dados.Multipoint.push_back( equip1 );

		TTexto equip1Text;
		equip1Text.texto = "Equipamento 1";
		equip1Text.Nivel = TAG;
		dados.Textos.push_back( equip1Text );
		
		TItemCelula itemCelula;
		itemCelula.Indice = dados.Multipoint.size() - 1;
		itemCelula.TipoVetorCW = VMULTIPOINT;
		TListaItensCelula lista;
		lista.Adiciona( itemCelula );
		lista.iTextos.push_back( dados.Textos.size() - 1 );
		dados.InfoCelula.ListaCelulasInstrumentos->Adiciona( lista );
	}

	// equipment 2
	{
		TMultipoint equip2;
		equip2.pontos.push_back( TPonto( 40.0, 20.0 ) );
		equip2.pontos.push_back( TPonto( 40.0, 10.0 ) );
		equip2.pontos.push_back( TPonto( 50.0, 10.0 ) );
		equip2.pontos.push_back( TPonto( 50.0, 20.0 ) );
		equip2.Nivel = INSTRUMENTO;
		dados.Multipoint.push_back( equip2 );

		TTexto equip2Text;
		equip2Text.texto = "Equipamento 2";
		equip2Text.Nivel = TAG;
		dados.Textos.push_back( equip2Text );

		TItemCelula itemCelula;
		itemCelula.Indice = dados.Multipoint.size() - 1;
		itemCelula.TipoVetorCW = VMULTIPOINT;
		TListaItensCelula lista;
		lista.Adiciona( itemCelula );
		lista.iTextos.push_back( dados.Textos.size() - 1 );
		dados.InfoCelula.ListaCelulasInstrumentos->Adiciona( lista );
	}

	// cable
	{
		TMultipoint cable;
		cable.Nivel = CABO;
		cable.pontos.push_back( TPonto( 20.0, 15.0 ) );
		cable.pontos.push_back( TPonto( 40.0, 15.0 ) );
		dados.Multipoint.push_back( cable );
	}

	
	// bandeirola
	{
		TMultipoint bandeirola;
		bandeirola.pontos.push_back( TPonto( 30.0, 5.0 ) );
		bandeirola.pontos.push_back( TPonto( 30.0, 14.0 ) );
		bandeirola.Nivel = BANDEIROLA;
		dados.Multipoint.push_back( bandeirola );

		TTexto bandeirolaText;
		bandeirolaText.texto = "Bandeirola1";
		bandeirolaText.Nivel = BANDEIROLA;
		dados.Textos.push_back( bandeirolaText );

		TItemCelula itemCelula;
		itemCelula.Indice = dados.Multipoint.size() - 1;
		itemCelula.TipoVetorCW = VMULTIPOINT;
		TListaItensCelula lista;
		lista.Adiciona( itemCelula );
		lista.iTextos.push_back( dados.Textos.size() - 1 );
		dados.InfoCelula.ListaCelulasBandeirolas->Adiciona( lista );
	}
}


void BasicTest::createFloor2( CDadosGenerico &dados )
{
    dados.IndiceDesenho = 1;
    dados.IDArquivo = 1;
    // equipment 3
    {
        TMultipoint equip3;
        equip3.pontos.push_back( TPonto( 40.0, 70.0 ) );
        equip3.pontos.push_back( TPonto( 60.0, 70.0 ) );
        equip3.pontos.push_back( TPonto( 60.0, 80.0 ) );
        equip3.pontos.push_back( TPonto( 40.0, 80.0 ) );
        equip3.Nivel = INSTRUMENTO;
        dados.Multipoint.push_back( equip3 );

        TTexto equip3Text;
        equip3Text.texto = "Equipamento 3";
        equip3Text.Nivel = TAG;
        dados.Textos.push_back( equip3Text );

        TItemCelula itemCelula;
        itemCelula.Indice = dados.Multipoint.size() - 1;
        itemCelula.TipoVetorCW = VMULTIPOINT;
        TListaItensCelula lista;
        lista.Adiciona( itemCelula );
        lista.iTextos.push_back( dados.Textos.size() - 1 );
        dados.InfoCelula.ListaCelulasInstrumentos->Adiciona( lista );
    }

    // equipment 2
    {
        TMultipoint equip2;
        equip2.pontos.push_back( TPonto( 40.0, 20.0 ) );
        equip2.pontos.push_back( TPonto( 40.0, 10.0 ) );
        equip2.pontos.push_back( TPonto( 50.0, 10.0 ) );
        equip2.pontos.push_back( TPonto( 50.0, 20.0 ) );
        equip2.Nivel = INSTRUMENTO;
        dados.Multipoint.push_back( equip2 );

        TTexto equip2Text;
        equip2Text.texto = "Equipamento 2";
        equip2Text.Nivel = TAG;
        dados.Textos.push_back( equip2Text );

        TItemCelula itemCelula;
        itemCelula.Indice = dados.Multipoint.size() - 1;
        itemCelula.TipoVetorCW = VMULTIPOINT;
        TListaItensCelula lista;
        lista.Adiciona( itemCelula );
        lista.iTextos.push_back( dados.Textos.size() - 1 );
        dados.InfoCelula.ListaCelulasInstrumentos->Adiciona( lista );
    }

    // cable
    {
        TMultipoint cable;
        cable.Nivel = CABO;
        cable.pontos.push_back( TPonto( 45.0, 20.0 ) );
        cable.pontos.push_back( TPonto( 45.0, 70.0 ) );
        dados.Multipoint.push_back( cable );
    }
}



TEST_F(BasicTest, singleDrawing) 
{
	CContainerDesenhos containerDesenhos;
	CDadosGenerico dados;

	createFloor1( dados );

	containerDesenhos.addDrawing( dados, 100.0 );
	callbackStatusCarregamento sc;
	containerDesenhos.Conclui( sc );

	double tam;
	string rota;
	TArestasCircuito *ArestasCircuito = NULL;
	vector<int> ListaBandeirolas;
	vector<string> DEBUG_arestas;
	string SubRotas; 
	TCircuitoAreas *CircuitoAreas = NULL;

	containerDesenhos.InfoCircuitos->GeraRota("Equipamento 2", "Equipamento 1", tam, rota, ArestasCircuito, &ListaBandeirolas,
		&DEBUG_arestas, SubRotas, CircuitoAreas);

	EXPECT_FLOAT_EQ( 20.0, tam );
	EXPECT_STREQ( "Equipamento 1/Bandeirola1/Equipamento 2", rota.c_str() );
}


TEST_F(BasicTest, multipleDrawings) 
{
	CContainerDesenhos containerDesenhos;

	CDadosGenerico floor1Data;
	createFloor1( floor1Data );
	containerDesenhos.addDrawing( floor1Data, 100.0 );

    CDadosGenerico floor2Data;
    createFloor2( floor2Data );
    containerDesenhos.addDrawing( floor2Data, 126.0 );

	callbackStatusCarregamento sc;
	containerDesenhos.Conclui( sc );

	double tam;
	string rota;
	TArestasCircuito *ArestasCircuito = NULL;
	vector<int> ListaBandeirolas;
	vector<string> DEBUG_arestas;
	string SubRotas; 
	TCircuitoAreas *CircuitoAreas = NULL;

	containerDesenhos.InfoCircuitos->GeraRota("Equipamento 3", "Equipamento 1", tam, rota, ArestasCircuito, &ListaBandeirolas,
		&DEBUG_arestas, SubRotas, CircuitoAreas);

	// 70 = 20 + 50 + (126.0 - 100.0)
	EXPECT_FLOAT_EQ( 96.0, tam );
	EXPECT_STREQ( "Equipamento 1/Bandeirola1/Equipamento 2/Equipamento 3", rota.c_str() );
}



TEST_F(BasicTest, complete)
{
    UserParams userParams;
    string xlsFileName = TestsUtil::getExePath() + "/../data/tests/user_params.xls";
    loadUserParams( xlsFileName, &userParams );

    CDadosGenerico dados;
    string fileName = "../data/tests/drawing2.dwg";
    DwgLoader *loader = new DwgLoader( fileName, &dados, &userParams );

    CContainerDesenhos containerDesenhos;

    containerDesenhos.addDrawing( dados, 100.0 );
    callbackStatusCarregamento sc;
    containerDesenhos.Conclui( sc );

    double tam;
    string rota;
    TArestasCircuito *ArestasCircuito = NULL;
    vector<int> ListaBandeirolas;
    vector<string> DEBUG_arestas;
    string SubRotas;
    TCircuitoAreas *CircuitoAreas = NULL;
    containerDesenhos.InfoCircuitos->GeraRota("Equipamento 1", "Equipamento 2", tam, rota, ArestasCircuito, &ListaBandeirolas,
        &DEBUG_arestas, SubRotas, CircuitoAreas);

    EXPECT_FLOAT_EQ( 500.0, tam );
    EXPECT_STREQ( "Equipamento 2/Equipamento 1", rota.c_str() );
}

}  // namespace
