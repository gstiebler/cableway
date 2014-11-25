/*
 * MainExecution.cpp
 *
 *  Created on: 15/10/2014
 *      Author: Guilherme
 */

#include "MainExecution.h"

#include "DwgLoader.h"
#include "InputCircuitsReader.h"
#include "UContainerDesenhos.h"
#include "UDadosGenerico.h"
#include "UInfoCircuitos.h"
#include "UserParams/LoadUserParams.h"
#include "UserParams/UserParams.h"
#include "UTCallbackStatusCarregamento.h"

using namespace std;

string MainExecution::exeFileName;

string MainExecution::getExePath()
{
    int index = exeFileName.find_last_of( '\\' );
    return exeFileName.substr( 0, index );
}



MainExecution::MainExecution( const string &userParametersFileName )
{
    _containerDesenhos = new CContainerDesenhos();
    UserParams userParams;
    loadUserParams( userParametersFileName, &userParams );

    for(int i(0); i < (int) userParams.drawingsParams.size(); ++i)
    {
        std::shared_ptr<CDadosGenerico> dados(new CDadosGenerico);
        DwgLoader dwgLoader( userParams.drawingsParams[i].fileName, dados, &userParams );
        _containerDesenhos->addDrawing( dados, userParams.drawingsParams[i].elevation );
    }

    callbackStatusCarregamento sc;
    _containerDesenhos->Conclui( sc );
}



MainExecution::~MainExecution()
{
    delete _containerDesenhos;
}



void MainExecution::execute( std::string inputCircuitsFileName )
{
	_inputCircuits.clear();
    readInputCircuits( inputCircuitsFileName, _inputCircuits );

	_resultCircuits.resize( _inputCircuits.size() );

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
    }
}



string MainExecution::ErrosDoCircuito( string Origem, string Destino, string route )
{
	string erro;
	if ( _containerDesenhos->InfoCircuitos->VerticesGerais->AchaVerticePeloTexto(Origem) < 0 )
	{
		bool exists;
		exists = false;
		for ( int j = 0 ; j < _containerDesenhos->NumDesenhos() ; j++ )
		{
			TDesenho *pnt = _containerDesenhos->getDesenho(j);
			for ( int i = 0 ; i < (int)pnt->GrafoDesenho->Dados->Textos.size() ; i++ )
			{
				if ( pnt->GrafoDesenho->Dados->Textos[i].texto == Origem )
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
			erro += "O texto de origem est� nos desenhos, por�m n�o est� associado a um equipamento ou bandeirola, ou ainda, est� fora do n�vel; ";
		}
		else
		{
			erro += "O texto de origem n�o existe nos desenhos; ";
		}
	}
	if ( _containerDesenhos->InfoCircuitos->VerticesGerais->AchaVerticePeloTexto(Destino) < 0 )
	{
		bool exists;
		exists = false;
		for ( int j = 0 ; j < _containerDesenhos->NumDesenhos() ; j++ )
		{
			TDesenho *pnt = _containerDesenhos->getDesenho(j);
			for ( int i = 0 ; i < (int)pnt->GrafoDesenho->Dados->Textos.size() ; i++ )
			{
				if ( pnt->GrafoDesenho->Dados->Textos[i].texto == Destino )
				{
					exists = true;
					break;
				}
			}
			if ( exists )
				break;
		}

		if ( exists )
			erro += "O texto de destino est� nos desenhos, por�m n�o est� associado a um equipamento ou bandeirola, ou ainda, est� fora do n�vel.";
		else
			erro += "O texto de destino n�o existe nos desenhos.";
	}
	if ( erro == "" && route == "" )
		erro = "N�o foi encontrado caminho.";

	return erro;
}
