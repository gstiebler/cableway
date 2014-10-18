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

using namespace std;

MainPresenter::MainPresenter()
{
    _mainWindow = new MainWindow;
    _mainWindow->show();

    connect( _mainWindow, SIGNAL( executeButtonClicked() ), this, SLOT( execute() ) );
}

MainPresenter::~MainPresenter()
{
    // TODO Auto-generated destructor stub
}


void MainPresenter::execute()
{
    string xlsFileName = _mainWindow->getUserParamsFileName();
    string inputCircuitsFileName = _mainWindow->getUserParamsFileName();

    MainExecution mainExecution( xlsFileName );
    vector<CircuitResult> circuitResults = mainExecution.execute( inputCircuitsFileName );
}

