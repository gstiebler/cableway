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

using namespace std;

MainPresenter::MainPresenter()
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

    MainExecution mainExecution( xlsFileName );
    vector<CircuitResult> circuitResults = mainExecution.execute( inputCircuitsFileName );

    DrawingPresenter *drawingPresenter = new DrawingPresenter;
}

