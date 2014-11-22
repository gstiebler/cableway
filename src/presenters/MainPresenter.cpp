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

    CGrafoDesenho *grafoDesenho = _mainExecution->_containerDesenhos->getDesenho(0)->GrafoDesenho;
    CInfoCircuitos *infoCircuitos = _mainExecution->_containerDesenhos->InfoCircuitos;

    DrawingPresenter *drawingPresenter = new DrawingPresenter( grafoDesenho, infoCircuitos );

	fillWindowGrid( _mainExecution->_inputCircuits );
}



void MainPresenter::fillWindowGrid( std::vector<InputCircuit> inputCircuits )
{
	for( int i(0); i < (int) inputCircuits.size(); ++i)
	{
		InputCircuit &circuit = inputCircuits[i];
		_window->setCircuit( i, circuit.name, circuit.source, circuit.dest, circuit.getFormatedRoute(), circuit.cable );
	}
}

