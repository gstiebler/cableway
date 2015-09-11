/*
 * MainExecution.cpp
 *
 *  Created on: 15/10/2014
 *      Author: Guilherme
 */

#include "MainExecution.h"

#include "file/CweLoader.h"
#include "file/InputCircuitsReader.h"
#include "UContainerDesenhos.h"
#include "UDadosGenerico.h"
#include "UInfoCircuitos.h"
#include "UserParams/LoadUserParams.h"
#include "UserParams/UserParams.h"
#include "TDesenho.h"
#include "Graph.h"
#include "InterfaceFeedback.h"

#include <QtCore>

using namespace std;

string MainExecution::exeFileName;

string MainExecution::getExePath()
{
	exeFileName = QCoreApplication::applicationFilePath().toStdString();
    int index = exeFileName.find_last_of( '/' );
    return exeFileName.substr( 0, index );
}



MainExecution::MainExecution( const string &userParametersFileName, shared_ptr<InterfaceFeedback> interfaceFeedback ) :
	_interfaceFeedback( interfaceFeedback )
{
    _containerDesenhos = make_shared<CContainerDesenhos>();
    UserParams userParams;
    loadUserParams( userParametersFileName, &userParams );

    for(int i(0); i < (int) userParams.drawingsParams.size(); ++i)
    {
        std::shared_ptr<CDadosGenerico> dados = make_shared<CDadosGenerico>();
		interfaceFeedback->showMessage( "Carregando desenho " + userParams.drawingsParams[i].fileName );
        CweLoader dwgLoader( userParams.drawingsParams[i].fileName, dados, &userParams );
        _containerDesenhos->addDrawing( dados, userParams.drawingsParams[i].elevation, userParams.drawingsParams[i].fileName );
    }

    _containerDesenhos->Conclui();
}



MainExecution::~MainExecution()
{
}



void MainExecution::execute( std::string inputCircuitsFileName )
{
	_inputCircuits.clear();
    readInputCircuits( inputCircuitsFileName, _inputCircuits );

	_resultCircuits.resize( _inputCircuits.size() );

	float tenPercent = _inputCircuits.size() * 0.1;
	int lastValue = 0;
	
	_interfaceFeedback->showMessage( "Processando circuitos" );
    for(int i(0); i < (int) _inputCircuits.size(); ++i)
    {
		TCircuito circuito;
		circuito.Origem = _inputCircuits[i].source;
		circuito.Destino = _inputCircuits[i].dest;
		circuito.RotaUsuario = _inputCircuits[i].getFormatedRoute();
		circuito.NomeCircuito = _inputCircuits[i].name;
		_containerDesenhos->InfoCircuitos->AdicionaCircuito( circuito );
		_resultCircuits[i].route = circuito.rota;
		_resultCircuits[i].length = circuito.metragem;

		_resultCircuits[i].errorMessage = ErrosDoCircuito( _inputCircuits[i].source, _inputCircuits[i].dest, _resultCircuits[i].route );

		int currValue = int(i / tenPercent);
		if( currValue > lastValue )
		{
			lastValue = currValue;
			char temp[256];
			sprintf( temp, "Circuitos processados: %d%%", currValue * 10 );
			_interfaceFeedback->showMessage( temp );
		}
    }
	
	_interfaceFeedback->showMessage( "Processamento concluído." );
}



string MainExecution::ErrosDoCircuito( string Origem, string Destino, vector<std::string> &route )
{
	string erro;
	if ( _containerDesenhos->InfoCircuitos->_graph->_verticesGerais->AchaVerticePeloTexto(Origem).get() ==  0 )
	{
		bool exists;
		exists = false;
		for ( int j = 0 ; j < _containerDesenhos->NumDesenhos() ; j++ )
		{
			shared_ptr<TDesenho> pnt = _containerDesenhos->getDesenho(j);
			for ( int i = 0 ; i < (int)pnt->GrafoDesenho->_dados->Textos.size() ; i++ )
			{
				if ( pnt->GrafoDesenho->_dados->Textos[i]->texto == Origem )
				{
					exists = true;
					break;
				}
			}
			if ( exists )
				break;
		}

		if ( exists )
		{
			erro += "O texto de origem está nos desenhos, porém não está associado a um equipamento ou bandeirola, ou ainda, está fora do nível; ";
		}
		else
		{
			erro += "O texto de origem não existe nos desenhos; ";
		}
	}
	if ( _containerDesenhos->InfoCircuitos->_graph->_verticesGerais->AchaVerticePeloTexto(Destino).get() == 0 )
	{
		bool exists;
		exists = false;
		for ( int j = 0 ; j < _containerDesenhos->NumDesenhos() ; j++ )
		{
			shared_ptr<TDesenho> pnt = _containerDesenhos->getDesenho(j);
			for ( int i = 0 ; i < (int)pnt->GrafoDesenho->_dados->Textos.size() ; i++ )
			{
				if ( pnt->GrafoDesenho->_dados->Textos[i]->texto == Destino )
				{
					exists = true;
					break;
				}
			}
			if ( exists )
				break;
		}

		if ( exists )
			erro += "O texto de destino está nos desenhos, porém não está associado a um equipamento ou bandeirola, ou ainda, está fora do nível.";
		else
			erro += "O texto de destino não existe nos desenhos.";
	}
	if ( erro == "" && route.size() == 0 )
		erro = "Não foi encontrado caminho.";

	return erro;
}
