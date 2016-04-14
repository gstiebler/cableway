
#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>
#include <string>
#include <vector>

#include "file/CweLoader.h"
#include "../UContainerDesenhos.h"
#include "../UDadosGenerico.h"
#include "../UDefines.h"
#include "../UInfoCelula.h"
#include "../UInfoCircuitos.h"
#include "../UListaItensCelula.h"
#include "../UserParams/LoadUserParams.h"
#include "../UserParams/UserParams.h"
#include "MainExecution.h"
#include "TestsUtil.h"
#include "InterfaceFeedback.h"

using namespace std;

namespace {

class BasicTest : public ::testing::Test {
 protected:

  BasicTest() {}

  virtual ~BasicTest() {}
  virtual void SetUp() {}

  virtual void TearDown() {}

  void createFloor1( std::shared_ptr<CDadosGenerico> dados );
  void createFloor2( std::shared_ptr<CDadosGenerico> dados );
};

void BasicTest::createFloor1( std::shared_ptr<CDadosGenerico> dados )
{
	// equipment 1
	{
		shared_ptr<TMultipoint> equip1( new TMultipoint() );
		equip1->pontos.push_back( TPonto( 10.0, 10.0 ) );
		equip1->pontos.push_back( TPonto( 20.0, 10.0 ) );
		equip1->pontos.push_back( TPonto( 20.0, 20.0 ) );
		equip1->pontos.push_back( TPonto( 10.0, 20.0 ) );
		equip1->Nivel = INSTRUMENTO;
		dados->Multipoint.push_back( equip1 );

		shared_ptr<TTexto> equip1Text( new TTexto() );
		equip1Text->texto = "Equipamento 1";
		equip1Text->Nivel = TAG;
		dados->Textos.push_back( equip1Text );
		
		TListaItensCelula lista;
		lista._multipoints.push_back( equip1 );
		lista._texts.push_back( dados->Textos.back() );
		dados->InfoCelula.ListaCelulasInstrumentos.push_back( lista );
	}

	// equipment 2
	{
		shared_ptr<TMultipoint> equip2( new TMultipoint() );
		equip2->pontos.push_back( TPonto( 40.0, 20.0 ) );
		equip2->pontos.push_back( TPonto( 40.0, 10.0 ) );
		equip2->pontos.push_back( TPonto( 50.0, 10.0 ) );
		equip2->pontos.push_back( TPonto( 50.0, 20.0 ) );
		equip2->Nivel = INSTRUMENTO;
		dados->Multipoint.push_back( equip2 );

		shared_ptr<TTexto> equip2Text( new TTexto() );
		equip2Text->texto = "Equipamento 2";
		equip2Text->Nivel = TAG;
		dados->Textos.push_back( equip2Text );

		TListaItensCelula lista;
		lista._multipoints.push_back( equip2 );
		lista._texts.push_back( dados->Textos.back() );
		dados->InfoCelula.ListaCelulasInstrumentos.push_back( lista );
	}

	// cable
	{
		shared_ptr<TMultipoint> cable( new TMultipoint() );
		cable->Nivel = CABO;
		cable->pontos.push_back( TPonto( 20.0, 15.0 ) );
		cable->pontos.push_back( TPonto( 40.0, 15.0 ) );
		dados->Multipoint.push_back( cable );
	}

	
	// bandeirola
	{
		shared_ptr<TMultipoint> bandeirola( new TMultipoint() );
		bandeirola->pontos.push_back( TPonto( 30.0, 5.0 ) );
		bandeirola->pontos.push_back( TPonto( 30.0, 14.95 ) );
		bandeirola->Nivel = BANDEIROLA;
		dados->Multipoint.push_back( bandeirola );

		shared_ptr<TTexto> bandeirolaText( new TTexto() );
		bandeirolaText->texto = "Bandeirola1";
		bandeirolaText->Nivel = BANDEIROLA;
		bandeirolaText->origem.x = 30.0;
		bandeirolaText->origem.y = 5.0;
		dados->Textos.push_back( bandeirolaText );

		TListaItensCelula lista;
		lista._multipoints.push_back( bandeirola );
		lista._texts.push_back( dados->Textos.back() );
		dados->InfoCelula.ListaCelulasBandeirolas.push_back( lista );
	}
}


void BasicTest::createFloor2( std::shared_ptr<CDadosGenerico> dados )
{
    // equipment 3
    {
        shared_ptr<TMultipoint> equip3( new TMultipoint() );
        equip3->pontos.push_back( TPonto( 40.0, 70.0 ) );
        equip3->pontos.push_back( TPonto( 60.0, 70.0 ) );
        equip3->pontos.push_back( TPonto( 60.0, 80.0 ) );
        equip3->pontos.push_back( TPonto( 40.0, 80.0 ) );
        equip3->Nivel = INSTRUMENTO;
        dados->Multipoint.push_back( equip3 );

        shared_ptr<TTexto> equip3Text( new TTexto() );
        equip3Text->texto = "Equipamento 3";
        equip3Text->Nivel = TAG;
        dados->Textos.push_back( equip3Text );

        TListaItensCelula lista;
		lista._multipoints.push_back( equip3 );
        lista._texts.push_back( dados->Textos.back() );
		dados->InfoCelula.ListaCelulasInstrumentos.push_back( lista );
    }

    // equipment 2
    {
        shared_ptr<TMultipoint> equip2( new TMultipoint() );
        equip2->pontos.push_back( TPonto( 40.0, 20.0 ) );
        equip2->pontos.push_back( TPonto( 40.0, 10.0 ) );
        equip2->pontos.push_back( TPonto( 50.0, 10.0 ) );
        equip2->pontos.push_back( TPonto( 50.0, 20.0 ) );
        equip2->Nivel = INSTRUMENTO;
        dados->Multipoint.push_back( equip2 );

        shared_ptr<TTexto> equip2Text( new TTexto() );
        equip2Text->texto = "Equipamento 2";
        equip2Text->Nivel = TAG;
        dados->Textos.push_back( equip2Text );

        TListaItensCelula lista;
		lista._multipoints.push_back( equip2 );
        lista._texts.push_back( dados->Textos.back() );
        dados->InfoCelula.ListaCelulasInstrumentos.push_back( lista );
    }

    // cable
    {
        shared_ptr<TMultipoint> cable( new TMultipoint() );
        cable->Nivel = CABO;
        cable->pontos.push_back( TPonto( 45.0, 20.0 ) );
        cable->pontos.push_back( TPonto( 45.0, 70.0 ) );
        dados->Multipoint.push_back( cable );
    }
}



TEST_F(BasicTest, singleDrawing) 
{
	CContainerDesenhos containerDesenhos;
	std::shared_ptr<CDadosGenerico> dados( new CDadosGenerico() );

	createFloor1( dados );

	containerDesenhos.addDrawing( dados, 100.0, "" );
	containerDesenhos.Conclui();

	double tam;
	vector<string> rota;
	shared_ptr<TArestasCircuito> ArestasCircuito( new TArestasCircuito );

	containerDesenhos.InfoCircuitos->GeraRota("Equipamento 2", "Equipamento 1", tam, rota, ArestasCircuito->Arestas);

	EXPECT_FLOAT_EQ( 20.0, tam );
	EXPECT_STREQ( "Equipamento 1/Bandeirola1/Equipamento 2", InputCircuit::getFormatedRoute( rota ).c_str() );
}


TEST_F(BasicTest, multipleDrawings) 
{
	CContainerDesenhos containerDesenhos;

	std::shared_ptr<CDadosGenerico> floor1Data( new CDadosGenerico() );
	createFloor1( floor1Data );
	containerDesenhos.addDrawing( floor1Data, 100.0, "" );

	std::shared_ptr<CDadosGenerico> floor2Data( new CDadosGenerico() );
    createFloor2( floor2Data );
    containerDesenhos.addDrawing( floor2Data, 126.0, "" );

	containerDesenhos.Conclui();

	double tam;
	vector<string> rota;
	shared_ptr<TArestasCircuito> ArestasCircuito( new TArestasCircuito );

	containerDesenhos.InfoCircuitos->GeraRota("Equipamento 3", "Equipamento 1", tam, rota, ArestasCircuito->Arestas );

	// 70 = 20 + 50 + (126.0 - 100.0)
	EXPECT_FLOAT_EQ( 96.0, tam );
	EXPECT_STREQ( "Equipamento 1/Bandeirola1/Equipamento 2/Equipamento 3", InputCircuit::getFormatedRoute( rota ).c_str() );
}



TEST_F(BasicTest, complete)
{
    UserParams userParams;
    string xlsFileName = TestsUtil::getDataPath() + "/tests/user_params.xls";
    loadUserParams( xlsFileName, &userParams );

    std::shared_ptr<CDadosGenerico> dados( new CDadosGenerico() );
    string dwgFileName = TestsUtil::getDataPath() + "\\tests\\drawing2.cwe";
    CweLoader *loader = new CweLoader( dwgFileName, dados, &userParams );

    CContainerDesenhos containerDesenhos;

    containerDesenhos.addDrawing( dados, 100.0, "" );
    containerDesenhos.Conclui();

    double tam;
    vector<string> rota;
	shared_ptr<TArestasCircuito> ArestasCircuito( new TArestasCircuito );
	containerDesenhos.InfoCircuitos->GeraRota("Equipamento 1", "Equipamento 2", tam, rota, ArestasCircuito->Arestas );

    EXPECT_FLOAT_EQ( 500.0, tam );
    EXPECT_STREQ( "Equipamento 2/Equipamento 1", InputCircuit::getFormatedRoute( rota ).c_str() );
}



TEST_F(BasicTest, complete2)
{
    string xlsFileName = TestsUtil::getDataPath() + "\\tests\\user_params2.xls";
    string inputCircuitsFileName = TestsUtil::getDataPath() + "\\tests\\input_circuits2.xls";

	shared_ptr<InterfaceFeedback> interfaceFeedback( new InterfaceFeedback( NULL ) );
    MainExecution mainExecution( xlsFileName, interfaceFeedback );
    mainExecution.execute( inputCircuitsFileName );

    ASSERT_EQ( 3, (int) mainExecution._resultCircuits.size() );

    EXPECT_STREQ( "Equipamento 1/Equipamento 2", InputCircuit::getFormatedRoute( mainExecution._resultCircuits[0].route ).c_str() );
    EXPECT_FLOAT_EQ( 500.0, mainExecution._resultCircuits[0].length );
	EXPECT_STREQ( "", mainExecution._resultCircuits[0].errorMessage.c_str() );

    EXPECT_STREQ( "Equipamento 2/Equipamento 1", InputCircuit::getFormatedRoute( mainExecution._resultCircuits[1].route ).c_str() );
    EXPECT_FLOAT_EQ( 500.0, mainExecution._resultCircuits[1].length );
	EXPECT_STREQ( "", mainExecution._resultCircuits[1].errorMessage.c_str() );

	EXPECT_STREQ( "O texto de origem não existe nos desenhos; O texto de destino não existe nos desenhos.", mainExecution._resultCircuits[2].errorMessage.c_str() );
}

}  // namespace
