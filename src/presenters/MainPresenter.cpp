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
#include <UContainerDesenhos.h>

using namespace std;

MainPresenter::MainPresenter() :
        _mainExecution( NULL )
{
    _window = new MainWindow;
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
    string xlsFileName = MainExecution::getExePath() + "\\" + _window->getUserParamsFileName();
    string inputCircuitsFileName = MainExecution::getExePath() + "\\" + _window->getInputCircuitsFileName();

    _mainExecution = new MainExecution( xlsFileName );
    _mainExecution->execute( inputCircuitsFileName );

	fillWindowGrid( _mainExecution->_inputCircuits, _mainExecution->_resultCircuits );
}



void MainPresenter::fillWindowGrid( const vector<InputCircuit> &inputCircuits, const vector<CircuitResult> &resultCircuits )
{
	for( int i(0); i < (int) inputCircuits.size(); ++i)
	{
		const InputCircuit &circuit = inputCircuits[i];
		_window->setCircuit( i, circuit.name, circuit.source, circuit.dest, circuit.getFormatedRoute(), resultCircuits[i].route, circuit.cable, resultCircuits[i].length, resultCircuits[i].errorMessage );
	}
}



void MainPresenter::showCircuit( const QModelIndex &index )
{
	int indexOnGrid = index.row();
	printf( "Index: %d\n", indexOnGrid );

    CGrafoDesenho *grafoDesenho = _mainExecution->_containerDesenhos->getDesenho(0)->GrafoDesenho;
    CInfoCircuitos *infoCircuitos = _mainExecution->_containerDesenhos->InfoCircuitos;

	int circuitId = infoCircuitos->ListaArestasDoCircuito( indexOnGrid );
	DrawingPresenter *drawingPresenter = new DrawingPresenter( grafoDesenho, infoCircuitos, circuitId );
}

