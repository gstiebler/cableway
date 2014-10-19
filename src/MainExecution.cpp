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
    int index = exeFileName.find_last_of( '/' );
    return exeFileName.substr( 0, index );
}



MainExecution::MainExecution( const string &userParametersFileName )
{
    _containerDesenhos = new CContainerDesenhos();
    UserParams userParams;
    loadUserParams( userParametersFileName, &userParams );

    for(int i(0); i < (int) userParams.drawingsParams.size(); ++i)
    {
        boost::shared_ptr<CDadosGenerico> dados(new CDadosGenerico);
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



vector<CircuitResult> MainExecution::execute( std::string inputCircuitsFileName )
{
    vector<InputCircuit> inputCircuits;
    readInputCircuits( inputCircuitsFileName, inputCircuits );

    vector<CircuitResult> result( inputCircuits.size() );

    for(int i(0); i < (int) inputCircuits.size(); ++i)
    {
        TArestasCircuito *ArestasCircuito = NULL;
        vector<int> ListaBandeirolas;
        vector<string> DEBUG_arestas;
        string SubRotas;
        TCircuitoAreas *CircuitoAreas = NULL;
        _containerDesenhos->InfoCircuitos->GeraRota(inputCircuits[i].source, inputCircuits[i].dest, result[i].length, result[i].route,
                ArestasCircuito, &ListaBandeirolas, &DEBUG_arestas, SubRotas, CircuitoAreas);
    }

    return result;
}
