/*
 * MainPresenter.cpp
 *
 *  Created on: 18/10/2014
 *      Author: Guilherme
 */

#include "MainPresenter.h"

#include <interface/MainWindow.h>
#include <vector>
#include <MainExecution.h>
#include "DrawingPresenter.h"
#include "interface/DrawingWindow.h"
#include "UMostraDesenho.h"
#include <UContainerDesenhos.h>
#include "Reports.h"
#include "TDesenho.h"
#include "Graph.h"
#include "UErros.h"
#include "InterfaceFeedback.h"
#include "Debug.h"

using namespace std;

MainPresenter::MainPresenter() :
        _mainExecution( NULL )
{
    _window = new MainWindow;
	_window->setInputFolder( MainExecution::getExePath() );
    _window->show();

    connect( _window, SIGNAL( executeButtonClicked() ), this, SLOT( execute() ) );
    connect( _window, SIGNAL( circuitGridClicked(QModelIndex) ), this, SLOT( showCircuit(QModelIndex) ) );
}



MainPresenter::~MainPresenter()
{
    // TODO Auto-generated destructor stub
}



void MainPresenter::execute()
{
	string xlsFileName = _window->getInputDirectory() + "/user_params.xls";
    string inputCircuitsFileName = _window->getInputDirectory() + "/input_circuits.xls";

	shared_ptr<InterfaceFeedback> interfaceFeecback( new InterfaceFeedback( _window ) );
    _mainExecution = new MainExecution( xlsFileName, interfaceFeecback );
    _mainExecution->execute( inputCircuitsFileName );

	fillWindowGrid( _mainExecution->_inputCircuits, _mainExecution->_resultCircuits );

	string circuitsReportFileName = _window->getInputDirectory() + "/CircuitsReport.csv";
	Reports::generateCirtuitsReport( circuitsReportFileName, _mainExecution->_inputCircuits, _mainExecution->_resultCircuits );

	string bandeirolasReportFileName = _window->getInputDirectory() + "/BandeirolasReport.csv";
	Reports::generateBandeirolaReport( bandeirolasReportFileName, _mainExecution->_inputCircuits, _mainExecution->_resultCircuits );

	string verticesDebugFileName = _window->getInputDirectory() + "/TextosEquipamentos.txt";
	Debug::printVertices( _mainExecution->_containerDesenhos->_graph->_verticesGerais, verticesDebugFileName );

	string graphDebugFileName = _window->getInputDirectory() + "/Grafo.txt";
	Debug::printGraph( _mainExecution->_containerDesenhos->_graph, graphDebugFileName );

	string dotGraphDebugFileName = _window->getInputDirectory() + "/graph.dot";
	Debug::generateDOTGraph( _mainExecution->_containerDesenhos->_graph->_verticesGerais->vertices, dotGraphDebugFileName );

	vector<string> errors;
	CErrosMsg::getInstance()->transferErrors( errors );
	for( int i(0); i < errors.size(); ++i )
		printf( "%s\n", errors[i].c_str() );
}



void MainPresenter::fillWindowGrid( const vector<InputCircuit> &inputCircuits, const vector<CircuitResult> &resultCircuits )
{
	for( int i(0); i < (int) inputCircuits.size(); ++i)
	{
		const InputCircuit &circuit = inputCircuits[i];
		_window->setCircuit( i, circuit.name, circuit.source, circuit.dest, circuit.getFormatedRoute(), InputCircuit::getFormatedRoute( resultCircuits[i].route ), circuit.cable, resultCircuits[i].length, resultCircuits[i].errorMessage );
	}
}



void MainPresenter::showCircuit( const QModelIndex &index )
{
	int indexOnGrid = index.row();
	printf( "Index: %d\n", indexOnGrid );
	
    std::shared_ptr<CInfoCircuitos> infoCircuitos = _mainExecution->_containerDesenhos->InfoCircuitos;

	string circuit, source, dest;
	_window->getCircuit( indexOnGrid, circuit, source, dest );
	shared_ptr<TArestasCircuito> arestasCircuito = infoCircuitos->ArestasDoCircuito[circuit];

	// circuit is OK
	if (arestasCircuito.get() != 0 && arestasCircuito->Arestas.size() > 0 )
    {
		for (int n=0; n < _mainExecution->_containerDesenhos->NumDesenhos(); n++)
		{
			shared_ptr<TDesenho> drawing = _mainExecution->_containerDesenhos->getDesenho( n );
			vector< shared_ptr<TAresta> > &arestasDesenho = arestasCircuito->ArestasDesenho[drawing.get()];
			if (arestasDesenho.size() > 0)
			{
				shared_ptr<CGrafoDesenho> grafoDesenho = drawing->GrafoDesenho;
				DrawingPresenter *drawingPresenter = new DrawingPresenter( grafoDesenho, infoCircuitos, arestasCircuito, drawing->NomeArquivo );
			}
		}
	}
	else // circuit has problems
	{
		shared_ptr<TVerticeGeral> vertice = infoCircuitos->_graph->_verticesGerais->AchaVerticePeloTexto( source );
		shared_ptr<TVerticeGeral> vertice2 = infoCircuitos->_graph->_verticesGerais->AchaVerticePeloTexto( dest );

		if( vertice.get() != 0 && vertice2.get() != 0 )
		{
			for (int n=0; n < _mainExecution->_containerDesenhos->NumDesenhos(); n++)
			{
				shared_ptr<TDesenho> drawing = _mainExecution->_containerDesenhos->getDesenho( n );
				shared_ptr<CGrafoDesenho> grafoDesenho = _mainExecution->_containerDesenhos->getDesenho( n )->GrafoDesenho;
				DrawingPresenter *drawingPresenter = new DrawingPresenter( grafoDesenho, infoCircuitos, arestasCircuito, drawing->NomeArquivo );
				drawingPresenter->_window->_mostraDesenho->MostraDoubleArvore(vertice, vertice2);
			}
		}
		else // just showing all the drawings
		{
			for (int n=0; n < _mainExecution->_containerDesenhos->NumDesenhos(); n++)
			{
				shared_ptr<TDesenho> drawing = _mainExecution->_containerDesenhos->getDesenho( n );
				shared_ptr<CGrafoDesenho> grafoDesenho = _mainExecution->_containerDesenhos->getDesenho( n )->GrafoDesenho;
				DrawingPresenter *drawingPresenter = new DrawingPresenter( grafoDesenho, infoCircuitos, arestasCircuito, drawing->NomeArquivo );
			}
		}
	}
}

