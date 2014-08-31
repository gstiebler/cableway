
#include "gtest/gtest.h"

#include "UParamsStructs.h"
#include "UGrafoDesenho.h"
#include "UNiveisProjeto.h"
#include "TDesenho.h"
#include "UContainerDesenhos.h"
#include "UListaCelulas.h"

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
	TNiveisProjetoTransfer niveisProjetoTransfer;
	niveisProjetoTransfer.ListaCabo.push_back( "5" );
	niveisProjetoTransfer.ListaInstrumento.push_back( "7" );
	niveisProjetoTransfer.ListaTag.push_back( "10" );
	
	CContainerDesenhos containerDesenhos( &niveisProjetoTransfer );
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
	TNiveisProjetoTransfer niveisProjetoTransfer;
	niveisProjetoTransfer.ListaCabo.push_back( "5" );
	niveisProjetoTransfer.ListaInstrumento.push_back( "7" );
	niveisProjetoTransfer.ListaTag.push_back( "10" );
	
	CContainerDesenhos containerDesenhos( &niveisProjetoTransfer );

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

}  // namespace
